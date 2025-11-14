/* ChessPiece-luokan otsikkotiedosto
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

#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include <set>
#include <utility>
#include <memory>

#include "chessboard.hh"

using namespace std;

// The two colors in chess used to identify the two sides.
enum ChessColor {
    WHITE,
    BLACK
};

// The different types of chess pieces.
enum PieceType {
    PAWN,
    ROOK,
    KING,
    BISHOP,
    QUEEN,
    KNIGHT
};

/**
 * @brief An abstract base class representing a chess piece.
 */
class ChessPiece
{
public:
    ChessPiece(ChessColor color, PieceType type, string name);
    ~ChessPiece() = default;

    /**
     * @brief get_allowed_moves Get the allowed moves for the piece
     * @param board The current state of the chess board
     * @return the set of coordinates where the piece can move
     */
    virtual set<Coord> get_allowed_moves(ChessBoard const& board) const = 0;

    /**
     * @brief get_name Get the name of the piece.
     * @return The color of the piece
     */
    virtual string get_name() const;

    /**
     * @brief get_color Get the color of the piece
     * @return the color of the piece
     */
    ChessColor get_color() const;

    /**
     * @brief get_type Get the type of the piece
     * @return the type of the piece
     */
    PieceType get_type() const;

    /**
     * @brief get_position Get the current position
     * @return the current position
     */
    Coord get_position() const;

    /**
     * @brief set_position Set the current position
     * @param pos the new position
     */
    void set_position(Coord pos);

    /**
     * @brief check_moves_in_direction Tarkistaa pysty- ja vaakarivin
     * ruudut positiiviseen tai negatiiviseen suuntaan ja onko niihin mahdollista siirtyä
     * @param board Pelilauta nykytilanteessa
     * @param direction Kertoo tarkistetaanko positiiviseen vai negatiiviseen suuntaan
     * @param moves Setti, joka sisältää mahdolliset siirrot
     */
    void check_moves_in_direction(const ChessBoard &board,
                                  const int direction, set<Coord> &moves) const;

    /**
     * @brief check_moves_diagonally Tarkistaa siirrot lomittain
     * ruudut positiiviseen tai negatiiviseen suuntaan ja onko niihin mahdollista siirtyä
     * @param board Pelilauta nykytilanteessa
     * @param hdirection Kertoo tarkistetaanko positiiviseen vai negatiiviseen suuntaan vaakatasossa
     * @param vdirection Kertoo tarkistetaanko positiiviseen vai negatiiviseen suuntaan pystytasossa
     * @param moves Setti, joka sisältää mahdolliset siirrot
     */
    void check_moves_diagonally(const ChessBoard &board, const int hdirection,
                                  const int vdirection, set<Coord> &moves) const;

private:
    // Attributes
    ChessColor color_;
    PieceType type_;
    string name_;
    Coord position_;
};

#endif // CHESSPIECE_H
