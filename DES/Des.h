//
// Created by Bartłomiej Jencz on 12.11.2018.
//

#ifndef CRYPTO_DES_H
#define CRYPTO_DES_H

#include<iostream>
#include <algorithm>
#include <stdexcept>
#include<memory>
//getting 56 bit key from 64 bit using the parity bits Permutacja PC-1
int keyp[56] = {57, 49, 41, 33, 25, 17, 9,
                1, 58, 50, 42, 34, 26, 18,
                10, 2, 59, 51, 43, 35, 27,
                19, 11, 3, 60, 52, 44, 36,
                63, 55, 47, 39, 31, 23, 15,
                7, 62, 54, 46, 38, 30, 22,
                14, 6, 61, 53, 45, 37, 29,
                21, 13, 5, 28, 20, 12, 4
};
//D box configuration each 28 to 24 bit PC-2
int key2[48] = {14, 17, 11, 24, 1, 5,
                3, 28, 15, 6, 21, 10,
                23, 19, 12, 4, 26, 8,
                16, 7, 27, 20, 13, 2,
                41, 52, 31, 37, 47, 55,
                30, 40, 51, 45, 33, 48,
                44, 49, 39, 56, 34, 53,
                46, 42, 50, 36, 29, 32};
int per[32] = {16, 7, 20, 21,
               29, 12, 28, 17,
               1, 15, 23, 26,
               5, 18, 31, 10,
               2, 8, 24, 14,
               32, 27, 3, 9,
               19, 13, 30, 6,
               22, 11, 4, 25};
int s[8][4][16] =
        {{
                 14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,
                 0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8,
                 4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,
                 15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13
         },
         {
                 15, 1,  8,  14, 6,  11, 3,  4,  9,  7,  2,  13, 12, 0,  5,  10,
                 3,  13, 4,  7,  15, 2,  8,  14, 12, 0, 1,  10, 6,  9,  11, 5,
                 0,  14, 7,  11, 10, 4,  13, 1,  5,  8,  12, 6,  9,  3,  2,  15,
                 13, 8,  10, 1,  3,  15, 4,  2,  11, 6,  7,  12, 0,  5, 14, 9
         },


         {
                 10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,
                 13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1,
                 13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7,
                 1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12
         },
         {
                 7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15,
                 13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9,
                 10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4,
                 3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14
         },
         {
                 2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,
                 14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6,
                 4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14,
                 11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3
         },
         {
                 12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11,
                 10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8,
                 9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,
                 4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13
         },
         {
                 4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,
                 13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6,
                 1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,
                 6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12
         },
         {
                 13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
                 1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2,
                 7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,
                 2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11
         }};

void Round(std::string &l, std::string &r, std::string &cip) {
    int i = 0;
    for (i = 0; i < 32; i += 4) {
        int te;
        te = (int(l[i]) - 48) * 8 + (int(l[i + 1]) - 48) * 4 + (int(l[i + 2]) - 48) * 2 + (int(l[i + 3]) - 48);
        if (te < 10)
            cip += char(te + 48);
        else
            cip += char(te + 55);
    }
    for (i = 0; i < 32; i += 4) {
        int te;
        te = (int(r[i]) - 48) * 8 + (int(r[i + 1]) - 48) * 4 + (int(r[i + 2]) - 48) * 2 + (int(r[i + 3]) - 48);
        if (te < 10)
            cip += char(te + 48);
        else
            cip += char(te + 55);
    }
};

void sboxcompression(std::string &xorout, std::string &sout ) {
    int row;
    int col;
    int temp;
    std::string soutt;
    int i;
    int j;
    for (i = 0; i < 48; i += 6) {
        row = (int(xorout[i + 5]) - 48) + (int(xorout[i]) - 48) * 2;
        col = (int(xorout[i + 1]) - 48) * 8 + (int(xorout[i + 2]) - 48) * 4 + (int(xorout[i + 3]) - 48) * 2 +
              (int(xorout[i + 4]) - 48);
        temp = s[i / 6][row][col];
        soutt = "";
        while (temp > 0) {
            soutt += char(temp % 2 + 48);
            temp /= 2;
        }
        while (soutt.length() != 4)
            soutt += '0';
        for (j = soutt.length() - 1; j >= 0; j--)
            sout += soutt[j];
    }
}

void keyGeneration(int key[16][48], std::string &kp) {
//key generation
    int key1[64];
    //Przepisz klucz na inty
    for (int i = 0; i < 64; i++)
        key1[i] = kp[i] - '0';
int keyl[28];
int keyr[28];
int nshift;
int temp1;
int temp2;
int pkey[56];
    int i;
    int j;
    j = 0;
    //Pierwsza permutacja klucza z 64 na 56 bit
    for (i = 0; i < 56; i++)
        pkey[i] = key1[keyp[i] - 1];
    //Podzial klucza na lewa i prawa polowe
    for (i = 0; i < 28; i++)
        keyl[i] = pkey[i];
    for (i = 0; i < 28; i++)
        keyr[i] = pkey[i + 28];
//Tworzenie 16 kluczy C i D z left shifts
    for (i = 0; i < 16; i++) {
        if (i == 0 || i == 1 || i == 8 || i == 15)
            nshift = 1;
        else
            nshift = 2;
        while (nshift--) {
            temp1 = keyl[0];
            temp2 = keyr[0];
            for (j = 0; j < 27; j++) {
                keyl[j] = keyl[j + 1];
                keyr[j] = keyr[j + 1];
            }
            keyl[27] = temp1;
            keyr[27] = temp2;
        }
        //Nadanie drugiej permutacji PC-2 i stworzenie 16 kluczy dla rund.
        for (j = 0; j < 24; j++)
            key[i][j] = keyl[key2[j] - 1];
        for (j = 24; j < 48; j++)
            key[i][j] = keyr[key2[j] - 1 - 28];
    }

}






#endif //CRYPTO_DES_H
