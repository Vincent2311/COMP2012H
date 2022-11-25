#ifndef TESTTASK2_H
#define TESTTASK2_H

#include <QtTest>

class TestTask2 : public QObject
{
    Q_OBJECT

private:
    bool hasPopup {false};

public:
    TestTask2() = default;
    ~TestTask2() = default;

public slots:
    void verifyInvalidPopup();

private slots:
    void testKeyPresses();
    void testKeyPressesWithDel();
    void testKeyPressesWithEnterSuccess();
    void testKeyPressesWithEnterEarly();
    void testKeyPressesWithEnterInvalidWord();

};

#endif // TESTTASK2_H
