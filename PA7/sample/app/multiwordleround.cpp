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
    if (curLetter == length) return;        // Already typed all letters for this guess
    if (curGuess == numGuesses) return;     // Out of guesses
    for (int n=0; n<numWords; ++n) {
        if (correctGuessesMulti[n]) continue; // This word has been guessed, skip
        guessesMulti[n][curGuess][curLetter]->setLetter(key);
    }
    ++curLetter;
}

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::registerDelKey().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 */
void MultiWordleRound::registerDelKey() {
    if (curLetter == 0) return;             // Nothing to delete
    if (curGuess == numGuesses) return;     // Out of guesses
    --curLetter;
    for (int n=0; n<numWords; ++n) {
        if (correctGuessesMulti[n]) continue; // This word has been guessed, skip
        guessesMulti[n][curGuess][curLetter]->setLetter(' ');
    }
}

/**
 * TODO: BONUS TASK 2
 *
 * Implement based on WordleRound::revertGuess().
 * Note that we don't update QGridLayouts where the answers have been guessed.
 */
void MultiWordleRound::revertGuess() {
    for (int n=0; n<numWords; ++n) {
        for (int i=0; i<length; ++i) {
            if (correctGuessesMulti[n]) continue; // This word has been guessed, skip
            guessesMulti[n][curGuess][i]->setLetter(' ');
        }
    }
    curLetter = 0;
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
    if (curLetter != length) return;        // Have not typed all letters
    if (curGuess == numGuesses) return;     // Out of guesses

    // Convert current guess to QString
    LetterBox** curGuessBox = nullptr;
    for (int n=0; n<numWords; ++n) {
        if (correctGuessesMulti[n]) continue; // This word has been guessed, skip
        curGuessBox = guessesMulti[n][curGuess];
    }
    if (!curGuessBox) return;   // should not happen
    QString guess = getCurrentGuess(curGuessBox, length);

    // Check if guess is in dictionary
    if (!validGuess(guess, dict)) {
        revertGuess();
        emit invalidWord();
        return;
    }

    // Color the boxes according to result
    for (int n=0; n<numWords; ++n) {
        if (correctGuessesMulti[n]) continue; // This word has been guessed, skip

        bool correctGuess = true;
        QColor* colors = analyzeGuess(guess, ansMulti[n]);
        for (int i=0; i<length; ++i) {
            correctGuess &= (colors[i] == GREEN);
            guessesMulti[n][curGuess][i]->setColor(colors[i]);
            emit updateKeyboardColor(guessesMulti[n][curGuess][i]->getLetter(), colors[i]);
        }
        delete [] colors;
        correctGuessesMulti[n] = correctGuess;
    }

    // Finish guess attempt
    bool win = true;
    for (int n=0; n<numWords; ++n) {
        win &= correctGuessesMulti[n];
    }
    endGuessAttempt(win);
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
