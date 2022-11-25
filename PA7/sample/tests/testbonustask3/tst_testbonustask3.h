#ifndef TST_TESTBONUSTASK3_H
#define TST_TESTBONUSTASK3_H

#include <QtTest>

class TestBonusTask3 : public QObject
{
    Q_OBJECT

public:
    TestBonusTask3() = default;
    ~TestBonusTask3() = default;

public slots:
    void confirmPopupReset();

private slots:
    void testTimedRoundWindowBasic();
    void testTimedRoundWindowInvalidWord();
    void testTimedRoundWindowInvalidHardWord();
    void testTimedRoundWindowRoundWin();
    void testTimedRoundWindowRoundLose();
    void testTimedRoundWindowSkipWord();
    void testTimedRoundWindowResetGame();

    void testTimedMainWindow1Min();
    void testTimedMainWindow3Min();
    void testTimedMainWindow10Min();
};

#endif // TST_TESTBONUSTASK3_H
