
#include<string>
#include <iostream>
#include<sstream>
#include "function.h"
#include"Des.h"
#include <memory>

using namespace std;

int main(int argc, char *args[]) {
    //<editor-fold desc="Wstępna deklaracja zmiennych">
    // string *Ciphertemp1;
    // string *plainTextTemp;
    // string *Ciphertemp;
    std::unique_ptr<std::string[]> plainTextTemp;
    string plainText;
    string klucz;
    string kp;
    int expansion = 0;
    int q = 0;
    int round;
    bool Debugging = true;
    unsigned int wybor = 0;
    std::unique_ptr<std::string[]> Ciphertemp1;
    std::shared_ptr<std::string[]> Ciphertemp;
    //</editor-fold>

    cleanfile();
    do {
        menu();
        cin >> wybor;
        //Dla rozwiazywania z konsoli
        plainText = args[1];
        klucz = args[2];
        check_if_valid(klucz);
        //<editor-fold desc="menu">

        switch (wybor) {
            case 0: {
                cout << "Wybrales zakonczenie programu";
                return 0;
            }
                break;
            case 1: {
                q = 0;
                plainText = "";
                plainText = wczytajplik("plain");
                cout << "Wczytuje plainText z pliku. \n";
                cout << "Generuje losowy klucz. \n";
                klucz = crypto();
                zapiszdopliku(klucz, "key");
                //check_if_valid(klucz);
                cout << "PlainText: " << plainText << endl;
                plainText = string_to_hex(plainText);
                cout << "Klucz: " << klucz << endl;

                if (Debugging) {
                    cout << "PlainTextHex:       ";
                    display16(plainText);
                    cout << endl;
                }

                Expand(plainText, expansion);
                if (Debugging) {
                    cout << "PlainTextHexExpand: ";
                    display16(plainText);
                    cout << endl;
                    cout << "Wielkosc rozszerzenia: " << expansion << endl;
                }
                //plainTextTemp = new string[plainText.length() / 16];
                //Ciphertemp = new string[plainText.length() / 16];
                Ciphertemp = std::make_unique<std::string[]>(plainText.length() / 16);
                plainTextTemp = std::make_unique<std::string[]>(plainText.length() / 16);
                SplitAndBinary(plainText, plainTextTemp, expansion, q);

            }
                break;
            case 2: {
                q = 0;
                plainText = "";
                plainText = wczytajplik("cipher");

//        cout << "Wprowadz klucz\n";
//        cin >> klucz;
//        check_if_valid(klucz);
                cout << "Wprowadzam klucz z pliku\n";
                klucz = wczytajplik("key");
                cout << "Pobieram tekst z pliku.\n";
                cout << "Zaszyfrowany tekst: " << plainText << endl;
                cout << "Klucz: " << klucz << endl;
                plainTextTemp = std::make_unique<std::string[]>(plainText.length() / 16);
                Ciphertemp1 = std::make_unique<std::string[]>(plainText.length() / 16);
                //plainTextTemp = new string[plainText.length() / 16];
                //Ciphertemp1 = new string[plainText.length() / 16];
                Split(plainText, Ciphertemp1, q);

            }
                break;
            case 3: {

                cout << "Czyszcze pliki key,cipher,decipher\n";
                cleanfile();
            }
                break;
        }
        //</editor-fold>
        //Dzielenie stringa na podciągi
        int IloscPodNapisow = q;
        q--;

        kp = hex_to_bin(klucz);
        int q1 = q;
        if (Debugging) {
            WyswietlPodciagi(plainTextTemp, IloscPodNapisow);
            cout << "Iloscpodnapisow: " << IloscPodNapisow << endl;
        }

        //<editor-fold desc="DES-Encryption">
        if (wybor == 1) {
            do {
                int key[16][48];
                string l = plainTextTemp[q1].substr(0, 32);
                string r = plainTextTemp[q1].substr(32, 32);
                keyGeneration(key, kp);
                round = 16;
                //DES Encryption
                string cip;

                while (round--) {
                   string rtem = r;
                    string ep;
                    string xorout;
                    string sout;
                    //the expansion P box
                    ep += r[31];
                    for (int i = 0, t = 1; i < 32; i++) {
                        if ((t + 1) % 6 == 0) {
                            ep += r[4 * ((t + 1) / 6)];
                            t++;
                        }
                        if (t % 6 == 0 && i != 0) {
                            ep += r[4 * (t / 6) - 1];
                            t++;
                        }
                        ep += r[i];
                        t++;
                    }
                    ep += r[0];
                    //Key xor with output of expansion plainText box
                    for (int i = 0; i < 48; i++)
                        xorout += char(((int(ep[i]) - 48) ^ key[16 - round - 1][i]) + 48);
                    //sbox compression 48bit to 32 bit
                    sboxcompression(xorout, sout);
                    //straight pbox that is permutation of the sbox output
                    char pc[32];
                    for (int i = 0; i < 32; i++)
                        pc[i] = sout[per[i] - 1];
                    r = "";
                    for (int i = 0; i < 32; i++)
                        r += char(((int(pc[i]) - 48) ^ (int(l[i]) - 48)) + 48);
                    l = rtem;
                    //Te dwie linijki zapewnaja przebieg po każdej rundzie
                    cip = "";
                    Round(l, r, cip);
                    if (Debugging == true) {
                        cout << "Output after Round" << 16 - round << ": ";
                        cout << cip << endl;
                    }
                    Ciphertemp[q1] = cip;
                }
                q1--;

            } while (q1 >= 0);
        }
        //</editor-fold>

        //<editor-fold desc="Wyswietlanie testowe">
        //    cout <<"plaintexttemp: "<<plainTextTemp[0]<<endl;
        //    cout<<"Ciph0: "<<Ciphertemp[0]<<endl;
        //    cout<<"Ciph1: "<<Ciphertemp[1]<<endl;
        //    cout<<"Ciphbin0: "<<hex_to_bin(Ciphertemp[0])<<endl;
        //    cout<<"Ciphbin1: "<<hex_to_bin(Ciphertemp[1])<<endl;
        //</editor-fold>

        //<editor-fold desc="DES-Decryption">
        if (wybor == 2) {
            do {
                int key[16][48];
                string l = hex_to_bin(Ciphertemp1[q]).substr(0, 32);
                string r = hex_to_bin(Ciphertemp1[q]).substr(32, 32);
                keyGeneration(key, kp);
                round = 16;
                string ltem;
                while (round--) {
                    ltem = l;
                    string xorout;
                    string ep;
                    string sout;
                    string cip;
                    //the expansion P box
                    ep += l[31];
                    for (int i = 0, t = 1; i < 32; i++) {
                        if ((t + 1) % 6 == 0) {
                            ep += l[4 * ((t + 1) / 6)];
                            t++;
                        }
                        if (t % 6 == 0 && i != 0) {
                            ep += l[4 * (t / 6) - 1];
                            t++;
                        }
                        ep += static_cast<char>(l[i]);
                        t++;
                    }
                    ep += l[0];
                    //Key xor with output of expansion plainText box
                    for (int i = 0; i < 48; i++)
                        xorout += char(((int(ep[i]) - 48) ^ key[round][i]) + 48);
                    //sbox compression 48bit to 32 bit
                    sboxcompression(xorout, sout);
                    //straight pbox that is permutation of the sbox output
                    char pc[32];
                    for (int i = 0; i < 32; i++)
                        pc[i] = sout[per[i] - 1];
                    l = "";
                    for (int i = 0; i < 32; i++)
                        l += char(((int(pc[i]) - 48) ^ (int(r[i]) - 48)) + 48);
                    r = ltem;
                    cip = "";
                    Round(l, r, cip);

                    if (Debugging == true) {
                        cout << "Decrypted Output after Round" << 16 - round << ": ";
                        cout << cip << endl;
                    }
                    plainTextTemp[q] = cip;
                }
                q--;
            } while (q >= 0);
        }
        //</editor-fold>

        //<editor-fold desc="Wyswietlanie odpowiedzi">

        switch (wybor) {
            case 1: {
                DisplayEncrypt(Ciphertemp, IloscPodNapisow);
            }
                break;
            case 2: {
                DisplayDecrypt(plainTextTemp, IloscPodNapisow, expansion);
            }
                break;
        }
        //</editor-fold>
        cin.get();
        // system("pause");
    } while (wybor != 0);
    return 0;
}