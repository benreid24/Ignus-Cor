#include "Editor/GUI/EditableCellTable.hpp"
#include "Editor/Helpers/Dialogs.hpp"
using namespace std;
using namespace sfg;

EditableCellTable::EditableCellTable(Desktop& dk, Widget::Ptr pr,
    const vector<string>& colNames, const string& nm, int width)
: desktop(dk), parent(pr), appendPressed(false), delUuid(-1), nextUuid(1), typeName(nm) {
    if (width == 0)
        width = colNames.size()*150;

	box = ScrolledWindow::Create();
	box->SetRequisition(sf::Vector2f(width, 120));
    box->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_ALWAYS );

    container = Box::Create(Box::Orientation::VERTICAL, 4);
    container->SetRequisition(sf::Vector2f(width, 120));
    appendBut = Button::Create("New " + typeName);
    appendBut->GetSignal(Button::OnLeftClick).Connect([this] { this->appendPressed = true; });
    container->Pack(appendBut, false, false);

    table = Table::Create();
    table->SetRequisition(sf::Vector2f(width, 0));
    table->SetColumnSpacings(10);
    table->SetRowSpacings(5);

    box->AddWithViewport(table);
    container->Pack(box);

    for (unsigned int i = 0; i<colNames.size(); ++i) {
        headerRow.push_back(Label::Create(colNames[i]));
    }
    refreshGui();
}

void EditableCellTable::refreshGui() {
    auto pos = [](int row, int col, int w = 1, int h = 1)
        -> sf::Rect<uint32_t>{ return sf::Rect<uint32_t>(col, row, w, h); };


    table->RemoveAll();
    for (unsigned int i = 0; i<headerRow.size(); ++i)
        table->Attach(headerRow[i], pos(0, i));
    table->Attach(Separator::Create(), pos(1, 0, headerRow.size()+1));

    int row = 2;
    for (unsigned int i = 0; i<rows.size(); ++i, row += 2) {
        for (unsigned int j = 0; j<rows[i]->entries.size(); ++j)
            table->Attach(rows[i]->entries[j], pos(row, j));
        table->Attach(rows[i]->delBut, pos(row, rows[i]->entries.size()));
        table->Attach(Separator::Create(), pos(row+1, 0, rows[i]->entries.size()+1));
    }
}

void EditableCellTable::clear() {
    rows.clear();
    refreshGui();
}

void EditableCellTable::appendRow(const vector<string>& values) {
    Row::Ptr row = Row::create();
    string val = "";
    for (unsigned int i = 0; i<headerRow.size(); ++i) {
        if (i<values.size())
            val = values[i];
        else
            val = "";
        row->entries.push_back(Entry::Create(val));
    }
    row->delBut = Button::Create("Delete");
    int uuid = nextUuid++;
    row->delBut->GetSignal(Button::OnLeftClick).Connect( [this, uuid] { this->delUuid = uuid; });
    row->uuid = uuid;
    rows.push_back(row);
    refreshGui();
}

vector<vector<string> > EditableCellTable::getAllValues() {
    vector<vector<string> > values;
    values.reserve(rows.size());
    for (unsigned int i = 0; i<rows.size(); ++i) {
        vector<string>& row = *(values.emplace(values.end()));
        row.reserve(rows[i]->entries.size());
        for (unsigned int j = 0; j<rows[i]->entries.size(); ++j) {
            row.push_back(rows[i]->entries[j]->GetText());
        }
    }
    return values;
}

void EditableCellTable::update() {
    if (appendPressed) {
        appendPressed = false;
        appendRow(vector<string>());
    }
    if (delUuid!=-1) {
        if (yesnobox(desktop, parent, "Delete "+typeName+"?", "Delete this "+typeName+"?")) {
            for (unsigned int i = 0; i<rows.size(); ++i) {
                if (rows[i]->uuid == delUuid) {
                    rows.erase(rows.begin()+i);
                    break;
                }
            }
            refreshGui();
        }
        delUuid = -1;
    }
}

void EditableCellTable::show(bool s) {
    container->Show(s);
}

void EditableCellTable::addToParent(Box::Ptr pr) {
    pr->Pack(container);
}
