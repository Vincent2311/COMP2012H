#include "tst_testbonustask2.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "multiroundwindow.h"
#include "ui_roundwindow.h"
#include "multiwordleround.h"
#include "letterbox.h"

#include <QComboBox>
#include <QSpinBox>

void TestBonusTask2::testGridLayoutInit_data() {
    QTest::addColumn<int>("numWords");

    QTest::newRow("2 words") << 2;
    QTest::newRow("3 words") << 3;
    QTest::newRow("5 words") << 5;
    QTest::newRow("8 words") << 8;
}

void TestBonusTask2::testGridLayoutInit() {
    QFETCH(int, numWords);

    MultiRoundWindow curRound {nullptr, 5, numWords};

    int gridCount = curRound.ui->horizontalLayoutGuesses->findChildren<QGridLayout*>(Qt::FindDirectChildrenOnly).count();
    QCOMPARE(gridCount, numWords);
}

#define KEY_PRESS(X) if (Key* key = curRound.ui->centralwidget->findChild<Key*>("toolButton_"#X)) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_##X);
#define DEL_PRESS    if (QPushButton* key = curRound.ui->centralwidget->findChild<QPushButton*>("pushButton_Del")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_Delete);
#define ENT_PRESS    if (QPushButton* key = curRound.ui->centralwidget->findChild<QPushButton*>("pushButton_Enter")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_Return);

QString answerMulti2[2] = {"about", "large"};
QString answerMulti4[4] = {"melee", "range", "magic", "rogue"};

void TestBonusTask2::testMultiRoundWindowSimple() {
    MultiRoundWindow curRound {nullptr, 5, 2};
    dynamic_cast<MultiWordleRound*>(curRound.wr)->setAnswerMulti(answerMulti2);

    KEY_PRESS(P);
    KEY_PRESS(A);
    KEY_PRESS(L);
    DEL_PRESS;
    KEY_PRESS(U);
    KEY_PRESS(S);
    KEY_PRESS(E);
    ENT_PRESS;

    QList<QGridLayout*> grids = curRound.ui->horizontalLayoutGuesses->findChildren<QGridLayout*>(Qt::FindDirectChildrenOnly);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->getLetter(), 'p');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 1)->widget())->getLetter(), 'a');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 1)->widget())->palette().color(QPalette::Base), YELLOW);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 2)->widget())->getLetter(), 'u');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 2)->widget())->palette().color(QPalette::Base), YELLOW);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 3)->widget())->getLetter(), 's');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 3)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 4)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 4)->widget())->palette().color(QPalette::Base), GREY);

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 0)->widget())->getLetter(), 'p');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 1)->widget())->getLetter(), 'a');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 1)->widget())->palette().color(QPalette::Base), GREEN);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 2)->widget())->getLetter(), 'u');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 2)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 3)->widget())->getLetter(), 's');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 3)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 4)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 4)->widget())->palette().color(QPalette::Base), GREEN);
}

void TestBonusTask2::testMultiRoundWindowComplex() {
    MultiRoundWindow curRound {nullptr, 5, 4};
    dynamic_cast<MultiWordleRound*>(curRound.wr)->setAnswerMulti(answerMulti4);

    KEY_PRESS(R);
    KEY_PRESS(A);
    KEY_PRESS(N);
    KEY_PRESS(G);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(R);
    KEY_PRESS(O);
    KEY_PRESS(G);
    KEY_PRESS(U);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(M);
    KEY_PRESS(A);
    KEY_PRESS(G);
    KEY_PRESS(I);
    KEY_PRESS(C);
    ENT_PRESS;

    KEY_PRESS(M);

    QList<QGridLayout*> grids = curRound.ui->horizontalLayoutGuesses->findChildren<QGridLayout*>(Qt::FindDirectChildrenOnly);

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->getLetter(), 'r');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(1, 4)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(1, 4)->widget())->palette().color(QPalette::Base), GREEN);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(2, 0)->widget())->getLetter(), 'm');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(2, 0)->widget())->palette().color(QPalette::Base), GREEN);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(3, 0)->widget())->getLetter(), 'm');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 0)->widget())->getLetter(), 'r');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(1, 0)->widget())->getLetter(), ' ');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(2, 0)->widget())->getLetter(), 'm');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(3, 0)->widget())->getLetter(), ' ');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(3)->itemAtPosition(1, 1)->widget())->getLetter(), 'o');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(3)->itemAtPosition(2, 1)->widget())->getLetter(), ' ');
}

#undef KEY_PRESS
#undef DEL_PRESS
#undef ENT_PRESS

#define KEY_PRESS(X) if (Key* key = w.curRound->ui->centralwidget->findChild<Key*>("toolButton_"#X)) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(w.curRound, Qt::Key_##X);
#define DEL_PRESS    if (QPushButton* key = w.curRound->ui->centralwidget->findChild<QPushButton*>("pushButton_Del")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(w.curRound, Qt::Key_Delete);
#define ENT_PRESS    if (QPushButton* key = w.curRound->ui->centralwidget->findChild<QPushButton*>("pushButton_Enter")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(w.curRound, Qt::Key_Return);

#define COMBOBOX_MODE w.ui->centralwidget->findChild<QComboBox*>("comboBoxMode")
#define SPINBOX_WORDLENGTH w.ui->centralwidget->findChild<QSpinBox*>("spinBoxWordLength")
#define SPINBOX_NUMWORDS w.ui->centralwidget->findChild<QSpinBox*>("spinBoxNumWords")
#define WIDGET_CHECK if(!COMBOBOX_MODE || !SPINBOX_WORDLENGTH || !SPINBOX_NUMWORDS) QFAIL("Required widgets not found.");

void TestBonusTask2::testMultiMainWindowSimple() {
    MainWindow w;

    WIDGET_CHECK;

    COMBOBOX_MODE->setCurrentText("Multiple");
    SPINBOX_WORDLENGTH->setValue(5);
    SPINBOX_NUMWORDS->setValue(2);

    QTest::mouseClick(w.ui->pushButton, Qt::LeftButton);
    while (!w.curRound) {
        // wait for curRound to be initialized
    }
    w.curRound->hide();
    dynamic_cast<MultiWordleRound*>(w.curRound->wr)->setAnswerMulti(answerMulti2);

    KEY_PRESS(P);
    KEY_PRESS(A);
    KEY_PRESS(L);
    DEL_PRESS;
    KEY_PRESS(U);
    KEY_PRESS(S);
    KEY_PRESS(E);
    ENT_PRESS;

    QList<QGridLayout*> grids = w.curRound->ui->horizontalLayoutGuesses->findChildren<QGridLayout*>(Qt::FindDirectChildrenOnly);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->getLetter(), 'p');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 1)->widget())->getLetter(), 'a');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 1)->widget())->palette().color(QPalette::Base), YELLOW);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 2)->widget())->getLetter(), 'u');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 2)->widget())->palette().color(QPalette::Base), YELLOW);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 3)->widget())->getLetter(), 's');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 3)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 4)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(0, 4)->widget())->palette().color(QPalette::Base), GREY);

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 0)->widget())->getLetter(), 'p');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 1)->widget())->getLetter(), 'a');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 1)->widget())->palette().color(QPalette::Base), GREEN);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 2)->widget())->getLetter(), 'u');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 2)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 3)->widget())->getLetter(), 's');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 3)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 4)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(0, 4)->widget())->palette().color(QPalette::Base), GREEN);
}

void TestBonusTask2::testMultiMainWindowComplex() {
    MainWindow w;

    WIDGET_CHECK;

    COMBOBOX_MODE->setCurrentText("Multiple");
    SPINBOX_WORDLENGTH->setValue(5);
    SPINBOX_NUMWORDS->setValue(4);

    QTest::mouseClick(w.ui->pushButton, Qt::LeftButton);
    while (!w.curRound) {
        // wait for curRound to be initialized
    }
    w.curRound->hide();
    dynamic_cast<MultiWordleRound*>(w.curRound->wr)->setAnswerMulti(answerMulti4);

    KEY_PRESS(R);
    KEY_PRESS(O);
    KEY_PRESS(G);
    KEY_PRESS(U);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(M);
    KEY_PRESS(E);
    KEY_PRESS(L);
    KEY_PRESS(E);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(R);
    KEY_PRESS(A);
    KEY_PRESS(N);
    KEY_PRESS(G);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(M);

    QList<QGridLayout*> grids = w.curRound->ui->horizontalLayoutGuesses->findChildren<QGridLayout*>(Qt::FindDirectChildrenOnly);

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(0, 0)->widget())->getLetter(), 'r');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(0, 0)->widget())->palette().color(QPalette::Base), GREY);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(1, 0)->widget())->getLetter(), 'm');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(1, 0)->widget())->palette().color(QPalette::Base), GREEN);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(2, 3)->widget())->getLetter(), 'g');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(2, 3)->widget())->palette().color(QPalette::Base), YELLOW);
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(2)->itemAtPosition(3, 0)->widget())->getLetter(), 'm');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(3)->itemAtPosition(0, 0)->widget())->getLetter(), 'r');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(3)->itemAtPosition(1, 0)->widget())->getLetter(), ' ');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(2, 2)->widget())->getLetter(), 'n');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(1)->itemAtPosition(3, 0)->widget())->getLetter(), ' ');

    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(1, 1)->widget())->getLetter(), 'e');
    QCOMPARE(dynamic_cast<LetterBox*>(grids.at(0)->itemAtPosition(2, 1)->widget())->getLetter(), ' ');
}

#undef KEY_PRESS
#undef DEL_PRESS
#undef ENT_PRESS

#undef COMBOBOX_MODE
#undef SPINBOX_WORDLENGTH
#undef SPINBOX_NUMWORDS
#undef WIDGET_CHECK

QTEST_MAIN(TestBonusTask2)
