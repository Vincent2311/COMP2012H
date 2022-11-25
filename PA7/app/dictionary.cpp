#include "dictionary.h"

#include <QDebug>
#include <QFile>
#include <QDir>

Dictionary::Dictionary(int word_length): word_length(word_length)
{
    QString wordsFilename {":/dictionary/words_{}.txt"};
    wordsFilename.replace("{}", QString::number(word_length));
    QFile wordsFile(wordsFilename);
    if (wordsFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&wordsFile);
        while (!in.atEnd()) {
            QString word = in.readLine();
            words.insert(word);
        }
        if (words.empty()) {
            throw DictionaryException("No valid English words with this length.");
        }
    }
    else {
        throw DictionaryException("Something went wrong reading words.");
    }

    /**
     * TODO: BONUS TASK 5
     *
     * Read the word frequency list from answers_{}.csv and put into 'answers'.
     *
     * Hint: You can reference the code above to read from a file
     * and throw exceptions if file is unavailable or there are no answers.
     */

}

/**
 * TODO: BONUS TASK 5
 *
 * Return a random word from the answer list.
 * The word count should reflect how likely this word would be picked as an answer.
 *
 * Hint: You can generate 2 random 32-bit numbers and combine into a 64-bit random unsigned long long.
 */
QString Dictionary::getRandomAnswer() const {

    // Default behaviour
    int i = rand() % words.size();
    return *std::next(words.begin(), i);
}

/**
 * TODO: BONUS TASK 5
 *
 * Return a word set using 'answers' instead of 'words'
 */
QSet<QString> Dictionary::getAllAnswers() const {

    return words;
}

bool Dictionary::isValidWord(const QString& word) const {
    QSet<QString>::const_iterator it = words.find(word);
    return it != words.end();
}
