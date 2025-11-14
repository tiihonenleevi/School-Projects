/* Rook-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#include "rook.hh"

Rook::Rook(ChessColor col) : ChessPiece(col, ROOK, "rook") {}

set<Coord> Rook::get_allowed_moves(const ChessBoard &board) const
{
    // Moves-settiin tallennetaan mahdolliset siirrot
    set<Coord> moves;

    // Tarkistetaan mahdolliset siirrot positiivisiin suuntiin
    check_moves_in_direction(board, 1, moves);

    // Tarkistetaan mahdolliset siirrot negatiivisiin suuntiin
    check_moves_in_direction(board, -1, moves);

    return moves;
}
