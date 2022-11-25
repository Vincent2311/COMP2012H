#include "absurdwordleround.h"

#include <QList>

AbsurdWordleRound::AbsurdWordleRound(QWidget *parent, const int len):
    WordleRound(parent, len, false)
{
    initGuesses(parent);
    solver.setEnabled(false);
}

// Add extra guesses for this mode
int AbsurdWordleRound::calculateNumGuesses(const int len) {
    return WordleRound::calculateNumGuesses(len) + 5;
}


/**
 * TODO: BONUS TASK 1
 *
 * @brief Change the 'answer' by one letter, so that it still belongs to 'allAnswers'.
 * @param answer: original answer word
 * @param allAnswers: all possible answers, new answer must come from this set
 * @return a word from 'allAnswers' that differs from 'answer' by (at most) one letter.
 *
 * Create a list of words considered "adjacent" to the current answer:
 * words that differ by the answer by exactly one letter.
 *
 * For each possible answer, loop over each letter
 * and count the number of times the letter is different
 * for the current answer and possible answer.
 * If this value is 1, add the possible answer to the "adjacent" list.
 *
 * Finally, select a random word from the "adjacent" list to be the new answer.
 * If this list is empty, return the original answer.
 */
QString alterAnswer(const QString& answer, const QSet<QString>& allAnswers) {
    // Create list of "adjacent" words
    QList<QString> adjacentWords {};
    // Loop over current dictionary and add "adjacent" words to list
    for (const QString& word : allAnswers) {
        int numSameLetters = 0;
        for (int i=0; i<word.size(); ++i) {
            if (word[i] == answer[i]) {
                ++numSameLetters;
            }
        }
        if (numSameLetters == word.size() - 1) {
            adjacentWords.append(word);
        }
    }
    // If no adjacent words, do nothing
    // Otherwise, pick a random new answer from list of adjacent words
    if (adjacentWords.size() > 0) {
        return adjacentWords.at(rand() % adjacentWords.size());
    }
    return answer;
}


void AbsurdWordleRound::endGuessAttempt(const bool correctGuess) {
    // Perform endGuess tasks like normal
    WordleRound::endGuessAttempt(correctGuess);

    // If incorrect guess and game has not ended, change the answer by one letter
    if (!correctGuess && curGuess != numGuesses) {
        answer = alterAnswer(answer, dict.getAllAnswers());
    }
}
