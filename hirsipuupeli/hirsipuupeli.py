"""
COMP.CS.100 Programming 1
Name: Leevi Tiihonen
Email: leevi.tiihonen@tuni.fi
Task: gui_project

Hirsipuupeli

Perinteinen hirsipuupeli. Pelaajan täytyy arvata mikä sana/nimi on kyseessä kirjain kerrallaan (kuvan alapuolella on
vihje). Kysytty sana/nimi arvotaan valmiina olevasta sanakirjasta. Pelaaja pystyy myös valitsemaan kategorian itse valikosta vasemmasta yläkulmasta.
Kun valikkoa klikkaa, vaihtoehdot tulevat esiin. Kun valitsee kategorian täytyy vielä painaa valikon alla olevaa Select-nappia, jotta peli valitsee
kyseisen kategorian. Kategorioissa on myös tarjolla valinta 'Random category', joka arpoo pelaajalle kategorian. Kun tämä on valittuna kategoria
arvotaan uusiksi myös kun painetaan restart-nappia.

Pelaajalla on käytössä virtuaalinen näppäimistö, jonka näppäintä painamalla kirjaimia pystyy veikkaamaan.
Jos arvaus menee oikein, kirjain muuttuu vihreäksi ja arvattavasta sanasta paljastuu oikein arvattu kirjain. Jos arvaus menee väärin, kirjain muuttuu
punaiseksi ja kuva päivittyy (hirsipuu rakentuu lisää tai ukolle tulee lisää kehonosia), sekä pelaajalle tulee yksi virhe lisää. Joka tapauksessa
kirjainta ei voi painaa uudelleen kun sitä on kerran painettu. Kun pelaaja veikkaa sanan/nimen kaikki kirjaimet oikein, hän voittaa. Jos pelaaja
veikkaa väärää kirjainta 8 kertaa (hirsipuu rakennettu kokonaan ja ukolla kaikki kehonosat), hän häviää.
Molemmissa tapauksissa näytölle tulostuu voittiko vai hävisikö pelaaja.

Pelaajalle näytetään kuinka monta kertaa putkeen hän on voittanut putkeen ilman häviötä (pisteet).
Pelaajalle näytetään myös hänen parhaat pisteensä.
Pelin voi aloittaa alusta painamalla restart-nappia (pisteet ei nollaannu, paitsi jos pelaaja on juuri hävinnyt). Pelin voi sulkea quit-napista.

Tähtäsin projektissa kehittynempään versioon.
"""

from tkinter import *
import random

# Luodaan matriisi (lista, joka sisältää listoja), jossa kirjaimet ovat samassa järjestyksessä kuin näppäimistössä
LETTERS_FOR_KEYBOARD = [["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "Å"],
                        ["A", "S", "D", "F", "G", "H", "J", "K", "L", "Ö", "Ä"],
                        ["Z", "X", "C", "V", "B", "N", "M"]]

# Luodaan lista, joka sisältää kuvien tiedostonimet
IMAGE_FILES = ["Hirsipuupeli_0.gif", "Hirsipuupeli_1.gif", "Hirsipuupeli_2.gif", "Hirsipuupeli_3.gif",
               "Hirsipuupeli_4.gif", "Hirsipuupeli_5.gif", "Hirsipuupeli_6.gif", "Hirsipuupeli_7.gif",
               "Hirsipuupeli_8.gif"]

# Luodaan sanakirja, jonka key sisältää kategorian ja niiden arvoina on listoja, jotka sisältävät arvattavia sanoja.
# Jos peliin haluaa lisätä arvattavia sanoja, täytyy ne lisätä tähän sanakirjaan!
# Myös uusia kategorioita voi luoda.
WORDS = {
    "A fictional character": [
        "Batman", "Robin", "Joker", "Riddler", "Penguin", "Catwoman",
        "Alfred Pennyworth", "James Gordon", "Harley Quinn", "Two-Face",
        "Scarecrow", "Bane", "Poison Ivy", "Ra's Al Ghul", "Harry Potter",
        "Hermione Granger", "Ron Weasley", "Albus Dumbledore", "Severus Snape",
        "Minerva McGonagall", "Rubeus Hagrid", "Ginny Weasley", "Voldemort",
        "Tom Riddle", "Bellatrix Lestrange", "Lucius Malfoy", "Draco Malfoy",
        "Neville Longbottom", "Luna Lovegood", "Sirius Black", "Remus Lupin",
        "Dobby", "Sherlock Holmes", "Frodo Baggins", "Bilbo Baggins", "Gollum",
        "Katniss Everdeen", "Luke Skywalker", "Anakin Skywalker", "Obi-Wan Kenobi",
        "Qui-Gon Jinn", "Darth Vader", "Princess Leia", "Han Solo", "Yoda",
        "Darth Sidious", "Chewbacca", "R2-D2", "C-3PO", "Kylo Ren", "Darth Maul",
        "Mace Windu", "Padme Amidala", "Boba Fett", "Jango Fett", "Jabba The Hutt",
        "Jar Jar Binks", "Gandalf", "Legolas", "Sauron", "Spider-Man", "Superman",
        "Indiana Jones", "Hulk", "Wonder Woman", "Iron Man", "Donald Duck"
    ],
    "A movie director": [
        "Christopher Nolan", "Quentin Tarantino", "Alfred Hitchcock", "Steven Spielberg",
        "Martin Scorsese", "Stanley Kubrick", "Chris Columbus", "Alfonso Cuaron",
        "Mike Newell", "David Yates", "Matt Reeves"
    ],
    "An actor": [
        "Ryan Gosling", "Chris Pratt", "Christian Bale", "Robert Pattison", "Heath Ledger",
        "Daniel Radcliffe", "Rupert Grint", "Johnny Depp", "Tom Cruise", "Leonardo DiCaprio",
        "Brad Pitt", "Keanu Reeves", "Joaquin Phoenix", "Hugh Jackman", "Jackie Chan",
        "Emma Watson", "Sydney Sweeney", "Margot Robbie", "Jennifer Lawrence", "Liam Hemsworth"
    ]
}


class GameGUI:
    """
    Tämä luokka mallintaa hirsipuupeliä
    """

    def __init__(self):
        """
        Rakentaja. Luo käyttöliittymän
        """

        self.__mainwindow = Tk()
        self.__mainwindow.title("Hirsipuupeli")

        # Luodaan muuttuja, joka pitää kirjaa montako sanaa pelaaja on saanut oikein putkeen
        self.__points_in_a_row = 0

        # Luodaan muuttuja, joka pitää kirjaa ennätyspisteistä
        self.__high_score = 0

        # Luodaan muuttuja, joka pitää kirjaa siitä, mikä kategoria on valittuna. Tämän avulla saadaan tehtyä niin, että kun pelaaja klikkaa
        # restart-nappia, peli käyttää viimeksi valittua kategoriaa. Asetetaan sen alkuarvoksi None, jotta peli arpoo ensimmäisen kategorian
        self.__current_category = None

        # Asetetaan käyttäjän valinta -muuttujan arvoksi None, jotta pystytään käsittelemään tilannetta, jossa käyttäjää ei valitse kategoriaa ja
        # klikkaa restart-nappia
        self.__selected_category = None

        # Luodaan PhotoImage-objekteja kuvatiedostoista ja tallennetaan ne listaan
        self.__images = []

        for image_file in IMAGE_FILES:

            new_image = PhotoImage(file=image_file)
            self.__images.append(new_image)

        # Alustaa pelin
        self.initialize_game()

        self.__mainwindow.mainloop()

    def initialize_game(self, chosen_category=None):
        """
        Alustaa pelin. Tämä on tehty funktiossa, koska muuten sama koodi pitäisi kirjoittaa kahteen eri kohtaan.
        :param chosen_category: str, kertoo funktiolle mistä kategorista kysyttävä sana tulee arpoa
        """

        # Luodaan valikko
        self.category_dropdown()

        # Näytetään pelaajalle pistetilanne
        self.__points_in_a_row_label = Label(self.__mainwindow, text=f"{self.__points_in_a_row} in a row")
        self.__points_in_a_row_label.grid(row=1, column=1, columnspan=12)

        # Näytetään pelaajalle, mikä hänen ennätyksensä on
        self.__high_score_label = Label(self.__mainwindow, text=f"High score: {self.__high_score}")
        self.__high_score_label.grid(row=0, column=1, columnspan=12)

        # Asetetaan aloituskuva näkyville
        self.__image_label = Label(self.__mainwindow, image=self.__images[0])
        self.__image_label.grid(row=2, column=1, columnspan=12)

        # Käytetään valittua kategoriaa ja arvotaan sieltä kysyttävä sana. Jos kategoriaa ei ole valittu, arvotaan kategoria.
        if chosen_category:

            self.__current_category = chosen_category

        else:

            # Tehdään sanakirjan avaimista (kategorioista) lista, jotta niistä voidaan arpoa, ja arvotaan kategoria
            self.__current_category = random.choice(list(WORDS.keys()))

        # Tulostetaan kategoria pelaajalle
        self.__category_label = Label(self.__mainwindow, text=self.__current_category)
        self.__category_label.grid(row=3, column=1, columnspan=12)

        # Arvotaan mitä kategorian sanoista kysytään
        self.__word = random.choice(WORDS[self.__current_category]).upper()

        # Luodaan tyhjä joukko, johon tullaan tallentamaan oikeat kirjaimet. Käytetään joukkoa, jotta jokainen
        # kirjain voi olla siinä vain kerran
        self.__correct_letters = set()

        # Luodaan tyhjä joukko, johon tullaan tallentamaan kaikki arvatut kirjaimet
        self.__guessed_letters = set()

        # Luodaan tyhjä lista kysytyn sanan symboleille
        word_in_a_list = []

        # Tallennetaan kysytyn sanan symbolit luotuun listaan
        for symbol in self.__word:

            word_in_a_list.append(symbol)

            if symbol.isalpha():

                # Tallennetaa kysytyn sanan kaikki kirjaimet joukkoon
                self.__correct_letters.add(symbol)

        # Luodaan lista, johon tallennetaan miltä sana tulee näyttämään käyttöliittymässä alussa
        word_at_start_list = []

        # Luodaan sanan labelille merkkijono, jossa kirjainten tilalla on _-symboli ja välilyönnit ja muut symbolit
        # ovat ennallaan
        for symbol in word_in_a_list:

            if symbol.isalpha():

                word_at_start_list.append("_")

            else:

                word_at_start_list.append(symbol)

        # Tehdään luodusta listasta merkkijono
        word_at_start = " ".join(word_at_start_list)

        # Luodaan sanan labeli
        self.__word_label = Label(self.__mainwindow, text=word_at_start)
        self.__word_label.grid(row=4, column=1, columnspan=12)

        # Luodaan muuttuja, joka pitää kirjaa monta kertaa käyttäjä on arvannut väärin
        self.__mistakes = 0

        # Asetetaan virheille maksimiraja ja tallennetaan se muuttujaan
        self.__max_mistakes = 8

        # Luodaan tyhjä labeli, jotta näppäimistöä saadaan vähän irti veikattavasta sanasta
        self.__empty_label = Label(self.__mainwindow, text="")
        self.__empty_label.grid(row=5, column=0)

        # Luodaan virtuaalinen näppäimistö
        self.create_keyboard()

        # Luodaan tyhjä labeli, jotta restart- ja quit-nappeja saadaan vähän irti näppäimistöstä
        self.__empty_label_2 = Label(self.__mainwindow, text="")
        self.__empty_label_2.grid(row=9, column=0)

        # Luodaan nappi, josta voi aloittaa pelin alusta
        self.__restart_button = Button(self.__mainwindow, text="Restart", command=self.restart)
        self.__restart_button.grid(row=10, column=4, columnspan=2)

        # Luodaan nappi, josta voi sulkea pelin
        self.__quit_button = Button(self.__mainwindow, text="Quit", command=self.close_game)
        self.__quit_button.grid(row=10, column=6, columnspan=3, stick=W)

    def create_keyboard(self):
        """
        Luo virtuaalisen näppäimistön
        """

        # Luodaan matriisi (listoja listan sisällä) johon tallennetaan luotujen nappien paikat. Aluksi matriisista
        # tehdään täysin sama kuin LETTERS_FOR_KEYBOARD-matriisista, mutta jokaisen arvon paikalla on None
        self.__buttons = [[None for _ in row] for row in LETTERS_FOR_KEYBOARD]

        # Luodaan näppäimistö
        for x, row in enumerate(LETTERS_FOR_KEYBOARD):
            for y, key in enumerate(row):

                # Funktio joka händlää napin painalluksen
                def button_press(letter=key, x_chord=x, y_chord=y):
                    self.guess(letter, x_chord, y_chord)

                # Luodaan nappi jokaiselle kirjaimelle
                new_button = Button(self.__mainwindow, text=key, command=button_press)

                # Tallennetaan luotu nappi listaan sille kuuluvalle paikalle
                self.__buttons[x][y] = new_button

                # Asetetaan kirjaimet samaan järjestykseen, kuin ne ovat listoissakin,
                # alimmalla rivillä on neljä kirjainta vähemmän kuin ylemmillä, joten siirretään jokaista kirjainta
                # kaksi saraketta oikealle, jolloin alimmalla rivillä molempiin reunoihin jää kaksi tyhjää saraketta
                if x != 2:

                    new_button.grid(row=x+6, column=y+1)

                else:

                    new_button.grid(row=x+6, column=y+3)

    def guess(self, key, x, y):
        """
        Tarkistaa, onko painetun napin kirjain kysytyssä sanassa. Jos on nappi, muuttuu vihreäksi;
        jos ei, nappi muuttuu punaiseksi. Kun nappia on painettu, sitä ei voi painaa uudelleen.

        :param key: str, napin sisältämä kirjain
        :param x: int, monennessako listassa nappi sijaitsee buttons-matriisissa (0-2)
        :param y: int, mikä napin indexi on sen omassa listassa
        """

        # Tallennetaan kyseessä oleva nappi muuttujaan, sen käyttämisen helpottamiseksi
        activated_button = self.__buttons[x][y]

        if key in self.__correct_letters:

            # Jos arvattu kirjain on sanassa, kyseisen kirjaimen nappi muuttuu vihreäksi ja sitä ei voi enää painaa
            activated_button.configure(background="green", state=DISABLED)

            # Lisätään oikein arvattu kirjain arvattujen kirjainten joukkoon
            self.__guessed_letters.add(key)

            # Päivitetään arvattavan sanan labeli oikean näköiseksi
            self.update_word_label()

            # Tarkistetaan onko sanan kaikki kirjaimet veikattu oikein
            if len(self.__guessed_letters) == len(self.__correct_letters):
                self.end_game(True)

        else:

            # Jos arvattu kirjain ei ole sanassa, kyseisen kirjaimen nappi muuttuu punaiseksi ja sitä ei voi enää painaa
            activated_button.configure(background="red", state=DISABLED)

            # Lisätään virheisiin 1
            self.__mistakes += 1

            # Päivitetään kuvaa virheiden määrän perusteella
            self.__image_label.configure(image=self.__images[self.__mistakes])

            # Tarkistetaan onko pelaajan virheet täynnä
            if self.__mistakes >= self.__max_mistakes:

                self.end_game(False)

    def update_word_label(self):
        """
        Päivittää sanan labelin korvaten alaviivat oikein arvatuilla kirjaimilla
        """

        # Luodaan tyhjä lista arvattavan sanan labelia varten
        letter_list = []

        # Jos kirjain on arvattu, listaan lisätään oikein arvattu kirjain. Jos kirjainta ei ole arvattu listaan lisätään
        # _-symboli
        for letter in self.__word:

            if letter in self.__guessed_letters or not letter.isalpha():

                letter_list.append(letter)

            else:

                letter_list.append("_")

        # Muodostetaan listasta merkkijono, jossa merkkien välissä on välilyönti, jotta alaviivat erottaa toisistaan,
        # eli pelaaja tietää montako kirjainta sanassa on
        display_word = " ".join(letter_list)

        # Päivitetään sanan labeli
        self.__word_label.configure(text=display_word)

    def end_game(self, won):
        """
        Näyttää viestin pelin päättymisestä.

        :param won: boolean, True, jos pelaaja sai arvattua sanan. False, jos pelaaja on arvannut liian monta kertaa
                    väärin
        """

        # Tarkistaa voittiko käyttäjä ja luo viestin sen perusteella
        if won:

            # Lisätään piste
            self.__points_in_a_row += 1

            # Päivitetään pelaajalle pistetilanne, jo ennen restart nappulan painamista
            self.__points_in_a_row_label.configure(text=f"{self.__points_in_a_row} in a row")

            result_label = Label(self.__mainwindow, text="You won!", foreground="green")

            # Tarkistetaan tarvitseeko ennätyspisteitä päivittää, jos tarvitsee päivitetään se pelaajalle näkyviin,
            # jo ennen restart-napin painamista
            if self.__points_in_a_row > self.__high_score:
                self.__high_score = self.__points_in_a_row
                self.__high_score_label.configure(text=f"High score: {self.__high_score}")

        else:

            # Nollataan pisteet
            self.__points_in_a_row = 0

            result_label = Label(self.__mainwindow, text="You lost!", foreground="red")

            # Näytetään pelaajalle mikä kysytty sana olisi ollut
            self.__word_label.configure(text=self.__word)

        result_label.grid(row=2, column=1, columnspan=12)

        # Lukitaan kirjainnapit, jotta niitä ei voi painaa
        for row in self.__buttons:
            for button in row:
                button.configure(state=DISABLED)

    def category_dropdown(self):
        """
        Luo kategoria-valikon ja liittää sen käyttöliittymään
        """

        # Luodaan lista kategorioista
        categories = list(WORDS.keys())

        # Lisätään valikkoon 'Random category', jolla voi arpoa kategorian
        categories.append("Random category")

        # Luodaan muuttuja, joka käyttää Tkinter- kirjaston StringVar-luokkaa. Käytetään StringVar-luokkaa, koska se tarjoaa menetelmän päivittää ja
        # seurata merkkijonon arvoa
        self.__category_choice = StringVar()

        # Luodaan dropdown-valikko, jossa on vaihtoehtona kaikki kategoriat
        dropdown = OptionMenu(self.__mainwindow, self.__category_choice, *categories)
        dropdown.grid(row=0, column=0)

        # Luodaan nappi, jolla voidaan vahvistaa valinta
        select_button = Button(self.__mainwindow, text="Select", command=self.select_category)
        select_button.grid(row=1, column=0)

    def select_category(self):
        """
        Metodi, jolla käyttäjä voi valita kategorian
        """

        # Tuhotaan pääikkunan lapset, jotta objektit eivät mene päällekkäin
        self.destroy_children()

        # Haetaan käyttäjän valitsema kategoria ja tallennetaan se muuttujaan
        chosen_category = self.__category_choice.get()

        # Päivitetään valittu kategoria. Tehdään se sillein, että jos pelaaja haluaa arpoa kategorian, niin ohjelma muistaa sen ja arpoo uuden
        # kategorian aina kun klikkaa restart-nappia
        self.__selected_category = chosen_category

        # Tarkistetaan haluaako pelaaja arpoa kategorian
        if chosen_category == "Random category":
            chosen_category = random.choice(list(WORDS.keys()))

        # Tallennetaan arvottu kategoria muuttujaan
        self.__current_category = chosen_category

        # Alustetaan peli uudelleen valitulla kategorialla
        self.initialize_game(chosen_category)

    def destroy_children(self):
        """
        Tuhoaa jokaisen pääikkunan lapsen
        """

        for widget in self.__mainwindow.winfo_children():
            widget.destroy()

    def restart(self):
        """
        Käynnistää ohjelman alusta
        """

        # Tuhotaan pääikkunan lapset, jotta objektit eivät mene päällekkäin
        self.destroy_children()

        # Haetaan pelaajan valitsema kategoria ja tallennetaan se muuttujaan
        category_now = self.__selected_category

        # Tarkistetaan onko pelaaja valinnut satunnaisen kategorian
        if category_now == "Random category":

            # Jos pelaaja haluaa arpoa kategorian, kategoria arvotaan joka kerta uudelleen kun klikataan restart-nappia
            randomized_category = random.choice(list(WORDS.keys()))
            self.initialize_game(randomized_category)

        elif not category_now:

            # Jos käyttäjä ei valitse kategoriaa ja klikkaa restart-nappia, arvotaan uusi kategoria
            randomized_category = random.choice(list(WORDS.keys()))
            self.initialize_game(randomized_category)

        else:

            # Jos pelaaja ei halua arpoa kategoriaa, käytetään pelaajan valitsemaa kategoriaa, kun klikataan restart-nappia
            self.initialize_game(self.__current_category)

    def close_game(self):
        """
        Sulkee ohjelman
        """

        self.__mainwindow.destroy()


def main():
    GameGUI()


if __name__ == "__main__":
    main()
