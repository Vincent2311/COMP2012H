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
    QString answersFilename {":/dictionary/answers_{}.csv"};
    answersFilename.replace("{}", QString::number(word_length));
    QFile answersFile(answersFilename);
    if (answersFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&answersFile);
        QString header = in.readLine();
        if (header.split(',')[0] != "word" || header.split(',')[1] != "count") {
            throw DictionaryException("Invalid answer .csv format.");
        }
        while (!in.atEnd()) {
            QString wordCount = in.readLine();
            WordFrequency wf {wordCount.split(',')[0], wordCount.split(',')[1].toULong()};
            answers.append(wf);
            answerCount += wf.count;
        }
        if (answers.empty()) {
            throw DictionaryException("No valid English answers with this length.");
        }
    }
    else {
        throw DictionaryException("Something went wrong reading answers.");
    }
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
    unsigned long long lrand = (static_cast<unsigned long long>(rand()) << (sizeof(int) * 8)) | rand();
    unsigned long long randInd = lrand % answerCount;
    for (const WordFrequency& wf : answers) {
        if (randInd < wf.count) {
            return wf.word;
        }
        else {
            randInd -= wf.count;
        }
    }

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
    QSet<QString> answersStr {};
    for (const WordFrequency& wf : answers) {
        answersStr.insert(wf.word);
    }
    return answersStr;

    // return words;
}

bool Dictionary::isValidWord(const QString& word) const {
    QSet<QString>::const_iterator it = words.find(word);
    return it != words.end();
}
