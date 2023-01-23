#include<iostream>
using namespace std;
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <conio.h>



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

  

 


       
double WyznacznikLaplace() {
    if (wiersze != kolumny) {
        cerr << "Macierz musi być kwadratowa, aby obliczyć wyznacznik." << endl;
        return 0;
    }

    double det = 0;
    if (wiersze == 1)
        return dane[0][0];
    if (wiersze == 2)
        return dane[0][0] * dane[1][1] - dane[0][1] * dane[1][0];

    for (int i = 0; i < wiersze; i++) {
        int mnoznik = pow(-1, i);
        macierz temp(wiersze - 1, kolumny - 1);
        for (int j = 0; j < wiersze - 1; j++) {
            for (int k = 0; k < kolumny - 1; k++) {
                temp.dane[j][k] = dane[j >= i ? j + 1 : j][k + 1];
            }
        }
        det += mnoznik * dane[i][0] * temp.WyznacznikLaplace();
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

macierz MacierzOdwrotna() {
    if (wiersze != 3 || kolumny != 3) {
        cerr << "Macierz musi być 3x3, aby obliczyć macierz odwrotną." << endl;
        return macierz(0, 0);
    }
    double det = WyznacznikLaplace();
    if (det == 0) {
        cerr << "Macierz odwrotna nie istnieje dla macierzy osobliwej." << endl;
        return macierz(0, 0);
    }
    macierz odwrotna(3, 3);
    odwrotna.dane[0][0] = (dane[1][1] * dane[2][2] - dane[1][2] * dane[2][1]) / det;
    odwrotna.dane[0][1] = (dane[0][2] * dane[2][1] - dane[0][1] * dane[2][2]) / det;
    odwrotna.dane[0][2] = (dane[0][1] * dane[1][2] - dane[0][2] * dane[1][1]) / det;
    odwrotna.dane[1][0] = (dane[1][2] * dane[2][0] - dane[1][0] * dane[2][2]) / det;
    odwrotna.dane[1][1] = (dane[0][0] * dane[2][2] - dane[0][2] * dane[2][0]) / det;
    odwrotna.dane[1][2] = (dane[0][2] * dane[1][0] - dane[0][0] * dane[1][2]) / det;
    odwrotna.dane[2][0] = (dane[1][0] * dane[2][1] - dane[1][1] * dane[2][0]) / det;
    odwrotna.dane[2][1] = (dane[0][1] * dane[2][0] - dane[0][0] * dane[2][1]) / det;
    odwrotna.dane[2][2] = (dane[0][0] * dane[1][1] - dane[0][1] * dane[1][0]) / det;
    return odwrotna;
}
    

      
   

    
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



int main()
{
   
  
    // tworzymy macierz o rozmiarze 3x3

    macierz m1(3, 3);
    macierz m2(3, 3);

  

    // drukujemy macierz
    cout << "Macierz przed dodaniem elementu:" << endl;
    m1.wypisz();
    _getch();

    // dodajemy element do macierzy
    m1.DodajElement(1, 1, 10);
    cout << "Macierz po dodaniu elementu:" << endl;
    m1.wypisz();
    _getch();

    // usuwamy element z macierzy
    m1.UsunElement(1, 1);
    cout << "Macierz po usunięciu elementu:" << endl;
    m1.wypisz();
    _getch();

    // testujemy metodę ZwrocElement()
    int element = m1.ZwrocElement(1, 1);
    cout << "Element (1,1) macierzy: " << element << endl;
    _getch();

    // transponujemy macierz
    m1.transponuj();
    cout << "Macierz po transponowaniu:" << endl;
    m1.wypisz();
    _getch();
    //Wyznacznik laplace
    cout<<"Wyznacznik macierzy: " << m1.WyznacznikLaplace()<< endl;
    _getch();

//////////////////////////////////////////////

    //Macierz Dopelnien
    cout<<"Macierz Dopelnien: " << endl;
    m1.MacierzDopelnien();
    m1.wypisz();
    _getch();


    //Macierz Odwrotna
    cout<<"Macierz Odwrotna: " << endl;
    m1.MacierzOdwrotna();
    _getch();

     //Export do pliku
     m1.ExportujDoPliku();
     _getch();


    // //Import z pliku
    // m1.ImportujZPliku();
    // _getch();

    //Dodawanie macierzy
    cout<<"Dodawanie macierzy: " << endl;
   

    cout << "Macierz m1" << endl;
    m1.wypisz();

    cout << "Macierz m2" << endl;
    m2.wypisz();

    macierz wynikdodawania(3,3);
    wynikdodawania =  m1.Dodaj(m2);

    cout << "wynik" << endl;
    wynikdodawania.wypisz();

    
    _getch();
    //Odejmowanie macierzy
    cout<<"Odejmowanie macierzy: " << endl;

    cout << "Macierz m1" << endl;
    m1.wypisz();

    cout << "Macierz m2" << endl;
    m2.wypisz();


    macierz wynikodejmowania(3,3);
    wynikodejmowania = m1.Odejmij(m2);
   
    cout << "wynik" << endl;
    wynikodejmowania.wypisz();

     _getch();

   
    return 0;
}