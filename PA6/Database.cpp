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

Database::Database(const String &filename)
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
    if(!tableHead) return;
    Table *cur = tableHead->next;
    Table *pre = tableHead;
    while (!cur)
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
        numTables++;
        return true;
    }
}

void Database::addTable(const String &name)
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
            ofs << curField->name << " " << curField->type <<endl;
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
    ;
}