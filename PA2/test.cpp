#include <iostream>
const int NUM_CHARS_PER_LINE = 10;
const int MAX_LINES = 15;
using namespace std;
void convertIntoLines(const char str[], char lines[MAX_LINES][NUM_CHARS_PER_LINE])
{
    int start = 0;
    int end = 0;
    int wordLength = 0;
    int lineLength = NUM_CHARS_PER_LINE;
    int lineCount = 0;
    int charCount = 0;
    while (str[start] != '\0') {
        while(str[end] != ' ' && str[end] != '\0') {
            ++end;
        }
        wordLength = end - start;
        if (wordLength <= lineLength - 1) {
            lineLength -= wordLength;
            while(start < end) {
                lines[lineCount][charCount] = str[start];
                ++start;
                ++charCount;
            }
        }
        else {
            lines[lineCount][charCount] = '\0';
            ++lineCount;
            charCount = 0;
            lineLength = NUM_CHARS_PER_LINE - wordLength;
            while(str[start] == ' '){
                ++start;
            }
            while(start < end) {
                lines[lineCount][charCount] = str[start];
                ++start;
                ++charCount;
            }
        }
        while(str[end] == ' ' || str[end] == ',' || str[end] == '.'){
            ++end;
        }
    }
    if(charCount) {
        lines[lineCount][charCount] = '\0';
        ++lineCount;
    }
    while (lineCount < MAX_LINES){
        lines[lineCount][0] = '\0';
        ++lineCount;
    }
}

int main(){
    char str[] ="Mary had a little lamb, little lamb, little lamb";
    char lines[MAX_LINES][NUM_CHARS_PER_LINE];
    convertIntoLines(str,lines);
    for (int i = 0; i < MAX_LINES;i++) {
        cout << "line" << i << endl;
        for (int j = 0; j < NUM_CHARS_PER_LINE;j++) {
            if (lines[i][j] == '\0') {
                cout << endl;
                break;
            }
            cout << lines[i][j];
        }
    }
    return 0;
}