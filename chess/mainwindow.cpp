/* MainWindow-luokan toteutustiedosto
*
* Ohjelman kirjoittajat:
* Nimi: Leevi Tiihonen
* Palautteen kieli (fi/en): fi
*
* Nimi: Joose Haippo
* Palautteen kieli (fi/en): fi
* */
#include "mainwindow.hh"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    chess_.Chess::start_game();
    init_board();

}

MainWindow::~MainWindow() {}

void MainWindow::on_restart_button_clicked()
{
    chess_.Chess::start_game();
    update_icons();
    enable_board();
    info_label_->setText("White's turn");

}

void MainWindow::on_square_clicked(int row, int col)
{
    if ( !waiting_second_click_ ){
        // tallennetaan ensimmäinen klikkaus
        first_click_ = QPoint(col, row);
        waiting_second_click_ = true;
        qDebug() << "first click in: " << col << ", " << row;

        // Haetaan nappulan tyyppi
        shared_ptr<ChessPiece> current_piece =
                chess_.Chess::get_board().ChessBoard::get_piece_at(first_click_.y(), first_click_.x());

        // Tarkastetaan, että paikka ei ole tyhjä
        if ( current_piece == nullptr ) {
            waiting_second_click_ = false;
            return;
          // Jos ruudussa on vuorossa olevan värin nappula haetaan sen mahdolliset siirrot
        } else if (current_piece->get_color() == chess_.Chess::get_current_turn()) {
            auto possible_moves = current_piece->get_allowed_moves(chess_.Chess::get_board());

            // Korostetaan mahdolliset siirrot laudalla
            highlight_possible_moves(possible_moves);
        } else {
            // Jos nappula on vastustajan
            waiting_second_click_ = false;
            return;
        }
    }
    else {
        // tallennetaan toinen klikkaus
        QPoint second_click(col, row);

        // muutetaan toinen klikkaus Coord-tyypiksi
        Coord dest;
        dest.col = second_click.x();
        dest.row = second_click.y();

        // Haetaan nappulan tyyppi
        shared_ptr<ChessPiece> current_piece =
                chess_.Chess::get_board().ChessBoard::get_piece_at(first_click_.y(), first_click_.x());

        // Haetaan nappulan mahdolliset siirrot korostusten poistoa varten
        auto possible_moves = current_piece->get_allowed_moves(chess_.Chess::get_board());
        clear_highlights(possible_moves);

        // Tehdään siirto
        if ( chess_.make_move(current_piece, dest) ) {
            // Päivitetään pelilauta, jos siirto onnistuu

            // Kohderuutu
            QPushButton* to_button = button_grid_[second_click.y()][second_click.x()];
            // Haetaan kohderuudun väri
            ChessColor dest_color = ((second_click.y() + second_click.x())
                                        % 2 == 0) ? BLACK : WHITE;
            // Muutetaan värin nimi samaan muotoon kuin kuvatiedostoissa
            QString dest_square_color = color_to_short(dest_color);
            // Haetaan nappulan väri ja muutetaan se samaan muotoon kuin kuvatiedostoissa
            QString piece_color = color_to_short(current_piece->get_color());

            // Haetaan current piece uusiksi siltä varalta, että kyseessä on ollut pawn ja se on promotattu queeniksi
            current_piece =
                chess_.Chess::get_board().ChessBoard::get_piece_at(second_click.y(), second_click.x());

            // Asetetaan kohderuutuun nappulan kuva
            QString dest_image_path = PREFIX + piece_color + "-" +
                       QString::fromStdString(current_piece->get_name())
                       + "-on-" + dest_square_color + SUFFIX;

            QPixmap dest_image(dest_image_path);
            to_button->setIcon(dest_image);
            to_button->setIconSize(to_button->size());

            // Lähtöruutu
            QPushButton* from_button = button_grid_[first_click_.y()][first_click_.x()];
            // Haetaan alkuperäisen ruudun väri
            ChessColor src_color = ((first_click_.y() + first_click_.x())
                                        % 2 == 0) ? BLACK : WHITE;
            // Muutetaan värin nimi samaan muotoon kuin kuvatiedostoissa
            QString src_square_color = color_to_short(src_color);

            // Asetetaan lähtöruutuun tyhjä kuva
            QString src_image_path = PREFIX + "empty-" + src_square_color + SUFFIX;

            QPixmap src_image(src_image_path);
            from_button->setIcon(src_image);
            from_button->setIconSize(from_button->size());

            if ( chess_.get_game_state() == IN_PROGRESS ) {
                QString player = chess_.get_current_turn() ? "Black's turn" : "White's turn";
                info_label_->setText(player);
            }
            else if ( chess_.get_game_state() == BLACK_WIN ){
                info_label_->setText("Black won!");
                disable_board();
            }
            else if ( chess_.get_game_state() == WHITE_WIN ){
                info_label_->setText("White won!");
                disable_board();
            }
        }

        waiting_second_click_ = false;
        qDebug() << "second click in: " << col << ", " << row;
    }
}

void MainWindow::init_board()
{
    QWidget* central = new QWidget(this);
    QGridLayout* g_layout = new QGridLayout(central);

    // Luodaan info-teksti laudan yläpuolelle
    info_label_ = new QLabel("White's turn", this);
    info_label_->setObjectName("infoLabel");
    info_label_->setAlignment(Qt::AlignCenter);
    g_layout->addWidget(info_label_, 0, 0, 1, BOARD_SIZE);

    // Lisätään tyhjä rivi nappien ja laudan väliin, joka on vakio
    QSpacerItem* top_spacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    g_layout->addItem(top_spacer, 1, 0, 1, BOARD_SIZE);

    // Asetetaan niin, että ruudut ovat kiinni toisissaan
    g_layout->setSpacing(0);
    g_layout->setContentsMargins(0, 0, 0, 0);

    // Luodaan laudan jokaiseen ruutuun nappi ja tallennetaan ne button_grid_-arrayhyn
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            // Luodaan nappi ruutua varten, jotta sitä voi klikata
            QPushButton* button = new QPushButton(this);
            g_layout->addWidget(button, row + 2, col);
            button->setFixedSize(SQUARE_SIZE, SQUARE_SIZE);

            // Liitetään nappi (eli ruutu)
            connect(button, &QPushButton::clicked,
                    this, [this, row, col]() {on_square_clicked(row,col);
            });

            button_grid_[row][col] = button;

        }
    }

    // Asetetaan nappeihin oikeat kuvat
    update_icons();

    // Lisätään tyhjä rivi nappien ja laudan väliin, joka on vakio
    QSpacerItem* bottom_spacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    g_layout->addItem(bottom_spacer, 10, 0, 1, BOARD_SIZE);

    // Luodaan uusi peli -nappi
    QPushButton* restart_button = new QPushButton("Restart", this);
    g_layout->addWidget(restart_button, 11, 1, 1, 2);

    // Liitetään uusi peli -nappi
    connect(restart_button, &QPushButton::clicked, this,
            &MainWindow::on_restart_button_clicked);

    // Luodaan poistu-nappi
    QPushButton* quit_button = new QPushButton("Quit", this);
    g_layout->addWidget(quit_button, 11, 5, 1, 2);

    // Liitetään poistu-nappi
    connect(quit_button, &QPushButton::clicked, this, &MainWindow::close);

    setCentralWidget(central);
}

QString MainWindow::color_to_short(ChessColor color) const
{
    QString color_short;

    if (color == WHITE) {
        color_short = "wt";
    } else {
        color_short = "bl";
    }

    return color_short;
}

void MainWindow::highlight_possible_moves(set<Coord> moves)
{
    // Käydään moves-settin mahdolliset siirrot läpi ja korostetaan kyseisissä koordinaateissa olevat ruudut
    for (const Coord& move : moves) {
        unsigned int row = move.row;
        unsigned int col = move.col;

        QPushButton* button = button_grid_[row][col];
        button->setIconSize(button->size() * 0.9);
        button->setStyleSheet("border: 3px solid red;");
    }
}

void MainWindow::clear_highlights(set<Coord> moves)
{
    // Käydään moves-settin mahdolliset siirrot läpi ja poistetaan kyseisissä
    // koordinaateissa olevien ruutujen korostukset
    for (const Coord& move : moves) {
        unsigned int row = move.row;
        unsigned int col = move.col;

        QPushButton* button = button_grid_[row][col];
        button->setIconSize(button->size());
        button->setStyleSheet("");
    }
}

void MainWindow::disable_board()
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            button_grid_[row][col]->setEnabled(false);
        }
    }
}

void MainWindow::enable_board()
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            button_grid_[row][col]->setEnabled(true);
        }
    }
}

void MainWindow::update_icons()
{

    // Luodaan juokseva numero, jonka avulla voidaan vuorotella valkoisen ja mustan välillä
    unsigned int running_num = 1;

    // Käydään lauta läpi
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            // Vaihdellaa mustan ja valkoisen välillä
            ChessColor current_color = (running_num % 2 == 0) ? WHITE : BLACK;

            // Muutetaan värin nimi samaan muotoon kuin kuvatiedostoissa
            QString square_color = color_to_short(current_color);

            // Haetaan tarkasteltavassa olevassa ruudussa olevan nappulan tyyppi
            shared_ptr<ChessPiece> current_piece =
                    chess_.Chess::get_board().ChessBoard::get_piece_at(row, col);

            // Haetaan tarvittavan ruudun kuva
            QPixmap image;
            // Jos ruudussa ei ole nappulaa
            if (current_piece == nullptr) {

                image.load(PREFIX + "empty-" + square_color + SUFFIX);

            } else { // Jos ruudussa on nappula

                // Haetaan nappulan väri ja muutetaan se samaan muotoon kuin kuvatiedostoissa
                QString piece_color = color_to_short(current_piece->get_color());

                image.load(PREFIX + piece_color + "-" +
                           QString::fromStdString(current_piece->get_name())
                           + "-on-" + square_color + SUFFIX);
            }

            // Asetetaan nappiin ruudun kuva
            auto button = button_grid_[row][col];
            button->setIcon(image);
            button->setIconSize(button->size());

            running_num++;
        }
        running_num++;
    }
}
