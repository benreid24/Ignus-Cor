#include "Editor/GUI/CellTable.hpp"
#include "Shared/Util/UUID.hpp"
#include "Shared/Util/Util.hpp"
#include "Shared/Properties.hpp"
using namespace std;
using namespace sfg;
using namespace sf;

namespace {
const int HEADER = -1234567;
}

CellTable::CellTable(sfg::Box::Ptr pr, const vector<string>& colNames, int width)
: columns(colNames), editId(-1), toRemove(-1), parent(pr) {
    if (width == 0)
        width = colNames.size()*270;

	cellArea = ScrolledWindow::Create();
	cellArea->SetRequisition(Vector2f(width, 800));
    cellArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );

    table = Table::Create();
    table->SetRequisition(Vector2f(colNames.size()*230, 0));
    table->SetColumnSpacings(10);
    table->SetRowSpacings(5);

    cellArea->AddWithViewport(table);
    parent->Pack(cellArea,false,false);

    appendRow(HEADER, columns, false);
}

void CellTable::editRow(int id) {
	editId = id;
}

void CellTable::appendRow(int uuid, const vector<string>& cells, bool buts) {
	CellTable* me = this;

	auto rowRef = rows.find(uuid);
	if (rowRef != rows.end()) {
        cout << "Warning: Row with id " << uuid << " already exists\n";
        return;
	}
	rowRef = rows.emplace(uuid, vector<Widget::Ptr>()).first;

	for (unsigned int i = 0; i<cells.size(); ++i) {
        auto lbl = Label::Create(cells[i]);
        rowRef->second.push_back(lbl);
	}

	if (buts) {
        Button::Ptr editBut = Button::Create("Edit");
        editBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->editRow(uuid); });
        rowRef->second.push_back(editBut);

        Button::Ptr delBut = Button::Create("Delete");
        delBut->GetSignal(Button::OnLeftClick).Connect( [me,uuid] { me->removeRow(uuid,false); });
        rowRef->second.push_back(delBut);
	}

	reorder();
}

void CellTable::removeRow(int uuid, bool imm) {
	if (rows.find(uuid)!=rows.end()) {
		if (imm) {
			doRowDelete(uuid);
			rows.erase(uuid);
		}
        else
			toRemove = uuid;
	}
}

void CellTable::removeAll() {
	parent->Remove(cellArea);
	table->RemoveAll();
	rows.clear();
	appendRow(HEADER, columns, false);
	parent->Pack(cellArea,false,false);
}

int CellTable::getEditCell() {
	int r = editId;
	editId = -1;
	return r;
}

int CellTable::getDeleteCell() {
    int r = toRemove;
    toRemove = -1;
    return r;
}

void CellTable::reorder() {
	parent->Remove(cellArea);
	table->RemoveAll();
	unsigned int row = 0;
	for (auto i = rows.begin(); i!=rows.end(); ++i, row += 2) {
		vector<Widget::Ptr>& cols = i->second;
		for (unsigned int col = 0; col<cols.size(); ++col) {
            table->Attach(cols[col], Rect<uint32_t>(col, row, 1, 1));
		}
		table->Attach(Separator::Create(), Rect<uint32_t>(0, row+1, cols.size(), 1));
	}
	parent->Pack(cellArea,false,false);
}

vector<int> CellTable::getIds() {
	vector<int> ids;
	for (auto i = rows.begin(); i!=rows.end(); ++i)
		ids.push_back(i->first);
	return ids;
}

void CellTable::doRowDelete(int uuid) {
    auto row = rows.find(uuid);
    if (row != rows.end()) {
        for (auto i = row->second.begin(); i!=row->second.end(); ++i) {
            table->Remove(*i);
        }
        rows.erase(row);
    }
}
