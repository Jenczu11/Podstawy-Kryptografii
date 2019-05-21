#include<iostream>
#include <stdlib.h>
#include"function.h"
#include <math.h>
#include <sstream>
#include "BigInteger.h"
#include <gmp.h>
#include <stdio.h>
using namespace std;

void getHex(string *hexstr, int *retarr, int q) {
    for (int i = 0; i < q; i++) {
        retarr[i] = (int) strtol(hexstr[i].c_str(), 0, 16);
    }
}

void display(string *hexstr, int q) {
    for (int i = 0; i < q; i++)
        cout << hexstr[i];
    cout << endl;
}

void display(int *hexstr, int q) {
    for (int i = 0; i < q; i++)
        cout << hexstr[i];
    cout << endl;
}

int main() {


int p;
    int q;
    int n;
    int e;
    int d;
    int extension;
    extension = 0;
    klucze_RSA(p, q, e, n, d);
    cout << "Wygenerowana liczba pierwsza p: " << p << endl;
    cout << "Wygenerowana liczba pierwsza q: " << q << endl;
    cout << "Wartosc n=p*q,  n: " << n << endl;
    cout << "Liczba e wzglednie pierwsza z funkcja eulera od n, e: " << e << endl;
    cout
            << "Znaleziona liczba d, gdzie jej roznica z odwrotnoscia modularna liczby e jest podzielna przez euler(n); d: "
            << d << endl;
    string plainText;
    string *plainTextTemp;
    int *plainTextTemp1;
    cout << "Wczytaj plainText\n";
    //cin>>plainText;
    plainText = wczytajplik("plain");
    //cout<<plainText;
    //cin.get();
    plainText = string_to_hex(plainText);
    cout << plainText << endl;
    plainTextTemp = new string[plainText.length()];
    plainTextTemp1 = new int[plainText.length()];
    Split(plainText, plainTextTemp, extension);
    cout << "Q: " << extension << endl;
    cout << "PlainText(16) :";
    display(plainTextTemp, extension);
    cout << endl;
    getHex(plainTextTemp, plainTextTemp1, extension);
    cout << "PlainText(10) :";
    display(plainTextTemp1, extension);
    cout << endl;
//    cout<<plainTextTemp1[0]<<endl;
//    cout<<pow(plainTextTemp1[0],e)<<endl;

//szyfrowanie
    cout << "Szyfruje tekst." << endl;
    pot_mod(plainTextTemp1, e, n, q);

    cout << "Cipher(10) :";
    for (int i = 0; i < q; i++) cout << plainTextTemp1[i];
    cout << endl;
//Deszyfrowanie
    cout << "Deszyfruje tekst." << endl;
    pot_mod(plainTextTemp1, d, n, q);
    stringstream stream;
    cout << "DeCipher(10) :";
    for (int i = 0; i < extension; i++) {
        cout << plainTextTemp1[i];
        stream << hex << uppercase << plainTextTemp1[i];
    }
    cout << endl;

    cout << "DeCipher(Tekst) :" << hex_to_string(stream.str());


    return 0;
}