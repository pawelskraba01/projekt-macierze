#include<iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

class macierz {
public:
    // konstruktor

    

    macierz(int wiersze, int kolumny);

    // metody
    void wypisz();
    void transponuj();
    bool odwroc();
    int wyznacznik() const;
    // metoda do obliczania wyznacznika macierzy
   
    int stopien() const;
    

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
    macierz m2(3, 3);

    
    m2.wypisz();

   // int wyznacznik = m2.wyznacznik();
  //  cout << wyznacznik << endl;
    m2.odwroc();
    cout << "Po odwrocie" << endl;
    m2.wypisz();
 
 
 
    

    return 0;
}