#include <fstream>
#include "Table.h"
#include "String.h"

using namespace std;

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

Table::Table(const String &name)
{
    primaryKey = nullptr;
    fieldHead = nullptr;
    next = nullptr;
    numRows = 0;
    numCols = 0;
    tableSize = 100;
    this->name = name;
}

Table::Table(ifstream &ifs, const String &name)
{
    this->name = name;
    tableSize = 100;
    ifs >> numCols;
    ifs >> numRows;
    primaryKey = nullptr;
    fieldHead = nullptr;
    next = nullptr;
    if (!numCols)
        return;

    String record;
    String type;
    Field *cur;
    if (numCols >= 2)
    {
        getline(ifs, record, ',');
        cur = new Field;
        fieldHead = cur;
        type = record.substr(record.length() - 1, record.length() - 1);
        cur->type = static_cast<TYPE>(stoi(type));
        cur->column = new String[tableSize];
        if (record[0] == '*')
        {
            primaryKey = cur;
            record = record.substr(1, record.length() - 3);
            cur->name = record;
        }
        else
        {
            record = record.substr(0, record.length() - 2);
            cur->name = record;
        }

        for (int i = 2; i < numCols; ++i)
        {
            getline(ifs, record, ',');
            cur->next = new Field;
            type = record.substr(record.length() - 1, record.length() - 1);
            cur->next->type = static_cast<TYPE>(stoi(type));
            cur->next->column = new String[tableSize];
            if (record[0] == '*')
            {
                primaryKey = cur->next;
                record = record.substr(1, record.length() - 3);
                cur->next->name = record;
            }
            else
            {
                record = record.substr(0, record.length() - 2);
                cur->next->name = record;
            }
            cur = cur->next;
        }
        getline(ifs, record, '\n');
        cur->next = new Field;

        type = record.substr(record.length() - 1, record.length() - 1);
        cur->next->type = static_cast<TYPE>(stoi(type));
        cur->next->column = new String[tableSize];
        if (record[0] == '*')
        {
            primaryKey = cur;
            record = record.substr(1, record.length() - 3);
            cur->next->name = record;
        }
        else
        {
            record = record.substr(0, record.length() - 2);
            cur->next->name = record;
        }
        cur->next->next = nullptr;
    }

    else
    {
        getline(ifs, record, '\n');
        cur = new Field;
        fieldHead = cur;
        type = record.substr(record.length() - 1, record.length() - 1);
        cur->type = static_cast<TYPE>(stoi(type));
        cur->column = new String[tableSize];
        if (record[0] == '*')
        {
            primaryKey = cur;
            record = record.substr(1, record.length() - 3);
            cur->name = record;
        }
        else
        {
            record = record.substr(0, record.length() - 2);
            cur->name = record;
        }
        cur->next = nullptr;
    }

    for (int i = 0; i < numRows; ++i)
    {
        cur = fieldHead;
        if (numCols >= 2)
        {
            getline(ifs, record, ',');
            cur->column[i] = record;
            cur = cur->next;
            for (int j = 2; j < numCols; ++j)
            {
                getline(ifs, record, ',');
                record = record.substr(0, record.length());
                cur->column[i] = record;
                cur = cur->next;
            }

            getline(ifs, record, '\n');
            record = record.substr(0, record.length());
            cur->column[i] = record;
        }
        else
        {
            getline(ifs, record, '\n');
            record = record.substr(0, record.length());
            cur->column[i] = record;
        }
    }
}

Table::~Table()
{
    if (!fieldHead)
        return;
    Field *cur = fieldHead->next;
    Field *pre = fieldHead;
    while (cur)
    {
        delete[] pre->column;
        delete pre;
        pre = cur;
        cur = cur->next;
    }
    delete[] pre->column;
    delete pre;
    primaryKey = nullptr;
    fieldHead = nullptr;
    next = nullptr;
    numRows = 0;
    numCols = 0;
}

void Table::addRecord(int index, String *record)
{
    if (!record)
    {
        cout << "Record is empty.\n";
        return;
    }
    if (index < 0 || index > numRows)
    {
        cout << "Table row index is out of bounds.\n";
        return;
    }
    Field *cur = fieldHead;
    int colIndex = 0;
    while (cur != primaryKey)
    {
        colIndex++;
        cur = cur->next;
    }
    if (record[colIndex] == "")
    {
        cout << "Empty or duplicate primary key.\n";
        return;
    }
    for (int j = 0; j < numRows; j++)
    {
        if (primaryKey->column[j] == record[colIndex])
        {
            cout << "Empty or duplicate primary key.\n";
            return;
        }
    }
    cur = fieldHead;
    colIndex = 0;
    while (cur)
    {
        if (cur->type == INT && !isInteger(record[colIndex]))
        {
            cout << "Cannot insert non-integer in integer field.\n";
            return;
        }
        colIndex++;
        cur = cur->next;
    }

    for (int i = numRows - 1; i >= index; i--)
    {
        for (int j = 0; j < numCols; j++)
        {
            (*this)[j][i + 1] = (*this)[j][i];
        }
    }
    for (int i = 0; i < numCols; i++)
    {
        (*this)[i][index] = record[i];
    }

    numRows++;
    String *tmp;
    if (numRows == tableSize)
    {
        tableSize += 100;
        cur = fieldHead;

        while (cur)
        {
            tmp = new String[tableSize];
            for (int i = 0; i < numRows; i++)
            {
                tmp[i] = cur->column[i];
            }
            delete[] cur->column;
            cur->column = tmp;
            tmp = nullptr;
            cur = cur->next;
        }
    }
}

void Table::addField(int index, const String &name, TYPE type)
{
    if (index < 0 || index > numCols)
    {
        cout << "Table column index is out of bounds.\n";
        return;
    }

    Field *cur = fieldHead;
    while (cur)
    {
        if (cur->name == name)
        {
            cout << "Field with given name already exists in table.\n";
            return;
        }
        cur = cur->next;
    }

    Field *newFieled = new Field;
    newFieled->column = new String[tableSize];
    newFieled->name = name;
    newFieled->type = type;
    numCols++;

    if (type == INT)
    {
        for (int i = 0; i < numRows; i++)
        {
            newFieled->column[i] = "0";
        }
    }
    else
    {
        for (int i = 0; i < numRows; i++)
        {
            newFieled->column[i] = "";
        }
    }

    if (!fieldHead)
    {
        newFieled->next = nullptr;
        fieldHead = newFieled;
        primaryKey = fieldHead;
    }
    else if (index == 0)
    {
        newFieled->next = fieldHead;
        fieldHead = newFieled;
    }

    else
    {
        cur = fieldHead;
        Field *nxt = fieldHead->next;
        for (int i = 1; i < index; i++)
        {
            cur = cur->next;
            nxt = nxt->next;
        }
        cur->next = newFieled;
        newFieled->next = nxt;
    }
}

void Table::deleteRecord(int row)
{
    if (row < 0 || row >= numRows)
    {
        cout << "Table row index is out of bounds.\n";
        return;
    }
    for (int i = row; i < numRows - 1; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            (*this)[j][i] = (*this)[j][i + 1];
        }
    }
    if (numRows > 0)
    {
        numRows--;
    }
}

void Table::modifyRecord(int row, int column, const String &newVal)
{
    if (row < 0 || row >= numRows)
    {
        cout << "Table row index is out of bounds.\n";
        return;
    }
    if (row < 0 || column >= numCols)
    {
        cout << "Table column index is out of bounds.\n";
        return;
    }
    Field *cur = fieldHead;
    for (int i = 0; i < column; i++)
    {
        cur = cur->next;
    }

    if (cur == primaryKey)
    {
        if (newVal == "")
        {
            cout << "Empty or duplicate primary key.\n";
            return;
        }
        for (int i = 0; i < numRows; i++)
        {
            if (primaryKey->column[i] == newVal)
            {
                cout << "Empty or duplicate primary key.\n";
                return;
            }
        }
    }

    if (cur->type == INT && !isInteger(newVal))
    {
        cout << "Cannot insert non-integer in integer field.\n";
        return;
    }

    (*this)[column][row] = newVal;
}

void Table::setPrimaryKey(const String &name)
{
    Field *cur = fieldHead;
    while (cur)
    {
        if (cur->name == name)
        {
            break;
        }
        cur = cur->next;
    }
    if (!cur)
    {
        cout << "No such field with provided name exists.\n";
        return;
    }

    String *list = new String[numRows];
    for (int i = 0; i < numRows; i++)
    {
        if (cur->column[i] == "")
        {
            cout << "Cannot set field with empty data as primary key.\n";
            delete[] list;
            return;
        }
        for (int j = 0; j < i; j++)
        {
            if (cur->column[i] == cur->column[j])
            {
                cout << "Cannot set field with duplicate elements as primary key.\n";
                delete[] list;
                return;
            }
        }
        list[i] = cur->column[i];
    }
    primaryKey = cur;
    delete[] list;
}

void Table::setColumnIndex(int index, const String &target)
{
    Field *cur = fieldHead;
    int count = 0;
    while (cur)
    {
        if (cur->name == target)
        {
            break;
        }
        cur = cur->next;
        count++;
    }
    if (!cur)
    {
        cout << "No such field with provided name exists.\n";
        return;
    }
    if (index < 0 || index >= numCols)
    {
        cout << "Table column index is out of bounds.\n";
        return;
    }

    if (index == cur - fieldHead)
        return;
    if (cur == fieldHead)
    {
        fieldHead = cur->next;
        Field *tar = fieldHead;
        for (int i = 1; i < index; i++)
        {
            tar = tar->next;
        }
        cur->next = tar->next;
        tar->next = cur;
    }
    else if (index == 0)
    {
        Field *pre = fieldHead;
        while (pre->next != cur)
        {
            pre = pre->next;
        }
        pre->next = cur->next;
        cur->next = fieldHead;
        fieldHead = cur;
    }
    else if (index < count)
    {
        Field *pre = fieldHead;
        while (pre->next != cur)
        {
            pre = pre->next;
        }
        Field *tar = fieldHead;
        for (int i = 0; i < index; i++)
        {
            tar = tar->next;
        }
        Field *tarPre = fieldHead;
        while (tarPre->next != tar)
        {
            tarPre = tarPre->next;
        }
        pre->next = cur->next;
        cur->next = tar;
        tarPre->next = cur;
    }
    else
    {
        Field *pre = fieldHead;
        while (pre->next != cur)
        {
            pre = pre->next;
        }
        Field *tar = fieldHead;
        for (int i = 0; i < index; i++)
        {
            tar = tar->next;
        }
        pre->next = cur->next;
        cur->next = tar->next;
        tar->next = cur;
    }
}

void Table::deleteField(const String &name)
{
    Field *cur = fieldHead;
    while (cur)
    {
        if (cur->name == name)
        {
            break;
        }
        cur = cur->next;
    }
    if (!cur)
    {
        cout << "No such field with provided name exists.\n";
        return;
    }

    if (cur == primaryKey)
    {
        cout << "Cannot delete primary key field.\n";
        return;
    }

    if (cur == fieldHead)
    {
        fieldHead = cur->next;
    }
    else
    {
        Field *pre = fieldHead;
        while (pre->next != cur)
        {
            pre = pre->next;
        }
        pre->next = cur->next;
    }
    delete[] cur->column;
    delete cur;
    if (numCols > 0)
    {
        numCols--;
    }
}

Field *Table::findField(const String &name) const
{
    Field *cur = fieldHead;
    while (cur)
    {
        if (cur->name == name)
        {
            break;
        }
        cur = cur->next;
    }
    if (!cur)
    {
        cout << "No such field with provided name exists.\n";
        return nullptr;
    }
    return cur;
}

void Table::sortTable(Field *field)
{
    if (!field)
    {
        cout << "Invalid field provided.\n";
        return;
    }
    String tmp;
    Field *cur;
    if (field->type == STRING)
    {
        for (int i = 0; i < numRows - 1; i++)
        {
            for (int j = 0; j < numRows - i - 1; j++)
            {
                if (field->column[j] > field->column[j + 1])
                {
                    cur = fieldHead;
                    while (cur)
                    {
                        tmp = cur->column[j];
                        cur->column[j] = cur->column[j + 1];
                        cur->column[j + 1] = tmp;
                        cur = cur->next;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < numRows - 1; i++)
        {
            for (int j = 0; j < numRows - i - 1; j++)
            {
                if (stoi(field->column[j]) > stoi(field->column[j + 1]))
                {
                    cur = fieldHead;
                    while (cur)
                    {
                        tmp = cur->column[j];
                        cur->column[j] = cur->column[j + 1];
                        cur->column[j + 1] = tmp;
                        cur = cur->next;
                    }
                }
            }
        }
    }
}