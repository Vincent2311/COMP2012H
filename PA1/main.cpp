#include <iostream>
using namespace std;

void printYear (int year) {
    cout << "Year " << year << endl;
    

}
void printDateHorizontal(int vacantDays, int days, bool sunday)
{
    if (sunday)
    {
        cout << "Su Mo Tu We Th Fr Sa" << endl;
        if (vacantDays == 0)
        {
            vacantDays = 6;
        }
        else
        {
            vacantDays -= 1;
        }
    }

    else
    {
        cout << "Mo Tu We Th Fr Sa Su" << endl;
        vacantDays -= 2;
        if (vacantDays < 0)
        {
            vacantDays += 7;
        }
    }

    for (int i = 0; i < vacantDays; i++)
    {
        cout << "   ";
    }
    int date = 1;
    for (; date < 7 - vacantDays; date++)
    {
        cout << " " << date << " ";
    }
    cout << " " << date << endl;
    ++date;
    int count = 1;
    for (; date <= days; date++)
    {
        if (count % 7 == 0)
        {
            if (date < 10)
            {
                cout << " " << date << endl;
            }
            else
            {
                cout << date << endl;
            }
        }
        else
        {
            if (date < 10)
            {
                cout << " " << date << " ";
            }
            else
            {
                cout << date << " ";
            }
        }
        count += 1;
    }
}

void printDateVertical(int vacantDays, int days, bool sunday)
{
    const char *dateInWordSu[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    const char *dateInWordMo[7] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
    if (sunday)
    {
        if (vacantDays == 0)
        {
            vacantDays = 6;
        }
        else
        {
            vacantDays -= 1;
        }
    }

    else
    {
        vacantDays -= 2;
        if (vacantDays < 0)
        {
            vacantDays += 7;
        }
    }

    int startDate = 8 - vacantDays;
    int initial = 1;

    for (int i = 0; i < 7; i++)
    {

        if (sunday)
        {
            cout << dateInWordSu[i] << " ";
        }
        else
        {
            cout << dateInWordMo[i] << " ";
        }

        if (vacantDays > 0)
        {
            cout << "   ";
        }
        else
        {
            cout << " " << initial << " ";
            ++initial;
        }
        if (startDate < 10)
        {
            cout << " " << startDate << " ";
        }
        else
        {
            cout << startDate << " ";
        }
        int followingDays = startDate + 7;
        while (followingDays < days - 7)
        {
            if (followingDays < 10)
            {
                cout << " " << followingDays << " ";
            }
            else
            {
                cout << followingDays << " ";
            }
            followingDays += 7;
        }
        cout << followingDays << endl;
        startDate += 1;
        vacantDays -= 1;
    }
}

int calculateDays(int year, int month)
{
    int totalDate;
    if (month == 2)
    {
        if (year % 4)
        {
            totalDate = 28;
        }
        else if (year % 100)
        {
            totalDate = 29;
        }
        else if (year % 400)
        {
            totalDate = 28;
        }
        else
        {
            totalDate = 29;
        }
    }
    else
    {
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        {
            totalDate = 31;
        }
        else
        {
            totalDate = 30;
        }
    }
    return totalDate;
}

int calculateDayinWeek(int year, int month)
{
    // h == 0 for Saturday, h == 6 for Friday
    if (month == 1)
    {
        month = 13;
        --year;
    }
    if (month == 2)
    {
        month = 14;
        --year;
    }
    int m = month;
    int k = year % 100;
    int j = year / 100;
    int h = (1 + 13 * (m + 1) / 5 + k + k / 4 +
             j / 4 + 5 * j) %
            7;
    return h;
}

int main()
{
    int year, month;
    char firstDay, printFormat;
    const char *monthInWord[12] = {"January", "February", "March", "April", "May", "June", "July", "August",
                                   "September", "October", "November", "December"};

    do
    {
        cout << "Enter year (in range 1800-9999): ";
        cin >> year;
    } while (year < 1800 || year > 9999);

    do
    {
        cout << "Enter month (in range 1-12): ";
        cin >> month;
    } while ((month < 1 && month != -1) || month > 12);

    if (month == -1)
    {
        printYear(year);
    }
    //TODO:
    else if (month == -3)
    {
        ;
    }

    else
    {
        do
        {
            cout << "Enter the first day of week ('s' for Sunday, 'm' for Monday): ";
            cin >> firstDay;
        } while (firstDay != 's' && firstDay != 'm');

        do
        {
            cout << "Print horizontally (h) or vertically (v): ";
            cin >> printFormat;
        } while (printFormat != 'h' && printFormat != 'v');

        cout << monthInWord[month - 1] << " " << year << endl;

        int totalDate = calculateDays(year, month);
        int h = calculateDayinWeek(year, month);

        if (firstDay == 's' && printFormat == 'h')
        {
            printDateHorizontal(h, totalDate, true);
        }
        else if (firstDay == 'm' && printFormat == 'h')
        {
            printDateHorizontal(h, totalDate, false);
        }
        else if (firstDay == 's' && printFormat == 'v')
        {
            printDateVertical(h, totalDate, true);
        }
        else
        {
            printDateVertical(h, totalDate, false);
        }
    }
    return 0;
}