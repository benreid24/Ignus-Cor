#include "Shared/GUI/CellTable.hpp"
#include "Shared/Util/UUID.hpp"
#include "Shared/Util/Util.hpp"
using namespace std;
using namespace sfg;

CellTable::Row::Row(int id, vector<vector<string> > colVals, Box::Ptr parent, CellTable* owner) {
	container = parent;
	for (unsigned int i = 0; i<colVals.size(); ++i) {
		if (colVals[i].size() == 0) {
			Entry::Ptr but = Entry::Create();
			but->SetRequisition(sf::Vector2f(80,20));
			parent->Pack(but,true,false);
			columns.push_back(but);
		}
		else {
			ComboBox::Ptr bx = ComboBox::Create();
			bx->SetRequisition(sf::Vector2f(80,20));
			for (unsigned int j = 0; j<colVals[i].size(); ++j) {
				bx->AppendItem(colVals[i][j]);
			}
			bx->SelectItem(0);
			parent->Pack(bx,false,false);
			columns.push_back(bx);
		}
		parent->Pack(Separator::Create(Separator::Orientation::VERTICAL),true,false);
	}
	delButton = Button::Create("Delete");
	delButton->GetSignal(Button::OnLeftClick).Connect( [owner,id] { owner->removeRow(id); });
	parent->Pack(delButton,true,false);
}

CellTable::CellTable(sfg::Box::Ptr parent, const string& title) {
	CellTable* me = this;

	titleBox = Box::Create();
	titleBox->Pack(Label::Create(title));

	container = Box::Create(Box::Orientation::VERTICAL,5);
	cellArea = ScrolledWindow::Create();
	cellArea->SetScrollbarPolicy( sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_ALWAYS );
	rowBox = Box::Create(Box::Orientation::VERTICAL,2);
	cellArea->SetRequisition(sf::Vector2f(1000,700));
	cellArea->AddWithViewport(rowBox);
	appendBut = Button::Create("Add Row");
	appendBut->GetSignal(Button::OnLeftClick).Connect( [me] { me->appendRow(); });

	container->Pack(titleBox,true,false);
	container->Pack(cellArea,false,false);
	container->Pack(appendBut,true,false);
	parent->Pack(container,false,false);
	container->RequestResize();
}

void CellTable::appendColumn(const string& name, vector<string> values) {
	int i = columns.size();
	columns[name] = i;
	columnValues[name] = values;
}

void CellTable::appendRow() {
	int id = UUID::create();
	Box::Ptr box = Box::Create(Box::Orientation::HORIZONTAL,5);
	vector<vector<string>> colVals;
	for (auto i = columnValues.begin(); i!=columnValues.end(); ++i)
		colVals.push_back(i->second);
	rows[id] = Row(id,colVals,box,this);
	rowBox->Pack(box,true,false);
}

void CellTable::removeRow(int uuid) {
	if (rows.find(uuid)!=rows.end()) {
		Row& row = rows[uuid];
		for (unsigned int i = 0; i<row.columns.size(); ++i) {
			//row.container->Remove(row.columns[i]);
		}
		rowBox->Remove(row.container);
		cout << "rm1\n";
		rows.erase(uuid);
		cout << "Rm2\n";
	}
}

string CellTable::getCellValue(int index, const string& colName) {
	int i = 0;
	for (auto j = rows.begin(); j!=rows.end(); ++j) {
		if (i==index) {
			Widget::Ptr w = j->second.columns[columns[colName]];
			Entry* e = dynamic_cast<Entry*>(w.get());
			ComboBox* c = dynamic_cast<ComboBox*>(w.get());
			if (e != nullptr)
				return e->GetText();
			else
				return c->GetSelectedText();
		}
		++i;
	}
	return "";
}

int CellTable::getCellValueAsInt(int index, const string& colName) {
	string val = getCellValue(index,colName);
	return stringToInt(val);
}

void CellTable::setCellValue(int index, const string& colName, const string& value) {
	int i = 0;
	for (auto j = rows.begin(); j!=rows.end(); ++j) {
		if (i==index) {
			Widget::Ptr w = j->second.columns[columns[colName]];
			Entry* e = dynamic_cast<Entry*>(w.get());
			ComboBox* c = dynamic_cast<ComboBox*>(w.get());
			if (e != nullptr)
				e->SetText(value);
			else {
				int ci = 0;
				for (auto k = c->Begin(); k!=c->End(); ++k) {
					if (*k==value) {
						c->SelectItem(ci);
						break;
					}
					ci++;
				}
			}
			break;
		}
	}
}

void CellTable::setCellValue(int index, const string& colName, int value) {
	setCellValue(index,colName,intToString(value));
}
