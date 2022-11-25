#include "tst_testbonustask4.h"

#include "solver.h"
#include "letterbox.h"

double computeEntropy(const QString& word, const QSet<QString>& possibleAnswers);

TestBonusTask4::TestBonusTask4()
{
    dummyAnswers.insert("spill");
    dummyAnswers.insert("spell");
    dummyAnswers.insert("smell");
    dummyAnswers.insert("still");
    dummyAnswers.insert("splat");
    dummyAnswers.insert("skill");
    dummyAnswers.insert("spies");
    dummyAnswers.insert("spoil");
    dummyAnswers.insert("split");
    dummyAnswers.insert("pills");
    dummyAnswers.insert("swell");
    dummyAnswers.insert("hatch");
    dummyAnswers.insert("latch");
    dummyAnswers.insert("colds");
    dummyAnswers.insert("bumpy");
    dummyAnswers.insert("wrath");
    dummyAnswers.insert("feign");
    dummyAnswers.insert("query");
    dummyAnswers.insert("jazzy");
    dummyAnswers.insert("vexed");
    dummyAnswers.insert("merit");
    dummyAnswers.insert("mimic");
    dummyAnswers.insert("posit");
    dummyAnswers.insert("issue");
    dummyAnswers.insert("tomes");
}

void TestBonusTask4::testComputeEntropy_data() {
    QTest::addColumn<QString>("wordSmallEntropy");
    QTest::addColumn<QString>("wordLargeEntropy");

    QTest::newRow("SPELL less entropy than SPOIL") << "spell" << "spoil";
    QTest::newRow("WATCH less entropy than CLOCK") << "watch" << "clock";
    QTest::newRow("ERROR less entropy than CRAFT") << "error" << "craft";
    QTest::newRow("QUIRK less entropy than SHOTS") << "quirk" << "shots";
    QTest::newRow("ANGRY less entropy than HAPPY") << "angry" << "happy";
    QTest::newRow("VAGUE less entropy than SELLS") << "vague" << "sells";
}

void TestBonusTask4::testComputeEntropy() {
    QFETCH(QString, wordSmallEntropy);
    QFETCH(QString, wordLargeEntropy);

    double smallEntropy = computeEntropy(wordSmallEntropy, dummyAnswers);
    double largeEntropy = computeEntropy(wordLargeEntropy, dummyAnswers);

    QVERIFY2(smallEntropy < largeEntropy,
             QString(QString::number(smallEntropy) + " is not smaller than " + QString::number(largeEntropy)).toStdString().c_str());
}

void TestBonusTask4::testSolverHint() {
    Solver solver {dummyAnswers};

    // Uncomment to debug entropy values
//    for (const QString& word : qAsConst(dummyAnswers)) {
//        qDebug() << word << ": " << computeEntropy(word, dummyAnswers);
//    }

    QCOMPARE(solver.hint(), "split");
}

void TestBonusTask4::testSolverHintWithUpdate1() {
    Solver solver {dummyAnswers};

    QColor colors[5] = {GREY, GREY, GREY, GREY, YELLOW};
    solver.updateAnswers("banks", colors);

    QSet<QString> expectedHints {"spell", "split", "pills", "spill", "spoil", "posit"};

    qDebug() << solver.possibleAnswers;

    QCOMPARE(solver.getRemainingNumAnswers(), 9);
    QVERIFY(expectedHints.find(solver.hint()) != expectedHints.end());
}

void TestBonusTask4::testSolverHintWithUpdate2() {
    Solver solver {dummyAnswers};

    QColor colors[5] = {GREEN, GREY, GREEN, GREEN, GREEN};
    solver.updateAnswers("shell", colors);

    qDebug() << solver.possibleAnswers;

    QCOMPARE(solver.getRemainingNumAnswers(), 3);
    QCOMPARE(solver.hint(), "bumpy");
}

void TestBonusTask4::testSolverHintWithUpdate3() {
    Solver solver {dummyAnswers};

    QColor colors[5] = {GREEN, GREEN, GREEN, GREEN, GREEN};
    solver.updateAnswers("query", colors);

    qDebug() << solver.possibleAnswers;

    QCOMPARE(solver.getRemainingNumAnswers(), 1);
    QCOMPARE(solver.hint(), "query");
}

QTEST_MAIN(TestBonusTask4)
