#include <iostream>
using namespace std;

int get_days_in_month(int month, int year) {
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29;
        } else {
            return 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

// Calculate how many days between the first day of given month,
// and Jan 1, 1800
int get_days_from_1800(int month, int year) {
    int days_from_1800 = 0;
    for (int i = 1800; i < year; ++i) {
        // check leap years
        if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) {
            days_from_1800 += 366;
        } else {
            days_from_1800 += 365;
        }
    }

    for (int i = 1; i < month; ++i) {
        days_from_1800 += get_days_in_month(i, year);
    }

    return days_from_1800;
}

// print the full name of month
void print_month_name(int month) {
    switch (month) {
        case 1: cout << "January "; break;
        case 2: cout << "February "; break;
        case 3: cout << "March "; break;
        case 4: cout << "April "; break;
        case 5: cout << "May "; break;
        case 6: cout << "June "; break;
        case 7: cout << "July "; break;
        case 8: cout << "August "; break;
        case 9: cout << "September "; break;
        case 10: cout << "October "; break;
        case 11: cout << "November "; break;
        case 12: cout << "December "; break;
        default: break;
    }
}

// get the length of full name of a month
// this is used to calculate how many spaces to print
int get_month_name_length(int month) {
    switch (month) {
        case 1: 
        case 10: 
            return 7;
        case 2: 
        case 11:
        case 12: 
            return 8;
        case 3: 
        case 4: 
            return 5;
        case 5: 
            return 3;
        case 6: 
        case 7: 
            return 4;
        case 8: 
            return 6;
        case 9: 
            return 9;
        default: 
            return -1;
    }
}

int main() {
    int year, month;
    char week_start;    // week starts on Sunday of Monday
    // keep reading inputs until user input a valid year and month
    do {
        cout << "Enter year (in range 1800-9999): ";
        cin >> year;
    } while (year < 1800 || year > 9999);

    do {
        cout << "Enter month (in range 1-12): ";
        cin >> month;
    } while ((month < 1 || month > 12) && (month != -1) && (month != -3));


    if (month == -1) {
        // print the calendar of whole year
        cout << "Year " << year << endl;

        // four rows, each row has three months
        for (int row = 0; row < 4; ++row) {
            // print title
            for (int i = 1; i < 4; ++i) {
                print_month_name(i + row * 3);
                // print spaces
                for (int j = 0; j < 21 - get_month_name_length(i + row * 3); ++j) {
                    cout << " ";
                }
            }
            cout << endl;

            // print weekdays
            for (int i = 1; i < 4; ++i) {
                cout << "Su Mo Tu We Th Fr Sa  ";
            }
            cout << endl;

            // 1. calculate which day is the first day of the three months
            // days_month: how many days in each month
            // first_day: what day is the first day of this month
            int days_month[3], first_day[3];
            for (int mon = 0; mon < 3; ++mon) {
                days_month[mon] = get_days_in_month(mon + 1 + row * 3, year);
                first_day[mon] = (get_days_from_1800(mon + 1 + row * 3, year) + 3) % 7;
            }

            // 2. this three variables record which date we're going to print for each month
            int dates_month[3];
            // start date at (1 - week_month), try if you can observe this pattern
            for (int mon = 0; mon < 3; ++mon) {
                dates_month[mon] = 1 - first_day[mon];
            }
            
            // 3. print the calendar
            while (dates_month[0] <= days_month[0] || dates_month[1] <= days_month[1] || dates_month[2] <= days_month[2]) {
                for (int mon = 0; mon < 3; ++mon) {
                    // print each month
                    while (true) {
                        if (dates_month[mon] < 1 || dates_month[mon] > days_month[mon]) {
                            cout << "   ";
                        } else if (dates_month[mon] < 10) {
                            cout << " " << dates_month[mon] << " ";
                        } else {
                            cout << dates_month[mon] << " ";
                        }
                        ++dates_month[mon];
                        if ((dates_month[mon] + first_day[mon] - 1) % 7 == 0) {
                            break;
                        }
                    }
                    // print space for first two month, new line for last month
                    if (mon == 2) cout << endl;
                    else cout << " ";
                }
            }

            cout << endl;
        } 
    } else if (month == -3) {
        // print the adjacent 3 months

        int years[3], months[3];

        years[1] = year;
        do {
            cout << "Enter the month in the center (in range 1-12): ";
            cin >> months[1];
        } while (months[1] < 1 || months[1] > 12 || 
                 (months[1] == 1 && years[1] == 1800) || 
                 (months[1] == 12 && years[1] == 9999));

        months[0] = months[1] - 1, years[0] = years[1];
        if (months[0] < 1) {
            months[0] = 12;
            --years[0];
        }
        months[2] = months[1] + 1, years[2] = years[1];
        if (months[2] > 12) {
            months[2] = 1;
            ++years[2];
        }

        // print title
        for (int idx = 0; idx < 3; ++idx) {
            print_month_name(months[idx]);
            cout << years[idx];
            for (int j = 0; j < 17 - get_month_name_length(months[idx]); ++j) {
                cout << " ";
            }
        }
        
        cout << endl;

        // print weekdays
        for (int i = 1; i < 4; ++i) {
            cout << "Su Mo Tu We Th Fr Sa  ";
        }
        cout << endl;

        // 1. calculate which day is the first day of the three months
        // days_month: how many days in each month
        // first_day: what day is the first day of this month
        int days_month[3], first_day[3];
        for (int idx = 0; idx < 3; ++idx) {
            days_month[idx] = get_days_in_month(months[idx], years[idx]);
            first_day[idx] = (get_days_from_1800(months[idx], years[idx]) + 3) % 7;
        }

        // 2. this three variables record which date we're going to print for each month
        int dates_month[3];
        // start date at (1 - week_month), try if you can observe this pattern
        for (int idx = 0; idx < 3; ++idx) {
            dates_month[idx] = 1 - first_day[idx];
        }
        
        // 3. print the calendar
        while (dates_month[0] <= days_month[0] || dates_month[1] <= days_month[1] || dates_month[2] <= days_month[2]) {
            for (int mon = 0; mon < 3; ++mon) {
                // print each month
                while (true) {
                    if (dates_month[mon] < 1 || dates_month[mon] > days_month[mon]) {
                        cout << "   ";
                    } else if (dates_month[mon] < 10) {
                        cout << " " << dates_month[mon] << " ";
                    } else {
                        cout << dates_month[mon] << " ";
                    }
                    ++dates_month[mon];
                    if ((dates_month[mon] + first_day[mon] - 1) % 7 == 0) {
                        break;
                    }
                }
                // print space for first two month, new line for last month
                if (mon == 2) cout << endl;
                else cout << " ";
            }
        }
    } else {
        // print a certain month

        do {
            cout << "Enter the first day of week ('s' for Sunday, 'm' for Monday): ";
            cin >> week_start;
        } while (week_start != 's' && week_start != 'm');

        // calculate how many days from Jan 1, 1800 to the first day of this month
        int days_from_1800 = get_days_from_1800(month, year);

        // calculate what day is the first day of inputted month, year
        int week = (days_from_1800 + 3) % 7;  // +3 because Jan 1, 1800 is a Wed
        // if week starts from Monday, then we need to subtract 1 from week
        // e.g. if week starts from Sunday, then Monday has index 1,
        //      if week starts from Monday, then Monday has index 0
        if (week_start == 'm') {
            week = (week - 1 + 7) % 7;  // +7 because we want to convert -1 to 6, i.e., when week = 0
        }


        // keep asking user to print horizontally or vertically until a valid input
        char print_type;
        do {
            cout << "Print horizontally (h) or vertically (v): ";
            cin >> print_type;
        } while (print_type != 'h' && print_type != 'v');

        // 1. print the month and year
        print_month_name(month);
        cout << year << endl;


        // 2. print the calendar of certain month
        int days_this_month = get_days_in_month(month, year);
        if (print_type == 'h') {
            // print horizontally

            // title
            if (week_start == 's') {
                cout << "Su Mo Tu We Th Fr Sa" << endl;
            } else {
                cout << "Mo Tu We Th Fr Sa Su" << endl;
            }
                    
            // each position has width of 3 spaces, fill the spaces before
            for (int i = 0; i < week; ++i) {
                cout << "   ";
            }
            
            for (int i = 1; i <= days_this_month; i++) {
                // if i has only one char, we need to align it with an extra space before
                if (i < 10) cout << " ";

                cout << i << " ";

                // after Saturday, start a new line
                if ((i + week) % 7 == 0) {
                    cout << endl;
                }
            }
            cout << endl;
        } else {
            // print vertically
            int delta = (week_start == 's' ? 1 : 0);
            for (int i = 1; i <= 7; i++) {      // row i
                switch (i - delta) {            // starts on Monday, first print case 1; on Sunday, first print case 0
                    case 1: cout << "Mo "; break;
                    case 2: cout << "Tu "; break;
                    case 3: cout << "We "; break;
                    case 4: cout << "Th "; break;
                    case 5: cout << "Fr "; break;
                    case 6: cout << "Sa "; break;
                    case 7: 
                    case 0:
                        cout << "Su "; break;
                    default: break;
                }
                /**
                 * Here we use a trick: each row start at (i - week), for example,
                 *
                 * Start from Sun, week = 3             Start from Mon, week = 2   
                 * January 1800                         January 1800    
                 * Su -2  5 12 19 26                    Mo -1  6 13 20 27
                 * Mo -1  6 13 20 27                    Tu  0  7 14 21 28
                 * Tu  0  7 14 21 28                    We  1  8 15 22 29
                 * We  1  8 15 22 29                    Th  2  9 16 23 30
                 * Th  2  9 16 23 30                    Fr  3 10 17 24 31
                 * Fr  3 10 17 24 31                    Sa  4 11 18 25   
                 * Sa  4 11 18 25                       Su  5 12 19 26    
                 * 
                 * When printing, ignore <= 0 terms
                 */
                for (int j = i - week; j <= days_this_month; j += 7) {
                    if (j < 1) {
                        cout << "   ";
                    } else {
                        if (j < 10) cout << " ";
                        cout << j << " ";
                    }
                }
                cout << endl;
            }
        }
    }

    return 0;
}
