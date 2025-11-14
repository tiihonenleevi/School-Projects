/* Bishop-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#include "bishop.hh"

Bishop::Bishop(ChessColor col) : ChessPiece(col, BISHOP, "bishop") {}

set<Coord> Bishop::get_allowed_moves(const ChessBoard &board) const
{
    // Moves-settiin tallennetaan mahdolliset siirrot
    set<Coord> moves;

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
