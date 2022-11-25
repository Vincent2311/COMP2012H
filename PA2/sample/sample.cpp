#include <iostream>
#include <climits>
#include <cstdio>

using namespace std;

const int MAX_STRLEN = 1000;
const int NUM_CHARS_PER_LINE = 50;
const int MAX_LINES = 15;

int countCharacters(const char str[]);
int countWords(const char str[]);
int countNumOccurences(const char str[], const char target[]);

void swapString(char str[], const char target[], const char to[]);
void encryptText(char str[], int shift);

void convertIntoLines(const char str[], char lines[MAX_LINES][NUM_CHARS_PER_LINE]);
void printRightJustified(const char str[]);
void printLeftJustified(const char str[]);
void printJustified(const char str[]);

void convertStrToPigLatin(char word[]);
void printPigLatin(const char str[]);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
// You codes start here. Please do not modify codes above
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

int countCharacters(const char str[])
{
    int count = -1;
    while (str[++count] != '\0')
        ;
    return count;
}

bool isWordChar(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '-');
}

int countWords(const char str[])
{
    int count = 0;
    bool word = false;
    for (int i = countCharacters(str) - 1; i >= 0; --i)
    {
        if (word && !isWordChar(str[i]))
        {
            ++count;
            word = false;
        }
        else if (!word && isWordChar(str[i]))
        {
            word = true;
        }
    }
    count += word;
    return count;
}

void swapString(char str[], const char target[], const char to[])
{
    int strLen = countCharacters(str);
    int tgtLen = countCharacters(target);
    int toLen = countCharacters(to);
    int tail = 0;
    for (int i = strLen - 1; i >= 0; --i)
    {
        str[MAX_STRLEN - strLen + i] = str[i];
    }
    int i = MAX_STRLEN - strLen;
    while (i < MAX_STRLEN)
    {
        bool match = true;
        for (int j = 0; j < tgtLen; ++j)
        {
            if (i + j >= MAX_STRLEN || target[j] != str[i + j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            for (int j = 0; j < toLen; ++j)
            {
                str[tail++] = to[j];
            }
            i += tgtLen;
        }
        else
        {
            str[tail++] = str[i++];
        }
    }
    str[tail] = '\0';
}

void encryptText(char str[], int shift)
{
    if (shift < 0)
    {
        shift = shift % 26 + 26;
    }
    shift = shift % 26;
    for (int i = 0; i < countCharacters(str); ++i)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = ((str[i] + shift - 'a') % 26) + 'a';
        }
        else if (str[i] >= 'A' && str[i] <= 'Z')
        {
            str[i] = ((str[i] + shift - 'A') % 26) + 'A';
        }
    }
}

int countNumOccurences(const char str[], const char target[])
{
    int count = 0;
    int strLen = countCharacters(str);
    int tgtLen = countCharacters(target);
    int i = 0;
    while (i < strLen - tgtLen + 1)
    {
        bool match = true;
        for (int j = 0; j < tgtLen; ++j)
        {
            if (target[j] != str[i + j])
            {
                match = false;
                break;
            }
        }
        count += match;
        i += (match) ? tgtLen : 1;
    }
    return count;
}

void convertIntoLines(const char str[], char lines[MAX_LINES][NUM_CHARS_PER_LINE])
{
    for (int i = 0; i < MAX_LINES; ++i)
    {
        lines[i][0] = '\0';
    }

    int strLen = countCharacters(str);
    int line = 0;
    int tail = 0;
    int i = 0;

    while (i < strLen)
    {
        if (isWordChar(str[i]))
        {
            int wordLen = 1;
            while (i + wordLen < strLen && isWordChar(str[i + wordLen]))
            {
                ++wordLen;
            }

            if (wordLen + tail < NUM_CHARS_PER_LINE)
            {
                for (int j = 0; j < wordLen; ++j)
                {
                    lines[line][tail++] = str[i + j];
                }
                i += wordLen;
            }
            else
            {
                lines[line++][tail] = '\0';
                tail = 0;
            }
        }
        else
        {
            if (tail == 0 && str[i] == ' ')
            {
                i++;
                continue;
            }
            lines[line][tail++] = str[i++];
        }
        if (tail == NUM_CHARS_PER_LINE - 1)
        {
            lines[line++][tail] = '\0';
            tail = 0;
        }
    }
    lines[line][tail] = '\0';

    for (int line = 0; line < MAX_LINES && lines[line][0] != '\0'; ++line)
    {
        int last = 0;
        for (int i = 0; lines[line][i] != '\0'; ++i)
        {
            last = (lines[line][i] != ' ') ? i : last;
        }
        lines[line][last + 1] = '\0';
    }
}

void printLeftJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);

    for (int i = 0; i < MAX_LINES && lines[i][0] != '\0'; ++i)
    {
        cout << lines[i] << endl;
    }
}

void printRightJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);

    for (int line = 0; line < MAX_LINES && lines[line][0] != '\0'; ++line)
    {
        for (int i = NUM_CHARS_PER_LINE - countCharacters(lines[line]) - 1; i > 0; --i)
        {
            cout << ' ';
        }
        cout << lines[line] << endl;
    }
}

void printJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);

    int nLines = 0;
    while (nLines < MAX_LINES && lines[nLines][0] != '\0')
    {
        ++nLines;
    }

    for (int line = 0; line < nLines - 1; ++line)
    {
        int len = countCharacters(lines[line]);
        int extra = NUM_CHARS_PER_LINE - len - 1;
        int nSpaces = 0;
        bool space = false;
        for (int i = 0; i < len; ++i)
        {
            if (!space && lines[line][i] == ' ')
            {
                space = true;
            }
            else if (space && lines[line][i] != ' ')
            {
                space = false;
                ++nSpaces;
            }
        }
        nSpaces += space;

        int cur = 0;
        space = false;
        for (int i = 0; i < len; ++i)
        {
            if (lines[line][i] == ' ' && !space)
            {
                int j = (extra / nSpaces) + ((cur < extra % nSpaces) ? 1 : 0);
                for (; j > 0; --j)
                {
                    cout << ' ';
                }
                cout << ' ';
                space = true;
                ++cur;
            }
            else
            {
                if (lines[line][i] != ' ' && space)
                {
                    space = false;
                }
                cout << lines[line][i];
            }
        }
        cout << endl;
    }

    if (nLines > 0)
    {
        cout << lines[nLines - 1] << endl;
    }
}

bool isVowel(char c)
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

void convertStrToPigLatin(char str[])
{
    char copy[MAX_STRLEN];
    int strLen = countCharacters(str);
    int tail = 0;
    int i = 0;
    for (; i <= strLen; ++i)
    {
        copy[i] = str[i];
    }
    i = 0;
    while (i < strLen)
    {
        if (!isWordChar(copy[i]))
        {
            str[tail++] = copy[i++];
        }
        else
        {
            int wordLen = 1;
            while (i + wordLen < MAX_STRLEN && isWordChar(copy[i + wordLen]))
            {
                ++wordLen;
            }

            if (isVowel(copy[i]))
            {
                for (int j = 0; j < wordLen; ++j)
                {
                    str[tail++] = copy[i + j];
                }
                str[tail++] = 'y';
                str[tail++] = 'a';
                str[tail++] = 'y';
            }
            else
            {
                int start = 1;
                for (; start < wordLen; ++start)
                {
                    if (isVowel(copy[i + start]))
                    {
                        break;
                    }
                }
                for (int j = start; j < wordLen; ++j)
                {
                    str[tail++] = copy[i + j];
                }
                for (int j = 0; j < start; ++j)
                {
                    str[tail++] = copy[i + j];
                }
                str[tail++] = 'a';
                str[tail++] = 'y';
            }
            i += wordLen;
        }
    }
    str[tail] = '\0';
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++
// You codes end here. Please do not modify codes below
// ++++++++++++++++++++++++++++++++++++++++++++++++++++

void printPigLatin(const char str[])
{
    char pigLatin[MAX_STRLEN];
    for (int i = 0; i < MAX_STRLEN; ++i)
    {
        pigLatin[i] = str[i];
    }
    convertStrToPigLatin(pigLatin);
    int strLen = countCharacters(pigLatin);
    for (int i = 0; i < strLen; ++i)
    {
        if (pigLatin[i] >= 'A' && pigLatin[i] <= 'Z')
        {
            pigLatin[i] = 'a' + pigLatin[i] - 'A';
        }
    }
    cout << pigLatin << endl;
}

void printMainDisplay(const char str[])
{
    cout << str << endl
         << "===================================\n"
         << "1. Character Count\n"
         << "2. Word Count\n"
         << "3. Encrypt (Caeser Cipher)\n"
         << "4. Find and Replace String Globally\n"
         << "5. Find Frequency of String\n"
         << "6. Print Left Align Text\n"
         << "7. Print Right Align Text\n"
         << "8. Print Left-Right Align Text\n"
         << "9. Print Pig Latin\n"
         << "Please make your choice: ";
}

int main()
{
    int choice = 0;
    char str[MAX_STRLEN];

    cout << "Enter text: ";
    cin.getline(str, MAX_STRLEN, '\n');

    do
    {
        printMainDisplay(str);
        cin >> choice;
    } while (choice < 1 || choice > 9);

    if (choice == 1)
    {
        cout << "Total character count: " << countCharacters(str) << endl;
    }
    else if (choice == 2)
    {
        cout << "Total word count: " << countWords(str) << endl;
    }
    else if (choice == 3)
    {
        cout << "Enter the shift amount for the caeser cipher encryption (rotation encryption): ";
        int shift;
        cin >> shift;
        encryptText(str, shift);
        cout << str << endl;
    }
    else if (choice == 4)
    {
        cout << "Enter the string to find and replace: ";
        char target[MAX_STRLEN];
        cin.ignore(INT_MAX, '\n');
        cin.getline(target, MAX_STRLEN, '\n');

        cout << "Enter the string to replace it with: ";
        char to[MAX_STRLEN];
        cin.getline(to, MAX_STRLEN, '\n');

        swapString(str, target, to);
        cout << str << endl;
    }
    else if (choice == 5)
    {
        cout << "Enter the string to count the occurences of: ";
        char target[MAX_STRLEN];
        cin.ignore(INT_MAX, '\n');
        cin.getline(target, MAX_STRLEN, '\n');

        cout << '"' << target << '"' << " appears in the text " << countNumOccurences(str, target) << " time(s).\n";
    }
    else if (choice == 6)
    {
        printLeftJustified(str);
    }
    else if (choice == 7)
    {
        printRightJustified(str);
    }
    else if (choice == 8)
    {
        printJustified(str);
    }
    else if (choice == 9)
    {
        printPigLatin(str);
    }
}
