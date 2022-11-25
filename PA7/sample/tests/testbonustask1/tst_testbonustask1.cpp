#include "tst_testbonustask1.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "absurdroundwindow.h"
#include "ui_roundwindow.h"

#include <QComboBox>
#include <QSpinBox>

QString alterAnswer(const QString& answer, const QSet<QString>& allAnswers);

TestBonusTask1::TestBonusTask1() {
    dummyAnswers.insert("spill");
    dummyAnswers.insert("spell");
    dummyAnswers.insert("still");
    dummyAnswers.insert("spilt");
    dummyAnswers.insert("skill");
    dummyAnswers.insert("spiel");
    dummyAnswers.insert("spoil");
    dummyAnswers.insert("split");
    dummyAnswers.insert("pills");
    dummyAnswers.insert("swell");
    dummyAnswers.insert("comps");
    dummyAnswers.insert("hatch");
    dummyAnswers.insert("latch");
}

void TestBonusTask1::testAlterAnswerFunction_data() {
    QTest::addColumn<QString>("answer");
    QTest::addColumn<QSet<QString>>("possibleAlters");

    QSet<QString> spillAdj {};
    spillAdj.insert("spell");
    spillAdj.insert("still");
    spillAdj.insert("spilt");
    spillAdj.insert("skill");
    spillAdj.insert("spiel");
    QTest::newRow("SPILL") << "spill" << spillAdj;

    QSet<QString> spellAdj {};
    spellAdj.insert("spill");
    spellAdj.insert("swell");
    QTest::newRow("SPELL") << "spell" << spellAdj;

    QSet<QString> spielAdj {};
    spielAdj.insert("spill");
    QTest::newRow("SPIEL") << "spiel" << spielAdj;

    QSet<QString> skillAdj {};
    skillAdj.insert("spill");
    skillAdj.insert("still");
    QTest::newRow("SKILL") << "skill" << skillAdj;

    QSet<QString> compsAdj {};
    compsAdj.insert("comps");
    QTest::newRow("COMPS") << "comps" << compsAdj;

    QSet<QString> pillsAdj {};
    pillsAdj.insert("pills");
    QTest::newRow("PILLS") << "pills" << pillsAdj;

    QSet<QString> hatchAdj {};
    hatchAdj.insert("latch");
    QTest::newRow("HATCH") << "hatch" << hatchAdj;

    QSet<QString> latchAdj {};
    latchAdj.insert("hatch");
    QTest::newRow("LATCH") << "latch" << latchAdj;
}

void TestBonusTask1::testAlterAnswerFunction() {
    QFETCH(QString, answer);
    QFETCH(QSet<QString>, possibleAlters);

    QString alteredAnswer = alterAnswer(answer, dummyAnswers);
    QVERIFY2(possibleAlters.find(alteredAnswer) != possibleAlters.end(), QString("got " + alteredAnswer).toStdString().c_str());
}

#define KEY_PRESS(X) if (Key* key = curRound.ui->centralwidget->findChild<Key*>("toolButton_"#X)) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_##X);
#define ENT_PRESS    if (QPushButton* key = curRound.ui->centralwidget->findChild<QPushButton*>("pushButton_Enter")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_Return);

void TestBonusTask1::testAbsurdRoundWindow() {
    AbsurdRoundWindow curRound {nullptr, 5};
    curRound.wr->setAnswer("pause");

    KEY_PRESS(T);
    KEY_PRESS(E);
    KEY_PRESS(S);
    KEY_PRESS(T);
    KEY_PRESS(S);
    ENT_PRESS;

    QString alter = curRound.wr->getAnswer();
    int numDiffLetters = (alter[0] != 'p') + (alter[1] != 'a') + (alter[2] != 'u') + (alter[3] != 's') + (alter[4] != 'e');
    QCOMPARE(numDiffLetters, 1);
}

#undef KEY_PRESS
#undef ENT_PRESS

#define KEY_PRESS(X) if (Key* key = w.curRound->ui->centralwidget->findChild<Key*>("toolButton_"#X)) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(w.curRound, Qt::Key_##X);
#define ENT_PRESS    if (QPushButton* key = w.curRound->ui->centralwidget->findChild<QPushButton*>("pushButton_Enter")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(w.curRound, Qt::Key_Return);

#define COMBOBOX_MODE w.ui->centralwidget->findChild<QComboBox*>("comboBoxMode")
#define SPINBOX_WORDLENGTH w.ui->centralwidget->findChild<QSpinBox*>("spinBoxWordLength")
#define WIDGET_CHECK if(!COMBOBOX_MODE || !SPINBOX_WORDLENGTH) QFAIL("Required widgets not found.");

void TestBonusTask1::testAbsurdMainWindow() {
    MainWindow w;

    WIDGET_CHECK;

    COMBOBOX_MODE->setCurrentText("Absurd");
    SPINBOX_WORDLENGTH->setValue(5);

    QTest::mouseClick(w.ui->pushButton, Qt::LeftButton);
    while (!w.curRound) {
        // wait for curRound to be initialized
    }
    w.curRound->hide();
    w.curRound->wr->setAnswer("pause");

    KEY_PRESS(T);
    KEY_PRESS(E);
    KEY_PRESS(S);
    KEY_PRESS(T);
    KEY_PRESS(S);
    ENT_PRESS;

    QString alter = w.curRound->wr->getAnswer();
    int numDiffLetters = (alter[0] != 'p') + (alter[1] != 'a') + (alter[2] != 'u') + (alter[3] != 's') + (alter[4] != 'e');
    QCOMPARE(numDiffLetters, 1);
}

#undef KEY_PRESS
#undef ENT_PRESS

#undef COMBOBOX_MODE
#undef SPINBOX_WORDLENGTH
#undef WIDGET_CHECK

QTEST_MAIN(TestBonusTask1)
