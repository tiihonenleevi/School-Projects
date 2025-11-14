/* Queen-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#include "queen.hh"

Queen::Queen(ChessColor col) : ChessPiece(col, QUEEN, "queen") {}

set<Coord> Queen::get_allowed_moves(const ChessBoard &board) const
{
    // Moves-settiin tallennetaan mahdolliset siirrot
    set<Coord> moves;

    // Tarkistetaan mahdolliset siirrot positiivisiin suuntiin
    check_moves_in_direction(board, 1, moves);

    // Tarkistetaan mahdolliset siirrot negatiivisiin suuntiin
    check_moves_in_direction(board, -1, moves);

    // mahdolliset siirrot yläoikealle
    check_moves_diagonally(board, 1, 1, moves);

    // mahdolliset siirrot ylävasemmalle
    check_moves_diagonally(board, -1, 1, moves);

    // mahdolliset siirrot alaoikealle
    check_moves_diagonally(board, 1, -1, moves);

    // mahdolliset siirrot alavasemmalle
    check_moves_diagonally(board, -1, -1, moves);

    return moves;
}
