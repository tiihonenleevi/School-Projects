/*
* Ohjelman testaustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */

#include <QtTest>

// add necessary includes here
#include "../gameboard.hh"
#include <memory>

using namespace std;

class PathTest : public QObject
{
    Q_OBJECT

public:
    PathTest();
    ~PathTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

    // Alustustoimenpiteet ennen kunkin testin suorittamista
    void init();

    // Testi onnistuneelle siirrolle
    void testSuccessfullMove();

    // Testi epäonnistuneelle siirrolle
    void testFailedMove();

    // Tieto-ohjatut testit sen varmistamiseksi, ettei ohjelma hyväksy laittomia koordinaatteja
    void testIsValidLocations();
    void testIsValidLocations_data();

private:
    shared_ptr<GameBoard> testBoard_;

};

PathTest::PathTest()
{

}

PathTest::~PathTest()
{

}

void PathTest::initTestCase()
{

}

void PathTest::cleanupTestCase()
{

}

void PathTest::init()
{
    testBoard_.reset();
    testBoard_ = make_shared<GameBoard>();
}

void PathTest::testSuccessfullMove()
{
    // Asetetaan lauta vastaamaan alkuasetelmaa
    testBoard_->initialFill();

    // Määritellään siirron aloitus- ja lopetuspisteet
    Location start = {1, 0};
    Location destination = {2, 2};

    // Testataan palauttaako move-metodi true arvon, kun sitä kutsutaan ja välissä ei ole estettä
    QVERIFY2(testBoard_->move(start, destination),
             "Method move is faulty when trying to move when the path is clear");
}

void PathTest::testFailedMove()
{
    // Asetetaan pelilaudalle este
    Location obstacle = {2, 2};
    testBoard_->getPlayArea()[obstacle.y][obstacle.x] = make_unique<Piece>("X");

    Location start = {1, 2};
    Location destination = {3, 3};

    // Testataan palauttaako move-metodi false arvon, kun reitillä on este
    QVERIFY2(!testBoard_->move(start, destination),
             "Method move is faulty when failing to move");

}

void PathTest::testIsValidLocations()
{
    QFETCH(Location, start);
    QFETCH(Location, destination);
    QFETCH(bool, result);

    // Asetetaan lauta vastaamaan alkuasetelmaa
    testBoard_->initialFill();

    // Testataan palauttaako move-metodi true arvon, kun reitillä on este
    QVERIFY2(testBoard_->isValidLocations(start, destination) == result,
             "Method isValidLocations is faulty");
}

void PathTest::testIsValidLocations_data()
{
    QTest::addColumn<Location>("start");
    QTest::addColumn<Location>("destination");
    QTest::addColumn<bool>("result");

    QTest::newRow("Start from valid location and go to a valid location") << Location{1, 0} << Location{2, 2} << true;
    QTest::newRow("Start from valid location and go to an invalid location") << Location{0, 0} << Location{2, 1} << false;
    QTest::newRow("Start from invalid location and go to a valid location") << Location{2, 1} << Location{2, 2} << false;
    QTest::newRow("Start from invalid location and got an invalid location") << Location{3, 2} << Location{0, 1} << false;
    QTest::newRow("Start from location where the isn't a piece and go to a valid location") << Location{1, 1} << Location{2, 2} << false;
    QTest::newRow("Start from location where the isn't a piece and go to an invalid location") << Location{1, 1} << Location{2, 3} << false;
    QTest::newRow("Start from location where the is a piece and go to a location that has also a piece") << Location{0, 0} << Location{1, 4} << false;

}


QTEST_APPLESS_MAIN(PathTest)

#include "tst_pathtest.moc"
