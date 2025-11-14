"""
COMP.CS.100 Ohjelmointi 1
            Programming 1

Ohjelma on perinteisestä laivanupotuspelistä yksinpeliversio, jossa pelaaja yrittää upottaa tietokoneen laivoja

Firstname Lastname: Leevi Tiihonen
Email: leevi.tiihonen@tuni.fi
"""


class Ship:
    """
    Tämä luokka mallintaa laivanupotuspelissä olevaa laivaa
    """

    def __init__(self, name, coordinates):
        """
        Rakentaja. Antaa laivalle nimen sekä koordinaatit

        :param name: str, laivan nimi
        :param coordinates: list, sisältää laivan koordinaatit
        """

        self.__coordinates = coordinates
        self.__name = name

        # Alla olevan listan avulla pidetään kirjaa siitä mihin laivan koordinaatteihin on osuttu
        self.__hits = [False] * len(coordinates)

    def get_name(self):
        """
        Palauttaa laivan nimen

        :return: str, laivan nimi
        """

        return self.__name

    def get_coordinates(self):
        """
        Palauttaa laivan koordinaatit

        :return: list, laivan koordinaatit
        """

        return self.__coordinates

    def printout(self):
        """
        Tulostaa laivan nimen ja sen koordinaatit, testaus tarkoitukseen
        """

        print(f"Ship's name: {self.__name}")
        print(f"Coordinates: {self.__coordinates}")

    def hit(self, coordinate):
        """
        Tarkistaa osuuko ammus laivaan. Jos osuu, niin se tallennetaan self.__hits listaan kyseisen
        koordinaatin paikalle. Palautaa True, jos laivaan osuttiin.

        :param coordinate: str, koordinaatti, johon ammuttiin
        """

        if coordinate in self.__coordinates:

            index = self.__coordinates.index(coordinate)
            self.__hits[index] = True

            return True

        else:

            return False

    def is_ship_sunk(self):
        """
        Tarkistaa onko laiva uponnut

        :return: boolean, True, jos laivan jokaiseen koordinaattiin on osuttu, eli self.__hits on täynnä True-arvoja
        """

        # all-metodi palauttaa True, jos lista sisältää pelkästään True-arvoja
        return all(self.__hits)


def read_file(file_name):
    """
    Tämä funktio lukee sille syötetyn tiedoston ja luo listan laivaolioista

    :param file_name: str, tiedoston nimi, joka halutaan lukea
    :return: list, lista laivaolioista
    """

    # Luodaan tyhjä lista, johon tallennetaan kaikki laivat
    ships = []

    # Luodaan tyhjä lista, johon tallenetaan kaikki koordinaatit.
    # Tämän avulla tarkistetaan, ettei tule päällekkäisyyksiä
    all_coordinates = []

    try:
        data_file = open(file_name, mode="r")

        for line in data_file:

            line = line.rstrip()

            # Jaetaan rivin tiedot listaan, ekana laivan nimi ja sen jälkeen koordinaatit
            fields = line.split(";")

            # Tallennetaan laivan nimi muuttujaan, jotta sitä on helpompi käyttää.
            ship = fields[0]

            # Luodaan tyhjä lista koordinaateille.
            coordinates = []

            # Tarkistetaan onko koordinaatit oikeassa muodossa ja, että ei ole päällekkäisyyksiä
            for coordinate in fields[1:]:

                if coordinate[0] > "J":

                    print("Error in ship coordinates!")

                    return None

                elif int(coordinate[1:]) < 0 or int(coordinate[1:]) > 9:

                    print("Error in ship coordinates!")

                    return None

                elif coordinate in all_coordinates:

                    print("There are overlapping ships in the input file!")

                    return None

                else:

                    coordinates.append(coordinate)
                    all_coordinates.append(coordinate)

            ships.append(Ship(ship, coordinates))

        data_file.close()

        return ships

    except OSError:

        print("File can not be read!")

        return None


def create_board():
    """
    Luo 10x10 tyhjän pelilaudan

    :return: list, lista, joka sisältää listoja
    """

    return [[" "] * 10 for _ in range(10)]


def print_board(board):
    """
    Lisää pelilaudalle koordinaatistot ja tulostaa sen jälkeen pelilaudan

    :param board: list, lista, joka sisältää listoja
    """

    letters = "  A B C D E F G H I J"
    print(letters)

    for i, row in enumerate(board):

        print(f"{i} {' '.join(row)} {i}")

    print(letters)


def main():
    file_name = input("Enter file name: ")
    ships = read_file(file_name)

    if ships is not None:
        # Luodaan pelilauta
        board = create_board()
        print("")

        # Luodaan silmukka, jonka sisällä peliä pelataan
        while True:

            print_board(board)
            print("")

            # Käyttäjän syöte
            command = input("Enter place to shoot (q to quit): ").strip().upper()

            # Jos käyttän syöte on "Q", peli lopettaa välittömästi toimintansa
            if command == "Q":

                print("Aborting game!")

                break

            elif len(command) != 2 or not command[0].isalpha() or not command[1].isdigit():

                print("Invalid command!")
                print("")

                continue

            # Tarkistetaan onko koordinaatit oikeassa muodossa
            elif command[0] > "J" or int(command[1:]) < 0 or int(command[1:]) > 9:

                print("Invalid command!")
                print("")

                continue

            # Muutetaan syöte numeraalisiksi koordinaateiksi, jotta sille voidaan löytää paikka laudalta.
            # ord muuttaa annetun merkin vastaavaksi Unicode- tai ASCII-arvoksi. Kun tästä vähennetää ord("A"):n arvo,
            # saadaan käyttäjän antaimen kirjaimen sijainti 0-indeksoidussa järjestelmässä.
            x = ord(command[0]) - ord("A")
            y = int(command[1])

            # Tarkistetaan, onko jo annettuihin koordinaatteihin ammuttu
            if board[y][x] != " ":

                print("Location has already been shot at!")
                print("")

                continue

            # Luodaan muuttuja, joka pitää kirjaa siitä osuiko ammus vai ei
            hit = False

            # Tarkistetaan alla olevalla silmukalla onko käyttäjän antamissa koordinaateissa mitään laivaa
            for ship in ships:

                if ship.hit(command):

                    hit = True

                    # Aina kun tulee osuma täytyy tarkistaa upposiko laiva kokonaan
                    if ship.is_ship_sunk():

                        print(f"You sank a {ship.get_name()}!")

                        # Laudalla olevat merkinnät pitää muuttaa laivan alkukirjaimeksi, kun laiva uppoaa
                        for coordinate in ship.get_coordinates():
                            # Muutetaan taas koordinaatit numeraalisiksi aina kyseessä olevan koordinaatin kohdalla
                            x2 = ord(coordinate[0].upper()) - ord("A")
                            y2 = int(coordinate[1])

                            # Vaihdetaan laudalla oleva merkintä vastaamaan upotetun laivan ensimmäistä kirjainta
                            board[y2][x2] = ship.get_name()[0].upper()
                    else:
                        # Jos laiva ei uppoa, täytyy sitä merkitä X-symbolilla
                        board[y][x] = "X"
                    break

            # Jos osumaa ei tule täytyy sitä merkitä *-symbolilla
            if not hit:

                board[y][x] = "*"

            # Tarkistetaan onko jokainen laiva uponnut
            if all(ship.is_ship_sunk() for ship in ships):

                print("")
                print_board(board)
                print("")
                print("Congratulations! You sank all enemy ships.")

                break

            print("")



if __name__ == "__main__":
    main()
