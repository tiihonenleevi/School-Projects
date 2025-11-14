/* ChessPiece-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
*
* Lisätietoja:
* Lisättiin metodit check_moves_in_direction ja check_moves_diagonally
* */

#include "chesspiece.hh"

ChessPiece::ChessPiece(ChessColor color, PieceType type, string name): color_(color), type_(type), name_(name) {}

ChessColor ChessPiece::get_color() const
{
    return color_;
}

PieceType ChessPiece::get_type() const
{
    return type_;
}

Coord ChessPiece::get_position() const
{
    return position_;
}

void ChessPiece::set_position(Coord pos)
{
    position_ = pos;
}

string ChessPiece::get_name() const
{
    return name_;
}

void ChessPiece::check_moves_in_direction(const ChessBoard &board, const int direction, set<Coord> &moves) const
{
    // Haetaan nappulan koordinaatit
    auto [row, col] = get_position();

    // Muuttuja vaakarivin tarkastelua varten
    int horizontal = 1;

    // Tarkistetaan vaakarivi
    while (true) {
        Coord dest = {row + horizontal * direction, col};

        // Lisätään siirto moves-settiin, jos ruutu on pelilaudalla ja
        // se on tyhjä tai siinä on vihollisen nappula
        if (board.coord_in_bounds(dest)) {
            shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
            if (piece == nullptr || piece->get_color() != get_color()) {

                moves.insert(dest);

                // Jos ruudussa on vihollisen nappula siirrytään pois while-silmukasta
                // syönnin jälkeen, sillä syödyn nappulan taakse ei voi liikkua
                if ( piece != nullptr && piece->get_color() != get_color() ) {
                    break;
                }
            } else {
                // Jos Ruudussa on toinen oma nappula siirrytään
                // pois while-silmukasta, sillä oman nappulan läpi ei voi liikkua
                break;
            }
        } else {
            // Jos ruutu ei ole laudalla siirrytään pois while-silmukasta
            break;
        }

        // Kasvatetaan horizontal muuttujaa, jotta seuraavalla silmukan
        // kierroksella tarkastellaan seuraavaa ruutua
        horizontal += 1;
    }

    // Muuttuja pystysarakkeen tarkastelua varten
    int vertical = 1;

    // Tarkistetaan pystysarake
    while (true) {
        Coord dest = {row, col + vertical * direction};

        // Lisätään siirto moves-settiin, jos ruutu on pelilaudalla ja
        // se on tyhjä tai siinä on vihollisen nappula
        if (board.coord_in_bounds(dest)) {
            shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
            if (piece == nullptr || piece->get_color() != get_color()) {

                moves.insert(dest);

                // Jos ruudussa on vihollisen nappula siirrytään pois while-silmukasta
                // syönnin jälkeen, sillä syödyn nappulan taakse ei voi liikkua
                if ( piece != nullptr && piece->get_color() != get_color() ) {
                    break;
                }
            } else {
                // Jos Ruudussa on toinen oma nappula siirrytään
                // pois while-silmukasta, sillä oman nappulan läpi ei voi liikkua
                break;
            }
        } else {
            // Jos ruutu ei ole laudalla siirrytään pois while-silmukasta
            break;
        }

        // Kasvatetaan horizontal muuttujaa, jotta seuraavalla silmukan
        // kierroksella tarkastellaan seuraavaa ruutua
        vertical += 1;
    }
}

void ChessPiece::check_moves_diagonally(const ChessBoard &board, const int hdirection, const int vdirection, set<Coord> &moves) const
{
    // Haetaan nappulan koordinaatit
    auto [row, col] = get_position();

    // muuttujat vaaka- ja pystyrivien tarkastelua varten
    int horizontal = 1;
    int vertical = 1;

    // tarkistetaan siirtyminen lomittain
    while (true) {
        Coord dest = {row + vertical * vdirection, col + horizontal * hdirection};

        // Lisätään siirto moves-settiin, jos ruutu on pelilaudalla ja
        // se on tyhjä tai siinä on vihollisen nappula
        if (board.coord_in_bounds(dest)) {
            shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
            if (piece == nullptr || piece->get_color() != get_color()) {

                moves.insert(dest);

                // Jos ruudussa on vihollisen nappula siirrytään pois while-silmukasta
                // syönnin jälkeen, sillä syödyn nappulan taakse ei voi liikkua
                if ( piece != nullptr && piece->get_color() != get_color() ) {
                    break;
                }
            } else {
                // Jos Ruudussa on toinen oma nappula siirrytään
                // pois while-silmukasta, sillä oman nappulan läpi ei voi liikkua
                break;
            }
        } else {
            // Jos ruutu ei ole laudalla siirrytään pois while-silmukasta
            break;
        }

        // Kasvatetaan muuttujia, jotta seuraavalla silmukan
        // kierroksella tarkastellaan seuraavaa ruutua
        horizontal += 1;
        vertical += 1;
    }
}
