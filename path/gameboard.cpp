/*
* GameBoard-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */

#include "gameboard.hh"

GameBoard::GameBoard():
    invalid_positions_{{0,1}, {2,1}, {3,1}, {0,2}, {3,2}, {0,3}, {2,3}, {3,3}}
{
    playing_area_ = vector<vector<unique_ptr<Piece>>>(ROWS);
    for (unsigned int row = 0; row < ROWS; row++) {
        for (unsigned int col = 0; col < COLUMNS; col++) {
            // Luodaan uusi Piece ja sijoitetaan se soluun
            playing_area_[row].push_back(make_unique<Piece>());
        }
    }
}

GameBoard::~GameBoard()
{

}

void GameBoard::initialFill()
{
    // Asetetaan nappulat lähtötilanteeseen
    for (unsigned int col = 0; col < COLUMNS; col++) {
        playing_area_[0][col] = make_unique<Piece>("G");
        playing_area_[ROWS - 1][col] = make_unique<Piece>("R");
    }

    // Lisätään tyhjät paikat
    for (unsigned int row = 1; row < ROWS - 1; row++) {
        for (unsigned int col = 0; col < COLUMNS; col++){
            playing_area_[row][col] = nullptr;
        }
    }
}

void GameBoard::print() const
{
    cout << "===============" << endl;
    cout << "|   | 1 2 3 4 |" << endl;
    cout << "---------------" << endl;
    // Käydään läpi pelialue ja tulostetaan mitä siinä ruudussa on
    for (unsigned int row = 0; row < ROWS; row++) {
        cout << "| " << row + 1 << " | ";
        for (unsigned int col = 0; col < COLUMNS; col++){
            // Jos ruudussa on nappula
            if (playing_area_[row][col] != nullptr) {
                cout << playing_area_[row][col]->type << " ";
            } else {
                // Jos koordinaatti ei ole käytössä
                // col ja row toisinpäin, sillä alla oleva luo väliaikaisen Location olion
                if (invalid_positions_.count({col, row})) {
                    cout << "  ";
                } else {  // Jos koordinaatissa on tyhjä reikä nappulalle
                    cout << "o ";
                }
            }
        }
        cout << "|" << endl;
    }
    cout << "===============" << endl;

}

bool GameBoard::isGameOver() const
{
    // Tarkistetaan ylärivi
    for (unsigned int col = 0; col < COLUMNS; col++) {
        if (playing_area_[0][col] == nullptr || playing_area_[0][col]->type == "G") {
            return false; // Jos pisteessä ei ole punaista nappulaa
        }
    }

    // Tarkistetaan alarivi
    for (unsigned int col = 0; col < COLUMNS; col++) {
        if (playing_area_[4][col] == nullptr || playing_area_[4][col]->type == "R") {
            return false; // Jos pisteessä ei ole vihreää nappulaa
        }
    }

    // Jos funktion suoritus ei pysähdy aikaisemmin, on peli loppunut
    return true;
}

bool GameBoard::isValidLocations(const Location &start, const Location &destination) const
{
    // Tarkastetaan ovatko molemmat pisteet pelilaudalla
    if ( start.y > ROWS - 1 || start.x > COLUMNS - 1
         || destination.y > ROWS - 1 || destination.x > COLUMNS - 1 ) {
        return false;
    }

    // Tarkistetaan ovatko molemmat pisteet käytössä olevalla alueella
    if ( invalid_positions_.count(start) || invalid_positions_.count(destination) ) {
        return false;
    }

    // Tarkastetaan onko aloituspisteessä nappula ja lopetuspiste tyhjä
    if ( playing_area_[start.y][start.x] == nullptr ||
         playing_area_[destination.y][destination.x] != nullptr ) {
        return false;
    }

    // Tarkastetaan onko alkupiste loppupiste
    if ( playing_area_[start.y][start.x] == playing_area_[destination.y][destination.x] ) {
        return false;
    }

    // Jos funktion suoritus ei pysähdy aikaisemmin, on siirto laillinen
    return true;

}

bool GameBoard::move(const Location &start, const Location &destination)
{
    // Jos liikutaan vain samalla rivillä
    if (start.y == destination.y) {
        return moveInRowOrCol(start, destination, "row");
    }

    // Jos täytyy liikkua myös rivien välillä
    Location mid1 = {1, start.y};  // Välietappi
    Location mid2 = {1, destination.y};  // Välietappi

    // Liikutaan ensin sarakkeeseen 2, jos on tarve
    if (start.x != 1) {
        if (!moveInRowOrCol(start, mid1, "row")) {
            return false; // Jos reitillä oli nappula
        }
    }

    // Liikutaan rivien välillä
    if (!moveInRowOrCol(mid1, mid2, "col")) {
        return false; // Jos reitillä oli nappula
    }

    // Jos päästiin oikealle riville liikutaan oikeaan sarakkeeseen
    if (destination.x != 1) {
        if (!moveInRowOrCol(mid2, destination, "row")) {
            return false; // Jos reitillä oli nappula
        }
    }

    return true; // Jos päästiin haluttuun pisteeseen
}

vector<vector<unique_ptr<Piece>>>& GameBoard::getPlayArea()
{
    return playing_area_;
}


bool GameBoard::moveInRowOrCol(const Location &start, const Location &destination, const string &mode)
{

    // Jos liikutaan rivillä
    if (mode == "row") {
        if (start.x == destination.x) {
            return true;
        }
        // Tallennetaan muuttujiin pienempi ja suurempi arvo, jotta toimii molempiin suuntiin
        unsigned int begin = min(start.x, destination.x);
        unsigned int end = max(start.x, destination.x);

        for (unsigned int col = begin + 1; col < end; col++) {

            // Tarkistetaan jokaisessa pisteessä onko siinä toista nappulaa
            if (playing_area_[start.y][col] != nullptr) {
                return false;
            }
        }
    } else if (mode == "col") { // Jos liikutaan sarakkeella
        if (start.y == destination.y) {
            return true;
        }
        // Tallennetaan muuttujiin pienempi ja suurempi arvo, jotta toimii molempiin suuntiin
        unsigned int begin = min(start.y, destination.y);
        unsigned int end = max(start.y, destination.y);

        for (unsigned int row = begin + 1; row < end; row++) {
            // Tarkistetaan jokaisessa pisteessä onko siinä toista nappulaa
            if (playing_area_[row][start.x] != nullptr) {
                return false;
            }
        }
    }

    if (!playing_area_[start.y][start.x]) {
        return false;
    }

    // Jos reitti on selvä liikutaan haluttuun pisteeseen ja asetetaan alku tyhjäksi
    playing_area_[destination.y][destination.x] = make_unique<Piece>(*playing_area_[start.y][start.x]);
    playing_area_[start.y][start.x].reset();

    return true;
}
