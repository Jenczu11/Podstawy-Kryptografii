//
// Created by Bartłomiej Jencz on 28.11.2018.
//

#include "function.h"
using namespace std;

void Split(std::string plainText, std::string *plainTextTemp, int &q) {
    while (plainText.length() >= 1) {
        plainTextTemp[q] = plainText.substr(0, 1);
        plainText.erase(plainText.begin(), plainText.begin() + 1);
        q++;
    }
}
std::string string_to_hex(const std::string &input) {
    //Funkcja zmieniajaca stringa na postac 16
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i) {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string hex_to_string(const std::string &input) {
    //Funkcja zmieniajaca stringa z postacia 16 na zwykly tekst
    static const char *const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("Nie parzysta dlugosc");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char *p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("Nie jest to liczba 16");

        char b = input[i + 1];
        const char *q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("Nie jest to liczba 16");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

std::string hex_to_bin(std::string p) //Hexadecimal to Binary
{
    std::string ap = "";
    int l = p.length();
    for (int i = 0; i < l; i++) {
        std::string st = "";
        if (p[i] >= '0' && p[i] <= '9') {
            int te = int(p[i]) - 48;
            while (te > 0) {
                st += char(te % 2 + 48);
                te /= 2;
            }
            while (st.length() != 4)
                st += '0';
            for (int j = 3; j >= 0; j--)
                ap += st[j];
        } else {
            int te = p[i] - 'A' + 10;
            while (te > 0) {
                st += char(te % 2 + 48);
                te /= 2;
            }
            for (int j = 3; j >= 0; j--)
                ap += st[j];
        }
    }
    return ap;
}

// Funkcja czeka na dowolny klawisz i czyści ekran
//------------------------------------------------
void czekaj(void)
{
    char c[1];

    cout << "\nZapisz te dane\n\n";
    cin.getline(c,1);
    cin.getline(c,1);
    cout<<endl;
    cout<<endl;
}

// Funkcja obliczająca NWD dla dwóch liczb
//----------------------------------------
int nwd(int a, int b)
{
    int t;

    while(b != 0)
    {
        t = b;
        b = a % b;
        a = t;
    };
    return a;
}

// Funkcja obliczania odwrotności modulo n
//----------------------------------------
int odwr_mod(int a, int n)
{
    int a0,n0,p0,p1,q,r,t;

    p0 = 0; p1 = 1; a0 = a; n0 = n;
    q  = n0 / a0;
    r  = n0 % a0;
    while(r > 0)
    {
        t = p0 - q * p1;
        if(t >= 0)
            t = t % n;
        else
            t = n - ((-t) % n);
        p0 = p1; p1 = t;
        n0 = a0; a0 = r;
        q  = n0 / a0;
        r  = n0 % a0;
    }
    return p1;
}

// Procedura generowania kluczy RSA
//---------------------------------
void klucze_RSA(int &p,int &q,int &e,int &n, int &d)
{
    const int tp[10] = {11,13,17,19,23,29,31,37,41,43};
    int phi;

    cout << "Generowanie kluczy RSA\n"
            "----------------------\n\n";

// generujemy dwie różne, losowe liczby pierwsze

    do
    {
        p = tp[rand() % 10];
        q = tp[rand() % 10];
    } while (p == q);

    phi = (p - 1) * (q - 1);
    n   = p * q;

// wyznaczamy wykładniki e i d

    for(e = 3; nwd(e,phi) != 1; e += 2);
    d = odwr_mod(e,phi);

// gotowe, wypisujemy klucze

//    cout << "KLUCZ PUBLICZNY\n"
//            "wykladnik e = " << e
//         << "\n    modul n = " << n
//         << "\n\nKLUCZ PRYWATNY\n"
//            "wykladnik d = " << d << endl;
}

// Funkcja oblicza modulo potęgę podanej liczby
//---------------------------------------------
void pot_mod(int *a, int w, int n,int numer) {
    int pot, wyn, q;
numer--;
// wykładnik w rozbieramy na sumę potęg 2
// przy pomocy algorytmu Hornera. Dla reszt
// niezerowych tworzymy iloczyn potęg a modulo n.
    do
    {
        pot = a[numer];
        //cout<<pot;
    wyn = 1;
    for (q = w; q > 0; q /= 2) {
        if (q % 2) wyn = (wyn * pot) % n;
        pot = (pot * pot) % n; // kolejna potęga

    }
    //cout<<numer<<":  "<<wyn<<endl;

    a[numer]=wyn;
    //cout<<"---------------"<<endl;
        numer--;
}while(numer >= 0);
}

std::string wczytajplik(std::string sciezka)
{
    std::fstream plik;
     std::string temp1="C:\\Users\\barte\\CLionProjects\\RSA\\";
 //   std::string temp1="..\\";
    sciezka.insert(0,temp1);
    cout<<sciezka<<"\n";
    plik.open(sciezka);
    std::stringstream s1;
    std::string napis;

    if( plik.good() )
    {
        std::cout << "Zawartosc pliku:" << std::endl;
        while( !plik.eof() )
        {
            getline( plik, napis );
            s1 << napis << std::endl;
        }
        plik.close();
    }
    else
        std::cout << "Error! Nie udalo otworzyc sie pliku!" << std::endl;
cout<<s1.str();
    return s1.str();
}



