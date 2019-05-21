//
// Created by Bartłomiej Jencz on 28.11.2018.
//

#ifndef RSA_FUNCTION_H
#define RSA_FUNCTION_H
#include <windows.h>
#include <sstream>
#include<string>
#include <iostream>
#include<fstream>
#include <memory>
#include <boost/lexical_cast.hpp>
std::string wczytajplik(std::string sciezka);
std::string string_to_hex(const std::string &input);
std::string hex_to_string(const std::string &input);
std::string hex_to_bin(std::string p);
void czekaj(void);
int nwd(int a, int b);
int odwr_mod(int a, int n);
void klucze_RSA(int &p,int &q,int &e,int &n, int &d);
void pot_mod(int *a, int w, int n,int numer);

void Split(std::string plainText, std::string *plainTextTemp, int &q);
#endif //RSA_FUNCTION_H
