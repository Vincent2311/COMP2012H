#ifndef TESTTASK3_H
#define TESTTASK3_H

#include <QtTest>

class TestTask3 : public QObject
{
    Q_OBJECT

private:
    bool hasPopup {false};

public:
    TestTask3() = default;
    ~TestTask3() = default;

public slots:
    void verifyInvalidPopup();

private slots:
    void testKeyPresses();
    void testKeyPressesWithDel();
    void testKeyPressesWithEnterSuccess();
    void testKeyPressesWithEnterEarly();
    void testKeyPressesWithEnterInvalidWord();

};

#endif // TESTTASK3_H
