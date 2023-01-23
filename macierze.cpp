#include<iostream>
using namespace std;
#include <cstdlib>
#include <iomanip>
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
    dane = new double*[wiersze];
    for (int i = 0; i < wiersze; i++) {
        dane[i] = new double[kolumny];
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
    double** temp = new double*[wiersze];
    for(int i = 0; i<wiersze; i++)
        temp[i] = new double[kolumny];

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
    double** temp = new double*[wiersze];
    for(int i = 0; i < wiersze; i++)
        temp[i] = new double[kolumny];

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
        double** nowe_dane = new double*[kolumny];
        for (int i = 0; i < kolumny; i++) {
            nowe_dane[i] = new double[wiersze];
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





macierz WyznaczMacierzOdwrotna() {
    if (wiersze != 3 || kolumny != 3) {
        cerr << "Macierz odwrotna może być wyznaczona tylko dla macierzy 3x3." << endl;
        return macierz(1,1);
    }
    // obliczanie wartości detA
    double detA = dane[0][0] * (dane[1][1] * dane[2][2] - dane[2][1] * dane[1][2]) -
               dane[0][1] * (dane[1][0] * dane[2][2] - dane[1][2] * dane[2][0]) +
               dane[0][2] * (dane[1][0] * dane[2][1] - dane[1][1] * dane[2][0]);
    if (detA == 0) {
        cerr << "Macierz odwrotna nie istnieje dla macierzy o wyznaczniku równym 0." << endl;
        return macierz(1,1);
    }
    // tworzenie macierzy odwrotnej
    macierz A_odwrotna(3, 3);
    A_odwrotna.dane[0][0] = (dane[1][1] * dane[2][2] - dane[2][1] * dane[1][2]) / detA;
    A_odwrotna.dane[0][1] = -(dane[0][1] * dane[2][2] - dane[0][2] * dane[2][1]) / detA;
    A_odwrotna.dane[0][2] = (dane[0][1] * dane[1][2] - dane[0][2] * dane[1][1]) / detA;
    A_odwrotna.dane[1][0] = -(dane[1][0] * dane[2][2] - dane[1][2] * dane[2][0]) / detA;
    A_odwrotna.dane[1][1] = (dane[0][0] * dane[2][2] - dane[0][2] * dane[2][0]) / detA;
    A_odwrotna.dane[1][2] = -(dane[0][0] * dane[1][2] - dane[1][0] * dane[0][2]) / detA;
    A_odwrotna.dane[2][0] = (dane[1][0] * dane[2][1] - dane[2][0] * dane[1][1]) / detA;
    A_odwrotna.dane[2][1] = -(dane[0][0] * dane[2][1] - dane[2][0] * dane[0][1]) / detA;
    A_odwrotna.dane[2][2] = (dane[0][0] * dane[1][1] - dane[1][0] * dane[0][1]) / detA;
    
    //  for(int i = 0; i < 3; i++) {
    //      for(int j = 0; j < 3; j++) {
    // //         cout << setprecision(2) << fixed << A_odwrotna.dane[i][j] << " ";
    //             if(floor(A_odwrotna.dane[i][j]) == A_odwrotna.dane[i][j])
    //         printf("%.0f ", A_odwrotna.dane[i][j]);
    //     else
    //         printf("%.2f ", A_odwrotna.dane[i][j]);
    //      }
    //      cout << endl;
    //  }
    
    return A_odwrotna;
    }
    





void wypisz() {
    for (int i = 0; i < wiersze; i++) {
        for (int j = 0; j < kolumny; j++) {
            cout << fixed << setprecision(2);
            cout << setw(4) << dane[i][j];
            if (j != kolumny-1) cout << " |";
        }
        cout << endl;
        if (i != wiersze-1) {
            for (int j = 0; j < kolumny; j++) {
                cout << "------";              
            }
            cout << endl;
        }
    }
}





private:
    double** dane;
    int wiersze;
    int kolumny;
};



int main()
{
   
  
    // tworzymy macierz o rozmiarze 3x3

    macierz m1(3, 3);
    macierz m2(3, 3);

  

    // drukujemy macierz
    cout << "\nMacierz przed dodaniem elementu:" << endl;
    m1.wypisz();
    _getch();

    // dodajemy element do macierzy
    m1.DodajElement(1, 1, 10);
    cout << "\nMacierz po dodaniu elementu 10 na pozycji (1,1)" << endl;
    m1.wypisz();
    _getch();

    // usuwamy element z macierzy
    m1.UsunElement(1, 1);
    cout << "\nMacierz po usunieciu elementu na pozycji (1,1)" << endl;
    m1.wypisz();
    _getch();

    // testujemy metodę ZwrocElement()
    int element = m1.ZwrocElement(1, 1);
    cout << "\nElement (1,1) macierzy: " << element << endl;
    _getch();

    // transponujemy macierz
    m1.transponuj();
    cout << "\nMacierz po transponowaniu:" << endl;
    m1.wypisz();
    _getch();

    //Wyznacznik laplace
    cout<<"\nWyznacznik macierzy: " << endl;
    double wyznacznik = m1.WyznacznikLaplace();
    cout << wyznacznik << endl;
    _getch();

//////////////////////////////////////////////

    //Macierz Dopelnien
    cout<<"\nMacierz Dopelnien: " << endl;
    macierz dopelnienia = m1.MacierzDopelnien();
    dopelnienia.wypisz();
    _getch();


    //Macierz Odwrotna
    cout<<"\nMacierz Odwrotna dla macierzy m1 " << endl;
    m1.wypisz();
    macierz modwrotna(3,3);
    modwrotna = m1.WyznaczMacierzOdwrotna();
    modwrotna.wypisz();
    _getch();

     //Export do pliku
     m1.ExportujDoPliku("gotowiec.txt");
     cout << "\nZostal utworzony plik tekstowy o nazwie gotowiec.txt zawierajacy dane z macierzy m1" << endl;
     _getch();


    // //Import z pliku
    cout << "\nTworzymy nowa macierz m3 do ktorej nastepnie zaimportujemy dane z pliku" << endl;
    macierz m3(3,3);
    cout << "\nW macierzy m3 znajduja sie aktualnie takie dane"; 
    m3.wypisz();
    cout << "\nImportujemy z pliku gotowiec.txt dane, ktore wczesniej eksportowalismy z macierzy m1" << endl;
    m3.ImportujZPliku("gotowiec.txt");
    cout << "\nAktualne dane w macierzy m3 po zaimportowaniu z pliku gotowiec.txt" << endl;
    m3.wypisz();
     _getch();

    //Dodawanie macierzy
    cout<<"\nDodawanie macierzy: " << endl;
   

    cout << "\nMacierz m1" << endl;
    m1.wypisz();

    cout << "\nMacierz m2" << endl;
    m2.wypisz();

    macierz wynikdodawania(3,3);
    wynikdodawania =  m1.Dodaj(m2);

    cout << "\nwynik" << endl;
    wynikdodawania.wypisz();

    
    _getch();
    //Odejmowanie macierzy
    cout<<"\nOdejmowanie macierzy: " << endl;

    cout << "\nMacierz m1" << endl;
    m1.wypisz();

    cout << "\nMacierz m2" << endl;
    m2.wypisz();


    macierz wynikodejmowania(3,3);
    wynikodejmowania = m1.Odejmij(m2);
   
    cout << "\nwynik" << endl;
    wynikodejmowania.wypisz();

     _getch();

   
    return 0;
}