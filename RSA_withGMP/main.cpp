//#include <stdlib.h>
//#include <stdio.h>
#include <string>
#include<iostream>
#include <vector>
#include"function.h"
#include<sstream>
using namespace std;
#define MODULUS_SIZE 1024                   /* This is the number of bits we want in the modulus */
#define BUFFER_SIZE ((MODULUS_SIZE/8))  /* This is the number of bytes in n and p */

int main()
{
    unsigned int wybor=0;
    mpz_t C;  mpz_init(C);
    vector<string> Ctemp;
    vector<string> DCtemp;
    mpz_t DC;  mpz_init(DC);
    private_key ku;
    public_key kp;

    // Initialize public key
    mpz_init(kp.n);
    mpz_init(kp.e);
    // Initialize private key
    mpz_init(ku.n);
    mpz_init(ku.e);
    mpz_init(ku.d);
    mpz_init(ku.p);
    mpz_init(ku.q);
    cleanfile();
    int q = 0;
do {
    menu();
    cin>>wybor;
    switch (wybor)
    {
        case 0: {
            cout << "Wybrales zakonczenie programu";
            return 0;
        }
            break;
        case 1: {
            generate_keys(&ku, &kp);
            printf("---------------Klucz prywatny--------------\n");
            printf("kp.n is [%s]\n", mpz_get_str(NULL, 16, kp.n));
            zapiszdopliku(mpz_get_str(NULL, 16, kp.n),"kpn");
            printf("kp.e is [%s]\n", mpz_get_str(NULL, 16, kp.e));
            zapiszdopliku(mpz_get_str(NULL, 16, kp.e),"kpe");
            printf("---------------Klucz publiczny-------------\n");
            printf("ku.n is [%s]\n", mpz_get_str(NULL, 16, ku.n));
            printf("ku.e is [%s]\n", mpz_get_str(NULL, 16, ku.e));
            zapiszdopliku(mpz_get_str(NULL, 16, ku.e),"kue");
            printf("ku.d is [%s]\n", mpz_get_str(NULL, 16, ku.d));
            zapiszdopliku(mpz_get_str(NULL, 16, ku.d),"kud");
            //Wyswietl liczby pierwsze p i q
            printf("ku.p is [%s]\n", mpz_get_str(NULL, 10, ku.p));
            printf("ku.q is [%s]\n", mpz_get_str(NULL, 10, ku.q));

        }
            break;
        case 2: {
            q=0;
            string plainText = wczytajplik("plainText");
            string Ctemp1;
            plainText = string_to_hex(plainText);
            for (auto &c: plainText) c = tolower(c);
            string *plainTextTemp = new string [plainText.length()];

            while (plainText.length() >= 2) {
                plainTextTemp[q] = plainText.substr(0, 2);
                plainText.erase(plainText.begin(), plainText.begin() + 2);
                q++;
            }
            mpz_t M;
            mpz_init(M);
            for (int i = 0; i < q; i++)
            {      mpz_set_str(M, plainTextTemp[i].c_str(), 16);
            mpz_powm(C, M, kp.e, kp.n);
            // printf("Zaszyfrowane [%s]\n", mpz_get_str(NULL, 10, C));
            Ctemp.push_back(mpz_get_str(NULL, 10, C));

        }
            for (int i=0; i<Ctemp.size(); i++)
            {
                Ctemp1+=Ctemp[i];
            }
            zapiszdopliku(Ctemp1,"cipher");
            //cout << "Tekst zaszyfrowany: " << Ctemp1 << endl;
            mpz_clear(C);
            mpz_clear(M);

        }
            break;
        case 3: {
            string DCtemp1;
            mpz_t Cipher;
            mpz_init(Cipher);
            for (int i = 0; i < q; i++)
            {
            mpz_set_str(Cipher, Ctemp[i].c_str(), 10);
            mpz_powm(DC, Cipher, ku.d, kp.n);
            //printf("Odszyfrowane [%s]\n", mpz_get_str(NULL, 10, DC));
            DCtemp.push_back(mpz_get_str(NULL, 16, DC));
        }
            cout<<"16kowy zapis: ";
            for (int i=0; i<DCtemp.size(); i++)
            {
                cout<<DCtemp[i];
                DCtemp1+=DCtemp[i];
            }
            cout << "\nTekst Odszyfrowany(string)";
            transform(DCtemp1.begin(), DCtemp1.end(), DCtemp1.begin(), ::toupper);
            cout<<hex_to_string(DCtemp1);
            zapiszdopliku(hex_to_string(DCtemp1),"decipher");
            cout<<"\n";
            mpz_clear(Cipher);
            mpz_clear(DC);
        }
            break;
        case 4: {cleanfile(); DCtemp.clear(); Ctemp.clear();} break;
    }
} while(wybor!=0);

    return 0;
}