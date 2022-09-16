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

void convertStrToPigLatin(char word[]);
void printPigLatin(const char str[]);

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++
// You codes start here. Please do not modify codes above
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

int countCharacters(const char str[])
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        ++count;
    }
    return count;
}

int countWords(const char str[])
{
    int count = 0;
    bool flag = true;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if ((str[i] == ' ' || str[i] == ',' || str[i] == '.'))
        {
            if (flag)
            {
                flag = false;
                ++count;
            }
        }
        else
        {
            flag = true;
        }
    }
    ++count;
    if (!flag)
    {
        --count;
    }
    return count;
}

void checkWord(char str[], int start, const char target[], const char to[], int sizeOfTo, int sizeOfStr)
{
    bool flag = true;
    int i = 0;
    for (; target[i] != '\0'; ++i)
    {
        if (str[start + i] != target[i])
        {
            flag = false;
            break;
        }
    }

    if (flag)
    {
        int index = start;
        char tmp[MAX_STRLEN];

        for (; index + i < sizeOfStr; index++)
        {
            tmp[index - start] = str[index + i];
        }

        int j = 0;
        for (; j < index - start; ++j)
        {
            str[start + sizeOfTo + j] = tmp[j];
        }
        str[start + sizeOfTo + j] = '\0';

        for (int j = 0; j < sizeOfTo; ++j)
        {
            str[start + j] = to[j];
        }
    }
}

void swapString(char str[], const char target[], const char to[])
{
    int sizeOfStr = 0;
    for (int index = 0; str[index] != '\0'; ++index)
    {
        ++sizeOfStr;
    }

    int sizeOfTo = 0;
    for (int index = 0; to[index] != '\0'; ++index)
    {
        ++sizeOfTo;
    }

    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == target[0])
        {
            checkWord(str, i, target, to, sizeOfTo, sizeOfStr);
        }
    }
}

void encryptText(char str[], int shift)
{
    int number;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if ('a' <= str[i] && str[i] <= 'z')
        {
            number = str[i] + shift - 'a';
            while (number < 0)
            {
                number += 26;
            }
            str[i] = static_cast<char>(number % 26 + 'a');
        }
        else if ('A' <= str[i] && str[i] <= 'Z')
        {
            number = str[i] + shift - 'A';
            while (number < 0)
            {
                number += 26;
            }
            str[i] = static_cast<char>(number % 26 + 'A');
        }
        else
        {
            continue;
        }
    }
}

int countNumOccurences(const char str[], const char target[])
{
    int count = 0;
    bool flag = true;
    int index = 0;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == target[0])
        {
            for (index = 0; target[index] != '\0'; ++index)
            {
                if (str[i + index] != target[index])
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                ++count;
                i += index - 1;
            }
        }
        flag = true;
    }
    return count;
}

void convertIntoLines(const char str[], char lines[MAX_LINES][NUM_CHARS_PER_LINE])
{
    int start = 0;
    int end = 0;
    int wordLength = 0;
    int lineLength = NUM_CHARS_PER_LINE;
    int lineCount = 0;
    int charCount = 0;
    while (str[start] != '\0')
    {
        while (str[end] != ' ' && str[end] != '\0')
        {
            ++end;
        }
        wordLength = end - start;
        if (wordLength <= lineLength - 1)
        {
            lineLength -= wordLength;
            while (start < end)
            {
                lines[lineCount][charCount] = str[start];
                ++start;
                ++charCount;
            }
        }
        else
        {
            lines[lineCount][charCount] = '\0';
            ++lineCount;
            charCount = 0;
            lineLength = NUM_CHARS_PER_LINE - wordLength;
            while (str[start] == ' ')
            {
                ++start;
            }
            while (start < end)
            {
                lines[lineCount][charCount] = str[start];
                ++start;
                ++charCount;
            }
        }
        while (str[end] == ' ' || str[end] == ',' || str[end] == '.')
        {
            ++end;
        }
    }
    if (charCount)
    {
        lines[lineCount][charCount] = '\0';
        ++lineCount;
    }
    while (lineCount < MAX_LINES)
    {
        lines[lineCount][0] = '\0';
        ++lineCount;
    }
}

void printLeftJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);
    for (int i = 0; i < MAX_LINES; i++)
    {
        if (lines[i][0] == '\0')
        {
            break;
        }
        for (int j = 0; j < NUM_CHARS_PER_LINE; j++)
        {
            if (lines[i][j] == '\0')
            {
                cout << endl;
                break;
            }
            cout << lines[i][j];
        }
    }
}

void printRightJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);
    int count = 0;
    for (int i = 0; i < MAX_LINES; i++)
    {
        if (lines[i][0] == '\0')
        {
            break;
        }
        for (count = 0; lines[i][count] != '\0'; ++count)
        {
            ;
        }
        while (count < NUM_CHARS_PER_LINE - 1)
        {
            cout << " ";
            ++count;
        }
        for (int j = 0; j < NUM_CHARS_PER_LINE; j++)
        {
            if (lines[i][j] == '\0')
            {
                cout << endl;
                break;
            }
            cout << lines[i][j];
        }
    }
}

void printJustified(const char str[])
{
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str, lines);
    for (int i = 0; i < MAX_LINES; i++)
    {
        if (lines[i + 1][0] == '\0')
        {
            for (int j = 0; lines[i][j] != '\0'; j++)
            {
                cout << lines[i][j];
            }
            cout << endl;
            break;
        }
        int numOfWord = countWords(lines[i]);
        int numOfChar = countCharacters(lines[i]);
        int spaceInRow = (NUM_CHARS_PER_LINE - 1 - numOfChar) / (numOfWord - 1);
        int extraSpace = (NUM_CHARS_PER_LINE - 1 - numOfChar) % (numOfWord - 1);
        bool flag = true;
        int wordCount = 0;
        for (int j = 0; j < NUM_CHARS_PER_LINE; j++)
        {
            if (lines[i][j] == '\0')
            {
                cout << endl;
                break;
            }
            if (lines[i][j] == ' ' && flag)
            {
                ++wordCount;
                if (wordCount <= extraSpace)
                {
                    cout << " ";
                }
                for (int index = 0; index < spaceInRow; ++index)
                {
                    cout << " ";
                }
                flag = false;
            }
            if (lines[i][j] != ' ')
            {
                flag = true;
            }
            cout << lines[i][j];
        }
    }
}

void convertStrToPigLatin(char str[])
{
    int start = 0;
    int end = 0;
    int temp[MAX_STRLEN];
    bool isVowel = false;
    int count = 0;
    while (str[start] != '\0')
    {
        isVowel = false;
        while ((str[end] <= 'z' && str[end] >= 'a') || (str[end] <= 'Z' && str[end] >= 'A') || str[end] == '-')
        {
            ++end;
        }
        if (str[start] == 'a' || str[start] == 'A' || str[start] == 'e' || str[start] == 'E' || str[start] == 'i' || str[start] == 'I' || str[start] == 'o' || str[start] == 'O' || str[start] == 'U' || str[start] == 'u')
        {
            isVowel = true;
        }
        if (isVowel)
        {
            while (start < end)
            {
                temp[count++] = str[start++];
            }
            temp[count++] = 'y';
            temp[count++] = 'a';
            temp[count++] = 'y';
        }
        else
        {
            int firstVowel = start + 1;
            while (str[firstVowel] != 'a' && str[firstVowel] != 'e' && str[firstVowel] != 'i' && str[firstVowel] != 'o' && str[firstVowel] != 'u')
            {
                ++firstVowel;
            }
            for (int i = firstVowel; i != end; i++)
            {
                temp[count++] = str[i];
            }
            while (start != firstVowel)
            {
                temp[count++] = str[start++];
            }
            temp[count++] = 'a';
            temp[count++] = 'y';
            while (start != end)
            {
                ++start;
            }
        }
        while (str[start] == ',' || str[start] == '.' || str[end] == ' ')
        {
            temp[count++] = str[start++];
            ++end;
        }
    }
    temp[count] = '\0';
    int index = 0;
    for (; temp[index] != '\0'; ++index)
    {
        str[index] = temp[index];
    }
    str[index] = '\0';
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