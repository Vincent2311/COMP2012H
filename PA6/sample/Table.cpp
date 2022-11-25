#include <fstream>
#include "Table.h"
#include "String.h"

using namespace std;

Table::Table(const String &name) : next{nullptr}, primaryKey{nullptr}, fieldHead{nullptr}, numRows{0}, numCols{0}, tableSize{100}, name{name} {}

void parse(String line, String *record, int numCols)
{
    int *commas = new int[numCols - 1];
    int tail = 0;
    for (int i = 0; i < line.length(); ++i)
    {
        if (line[i] == ',')
        {
            commas[tail++] = i;
        }
    }
    for (int i = 0; i < numCols; ++i)
    {
        if (i == 0)
        {
            record[i] = line.substr(0, commas[i]);
        }
        else if (i == numCols - 1)
        {
            record[i] = line.substr(commas[i - 1] + 2, line.length() - commas[i - 1] - 2);
        }
        else
        {
            record[i] = line.substr(commas[i - 1] + 2, commas[i] - commas[i - 1] - 2);
        }
    }
    delete[] commas;
}

Table::Table(ifstream &ifs, const String &name) : next{nullptr}, primaryKey{nullptr}, fieldHead{nullptr}, numRows{0}, numCols{0}, tableSize{100}, name{name}
{
    int numCols, numRows;
    ifs >> numCols >> numRows;
    if (numCols != 0)
    {
        for (int i = 0; i < numCols; ++i)
        {
            String row;
            ws(ifs);
            if (i == (numCols - 1))
            {
                getline(ifs, row, '\n');
            }
            else
            {
                getline(ifs, row, ',');
            }
            int type = row[row.length() - 1] - '0';
            if (row[0] == '*')
            {
                addField(i, row.substr(1, row.length() - 3), static_cast<TYPE>(type));
                setPrimaryKey(row.substr(1, row.length() - 3));
            }
            else
            {
                addField(i, row.substr(0, row.length() - 2), static_cast<TYPE>(type));
            }
        }
        if (numRows != 0)
        {
            for (int i = 0; i < numRows; ++i)
            {
                String *record = new String[numCols];
                String line;
                getline(ifs, line, '\n');
                parse(line, record, numCols);
                addRecord(this->numRows, record);
                delete[] record;
            }
        }
    }
}

Table::~Table()
{
    Field *temp = nullptr;
    while (fieldHead != nullptr)
    {
        temp = fieldHead;
        fieldHead = fieldHead->next;
        delete[] temp->column;
        delete temp;
    }
}

void Table::addRecord(int index, String *record)
{
    if (record == nullptr)
    {
        cout << "Record is empty." << endl;
        return;
    }

    if (index < 0 || index > numRows)
    {
        cout << "Table row index is out of bounds." << endl;
        return;
    }

    Field *cur = fieldHead;
    int keyIdx = 0;
    while (cur != primaryKey)
    {
        cur = cur->next;
        keyIdx++;
    }

    if (record[keyIdx] == "")
    {
        cout << "Empty or duplicate primary key." << endl;
        return;
    }
    for (int i = 0; i < numRows; ++i)
    {
        if (primaryKey->column[i] == record[keyIdx])
        {
            cout << "Empty or duplicate primary key." << endl;
            return;
        }
    }

    keyIdx = 0;
    cur = fieldHead;
    while (cur != nullptr)
    {
        if (cur->type == INT && !isInteger(record[keyIdx]))
        {
            cout << "Cannot insert non-integer in integer field." << endl;
            return;
        }
        cur = cur->next;
        keyIdx++;
    }

    if (index < numRows)
    {
        cur = fieldHead;
        while (cur != nullptr)
        {
            for (int j = numRows; j > index; --j)
            {
                cur->column[j] = cur->column[j - 1];
            }
            cur = cur->next;
        }
    }

    keyIdx = 0;
    cur = fieldHead;
    while (cur != nullptr)
    {
        cur->column[index] = record[keyIdx];
        cur = cur->next;
        keyIdx++;
    }

    if (++numRows == tableSize)
    {
        cur = fieldHead;
        while (cur != nullptr)
        {
            String *newColumn = new String[numRows + 100];
            for (int j = 0; j < numRows; ++j)
            {
                newColumn[j] = cur->column[j];
            }
            delete[] cur->column;
            cur->column = newColumn;
            cur = cur->next;
        }
        tableSize += 100;
    }
}

void Table::addField(int index, const String &name, TYPE type)
{

    if (index < 0 || index > numCols)
    {
        cout << "Table column index is out of bounds." << endl;
        return;
    }

    Field *f;
    for (f = fieldHead; f != nullptr; f = f->next)
    {
        if (f->name == name)
        {
            cout << "Field with given name already exists in table." << endl;
            return;
        }
    }

    f = new Field();
    f->name = name;
    f->next = nullptr;
    f->column = new String[tableSize];
    f->type = type;
    String init = (f->type == INT) ? "0" : "";
    for (int i = 0; i < numRows; ++i)
    {
        f->column[i] = init;
    }

    if (fieldHead == nullptr || index == 0)
    {
        f->next = fieldHead;
        fieldHead = f;
    }
    else
    {
        Field *cur = fieldHead;
        for (int i = 0; i < index - 1; ++i)
        {
            cur = cur->next;
        }
        f->next = cur->next;
        cur->next = f;
    }

    if (numCols++ == 0)
    {
        primaryKey = f;
    }
}

void Table::deleteRecord(int row)
{

    if (row >= numRows || row < 0)
    {
        cout << "Table row index is out of bounds." << endl;
        return;
    }

    Field *cur = fieldHead;
    while (cur != nullptr)
    {
        for (int j = row; j < numRows - 1; ++j)
        {
            cur->column[j] = cur->column[j + 1];
        }
        cur = cur->next;
    }

    --numRows;
}

void Table::modifyRecord(int row, int column, const String &newVal)
{
    if (row >= numRows || row < 0)
    {
        cout << "Table row index is out of bounds." << endl;
        return;
    }

    if (column >= numCols || column < 0)
    {
        cout << "Table column index is out of bounds." << endl;
        return;
    }

    if (newVal == (*this)[column][row])
    {
        return;
    }

    Field *key = fieldHead;
    int keyIdx = 0;
    while (key != primaryKey)
    {
        key = key->next;
        keyIdx++;
    }

    if (column == keyIdx)
    {
        if (newVal == "")
        {
            cout << "Empty or duplicate primary key." << endl;
            return;
        }
        for (int i = 0; i < numRows; ++i)
        {
            if (newVal == key->column[i])
            {
                cout << "Empty or duplicate primary key." << endl;
                return;
            }
        }
    }

    key = fieldHead;
    for (int i = 0; i < column; ++i)
    {
        key = key->next;
    }
    if (key->type == INT && !isInteger(newVal))
    {
        cout << "Cannot insert non-integer in integer field." << endl;
        return;
    }
    key->column[row] = newVal;
}

void Table::setPrimaryKey(const String &name)
{
    for (Field *cur = fieldHead; cur != nullptr; cur = cur->next)
    {
        if (cur->name == name)
        {
            for (int i = 0; i < numRows; ++i)
            {
                for (int j = i + 1; j < numRows; ++j)
                {
                    if (cur->column[i] == cur->column[j])
                    {
                        cout << "Cannot set field with duplicate elements as primary key." << endl;
                        return;
                    }
                }
                if (cur->column[i] == "")
                {
                    cout << "Cannot set field with empty data as primary key." << endl;
                    return;
                }
            }

            primaryKey = cur;
            return;
        }
    }

    cout << "No such field with provided name exists." << endl;
}

void Table::setColumnIndex(int index, const String &target)
{
    Field *tgt = fieldHead, *prev = fieldHead;
    while (tgt != nullptr && tgt->name != target)
    {
        prev = tgt;
        tgt = tgt->next;
    }

    if (tgt == nullptr)
    {
        cout << "No such field with provided name exists." << endl;
        return;
    }

    if (index >= numCols || index < 0)
    {
        cout << "Table column index is out of bounds." << endl;
        return;
    }

    if (tgt == fieldHead)
    {
        fieldHead = tgt->next;
    }
    else
    {
        prev->next = tgt->next;
    }

    if (index == 0)
    {
        tgt->next = fieldHead;
        fieldHead = tgt;
    }
    else
    {
        prev = fieldHead;
        for (int i = 0; i < index - 1; ++i)
        {
            prev = prev->next;
        }
        tgt->next = prev->next;
        prev->next = tgt;
    }
}

void Table::deleteField(const String &name)
{
    if (primaryKey->name == name)
    {
        cout << "Cannot delete primary key field." << endl;
        return;
    }

    Field *deleted = nullptr;
    if (fieldHead->name == name)
    {
        deleted = fieldHead;
        fieldHead = fieldHead->next;
    }
    else
    {
        Field *cur = fieldHead;
        for (; cur->next != nullptr; cur = cur->next)
        {
            if (cur->next->name == name)
            {
                deleted = cur->next;
                cur->next = cur->next->next;
                break;
            }
        }
    }

    if (deleted == nullptr)
    {
        cout << "No such field with provided name exists." << endl;
        return;
    }
    delete[] deleted->column;
    delete deleted;
    --numCols;
}

Field *Table::findField(const String &name) const
{
    Field *cur = fieldHead;
    for (; cur != nullptr; cur = cur->next)
    {
        if (cur->name == name)
            break;
    }
    if (cur == nullptr)
    {
        cout << "No such field with provided name exists." << endl;
    }
    return cur;
}

void Table::sortTable(Field *field)
{
    if (field == nullptr)
    {
        cout << "Invalid field provided." << endl;
        return;
    }
    for (int i = 0; i < (numRows - 1); ++i)
    {
        for (int j = i + 1; j < numRows; ++j)
        {
            if (field->type == INT)
            {
                if (stoi(field->column[i]) > stoi(field->column[j]))
                {
                    for (int k = 0; k < numCols; ++k)
                    {
                        String temp = (*this)[k][i];
                        (*this)[k][i] = (*this)[k][j];
                        (*this)[k][j] = temp;
                    }
                }
            }
            else
            {
                if (field->column[i] > field->column[j])
                {
                    for (int k = 0; k < numCols; ++k)
                    {
                        String temp = (*this)[k][i];
                        (*this)[k][i] = (*this)[k][j];
                        (*this)[k][j] = temp;
                    }
                }
            }
        }
    }
}

bool isInteger(const String &test)
{
    if (!test.length())
        return false;

    if (test[0] != '-' && !isdigit(test[0]))
        return false;

    for (int i = 1; i < test.length(); ++i)
    {
        if (!isdigit(test[i]))
            return false;
    }
    return true;
}

String *Table::operator[](int index) const
{

    if (index >= numCols || index < 0)
    {
        cout << "Table column index is out of bounds." << endl;
        return nullptr;
    }

    Field *cur = fieldHead;
    for (int count = 0; count < index; cur = cur->next, ++count)
        ;
    return cur->column;
}