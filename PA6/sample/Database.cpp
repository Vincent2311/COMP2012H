#include <fstream>
#include <iomanip>
#include <iostream>
#include "Database.h"
#include "String.h"
#include "Table.h"

using namespace std;

Database::Database(const String &name, int numTables) : name{name}, tableHead{nullptr}, currentTable{nullptr}, numTables{0}
{
    for (int i = 1; i < numTables + 1; ++i)
    {
        char tableName[10] = "Table_";
        sprintf(tableName + 6, "%d", i);
        String name(tableName);
        addTable(name);
    }
}

Database::Database(const String &filename) : name{""}, tableHead{nullptr}, currentTable{nullptr}, numTables{0}
{
    ifstream ifs(filename.getStr());
    ifs >> name;
    int tableCount;
    ifs >> tableCount;
    for (int i = 0; i < tableCount; ++i)
    {
        String name;
        ws(ifs);
        getline(ifs, name, '\n');
        Table *table = new Table(ifs, name);
        if (!addTable(table))
            delete table;
    }
}

Database::~Database()
{
    Table *temp = nullptr;
    while (tableHead != nullptr)
    {
        temp = tableHead;
        tableHead = tableHead->next;
        delete temp;
    }
}

bool Database::addTable(Table *table)
{
    if (table == nullptr)
    {
        cout << "Table is a null pointer." << endl;
        return false;
    }
    if (tableHead == nullptr)
    {
        tableHead = table;
        ++numTables;
        return true;
    }
    for (Table *cur = tableHead;; cur = cur->next)
    {
        if (cur->getName() == table->getName())
        {
            cout << "Table with given name already exists in the database." << endl;
            return false;
        }
        if (cur->next == nullptr)
        {
            cur->next = table;
            ++numTables;
            return true;
        }
    }
}

void Database::addTable(const String &name)
{
    Table *t = new Table(name);
    if (!addTable(t))
        delete t;
}

Table *Database::findTable(const String &name) const
{
    Table *cur;
    for (cur = tableHead; cur != nullptr; cur = cur->next)
    {
        if (cur->getName() == name)
            break;
    }
    return cur;
}

void Database::listTables() const
{
    cout << "The tables currently existing in the database are:\n";
    for (Table *cur = tableHead; cur != nullptr; cur = cur->next)
    {
        cout << cur->getName() << '\n';
    }
}

void Database::deleteTable(const String &name)
{
    if (tableHead == nullptr)
    {
        cout << "No such table exists in the database." << endl;
        return;
    }
    Table *toDelete;
    if (tableHead->getName() == name)
    {
        toDelete = tableHead;
        tableHead = tableHead->next;
        toDelete->next = nullptr;
        delete toDelete;
        --numTables;
    }
    else
    {
        Table *cur;
        for (cur = tableHead; cur->next != nullptr && cur->next->getName() != name; cur = cur->next)
            ;
        if (cur->next == nullptr)
        {
            cout << "No such table exists in the database." << endl;
            return;
        }
        else
        {
            toDelete = cur->next;
            cur->next = toDelete->next;
            toDelete->next = nullptr;
            delete toDelete;
            --numTables;
        }
    }
}

void Database::setCurrentTable(const String &name)
{
    currentTable = findTable(name);
    if (currentTable != nullptr)
    {
        return;
    }
    cout << "No such table exists in the database." << endl;
}

void Database::saveDatabase(const String &filename) const
{
    ofstream ofs(filename.getStr());
    ofs << this->name << '\n';
    ofs << numTables << '\n';
    for (Table *table = tableHead; table != nullptr; table = table->next)
    {
        ofs << table->getName() << '\n';
        ofs << table->getNumCols() << '\n';
        ofs << table->getNumRows() << '\n';

        for (Field *field = table->getFieldHead(); field != nullptr; field = field->next)
        {
            ofs << ((field == table->getPrimaryKey()) ? "*" : "") << field->name << " " << ((field->type == INT) ? '0' : '1') << ((field->next == nullptr) ? "\n" : ", ");
        }
        for (int i = 0; i < table->getNumRows(); ++i)
        {
            for (int j = 0; j < table->getNumCols(); ++j)
            {
                ofs << (*table)[j][i] << ((j == (table->getNumCols() - 1)) ? "\n" : ", ");
            }
        }
    }
    ofs.close();
}

void Database::innerJoin(Table *table1, Table *table2)
{
    if (table1 == nullptr || table2 == nullptr)
    {
        cout << "No such table exists in the database." << endl;
        return;
    }

    Field *f1 = table1->getPrimaryKey();
    Field *f2 = table2->getPrimaryKey();

    if (f1->type != f2->type)
    {
        cout << "Type mismatch between target fields." << endl;
        return;
    }

    Table *merge = new Table(table1->getName() + '+' + table2->getName());
    merge->addField(0, f1->name + '+' + f2->name, f1->type);

    int f1idx, f2idx;
    for (Field *cur = table1->getFieldHead(); cur != nullptr; cur = cur->next)
    {
        if (cur->name == f1->name)
        {
            f1idx = merge->getNumCols() - 1;
            continue;
        }
        merge->addField(merge->getNumCols(), cur->name, cur->type);
    }
    for (Field *cur = table2->getFieldHead(); cur != nullptr; cur = cur->next)
    {
        if (cur->name == f2->name)
        {
            f2idx = merge->getNumCols() - table1->getNumCols();
            continue;
        }
        String fname = (merge->findField(cur->name) != nullptr) ? cur->name + "(T2)" : cur->name;
        merge->addField(merge->getNumCols(), fname, cur->type);
    }

    for (int i = 0; i < table1->getNumRows(); ++i)
    {
        for (int j = 0; j < table2->getNumRows(); ++j)
        {
            if ((*table1)[f1idx][i] == (*table2)[f2idx][j])
            {
                String *record = new String[merge->getNumCols()];
                record[0] = (*table1)[f1idx][i];
                int count = 1;
                for (int k = 0; k < table1->getNumCols(); ++k)
                {
                    if (k != f1idx)
                        record[count++] = (*table1)[k][i];
                }
                for (int k = 0; k < table2->getNumCols(); ++k)
                {
                    if (k != f2idx)
                        record[count++] = (*table2)[k][j];
                }
                merge->addRecord(merge->getNumRows(), record);
                delete[] record;
            }
        }
    }
    addTable(merge);
}

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