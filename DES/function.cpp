//
// Created by Bartłomiej Jencz on 27.11.2018.
//
#include"function.h"
#include "function.h"


std::string wczytajplik(std::string sciezka)
{
    std::fstream plik;
    std::string temp1="C:\\Users\\barte\\CLionProjects\\crypto\\";
   sciezka.insert(0,temp1);
    plik.open(sciezka);
    std::stringstream s1;
    if( plik.good() )
    {
        std::string napis;

//        std::cout << "Zawartosc pliku:" << std::endl;
        while( !plik.eof() )
        {
            getline( plik, napis );
            s1 << napis << std::endl;
        }
        plik.close();
    }
    else
        std::cout << "Error! Nie udalo otworzyc sie pliku!" << std::endl;

    return s1.str();
}
void zapiszdopliku(std::string item, std::string sciezka)
{
    std::fstream plik;
    std::string temp1="C:\\Users\\barte\\CLionProjects\\crypto\\";
    sciezka.insert(0,temp1);
//    std::cout<<"Zapisuje "<<item<<" do pliku "<<sciezka<<std::endl;
    plik.open(sciezka);
    if( plik.good() )
    {
        std::string napis;
            plik<<item;
        plik.close();
    }
    else
        std::cout << "Error! Nie udalo otworzyc sie pliku!" << std::endl;
}
std::string crypto() {
    HCRYPTPROV hProvider = 0;
    std::stringstream s;
    if (!::CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return "1";

    const DWORD dwLength = 16;
    BYTE pbBuffer[dwLength] = {};

    if (!::CryptGenRandom(hProvider, dwLength, pbBuffer))
    {
        ::CryptReleaseContext(hProvider, 0);
        return "1";
    }

    for (DWORD i = 0; i < dwLength; ++i)
        s << std::hex <<std::uppercase<<static_cast<unsigned int>(pbBuffer[i]);

    if (!::CryptReleaseContext(hProvider, 0))
        return "1";
    return s.str().substr(0,16);
}

void menu() {
    std::cout<<"*****************************************\n";
    std::cout<<"*                                       *\n";
    std::cout<<"*             Program DES               *\n";
    std::cout<<"*     1) Szyfrowanie                    *\n";
    std::cout<<"*     2) Deszyfrowanie                  *\n";
    std::cout<<"*     3) Czyszczenie plikow             *\n";
    std::cout<<"*     0) Zakonczenie programu           *\n";
    std::cout<<"*                                       *\n";
    std::cout<<"*****************************************\n";

}

void display16(std::string plainText) {

    for(int i=0; i<plainText.length(); i++) {
        std::cout<<plainText[i];
        if (i % 16 == 15 && i<16*plainText.length()-1) std::cout<<" ";
    }


}

void Split(std::string plainText, std::string *plainTextTemp, int &q) {
    while (plainText.length() >= 16) {
        plainTextTemp[q] = plainText.substr(0, 16);
        plainText.erase(plainText.begin(), plainText.begin() + 16);
        q++;
    }
}

void Expand(std::string &plainText, int &expansion) {//Funkcja rozszerzajaca sekcje do sekcji 16 bitowych przez dodanie 0
    while (plainText.length() % 16 != 0) {
        plainText.append("0");
        //cout << plainText << endl;
        expansion++;
    }
}

int check_if_valid(std::string &p) {
    //Funkcja sprawdzajaca czy podany string jest w formie szesnastkowej.
    if (p.length() != 16) {
        std::cout << "Wprowadz wszystkie bity klucza\n";
        std::cin>>p;
    }
    for (int i = 0; i < 16; i++) {
        if ((p[i] >= '0' && p[i] <= '9') || (p[i] >= 'A' && p[i] <= 'F'));
        else {
            std::cout << "Nie jest to klucz 16 bitowy\n";
            std::cin>>p;
        }
    }
    return 0;
}

void SplitAndBinary(std::string plainText, std::string *plainTextTemp, int &expansion, int &q) {


    while (plainText.length() >= 16) {
        plainTextTemp[q] = plainText.substr(0, 16);
        //cout << "plainTextTemp " << plainTextTemp[q] << endl;
        plainTextTemp[q] = hex_to_bin(plainTextTemp[q]);
        plainText.erase(plainText.begin(), plainText.begin() + 16);
        //cout << "PlainTexterase " << plainText << endl;
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

void WyswietlPodciagi(std::string *plainTexttemp, int IloscPodNapisow) {
    //Wyswietlanie odpowiednich podciągów napisów w formie binarnej
    std::cout << "Ilosc podciagow do przerobienia " << IloscPodNapisow << std::endl;
    for (int i = 0; i < IloscPodNapisow; i++)
        std::cout << "i=" << i + 1 << " " << plainTexttemp[i] << std::endl;
    std::cout << std::endl;
}
void WyswietlPodciagi(std::unique_ptr<std::string[]> &plainTexttemp, int IloscPodNapisow) {
    //Wyswietlanie odpowiednich podciągów napisów w formie binarnej
    std::cout << "Ilosc podciagow do przerobienia " << IloscPodNapisow << std::endl;
    for (int i = 0; i < IloscPodNapisow; i++)
        std::cout << "i=" << i + 1 << " " << plainTexttemp[i] << std::endl;
    std::cout << std::endl;
}

void DisplayDecrypt(std::string *plainTextTemp, int IloscPodNapisow, int expansion) {
    std::string ciptemp;
    for (int i = 0; i < IloscPodNapisow; i++)
        ciptemp += plainTextTemp[i];
    //cout << "ciptemp:" << ciptemp << endl;

    //Usuwanie '0' powstałych przez rozszerzenie
    for (; expansion > 0; expansion--) {
        ciptemp.erase(ciptemp.end() - 1);
    }
    std::cout << "Decipher16 :";
    display16(ciptemp);
    std::cout << std::endl;
//    //</editor-fold>

    std::cout << "Tekst odszyfrowany: " << hex_to_string(ciptemp) << std::endl;
    std::cout<<"Zapisuje odszyfrowany tekst do pliku."<<std::endl;
    zapiszdopliku(hex_to_string(ciptemp),"decipher");
};
void DisplayDecrypt(std::unique_ptr<std::string[]> &plainTextTemp, int IloscPodNapisow, int expansion) {
    std::string ciptemp;
    for (int i = 0; i < IloscPodNapisow; i++)
        ciptemp += plainTextTemp[i];
    //cout << "ciptemp:" << ciptemp << endl;

    //Usuwanie '0' powstałych przez rozszerzenie
    for (; expansion > 0; expansion--) {
        ciptemp.erase(ciptemp.end() - 1);
    }
    std::cout << "Decipher16 :";
    display16(ciptemp);
    std::cout << std::endl;
//    //</editor-fold>

    std::cout << "Tekst odszyfrowany: " << hex_to_string(ciptemp) << std::endl;
    std::cout<<"Zapisuje odszyfrowany tekst do pliku."<<std::endl;
    zapiszdopliku(hex_to_string(ciptemp),"decipher");
};

void DisplayEncrypt(std::unique_ptr<std::string[]> &Ciphertemp, int IloscPodNapisow)
{
    std::string CipherTemp1;
    for (int i = 0; i < IloscPodNapisow; i++) {
        CipherTemp1 += Ciphertemp[i];
    }
    std::cout << "Tekst Zaszyfrowany: ";
    // display16(CipherTemp1);
    std::cout << CipherTemp1;
    std::cout << std::endl;
    zapiszdopliku(CipherTemp1,"cipher");
}
void DisplayEncrypt(std::string Ciphertemp, int IloscPodNapisow)
{
    std::string CipherTemp1;
    for (int i = 0; i < IloscPodNapisow; i++) {
        CipherTemp1 += Ciphertemp[i];
    }
    std::cout << "Tekst Zaszyfrowany: ";
    // display16(CipherTemp1);
    std::cout << CipherTemp1;
    std::cout << std::endl;
    zapiszdopliku(CipherTemp1,"cipher");
}

void Split(std::string plainText, std::unique_ptr<std::string[]> &plainTextTemp, int &q) {
    while (plainText.length() >= 16) {
        plainTextTemp[q] = plainText.substr(0, 16);
        plainText.erase(plainText.begin(), plainText.begin() + 16);
        q++;
    }
}

    void SplitAndBinary(std::string plainText, std::unique_ptr<std::string[]> &plainTextTemp, int &expansion, int &q) {
        while (plainText.length() >= 16) {
            plainTextTemp[q] = plainText.substr(0, 16);
            //cout << "plainTextTemp " << plainTextTemp[q] << endl;
            plainTextTemp[q] = hex_to_bin(plainTextTemp[q]);
            plainText.erase(plainText.begin(), plainText.begin() + 16);
            //cout << "PlainTexterase " << plainText << endl;
            q++;
        }
    }

void cleanfile() {
    std::ofstream plik;
    std::ofstream plik1;
    std::ofstream plik2;
    plik.open("C:\\Users\\barte\\CLionProjects\\crypto\\key",std::ofstream::out|std::ofstream::trunc);
    plik1.open("C:\\Users\\barte\\CLionProjects\\crypto\\cipher",std::ofstream::out|std::ofstream::trunc);
    plik2.open("C:\\Users\\barte\\CLionProjects\\crypto\\decipher",std::ofstream::out|std::ofstream::trunc);
    plik.close();
    plik1.close();
    plik2.close();
}

void DisplayEncrypt(std::shared_ptr<std::string[]> Ciphertemp, int IloscPodNapisow) {
    std::string CipherTemp1;
    for (int i = 0; i < IloscPodNapisow; i++) {
        CipherTemp1 += Ciphertemp[i];
    }
    std::cout << "Tekst Zaszyfrowany: ";
    // display16(CipherTemp1);
    std::cout << CipherTemp1;
    std::cout << std::endl;
    zapiszdopliku(CipherTemp1,"cipher");
}
