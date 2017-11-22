#include "Shared/GUI/CellTable.hpp"
#include "Shared/Util/UUID.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;
using namespace sfg;

CellTable::CellTable(sfg::Box::Ptr parent) {
	editId = -1;

	cellBox = Box::Create(Box::Orientation::VERTICAL,4);
	cellArea = ScrolledWindow::Create();
    cellArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC );
    cellBox->Pack(cellArea);
}

void CellTable::editRow(int id) {
	editId = id;
}

void CellTable::appendRow(int uuid, Box::Ptr row) {
	CellTable* me = this;
	if (rows.find(uuid)!=rows.end())
		cellBox->Remove(rows[uuid]);
	Button::Ptr editBut = Button::Create("Edit");
	editBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->editRow(uuid); });
	row->Pack(editBut,false,false);
	Button::Ptr delBut = Button::Create("Delete");
	delBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->removeRow(uuid); });
	row->Pack(delBut,false,false);
	rows[uuid] = row;
	cellBox->Pack(row,false,false);
}

void CellTable::removeRow(int uuid) {
	if (rows.find(uuid)!=rows.end()) {
		cellBox->Remove(rows[uuid]);
		rows.erase(uuid);
	}
}

int CellTable::getEditCell() {
	int r = editId;
	editId = -1;
	return r;
}

void CellTable::reorder() {
	cellBox->RemoveAll();
	for (map<int,Box::Ptr>::iterator i = rows.begin(); i!=rows.end(); ++i)
		cellBox->Pack(i->second,false,false);
}
