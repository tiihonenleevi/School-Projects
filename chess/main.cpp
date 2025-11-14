/* Shakki
 *
 *  Säännöt ja toteutus löytyy instructions.txt-tiedostosta
 *
 * Ohjelman kirjoittajat:
 * Nimi: Leevi Tiihonen
 * Palautteen kieli (fi/en): fi
 *
 * Nimi: Joose Haippo
 * Palautteen kieli (fi/en): fi
 * */
#include "mainwindow.hh"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
