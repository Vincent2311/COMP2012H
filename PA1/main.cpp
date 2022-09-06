#include <iostream>
using namespace std;

const char *monthInWord[12] = {"January", "February", "March", "April", "May", "June", "July", "August",
                               "September", "October", "November", "December"};

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
             j / 4 + 5 * j) % 7;
    return h;
}

void printDatesInRow(int year, int month)
{
    // month should be the middle month of a row
    int totalDays1, totalDays2, totalDays3, vacantDays1, vacantDays2,vacantDays3;
    if (month == 1)
    {
        totalDays1 = calculateDays(year - 1, 12);
        totalDays2 = calculateDays(year, 1);
        totalDays3 = calculateDays(year, 2);
        vacantDays1 = calculateDayinWeek(year - 1, 12);
        vacantDays2 = calculateDayinWeek(year, 1);
        vacantDays3 = calculateDayinWeek(year, 2);
    }
    else if (month == 12)
    {
        totalDays1 = calculateDays(year, 11);
        totalDays2 = calculateDays(year, 12);
        totalDays3 = calculateDays(year + 1, 1);
        vacantDays1 = calculateDayinWeek(year, 11);
        vacantDays2 = calculateDayinWeek(year, 12);
        vacantDays3 = calculateDayinWeek(year + 1, 1);
    }
    else
    {
        totalDays1 = calculateDays(year, month - 1);
        totalDays2 = calculateDays(year, month);
        totalDays3 = calculateDays(year, month + 1);
        vacantDays1 = calculateDayinWeek(year, month - 1);
        vacantDays2 = calculateDayinWeek(year, month);
        vacantDays3 = calculateDayinWeek(year, month + 1);
    }
    if (vacantDays1 == 0)
    {
        vacantDays1 = 6;
    }
    else
    {
        vacantDays1 -= 1;
    }

    if (vacantDays2 == 0)
    {
        vacantDays2 = 6;
    }
    else
    {
        vacantDays2 -= 1;
    }

    if (vacantDays3 == 0)
    {
        vacantDays3 = 6;
    }
    else
    {
        vacantDays3 -= 1;
    }

    // print date line one
    for (int i = 0; i < vacantDays1; i++)
    {
        cout << "   ";
    }
    int date1 = 1;
    for (; date1 < 7 - vacantDays1; date1++)
    {
        cout << " " << date1 << " ";
    }
    cout << " " << date1 << "  ";

    for (int i = 0; i < vacantDays2; i++)
    {
        cout << "   ";
    }
    int date2 = 1;
    for (; date2 < 7 - vacantDays2; date2++)
    {
        cout << " " << date2 << " ";
    }
    cout << " " << date2 << "  ";

    for (int i = 0; i < vacantDays3; i++)
    {
        cout << "   ";
    }
    int date3 = 1;
    for (; date3 < 7 - vacantDays3; date3++)
    {
        cout << " " << date3 << " ";
    }
    cout << " " << date3 << endl;
    ++date1;
    ++date2;
    ++date3;

    // print middle block
    while (date1 <= totalDays1 - 7 && date2 <= totalDays2 - 7 && date3 <= totalDays3 - 7)
    {
        for (int count = 1; count <= 7; ++count)
        {
            if (date1 < 10)
            {
                cout << " " << date1 << " ";
            }
            else
            {
                cout << date1 << " ";
            }
            ++date1;
        }
        cout << " ";

        for (int count = 1; count <= 7; ++count)
        {
            if (date2 < 10)
            {
                cout << " " << date2 << " ";
            }
            else
            {
                cout << date2 << " ";
            }
            ++date2;
        }
        cout << " ";

        for (int count = 1; count < 7; ++count)
        {
            if (date3 < 10)
            {
                cout << " " << date3 << " ";
            }
            else
            {
                cout << date3 << " ";
            }
            ++date3;
        }

        if (date3 < 10)
        {
            cout << " " << date3 << endl;
        }
        else
        {
            cout << date3 << endl;
        }
        ++date3;
    }

    // print last line
    int remainingDays1 = totalDays1 - date1 + 1;
    int remainingDays2 = totalDays2 - date2 + 1;
    int remainingDays3 = totalDays3 - date3 + 1;
    if (remainingDays1 <= 7)
    {
        while (date1 <= totalDays1)
        {
            cout << date1 << " ";
            ++date1;
        }
        for (int j = 7 - remainingDays1; j > 0; j--)
        {
            cout << "   ";
        }
    }
    else
    {
        for (int j = 0; j < 7; j++)
        {
            cout << date1 << " ";
            ++date1;
        }
    }
    cout << " ";

    if (remainingDays2 <= 7)
    {
        while (date2 <= totalDays2)
        {
            cout << date2 << " ";
            ++date2;
        }
        for (int j = 7 - remainingDays2; j > 0; j--)
        {
            cout << "   ";
        }
    }
    else
    {
        for (int j = 0; j < 7; j++)
        {
            cout << date2 << " ";
            ++date2;
        }
    }
    cout << " ";

    if (remainingDays3 <= 7)
    {
        while (date3 <= totalDays3)
        {
            cout << date3 << " ";
            ++date3;
        }
        for (int j = 7 - remainingDays3; j > 1; j--)
        {
            cout << "   ";
        }
        cout << "  " << endl;
    }
    else
    {
        for (int j = 0; j < 6; j++)
        {
            cout << date3 << " ";
            ++date3;
        }
        cout << date3 << endl;
        ++date3;
    }

    // print very last line
    if (remainingDays1 > 7)
    {
        while (date1 <= totalDays1)
        {
            cout << date1 << " ";
            ++date1;
        }
        for (int j = 14 - remainingDays1; j > 0; j--)
        {
            cout << "   ";
        }
        cout << " ";
    }
    else
    {
        cout << "                      ";
    }

    if (remainingDays2 > 7)
    {
        while (date2 <= totalDays2)
        {
            cout << date2 << " ";
            ++date2;
        }
        for (int j = 14 - remainingDays2; j > 0; j--)
        {
            cout << "   ";
        }
        cout << " ";
    }
    else
    {
        cout << "                      ";
    }

    if (remainingDays3 > 7)
    {
        while (date3 <= totalDays3)
        {
            cout << date3 << " ";
            ++date3;
        }
    }
    if(remainingDays1 > 7 || remainingDays2 > 7 || remainingDays3 > 7)
    {
        cout << endl;
    }
}
void printYear(int year)
{
    cout << "Year " << year << endl;

    cout << "January               February              March               " << endl;
    cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;
    printDatesInRow(year, 2);
    cout << endl;

    cout << "April                 May                   June                " << endl;
    cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;
    printDatesInRow(year, 5);
    cout << endl;

    cout << "July                  August                September           " << endl;
    cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;
    printDatesInRow(year, 8);
    cout << endl;

    cout << "October               November              December            " << endl;
    cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;
    printDatesInRow(year, 11);
}

void printConsecutiveMonth(int year, int month)
{
    if (month == 1)
    {
        cout << "December " << year - 1 << "         "
             << "January " << year << "          "
             << "February " << year << endl;
        cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;

    }
    else if (month == 12)
    {
        cout << "November " << year << "         "
             << "December " << year << "         "
             << "January " << year + 1 << endl;
        cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;

    }
    else
    {
        int space1 = 18 - sizeof(monthInWord[month - 2]) / sizeof(char);
        cout << monthInWord[month - 2] << " " << year;
        while (space1)
        {
            cout << " ";
            --space1;
        }

        int space2 = 17 - sizeof(monthInWord[month - 1]) / sizeof(char);
        cout << monthInWord[month - 1] << " " << year;
        while (space2)
        {
            cout << " ";
            --space2;
        }
        cout << monthInWord[month] << " " << year << endl;
        cout << "Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa" << endl;
    }
    printDatesInRow(year, month);
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
        while (followingDays <= days - 7)
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

int main()
{
    int year, month;
    char firstDay, printFormat;

    do
    {
        cout << "Enter year (in range 1800-9999): ";
        cin >> year;
    } while (year < 1800 || year > 9999);

    do
    {
        cout << "Enter month (in range 1-12): ";
        cin >> month;
    } while ((month < 1 && month != -1 && month != -3) || month > 12);

    if (month == -1)
    {
        printYear(year);
    }

    else if (month == -3)
    {
        do
        {
            cout << "Enter the month in the center (in range 1-12): ";
            cin >> month;
        } while (month < 1 || month > 12 || (year == 1800 && month == 1) || (year == 9999 && month == 12));

        printConsecutiveMonth(year, month);
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