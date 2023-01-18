#include<iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>



class macierz {
public:
    // konstruktor

    

    macierz(int wiersze, int kolumny);



   


    //fasada
    // metoda importująca dane z pliku
    void ImportujZPliku(const string &nazwaPliku) {
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cerr << "Nie udało się otworzyć pliku " << nazwaPliku << endl;
            return;
        }

        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                plik >> dane[i][j];
            }
        }

        plik.close();
    }

    // metoda eksportująca dane do pliku
    void ExportujDoPliku(const string &nazwaPliku) {
        ofstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cerr << "Nie udało się otworzyć pliku " << nazwaPliku << endl;
            return;
        }

        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                plik << dane[i][j] << " ";
            }
            plik << endl;
        }

        plik.close();
    }



    // metody
    void wypisz();
    void transponuj();
    bool odwroc();
    int wyznacznik() const;
    // metoda do obliczania wyznacznika macierzy
   
    int stopien() const;



 // metoda do dodawania macierzy
    macierz Dodaj(const macierz& druga) {
        if (wiersze != druga.wiersze || kolumny != druga.kolumny) {
            cerr << "Macierze mają różne rozmiary." << endl;
            return macierz(0,0);
        }

        macierz wynik(wiersze, kolumny);
        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                wynik.dane[i][j] = dane[i][j] + druga.dane[i][j];
            }
        }

        return wynik;
    }   


 // metoda do odejmowania macierzy
    macierz Odejmij(const macierz& druga) {
        if (wiersze != druga.wiersze || kolumny != druga.kolumny) {
        cerr << "Macierze mają różne rozmiary." << endl;
        return macierz(0,0);
        }
    
            macierz wynik(wiersze, kolumny);
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            wynik.dane[i][j] = dane[i][j] - druga.dane[i][j];
        }
    }

    return wynik;
}


private:
    // pola
    int** dane;
    int wiersze;
    int kolumny;
};

int macierz::stopien() const {
    int stopien = 0;
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            if (dane[i][j] != 0) {
                stopien = max(stopien, i+j);
            }
        }
    }
    cout << "Stopień macierzy: " << stopien << endl;
    return stopien;
}



int macierz::wyznacznik() const {
        // sprawdzamy, czy macierz jest kwadratowa
        if (wiersze != kolumny) {
            cerr << "Macierz nie jest kwadratowa." << endl;
            return 0;
        }

        // obliczamy wyznacznik metodą Laplace'a
        int det = 1;
        for (int i = 0; i < wiersze; i++) {
            // szukamy największego elementu w kolumnie i
            int max_wiersz = i;
            for (int j = i+1; j < wiersze; j++) {
                if (abs(dane[j][i]) > abs(dane[max_wiersz][i])) {
                    max_wiersz = j;
                }
            }

            // jeśli największy element jest równy 0, to wyznacznik jest równy 0
            if (dane[max_wiersz][i] == 0) {
                return 0;
            }

            // zamieniamy wiersze, jeśli trzeba
            if (max_wiersz != i) {
                det *= -1;
                swap(dane[i], dane[max_wiersz]);
            }

            // eliminujemy elementy poniżej i
            for (int j = i+1; j < wiersze; j++) {
                int f = dane[j][i] / dane[i][i];
                for (int k = i; k < wiersze; k++) {
                    dane[j][k] -= f * dane[i][k];
                }
            }
        }

        // mnożymy elementy na przekątnej
        for (int i = 0; i < wiersze; i++) {
            det *= dane[i][i];
        }

        return det;
    }




// implementacja konstruktora z parametrami
macierz::macierz(int wiersze, int kolumny) {
    // tworzymy macierz o podanych wymiarach
    this->wiersze = wiersze;
    this->kolumny = kolumny;
    dane = new int*[wiersze];
    for (int i = 0; i < wiersze; i++) {
        dane[i] = new int[kolumny];
    }

    // ustawiamy seed generatora liczb pseudolosowych
    srand(time(NULL));

    // wypełniamy macierz losowymi wartościami
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            dane[i][j] = rand() % 10 + 1;
        }
    }
}



void macierz::transponuj() {
        int** nowe_dane = new int*[kolumny];
        for (int i = 0; i < kolumny; i++) {
            nowe_dane[i] = new int[wiersze];
        }

        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                nowe_dane[j][i] = dane[i][j];
            }
        }

        // zamieniamy wskazania na dane
        delete[] dane;
        dane = nowe_dane;

        // zamieniamy liczbę wierszy i kolumn
        int tmp = wiersze;
        wiersze = kolumny;
        kolumny = tmp;
    }



// metoda do obliczania macierzy odwrotnej
// metoda ta zwraca true jeśli dla danej macierzy
//można znaleźć macierz odwrotną. Natomiast metoda
//zwraca false jeśli nie można znaleźć macierzy
//odwrotnej, czyli jeśli macierz jest osobliwa.
    bool macierz::odwroc() {
        // tworzymy wektory do przechowywania permutacji wierszy
        vector<int> p(wiersze);
        for (int i = 0; i < wiersze; i++) {
            p[i] = i;
        }
        // obliczamy rozkład LU
        for (int i = 0; i < wiersze; i++) {
            // szukamy pivotu
            int pivot = i;
            for (int j = i + 1; j < wiersze; j++) {
                if (fabs(dane[p[j]][i]) > fabs(dane[p[pivot]][i])) {
                    pivot = j;
                }
            }
        // zamieniamy wiersze, jeśli pivot nie jest na swoim miejscu
            if (pivot != i) {
                swap(p[i], p[pivot]);
            }

            // obliczamy elementy macierzy L i U
            for (int j = i + 1; j < wiersze; j++) {
                dane[p[j]][i] /= dane[p[i]][i];
                for (int k = i + 1; k < wiersze; k++) {
                    dane[p[j]][k] -= dane[p[j]][i] * dane[p[i]][k];
                }
            }
        }
        // tworzymy macierz odwrotną jako macierz jednostkową
        macierz odwrotna(wiersze, kolumny);
        for (int i = 0; i < wiersze; i++) {
            odwrotna.dane[i][i] = 1;
        }

        // obliczamy macierz odwrotną za pomocą rozwiązywania układów równań
        for (int i = wiersze - 1; i >= 0; i--) {
            for (int j = i - 1; j >= 0; j--) {
                for (int k = wiersze - 1; k >= i; k--) {
                    odwrotna.dane[p[j]][k] -= dane[p[j]][i] * odwrotna.dane[p[i]][k];
                }
            }
        }
        // mnożymy macierz odwrotną przez elementy na przekątnej macierzy U
        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < wiersze; j++) {
                odwrotna.dane[i][j] /= dane[p[i]][i];
            }
        }

        // zamieniamy macierze
        *this = odwrotna;
        return true;
    }


void macierz::wypisz() {
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            cout << dane[i][j] << " ";
        }
        cout << endl;
    }

}

int main()
{
  
    int wybor;
    int liczbaMacierzy;
    cout << "Ile macierzy chcesz utworzyc (minimum 2)?: ";
    cin >> liczbaMacierzy;

    if (liczbaMacierzy < 2) {
        cout << "Musisz utworzyc conajmniej 2 macierze" << endl;
        return 0;
    }
    vector<macierz> macierze;
    for (int i = 0; i < liczbaMacierzy; i++) {
        int wiersze, kolumny;
        cout << "Podaj liczbe wierszy dla macierzy " << i + 1 << ": ";
        cin >> wiersze;
        cout << "Podaj liczbe kolumn dla macierzy " << i + 1 << ": ";
        cin >> kolumny;
        macierze.push_back(macierz(wiersze, kolumny));
    }

    while(true) {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Wypisz macierze" << endl;
        cout << "2. Transponuj macierze" << endl;
        cout << "3. Odwroc macierze" << endl;
        cout << "4. Oblicz wyznacznik macierzy" << endl;
        cout << "5. Oblicz stopien macierzy" << endl;
        cout << "6. Dodaj macierze" << endl;
        cout << "7. Odejmij macierze" << endl;
        cout << "8. Importuj dane z pliku" << endl;
        cout << "9. Exportuj dane do pliku" << endl;
        cout << "10. Wyjdz z programu" << endl;
        cin >> wybor;

        if (wybor == 1) {
            for (int i = 0; i < liczbaMacierzy; i++) {
                cout << "Macierz " << i + 1 << ":" << endl;
                macierze[i].wypisz();
                cout << endl;
            }
        } else if (wybor == 2) {
            for (int i = 0; i < liczbaMacierzy; i++) {
                cout << "Macierz " << i + 1 << " po transponowaniu:" << endl;
                macierze[i].transponuj();
                macierze[i].wypisz();
                cout << endl;
            }
        } else if (wybor == 3) {
            for (int i = 0; i < liczbaMacierzy; i++) {
                if (macierze[i].odwroc()) {
                    cout << "Macierz " << i + 1 << " po odwroceniu:" << endl;
                    macierze[i].wypisz();
                    cout << endl;
                }
               else {
                    cout << "Macierz " << i + 1 << " jest nieodwracalna." << endl;
}
}
} else if (wybor == 4) {
for (int i = 0; i < liczbaMacierzy; i++) {
cout << "Wyznacznik macierzy " << i + 1 << ": " << macierze[i].wyznacznik() << endl;
}
} else if (wybor == 5) {
for (int i = 0; i < liczbaMacierzy; i++) {
cout << "Stopien macierzy " << i + 1 << ": " << macierze[i].stopien() << endl;
}
} else if (wybor == 6) {
int macierz1, macierz2;
cout << "Wybierz pierwsza macierz do dodania (1-" << liczbaMacierzy << "): ";
cin >> macierz1;
cout << "Wybierz druga macierz do dodania (1-" << liczbaMacierzy << "): ";
cin >> macierz2;
if (macierz1 < 1 || macierz1 > liczbaMacierzy || macierz2 < 1 || macierz2 > liczbaMacierzy) {
cout << "Nieprawidlowy wybor macierzy" << endl;
continue;
}
macierz wynik = macierze[macierz1 - 1].Dodaj(macierze[macierz2 - 1]);
cout << "Wynik dodawania:" << endl;
wynik.wypisz();
cout << endl;
} else if (wybor == 7) {
int macierz1, macierz2;
cout << "Wybierz pierwsza macierz do odejmowania (1-" << liczbaMacierzy << "): ";
cin >> macierz1;
cout << "Wybierz druga macierz do odejmowania (1-" << liczbaMacierzy << "): ";
cin >> macierz2;
if (macierz1 < 1 || macierz1 > liczbaMacierzy || macierz2 < 1 || macierz2 > liczbaMacierzy) {
cout << "Nieprawidlowy wybor macierzy" << endl;
continue;
}
macierz wynik = macierze[macierz1 - 1].Odejmij(macierze[macierz2 - 1]);
cout << "Wynik odejmowania:" << endl;
wynik.wypisz();
cout << endl;
} else if (wybor == 8) {
int macierz1;
string nazwaPliku;
cout << "Wybierz macierz, która ma zaimportować dane (1-" << liczbaMacierzy << "): ";
cin >> macierz1;
cout << "Podaj nazwę pliku do importu: ";
cin >> nazwaPliku;
if (macierz1 < 1 || macierz1 > liczbaMacierzy) {
cout << "Nieprawidlowy wybor macierzy" << endl;
continue;
}
macierze[macierz1 - 1].ImportujZPliku(nazwaPliku);
cout << "Dane z pliku " << nazwaPliku << " zaimportowane do macierzy " << macierz1 << endl;
} else if (wybor == 9) {
            int macierz1;
            string nazwaPliku;
            cout << "Wybierz macierz, która ma zaimportować dane (1-" << liczbaMacierzy << "): ";
            cin >> macierz1;
            cout << "Podaj nazwę pliku do exportu (z rozszerzeniem .txt): ";
            cin >> nazwaPliku;
            if (macierz1 < 1 || macierz1 > liczbaMacierzy) {
                cout << "Nieprawidlowy wybor macierzy" << endl;
                continue;
            }
            if(nazwaPliku.find(".txt") == string::npos){
                nazwaPliku += ".txt";
            }
            macierze[macierz1 - 1].ExportujDoPliku(nazwaPliku);
            cout << "Dane z macierzy " << macierz1 << " zapisane do pliku " << nazwaPliku << endl;
        } else if (wybor == 10) {
break;
} else {
cout << "Nieprawidlowy wybor." << endl;
}
}

    return 0;
}