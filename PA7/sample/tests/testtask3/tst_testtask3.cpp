#include "tst_testtask3.h"

#include "regularroundwindow.h"
#include "ui_roundwindow.h"
#include "letterbox.h"

#define BOX(X, Y) dynamic_cast<LetterBox*>(curRound.ui->gridLayoutBox->itemAtPosition(X, Y)->widget())
#define KEY_PRESS(X) QTest::keyClick(&curRound, Qt::Key_##X)
#define DEL_PRESS    QTest::keyClick(&curRound, Qt::Key_Delete)
#define ENT_PRESS    QTest::keyClick(&curRound, Qt::Key_Return)

void TestTask3::testKeyPresses() {
    RegularRoundWindow curRound {nullptr, 5, false};

    KEY_PRESS(W);
    QCOMPARE(BOX(0, 0)->getLetter(), 'w');

    KEY_PRESS(I);
    QCOMPARE(BOX(0, 1)->getLetter(), 'i');

    KEY_PRESS(T);
    QCOMPARE(BOX(0, 2)->getLetter(), 't');

    KEY_PRESS(C);
    QCOMPARE(BOX(0, 3)->getLetter(), 'c');

    KEY_PRESS(H);
    QCOMPARE(BOX(0, 4)->getLetter(), 'h');
}

void TestTask3::testKeyPressesWithDel() {
    RegularRoundWindow curRound {nullptr, 5, false};

    KEY_PRESS(A);
    QCOMPARE(BOX(0, 0)->getLetter(), 'a');

    KEY_PRESS(B);
    QCOMPARE(BOX(0, 1)->getLetter(), 'b');

    DEL_PRESS;
    QCOMPARE(BOX(0, 1)->getLetter(), ' ');

    KEY_PRESS(F);
    QCOMPARE(BOX(0, 1)->getLetter(), 'f');

    DEL_PRESS;
    QCOMPARE(BOX(0, 1)->getLetter(), ' ');

    DEL_PRESS;
    QCOMPARE(BOX(0, 0)->getLetter(), ' ');

    DEL_PRESS;
    QCOMPARE(BOX(0, 0)->getLetter(), ' ');

    KEY_PRESS(H);
    QCOMPARE(BOX(0, 0)->getLetter(), 'h');

    KEY_PRESS(O);
    QCOMPARE(BOX(0, 1)->getLetter(), 'o');

    KEY_PRESS(N);
    QCOMPARE(BOX(0, 2)->getLetter(), 'n');

    KEY_PRESS(K);
    QCOMPARE(BOX(0, 3)->getLetter(), 'k');

    KEY_PRESS(Y);
    QCOMPARE(BOX(0, 4)->getLetter(), 'y');

    DEL_PRESS;
    QCOMPARE(BOX(0, 4)->getLetter(), ' ');
}

void TestTask3::testKeyPressesWithEnterSuccess()
{
    RegularRoundWindow curRound {nullptr, 5, false};
    curRound.wr->setAnswer("crate");

    KEY_PRESS(E);
    QCOMPARE(BOX(0, 0)->getLetter(), 'e');

    KEY_PRESS(R);
    QCOMPARE(BOX(0, 1)->getLetter(), 'r');

    KEY_PRESS(R);
    QCOMPARE(BOX(0, 2)->getLetter(), 'r');

    KEY_PRESS(E);
    QCOMPARE(BOX(0, 3)->getLetter(), 'e');

    KEY_PRESS(D);
    QCOMPARE(BOX(0, 4)->getLetter(), 'd');

    ENT_PRESS;
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_E")->getColor(), YELLOW);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_R")->getColor(), GREEN);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_D")->getColor(), GREY);

    KEY_PRESS(R);
    QCOMPARE(BOX(1, 0)->getLetter(), 'r');

    KEY_PRESS(E);
    QCOMPARE(BOX(1, 1)->getLetter(), 'e');

    KEY_PRESS(U);
    QCOMPARE(BOX(1, 2)->getLetter(), 'u');

    KEY_PRESS(S);
    QCOMPARE(BOX(1, 3)->getLetter(), 's');

    KEY_PRESS(E);
    QCOMPARE(BOX(1, 4)->getLetter(), 'e');

    ENT_PRESS;
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_R")->getColor(), GREEN);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_E")->getColor(), GREEN);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_U")->getColor(), GREY);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_S")->getColor(), GREY);
}

void TestTask3::testKeyPressesWithEnterEarly() {
    RegularRoundWindow curRound {nullptr, 5, false};

    KEY_PRESS(G);
    QCOMPARE(BOX(0, 0)->getLetter(), 'g');

    KEY_PRESS(L);
    QCOMPARE(BOX(0, 1)->getLetter(), 'l');

    KEY_PRESS(M);
    QCOMPARE(BOX(0, 2)->getLetter(), 'm');

    KEY_PRESS(P);
    QCOMPARE(BOX(0, 3)->getLetter(), 'p');

    ENT_PRESS;
    QCOMPARE(BOX(0, 4)->getLetter(), ' ');
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_G")->getColor(), Qt::lightGray);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_L")->getColor(), Qt::lightGray);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_M")->getColor(), Qt::lightGray);
    QCOMPARE(curRound.ui->centralwidget->findChild<Key*>("toolButton_P")->getColor(), Qt::lightGray);
}

void TestTask3::testKeyPressesWithEnterInvalidWord() {
    RegularRoundWindow curRound {nullptr, 5, false};
    curRound.wr->setAnswer("sleep");

    KEY_PRESS(Q);
    QCOMPARE(BOX(0, 0)->getLetter(), 'q');

    KEY_PRESS(Z);
    QCOMPARE(BOX(0, 1)->getLetter(), 'z');

    KEY_PRESS(X);
    QCOMPARE(BOX(0, 2)->getLetter(), 'x');

    KEY_PRESS(J);
    QCOMPARE(BOX(0, 3)->getLetter(), 'j');

    KEY_PRESS(V);
    QCOMPARE(BOX(0, 4)->getLetter(), 'v');

    QTimer::singleShot(500, this, &TestTask3::verifyInvalidPopup);
    ENT_PRESS;

    if (!hasPopup)
        QFAIL("Did not see the correct popup.");
}

void TestTask3::verifyInvalidPopup() {
    QWidgetList allToplevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : allToplevelWidgets) {
        if (w->inherits("QMessageBox")) {
            QMessageBox *mb = qobject_cast<QMessageBox*>(w);
            hasPopup = mb->text() == "The word you entered is not in the dictionary!";
            QTest::keyClick(mb, Qt::Key_Enter);
            break;
        }
    }
}

#undef BOX
#undef KEY_PRESS
#undef DEL_PRESS
#undef ENT_PRESS

QTEST_MAIN(TestTask3)
