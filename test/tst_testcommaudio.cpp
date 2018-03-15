#include <QtTest>
#include <QCoreApplication>

// add necessary includes here

class TestCommAudio : public QObject
{
    Q_OBJECT

public:
    TestCommAudio();
    ~TestCommAudio();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

TestCommAudio::TestCommAudio()
{

}

TestCommAudio::~TestCommAudio()
{

}

void TestCommAudio::initTestCase()
{

}

void TestCommAudio::cleanupTestCase()
{

}

void TestCommAudio::test_case1()
{

}

QTEST_MAIN(TestCommAudio)

#include "tst_testcommaudio.moc"
