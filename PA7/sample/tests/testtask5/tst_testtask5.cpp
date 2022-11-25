#include "tst_testtask5.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regularroundwindow.h"
#include "ui_roundwindow.h"

#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>

bool validHardModeGuess(const QString& guess, const bool correctLetters[]);

struct CorrectLetters {
    bool isCorrect[26] = {false};
    CorrectLetters& setLetter(const char letter) { isCorrect[letter - 'a'] = true; return *this; }
};

void TestTask5::testHardModeFunction_data() {
    QTest::addColumn<QString>("guess");
    QTest::addColumn<CorrectLetters>("correctLetters");
    QTest::addColumn<bool>("expectedValid");

    QTest::newRow("COMPS vs. A") << "comps" << (CorrectLetters){}.setLetter('a') << false;
    QTest::newRow("COMPS vs. C M") << "comps" << (CorrectLetters){}.setLetter('c').setLetter('m') << true;
    QTest::newRow("COMPS vs. C E") << "comps" << (CorrectLetters){}.setLetter('c').setLetter('e') << false;
    QTest::newRow("COMPS vs. O P S") << "comps" << (CorrectLetters){}.setLetter('o').setLetter('p').setLetter('s') << true;
    QTest::newRow("COMPS vs. null") << "comps" << (CorrectLetters){} << true;
}

void TestTask5::testHardModeFunction() {
    QFETCH(QString, guess);
    QFETCH(CorrectLetters, correctLetters);
    QFETCH(bool, expectedValid);

    bool actualValid = validHardModeGuess(guess, correctLetters.isCorrect);
    QCOMPARE(actualValid, expectedValid);
}

#define KEY_PRESS(X) if (Key* key = curRound.ui->centralwidget->findChild<Key*>("toolButton_"#X)) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_##X);
#define ENT_PRESS    if (QPushButton* key = curRound.ui->centralwidget->findChild<QPushButton*>("pushButton_Enter")) \
                        QTest::mouseClick(key, Qt::LeftButton); \
                    else \
                        QTest::keyClick(&curRound, Qt::Key_Return);

void TestTask5::testHardModeRoundWindow() {
    hasPopup = false;
    RegularRoundWindow curRound {nullptr, 5, true};
    curRound.wr->setAnswer("crate");

    KEY_PRESS(P);
    KEY_PRESS(A);
    KEY_PRESS(U);
    KEY_PRESS(S);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(C);
    KEY_PRESS(R);
    KEY_PRESS(A);
    KEY_PRESS(F);
    KEY_PRESS(T);
    QTimer::singleShot(500, this, &TestTask5::verifyInvalidHardModePopup);
    ENT_PRESS;

    if (!hasPopup)
        QFAIL("Did not see the correct popup.");
}

void TestTask5::testHardModeRoundWindow2() {
    hasPopup = false;
    RegularRoundWindow curRound {nullptr, 5, true};
    curRound.wr->setAnswer("crate");

    KEY_PRESS(E);
    KEY_PRESS(R);
    KEY_PRESS(R);
    KEY_PRESS(E);
    KEY_PRESS(D);
    ENT_PRESS;

    KEY_PRESS(P);
    KEY_PRESS(A);
    KEY_PRESS(U);
    KEY_PRESS(S);
    KEY_PRESS(E);
    QTimer::singleShot(500, this, &TestTask5::verifyInvalidHardModePopup);
    ENT_PRESS;

    if (!hasPopup)
        QFAIL("Did not see the correct popup.");
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
#define CHECKBOX_HARDMODE w.ui->centralwidget->findChild<QCheckBox*>("checkBoxHardMode")
#define WIDGET_CHECK if(!COMBOBOX_MODE || !SPINBOX_WORDLENGTH || !CHECKBOX_HARDMODE) QFAIL("Required widgets not found.");

void TestTask5::testHardModeMainWindow() {
    hasPopup = false;
    MainWindow w;

    WIDGET_CHECK;

    COMBOBOX_MODE->setCurrentText("Regular");
    SPINBOX_WORDLENGTH->setValue(5);
    CHECKBOX_HARDMODE->setCheckState(Qt::Checked);

    QTest::mouseClick(w.ui->pushButton, Qt::LeftButton);
    while (!w.curRound) {
        // wait for curRound to be initialized
    }
    w.curRound->hide();
    w.curRound->wr->setAnswer("crate");

    KEY_PRESS(P);
    KEY_PRESS(A);
    KEY_PRESS(U);
    KEY_PRESS(S);
    KEY_PRESS(E);
    ENT_PRESS;

    KEY_PRESS(G);
    KEY_PRESS(E);
    KEY_PRESS(E);
    KEY_PRESS(S);
    KEY_PRESS(E);
    QTimer::singleShot(500, this, &TestTask5::verifyInvalidHardModePopup);
    ENT_PRESS;

    if (!hasPopup)
        QFAIL("Did not see the correct popup.");
}

#undef KEY_PRESS
#undef ENT_PRESS

#undef COMBOBOX_MODE
#undef SPINBOX_WORDLENGTH
#undef CHECKBOX_HARDMODE
#undef WIDGET_CHECK

void TestTask5::verifyInvalidHardModePopup() {
    QWidgetList allToplevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : allToplevelWidgets) {
        if (w->inherits("QMessageBox")) {
            QMessageBox *mb = qobject_cast<QMessageBox*>(w);
            hasPopup = mb->text() == "You must use yellow and green letters in Hard Mode!";
            QTest::keyClick(mb, Qt::Key_Enter);
            break;
        }
    }
}

QTEST_MAIN(TestTask5)
