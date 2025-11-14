/* King-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#include "king.hh"

King::King(ChessColor col): ChessPiece(col, KING, "king") {}

set<Coord> King::get_allowed_moves(const ChessBoard &board) const
{
    // Moves-settiin tallennetaan mahdolliset siirrot
    set<Coord> moves;

    // Haetaan nappulan koordinaatit
    auto [row, col] = get_position();

    // yläpuoli
    check_move(board, {row - 1, col}, moves);

    // yläpuolella oikealla
    check_move(board, {row - 1, col + 1}, moves);

    // oikealla
    check_move(board, {row, col + 1}, moves);

    // alapuolella oikealla
    check_move(board, {row + 1, col + 1}, moves);

    // alapuolella
    check_move(board, {row + 1, col}, moves);

    // alapuolella vasemmalla
    check_move(board, {row + 1, col - 1}, moves);

    // vasemmalla
    check_move(board, {row, col - 1}, moves);

    // yläpuolella vasemmalla
    check_move(board, {row - 1, col - 1}, moves);

    // Palautetaan mahdolliset siirrot
    return moves;
}

void King::check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const
{
    // Lisätään siirto moves-settiin, jos ruutu on pelilaudalla ja se on tyhjä tai siinä on vihollisen nappula
        if (board.coord_in_bounds(dest))
        {
            shared_ptr<ChessPiece> piece = board.get_piece_at(dest);
            if (piece == nullptr || piece->get_color() != get_color())
            {
                moves.insert(dest);
            }
        }
}



