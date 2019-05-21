//
// Created by Bartłomiej Jencz on 27.11.2018.
//

#ifndef DES_FUNCTION_H
#define DES_FUNCTION_H
#include <windows.h>
#include <sstream>
#include<string>
#include <iostream>
#include<fstream>
#include <memory>
#pragma comment(lib, "advapi32.lib")
void cleanfile();
void SplitAndBinary(std::string plainText, std::string plainTextTemp[], int &expansion, int &q);
void SplitAndBinary(std::string plainText,  std::unique_ptr<std::string[]> &plainTextTemp, int &expansion, int &q);
std::string crypto();
void menu();
void display16(std::string plainText);
void Split (std::string plainText, std::string plainTextTemp[], int &q);
void Split (std::string plainText,  std::unique_ptr<std::string[]> &plainTextTemp, int &q);
void Expand(std::string &plainText, int &expansion);
int check_if_valid(std::string &p);
std::string string_to_hex(const std::string &input);
std::string hex_to_string(const std::string &input);
std::string hex_to_bin(std::string p);
void WyswietlPodciagi(std::string plainTexttemp[],int IloscPodNapisow);
void WyswietlPodciagi(std::unique_ptr<std::string[]> &plainTexttemp,int IloscPodNapisow);


void DisplayDecrypt(std::string plainTextTemp[],int IloscPodNapisow, int expansion);
std::string wczytajplik(std::string);
void zapiszdopliku(std::string item, std::string sciezka);
void DisplayEncrypt(std::unique_ptr<std::string[]> &Ciphertemp, int IloscPodNapisow);
void DisplayEncrypt(std::shared_ptr<std::string[]> Ciphertemp, int IloscPodNapisow);
void DisplayDecrypt(std::unique_ptr<std::string[]> &plainTextTemp, int IloscPodNapisow, int expansion);
void DisplayEncrypt(std::string Ciphertemp, int IloscPodNapisow);
//



#endif //DES_FUNCTION_H
