#include <fstream>
#include <iomanip>
#include <iostream>
#include "Database.h"
#include "String.h"
#include "Table.h"

using namespace std;

void Database::printTable(Table *table) const
{
    if (table == nullptr)
    {
        cout << "No such table exists in the database." << endl;
        return;
    }

    for (Field *cur = table->getFieldHead(); cur != nullptr; cur = cur->next)
    {
        cout << setw(15) << left;
        String curPrintName = (cur == table->getPrimaryKey() ? "*" : "") + cur->name + ' ' + (cur->type == INT ? "0" : "1");
        cout << curPrintName;
    }
    cout << endl;
    for (int i = 0; i < table->getNumRows(); ++i)
    {
        for (int j = 0; j < table->getNumCols(); ++j)
        {
            cout << setw(15) << left;
            cout << (*table)[j][i];
        }
        cout << endl;
    }

    return;
}

Database::Database(const String &name, int numTables)
{
    currentTable = nullptr;
    this->numTables = numTables;
    this->name = name;
    if (!numTables)
    {
        tableHead = nullptr;
        return;
    }
    tableHead = new Table("Table_1");
    String prefix = "Table_";
    Table *cur = tableHead;
    for (int i = 2; i <= numTables; ++i)
    {
        char *tmp = new char;
        sprintf(tmp, "%d", i);
        cur->next = new Table(prefix + tmp);
        cur = cur->next;
        delete tmp;
    }
    cur->next = nullptr;
}

Database::Database(const String &filename) // TODO:
{
    currentTable = nullptr;
    const char *ifname = filename.getStr();
    ifstream ifs(ifname);

    ifs >> this->name;
    ifs >> this->numTables;
    if (!numTables)
    {
        tableHead = nullptr;
        return;
    }

    String tableName;
    ifs >> tableName;
    tableHead = new Table(ifs, tableName);
    Table *cur = tableHead;
    for (int i = 1; i < numTables; i++)
    {
        ifs >> tableName;
        cur->next = new Table(ifs, tableName);
        cur = cur->next;
    }
    cur->next = nullptr;
    ifs.close();
}

Database::~Database()
{
    if (!tableHead)
        return;
    Table *cur = tableHead->next;
    Table *pre = tableHead;
    while (cur)
    {
        delete pre;
        pre = cur;
        cur = cur->next;
    }
    delete pre;
    tableHead = nullptr;
    currentTable = nullptr;
    numTables = 0;
}

bool Database::addTable(Table *table)
{
    if (!table)
    {
        cout << "Table is a null pointer.\n";
        return false;
    }
    else
    {
        if (!tableHead)
        {
            tableHead = table;
        }
        else
        {
            Table *cur = tableHead;
            Table *nxt = tableHead->next;
            if (cur->getName() == table->getName())
            {
                cout << "Table with given name already exists in the database.\n";
                return false;
            }
            while (nxt)
            {
                if (nxt->getName() == table->getName())
                {
                    cout << "Table with given name already exists in the database.\n";
                    return false;
                }
                cur = nxt;
                nxt = nxt->next;
            }
            cur->next = table;
        }
        numTables++;
        return true;
    }
}

void Database::addTable(const String &name)
{
    if (!tableHead)
    {
        tableHead = new Table(name);
    }

    else
    {
        Table *cur = tableHead;
        Table *nxt = tableHead->next;
        if (cur->getName() == name)
        {
            cout << "Table with given name already exists in the database.\n";
            return;
        }
        while (nxt)
        {
            if (nxt->getName() == name)
            {
                cout << "Table with given name already exists in the database.\n";
                return;
            }
            cur = nxt;
            nxt = nxt->next;
        }
        cur->next = new Table(name);
    }
    numTables++;
    return;
}

Table *Database::findTable(const String &name) const
{
    Table *cur = tableHead;
    while (cur)
    {
        if (cur->getName() == name)
        {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void Database::listTables() const
{
    cout << "The tables currently existing in the database are:\n";
    Table *cur = tableHead;
    while (cur)
    {
        cout << cur->getName() << '\n';
        cur = cur->next;
    }
    return;
}

void Database::deleteTable(const String &name)
{
    if (!tableHead->next)
    {
        if (tableHead->getName() == name)
        {
            delete tableHead;
            numTables--;
            tableHead = nullptr;
        }
        else {
            cout << "No such table exists in the database.\n";
        }
        return;
    }
    Table *cur = tableHead;
    Table *nxt = tableHead->next;
    if (cur->getName() == name)
    {
        tableHead = nxt;
        delete cur;
        numTables--;
        return;
    }
    while (nxt)
    {
        if (nxt->getName() == name)
        {
            cur->next = nxt->next;
            delete nxt;
            numTables--;
            return;
        }
        cur = nxt;
        nxt = nxt->next;
    }
    cout << "No such table exists in the database.\n";
    return;
}

void Database::setCurrentTable(const String &name)
{
    currentTable = findTable(name);
    if (!currentTable)
    {
        cout << "No such table exists in the database.\n";
    }
    return;
}

void Database::saveDatabase(const String &filename) const
{
    const char *ofname = filename.getStr();
    ofstream ofs(ofname);
    ofs << name << endl;
    ofs << numTables << endl;
    Table *curTable = tableHead;
    while (curTable)
    {
        ofs << curTable->getName() << endl;
        ofs << curTable->getNumCols() << endl;
        ofs << curTable->getNumRows() << endl;
        if (!curTable->getNumCols())
        {
            curTable = curTable->next;
            continue;
        }

        // print field
        Field *curField = curTable->getFieldHead();
        while (curField->next)
        {
            if (curField == curTable->getPrimaryKey())
            {
                ofs << "*" << curField->name << " " << curField->type << ", ";
            }
            else
            {
                ofs << curField->name << " " << curField->type << ", ";
            }
            curField = curField->next;
        }
        if (curField == curTable->getPrimaryKey())
        {
            ofs << "*" << curField->name << " " << curField->type << endl;
        }
        else
        {
            ofs << curField->name << " " << curField->type << endl;
        }

        // print record
        for (int i = 0; i < curTable->getNumRows(); i++)
        {
            for (int j = 0; j < curTable->getNumCols() - 1; j++)
            {
                ofs << (*curTable)[j][i] << ", ";
            }
            ofs << (*curTable)[curTable->getNumCols() - 1][i] << endl;
        }
        curTable = curTable->next;
    }
    ofs.close();
}

void Database::innerJoin(Table *table1, Table *table2)
{
    if (!table1 || !table2)
    {
        cout << "No such table exists in the database.\n";
        return;
    }
    if (table1->getPrimaryKey()->type != table2->getPrimaryKey()->type)
    {
        cout << "Type mismatch between target fields.\n";
        return;
    }
    Table *new_table = new Table(table1->getName() + "+" + table2->getName());
    new_table->addField(0, table1->getPrimaryKey()->name + "+" + table2->getPrimaryKey()->name, table1->getPrimaryKey()->type);
    String *used_name = new String[table1->getNumCols()];

    int index = 1;
    int pri_index1;
    int pri_index2;
    Field *cur_field = table1->getFieldHead();
    for (int i = 0; i < table1->getNumCols(); i++)
    {
        if (cur_field != table1->getPrimaryKey())
        {
            new_table->addField(index, cur_field->name, cur_field->type);
            used_name[index - 1] = cur_field->name;
            index++;
        }
        else
        {
            pri_index1 = i;
        }
        cur_field = cur_field->next;
    }

    bool flag = false;
    int index2 = index;
    cur_field = table2->getFieldHead();
    for (int i = 0; i < table2->getNumCols(); i++)
    {
        if (cur_field != table2->getPrimaryKey())
        {
            flag = false;
            for (int j = 0; j < index - 1; j++)
            {
                if (cur_field->name == used_name[j])
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                new_table->addField(index2, cur_field->name + "(T2)", cur_field->type);
            }
            else
            {
                new_table->addField(index2, cur_field->name, cur_field->type);
            }
            index2++;
        }
        else
        {
            pri_index2 = i;
        }
        cur_field = cur_field->next;
    }
    delete[] used_name;

    int *t1_row = new int[table1->getNumCols()];
    int *t2_row = new int[table1->getNumCols()];
    int count = 0;
    for (int i = 0; i < table1->getNumRows(); i++)
    {
        for (int j = 0; j < table2->getNumRows(); j++)
        {
            if ((*table1)[pri_index1][i] == (*table2)[pri_index2][j])
            {
                t1_row[count] = i;
                t2_row[count] = j;
                count++;
                break;
            }
        }
    }
    String *record = new String[table1->getNumCols() + table2->getNumCols()];
    for (int i = 0; i < count; i++)
    {
        index = 1;
        record[0] = (*table1)[pri_index1][t1_row[i]];
        for (int j = 0; j < table1->getNumCols(); j++)
        {
            if (j != pri_index1)
            {
                record[index] = (*table1)[j][t1_row[i]];
                index++;
            }
        }
        index2 = index;
        for (int k = 0; k < table2->getNumCols(); k++)
        {
            if (k != pri_index2)
            {
                record[index2] = (*table2)[k][t2_row[i]];
                index2++;
            }
        }
        new_table->addRecord(i, record);
    }

    delete[] t1_row;
    delete[] t2_row;
    delete[] record;
    this->addTable(new_table);
}