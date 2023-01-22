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

    macierz(int wiersze, int kolumny) {
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


    void DodajElement(int wiersz, int kolumna, int wartosc) {

        wiersz = wiersz-1;
        kolumna = kolumna-1;

    if (wiersz < 0 || wiersz >= wiersze || kolumna < 0 || kolumna >= kolumny) {
        cerr << "Nieprawidłowy indeks elementu macierzy." << endl;
        return;
    }
    int** temp = new int*[wiersze];
    for(int i = 0; i<wiersze; i++)
        temp[i] = new int[kolumny];

    for(int i = 0; i<wiersze; i++)
        for(int j = 0; j<kolumny; j++)
            temp[i][j] = dane[i][j];

    temp[wiersz][kolumna] += wartosc;

    for(int i = 0; i<wiersze; i++)
        delete[] dane[i];
    delete[] dane;

    dane = temp;
}

    void UsunElement(int wiersz, int kolumna) {
    wiersz = wiersz-1;
    kolumna = kolumna-1;
    if (wiersz < 0 || wiersz >= wiersze || kolumna < 0 || kolumna >= kolumny) {
        cerr << "Nieprawidłowy indeks elementu macierzy." << endl;
        return;
    }
    int** temp = new int*[wiersze];
    for(int i = 0; i < wiersze; i++)
        temp[i] = new int[kolumny];

    for(int i = 0; i < wiersze; i++)
        for(int j = 0; j < kolumny; j++)
            temp[i][j] = dane[i][j];

    temp[wiersz][kolumna] = 0; //zamieniamy element na 0 oznaczający brak elmentu

    for(int i = 0; i < wiersze; i++)
        delete[] dane[i];
    delete[] dane;

    dane = temp;
}


    int ZwrocElement(int wiersz, int kolumna) {
            if ((wiersz-1) < 0 || (wiersz-1) >= wiersze || (kolumna-1) < 0 || (kolumna-1) >= kolumny) {
                cerr << "Nieprawidłowy indeks elementu macierzy." << endl;
                return 0;
            }
            return dane[(wiersz-1)][(kolumna-1)];
        }

    void transponuj() {
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

  

 

    int wyznacznik() const {
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

    


    macierz MacierzDopelnien() const {
            if (wiersze != 3 || kolumny != 3) {
                cerr << "Macierz nie jest 3x3." << endl;
                return macierz(0,0);
            }
            macierz dopelnienia(3,3);
            dopelnienia.dane[0][0] = (dane[1][1] * dane[2][2]) - (dane[2][1] * dane[1][2]);
            dopelnienia.dane[0][1] = (dane[1][2] * dane[2][0]) - (dane[2][2] * dane[1][0]);
            dopelnienia.dane[0][2] = (dane[1][0] * dane[2][1]) - (dane[2][0] * dane[1][1]);
            dopelnienia.dane[1][0] = (dane[0][2] * dane[2][1]) - (dane[2][2] * dane[0][1]);
            dopelnienia.dane[1][1] = (dane[0][0] * dane[2][2]) - (dane[2][0] * dane[0][2]);
            dopelnienia.dane[1][2] = (dane[0][1] * dane[2][0]) - (dane[2][1] * dane[0][0]);
            dopelnienia.dane[2][0] = (dane[0][1] * dane[1][2]) - (dane[1][1] * dane[0][2]);
            dopelnienia.dane[2][1] = (dane[0][2] * dane[1][0]) - (dane[1][2] * dane[0][0]);
            dopelnienia.dane[2][2] = (dane[0][0] * dane[1][1]) - (dane[1][0] * dane[0][1]);
            return dopelnienia;
        }


    

      
    // metoda do obliczania dopelnienia algebraicznego dla danego elementu macierzy
    /*double DopelnienieAlgebraiczne(int wiersz, int kolumna) {
        // Tworzymy macierz bez wiersza i kolumny danego elementu
        int sub_wiersze = wiersze - 1;
        int sub_kolumny = kolumny - 1;
        macierz sub_macierz(sub_wiersze, sub_kolumny);
        int x = 0, y = 0;
        for (int i = 0; i < wiersze; i++) {
            if (i == wiersz) continue;
            y = 0;
            for (int j = 0; j < kolumny; j++) {
                if (j == kolumna) continue;
                sub_macierz.dane[x][y] = dane[i][j];
                y++;
            }
            x++;
        }
        // Obliczamy wyznacznik macierzy bez wiersza i kolumny danego elementu
        double wyznacznik = sub_macierz.wyznacznik();
        // Zwracamy dopelnienie algebraiczne dla danego elementu macierzy
        return pow(-1, wiersz + kolumna) * wyznacznik;
    }
    */

    /*macierz DopelnienieMacierzy() {
        if (wiersze != kolumny) {
            cerr << "Dopelnienie macierzy można obliczyć tylko dla macierzy kwadratowej." << endl;
            return macierz(0,0);
        }

        macierz dopelnienie(wiersze, kolumny);
        for (int i = 0; i < wiersze; i++) {
            for (int j = 0; j < kolumny; j++) {
                dopelnienie.dane[i][j] = DopelnienieAlgebraiczne(i, j);
            }
        }

        return dopelnienie;
    }
    */

    
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



void wypisz() {
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            cout << dane[i][j] << " ";
        }
        cout << endl;
    }






















    

}




private:
    int** dane;
    int wiersze;
    int kolumny;
};


// metoda do obliczania macierzy odwrotnej
// metoda ta zwraca true jeśli dla danej macierzy
//można znaleźć macierz odwrotną. Natomiast metoda
//zwraca false jeśli nie można znaleźć macierzy
//odwrotnej, czyli jeśli macierz jest osobliwa.



int main()
{
   
    
    // macierz m1(3,3);
    // m1.wypisz();

    // m1.transponuj(); //dobrze działa
    // m1.wypisz();
/////////////////////////////////////////

    //  macierz m1(3,3);
    //  m1.wypisz();

    //  m1.odwroc();  // źle
    //  m1.wypisz();
//////////////////////////////////////////

    //  macierz m1(3,3);
    //  m1.wypisz();

    //  m1.wyznacznik(); //źle
    //  m1.wypisz();

//////////////////////////////////////
    //   macierz m1(3,3);
    //   m1.wypisz();

      
    //   macierz m1dop(3,3);
    //   m1dop = m1.MacierzDopelnien(); //działa
    //   m1dop.wypisz();


    // dorobić metodę obróć i wyznacznik


    return 0;
}