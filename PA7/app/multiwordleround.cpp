#include "multiwordleround.h"

MultiWordleRound::MultiWordleRound(QWidget *parent, const int len, const int numWords):
    WordleRound(parent), numWords(numWords)
{
    this->length = len;
    this->ansMulti = new QString[numWords];
    this->correctGuessesMulti = new bool[numWords];
    dict = Dictionary(length);
    for (int n=0; n<numWords; ++n) {
        this->ansMulti[n] = dict.getRandomAnswer();
        correctGuessesMulti[n] = false;
    }
    initMultiGuesses(parent);
}

MultiWordleRound::~MultiWordleRound() {
    delete [] ansMulti;
    delete [] correctGuessesMulti;
    if (guessesMulti) {
        for (int n=0; n<numWords; ++n) {
            for (int i=0; i<numGuesses; ++i) {
                for (int j=0; j<length; ++j) {
                    SAFE_DELETE(guessesMulti[n][i][j]);
                }
                delete [] guessesMulti[n][i];
            }
            delete [] guessesMulti[n];
        }
        delete [] guessesMulti;
    }
}

int MultiWordleRound::calculateNumGuesses(const int len) {
    return WordleRound::calculateNumGuesses(len) + numWords - 1;
}

void MultiWordleRound::initMultiGuesses(QWidget *parent) {
    numGuesses = MultiWordleRound::calculateNumGuesses(length);
    guessesMulti = new LetterBox***[numWords];
    for (int n=0; n<numWords; ++n) {
        guessesMulti[n] = new LetterBox**[numGuesses];
        for (int i=0; i<numGuesses; ++i) {
            guessesMulti[n][i] = new LetterBox*[length];
            for (int j=0; j<length; ++j) {
                guessesMulti[n][i][j] = new LetterBox(parent);
            }
        }
    }
}

QString MultiWordleRound::getAllAnswers() const {
    QString concat = "";
    for (int n=0; n<numWords; ++n) {
        concat += ansMulti[n];
        if (n != numWords-1) concat += ", ";
    }
    return concat.toUpper();
}

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::registerKey().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 */
void MultiWordleRound::registerKey(const QChar& key) {

}

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::registerDelKey().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 */
void MultiWordleRound::registerDelKey() {

}

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::revertGuess().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 */
void MultiWordleRound::revertGuess() {

}

// To let this .cpp file know these functions are defined in another source file
QColor* analyzeGuess(const QString& guess, const QString& answer);
QString getCurrentGuess(LetterBox** guess, const int len);
bool validGuess(const QString& guess, const Dictionary& dict);

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::registerEnterKey().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 * You can ignore hard mode and solver related code for this task, as we don't have those options for this mode.
 * At the end, call endGuessAttempt() normally, with parameter as whether *all* words have been guessed correctly.
 *
 * Hint: use and maintain the correctGuessesMulti[] array.
 */
void MultiWordleRound::registerEnterKey() {

}

void MultiWordleRound::resetRound() {
    for (int n=0; n<numWords; ++n) {
        ansMulti[n] = dict.getRandomAnswer();
        correctGuessesMulti[n] = false;
        for (int i=0; i<numGuesses; ++i) {
            for (int j=0; j<length; ++j) {
                guessesMulti[n][i][j]->reset();
            }
        }
    }
    emit resetKeyboardColor();
    curGuess = 0;
    curLetter = 0;
    numTries = 0;
}
