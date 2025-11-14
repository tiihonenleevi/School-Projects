/* MainWindow-luokan otsikkotiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>

#include "chess.hh"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * @brief on_restart_button_clicked Kun restart-nappia klikataan
     */
    void on_restart_button_clicked();

    /**
     * @brief on_square_clicked Kun ruutua klikataan pelilaudalla
     * @param row Klikatun ruudun rivi
     * @param col Klikatun ruudun sarake
     */
    void on_square_clicked(int row, int col);

private:
    Chess chess_;

    // Nappien tallennusta varten, jotta niiden kuvia on helppo muuttaa siirtoja tehdessä
    array<array<QPushButton*, BOARD_SIZE>, BOARD_SIZE> button_grid_;

    const int SQUARE_SIZE = 64;

    // Vakiot kuvien polkuja varten
    const QString PREFIX = (":pieces/");
    const QString SUFFIX = (".png");

    QLabel* info_label_;

    /**
     * @brief init_board Asettaa laudan alkuasetelmiin
     */
    void init_board();

    /**
     * @brief color_to_short Antaa nappulan väri siinä muodossa, jota
     * käytetään kuvatiedostoissa
     * @param color Nappulan väri
     * @return Palauttaa nappulan värin siinä muodossa,
     *  jota käytetään kuvatiedostoissa
     */
    QString color_to_short(ChessColor color) const;

    /**
     * @brief highlight_possible_moves Korostaa laudalla ruudut,
     * joihin nappula on mahdollista siirtää
     * @param moves Nappulan mahdolliset siirrot
     */
    void highlight_possible_moves(set<Coord> moves);

    /**
     * @brief clear_highlights Poistaa ruutujen korostukset
     * @param moves Nappulan mahdolliset siirrot, eli ruudut jotka korostettiin
     */
    void clear_highlights(set<Coord> moves);

    /**
     * @brief disable_board Asettaa pelilaudan ruudut sellaisiksi, että niitä ei voi klikata
     */
    void disable_board();

    /**
     * @brief enable_board Asettaa pelilaudan ruudut sellaisiksi, että niitä voi klikata
     */
    void enable_board();

    /**
     * @brief update_icons Päivittää ruutujen kuvat oikeiksi, kun peli alkaa tai kun peli restartataan
     */
    void update_icons();

    QPoint first_click_;
    bool waiting_second_click_ = false;
};

#endif // MAINWINDOW_HH
