#ifndef TESTBONUSTASK5_H
#define TESTBONUSTASK5_H

#include <QtTest>

class TestBonusTask5 : public QObject
{
    Q_OBJECT

private:
    QSet<QString> dummyWords {};
    QSet<QString> dummyAnswers {};

public:
    TestBonusTask5();
    ~TestBonusTask5() = default;

private slots:
    void testDummyAnswerGetRandom();
    void testDummyAnswerGetAll();

    void testReadAnswerFile_data();
    void testReadAnswerFile();
};

#endif // TESTBONUSTASK5_H
