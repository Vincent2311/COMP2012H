#include "tst_testtask1.h"

// add necessary includes here
#include "letterbox.h"
QColor* analyzeGuess(const QString& guess, const QString& answer);

struct Color5 {
    QColor color[5];

    inline bool operator==(const Color5& other) const {
        for (int i=0; i<5; ++i) if (color[i] != other.color[i]) return false;
        return true;
    }
};

struct Color9 {
    QColor color[9];

    inline bool operator==(const Color9& other) const {
        for (int i=0; i<9; ++i) if (color[i] != other.color[i]) return false;
        return true;
    }
};

struct Color18 {
    QColor color[18];

    inline bool operator==(const Color18& other) const {
        for (int i=0; i<18; ++i) if (color[i] != other.color[i]) return false;
        return true;
    }
};

void TestTask1::testAnalyzeGuess_data() {
    QTest::addColumn<QString>("guess");
    QTest::addColumn<QString>("answer");
    QTest::addColumn<Color5>("expectedColor");

    QTest::newRow("CHART vs. ARRAY") << "chart" << "array" << (Color5){GREY, GREY, YELLOW, YELLOW, GREY};
    QTest::newRow("AMASS vs. ARRAY") << "amass" << "array" << (Color5){GREEN, GREY, YELLOW, GREY, GREY};
    QTest::newRow("ERROR vs. ARRAY") << "error" << "array" << (Color5){GREY, GREEN, GREEN, GREY, GREY};
    QTest::newRow("YEARS vs. ARRAY") << "years" << "array" << (Color5){YELLOW, GREY, YELLOW, YELLOW, GREY};

    QTest::newRow("LUCKY vs. ERASE") << "lucky" << "erase" << (Color5){GREY, GREY, GREY, GREY, GREY};
    QTest::newRow("TRUTH vs. ERASE") << "truth" << "erase" << (Color5){GREY, GREEN, GREY, GREY, GREY};
    QTest::newRow("HASTY vs. ERASE") << "hasty" << "erase" << (Color5){GREY, YELLOW, YELLOW, GREY, GREY};
    QTest::newRow("STAMP vs. ERASE") << "stamp" << "erase" << (Color5){YELLOW, GREY, GREEN, GREY, GREY};
    QTest::newRow("HOUSE vs. ERASE") << "house" << "erase" << (Color5){GREY, GREY, GREY, GREEN, GREEN};
    QTest::newRow("AVAST vs. ERASE") << "avast" << "erase" << (Color5){GREY, GREY, GREEN, GREEN, GREY};
    QTest::newRow("SUSHI vs. ERASE") << "sushi" << "erase" << (Color5){YELLOW, GREY, GREY, GREY, GREY};
    QTest::newRow("SASSY vs. ERASE") << "sassy" << "erase" << (Color5){GREY, YELLOW, GREY, GREEN, GREY};
    QTest::newRow("RESET vs. ERASE") << "reset" << "erase" << (Color5){YELLOW, YELLOW, YELLOW, YELLOW, GREY};
    QTest::newRow("EERIE vs. ERASE") << "eerie" << "erase" << (Color5){GREEN, GREY, YELLOW, GREY, GREEN};
    QTest::newRow("SEERS vs. ERASE") << "seers" << "erase" << (Color5){YELLOW, YELLOW, YELLOW, YELLOW, GREY};
    QTest::newRow("ERASE vs. ERASE") << "erase" << "erase" << (Color5){GREEN, GREEN, GREEN, GREEN, GREEN};
}

void TestTask1::testAnalyzeGuess()
{
    QFETCH(QString, guess);
    QFETCH(QString, answer);
    QFETCH(Color5, expectedColor);
    QColor *color = analyzeGuess(guess, answer);
    if (!color) {
        QFAIL("Got nullptr for color array");
    }
    Color5 actualColor;
    for (int i=0; i<5; ++i) {
        actualColor.color[i] = color[i];
    }
    delete [] color;

    QCOMPARE(actualColor, expectedColor);
}

void TestTask1::testAnalyzeGuessLong_data() {
    QTest::addColumn<QString>("guess");
    QTest::addColumn<QString>("answer");
    QTest::addColumn<Color9>("expectedColor");

    QTest::newRow("DISPROVEN vs. INTERESTS") << "disproven" << "interests"
             << (Color9){GREY, YELLOW, YELLOW, GREY, GREEN, GREY, GREY, YELLOW, YELLOW};

    QTest::newRow("INTERSECT vs. INTERESTS") << "intersect" << "interests"
             << (Color9){GREEN, GREEN, GREEN, GREEN, GREEN, YELLOW, YELLOW, GREY, YELLOW};

    QTest::newRow("REINVESTS vs. INTERESTS") << "reinvests" << "interests"
             << (Color9){YELLOW, YELLOW, YELLOW, YELLOW, GREY, GREEN, GREEN, GREEN, GREEN};
}

void TestTask1::testAnalyzeGuessLong()
{
    QFETCH(QString, guess);
    QFETCH(QString, answer);
    QFETCH(Color9, expectedColor);
    QColor *color = analyzeGuess(guess, answer);
    if (!color) {
        QFAIL("Got nullptr for color array");
    }
    Color9 actualColor;
    for (int i=0; i<9; ++i) {
        actualColor.color[i] = color[i];
    }
    delete [] color;

    QCOMPARE(actualColor, expectedColor);
}

void TestTask1::testAnalyzeGuessVeryLong_data() {
    QTest::addColumn<QString>("guess");
    QTest::addColumn<QString>("answer");
    QTest::addColumn<Color18>("expectedColor");

    QTest::newRow("THERMOGEOGRAPHICAL vs. NEUROPHARMACOLOGIC") << "thermogeographical" << "neuropharmacologic"
             << (Color18){GREY, YELLOW, YELLOW, GREEN, YELLOW, YELLOW, YELLOW, GREY, YELLOW,
                          GREY, YELLOW, YELLOW, YELLOW, GREY, YELLOW, YELLOW, YELLOW, YELLOW};
}

void TestTask1::testAnalyzeGuessVeryLong()
{
    QFETCH(QString, guess);
    QFETCH(QString, answer);
    QFETCH(Color18, expectedColor);
    QColor *color = analyzeGuess(guess, answer);
    if (!color) {
        QFAIL("Got nullptr for color array");
    }
    Color18 actualColor;
    for (int i=0; i<18; ++i) {
        actualColor.color[i] = color[i];
    }
    delete [] color;

    QCOMPARE(actualColor, expectedColor);
}

QTEST_MAIN(TestTask1)
