/*
* GameBoard-luokan otsikkotiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <iostream>
#include <vector>
#include <memory>
#include <set>

using namespace std;

// Gameboard size
const unsigned int ROWS = 5;
const unsigned int COLUMNS = 4;

struct Location
{
    unsigned int x;
    unsigned int y;
    bool operator==(const Location& rhs) const
    {
        return x == rhs.x and y == rhs.y;
    }
    bool operator<(const Location& rhs) const
    {
        return (x < rhs.x) or (x == rhs.x and y < rhs.y);
    }
};

struct Piece
{
    string type;

    // Oletusrakentaja tyhjään alustukseen
    Piece() = default;

    // Rakentaja merkkijonon alustukseen
    Piece(const string& t) : type(t) {}
};

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    virtual ~GameBoard();

    // Fill the gameboard initially: all green buttons are put on the top row
    // and all red buttons on the bottom rows, other locations are nullptrs.
    void initialFill();

    // Print the gameboard.
    void print() const;

    // Return true, if the game is over, i.e. if all red buttons are on the
    // top row and all green buttons are on the bottom row.
    bool isGameOver() const;

    // Check the validity of given locations. To be valid, a location must be
    // inside the gameboard, it must not be an unused location, and the start
    // must have a button, and the destination must not have a button.
    // (Implicitly also check that p1 and p2 are not the same points.)
    // Returns true, if both the given locations are valid.
    bool isValidLocations(const Location& start,
                          const Location& destination) const;

    // If possible, move a button from the start location to the destination
    // one. A move may consists of horizontal and vertical steps.
    // Return true, if move is possible, i.e. if there is a path between
    // the given locations.
    bool move(const Location& start, const Location& destination);

    // Palauttaa pelialueen
    vector<vector<unique_ptr<Piece>>>& getPlayArea();

private:
    // Vektori, johon pelialue tallennetaan
    vector<vector<unique_ptr<Piece>>> playing_area_;
    // Setti, johon tallennetaan pelialueen kohdat, jotka eivät ole käytössä
    set<Location> invalid_positions_;

    // Tarkistaa onko haluttu reitti selvä. Toimii jos liikutaan pelkästään rivillä tai sarakkeella
    bool moveInRowOrCol(const Location& start, const Location& destination, const string& mode);
};

#endif // GAMEBOARD_HH
