#include "tst_testbonustask5.h"

#include "dictionary.h"

TestBonusTask5::TestBonusTask5() {
    dummyWords.insert("spill");
    dummyWords.insert("spell");
    dummyWords.insert("still");
    dummyWords.insert("spilt");
    dummyWords.insert("skill");
    dummyWords.insert("spiel");
    dummyWords.insert("spoil");
    dummyWords.insert("split");
    dummyWords.insert("pills");
    dummyWords.insert("swell");
    dummyWords.insert("comps");
    dummyWords.insert("hatch");
    dummyWords.insert("latch");

    dummyAnswers.insert("shell");
    dummyAnswers.insert("pools");
    dummyAnswers.insert("space");
    dummyAnswers.insert("camps");
    dummyAnswers.insert("watch");
}

void TestBonusTask5::testDummyAnswerGetRandom() {
    Dictionary dummyDict {};
    dummyDict.word_length = 5;
    dummyDict.words = dummyWords;
    for (const QString& word : dummyAnswers) {
        dummyDict.answers.append(Dictionary::WordFrequency{word, 1});
    }
    dummyDict.answerCount = dummyAnswers.count();

    QString randomAnswer = dummyDict.getRandomAnswer();
    QVERIFY(dummyAnswers.find(randomAnswer) != dummyAnswers.end());
}

void TestBonusTask5::testDummyAnswerGetAll() {
    Dictionary dummyDict {};
    dummyDict.word_length = 5;
    dummyDict.words = dummyWords;
    for (const QString& word : dummyAnswers) {
        dummyDict.answers.append(Dictionary::WordFrequency{word, 1});
    }
    dummyDict.answerCount = dummyAnswers.count();

    QSet<QString> allAnswers = dummyDict.getAllAnswers();
    QCOMPARE(allAnswers, dummyAnswers);
}

void TestBonusTask5::testReadAnswerFile_data() {
    QTest::addColumn<int>("wordLength");
    QTest::addColumn<int>("expectedAnswerCount");
    QTest::addColumn<QString>("word");
    QTest::addColumn<unsigned long long>("expectedCount");

    QTest::newRow("4 letters") << 4 << 5848 << "will" << 1356293641ULL;
    QTest::newRow("5 letters") << 5 << 9385 << "music" << 414028837ULL;
    QTest::newRow("9 letters") << 9 << 12246 << "questions" << 156703712ULL;
    QTest::newRow("16 letters") << 16 << 332 << "acknowledgements" << 2605073ULL;
}

void TestBonusTask5::testReadAnswerFile() {
    QFETCH(int, wordLength);
    QFETCH(int, expectedAnswerCount);
    QFETCH(QString, word);
    QFETCH(unsigned long long, expectedCount);

    try {
        Dictionary dict {wordLength};

        QCOMPARE(dict.getAllAnswers().count(), expectedAnswerCount);
        for (const Dictionary::WordFrequency& wf : dict.answers) {
            if (wf.word == word) {
                QCOMPARE(wf.count, expectedCount);
                break;
            }
        }
    }
    catch (const DictionaryException& e) {
        QFAIL("Unexpected dictionary exception occured.");
    }
}

QTEST_MAIN(TestBonusTask5)
