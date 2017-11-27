#include "Shared/GUI/CellTable.hpp"
#include "Shared/Util/UUID.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sfg;
using namespace sf;

CellTable::CellTable(sfg::Box::Ptr pr) {
	editId = -1;
	parent = pr;

	cellBox = Box::Create(Box::Orientation::VERTICAL,8);
	cellArea = ScrolledWindow::Create();
	cellArea->SetRequisition(Vector2f(Properties::ScreenWidth+450,Properties::ScreenHeight-20));
    cellArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );
    cellArea->AddWithViewport(cellBox);
    parent->Pack(cellArea,false,true);
}

void CellTable::editRow(int id) {
	editId = id;
}

void CellTable::appendRow(int uuid, Box::Ptr row) {
	CellTable* me = this;
	Button::Ptr editBut = Button::Create("Edit");
	editBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->editRow(uuid); });
	row->Pack(editBut,false,false);
	Button::Ptr delBut = Button::Create("Delete");
	delBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->removeRow(uuid,false); });
	row->Pack(delBut,false,false);
	rows[uuid] = row;
	reorder();
}

void CellTable::removeRow(int uuid, bool imm) {
	if (rows.find(uuid)!=rows.end()) {
		removed.push_back(uuid);
		if (imm) {
			cellBox->Remove(rows[uuid]);
			rows.erase(uuid);
		}
        else
			toRemove.push_back(uuid);
	}
}

void CellTable::removeAll() {
	parent->Remove(cellArea);
	cellBox->RemoveAll();
	rows.clear();
	parent->Pack(cellArea,false,true);
}

int CellTable::getEditCell() {
	int r = editId;
	editId = -1;
	return r;
}

bool CellTable::needsReorder() {
	return toRemove.size()>0;
}

void CellTable::reorder() {
	for (unsigned int i = 0; i<toRemove.size(); ++i)
		rows.erase(toRemove[i]);
	parent->Remove(cellArea);
	cellBox->RemoveAll();
	for (map<int,Box::Ptr>::iterator i = rows.begin(); i!=rows.end(); ++i) {
		cellBox->Pack(i->second,true,false);
		cellBox->Pack(Separator::Create());
	}
	parent->Pack(cellArea,false,true);
}

vector<int> CellTable::getIds() {
	vector<int> ids;
	for (map<int,Box::Ptr>::iterator i = rows.begin(); i!=rows.end(); ++i)
		ids.push_back(i->first);
	return ids;
}

vector<int> CellTable::getDeletedIds() {
	vector<int> ret = removed;
	removed.clear();
	return ret;
}

Box::Ptr packRow(vector<string> cells) {
	Box::Ptr row = Box::Create(Box::Orientation::HORIZONTAL,3);
	row->Pack(Label::Create(cells[0]),false,false);
	for (unsigned int i = 1; i<cells.size(); ++i) {
        row->Pack(Separator::Create(Separator::Orientation::VERTICAL),false,false);
        row->Pack(Label::Create(cells[i]),false,false);
	}
	return row;
}
