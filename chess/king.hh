/* King-luokan otsikkotiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#ifndef KING_HH
#define KING_HH

#include "chesspiece.hh"

/**
 * @brief The King class
 * The king moves one square in any direction, horizontally, vertically, or diagonally.
 */
class King : public ChessPiece
{
public:
    King(ChessColor col);
    virtual ~King() = default;

    virtual set<Coord> get_allowed_moves(ChessBoard const& board) const override;

private:
    void check_move(const ChessBoard &board, const Coord &dest, set<Coord> &moves) const;
};

#endif // KING_HH
