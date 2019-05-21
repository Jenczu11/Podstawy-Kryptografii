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
#include <gmp.h>
#include <gmpxx.h>
#define MODULUS_SIZE 1024                   /* This is the number of bits we want in the modulus */
#define BUFFER_SIZE ((MODULUS_SIZE/8))  /* This is the number of bytes in n and p */
typedef struct {
    mpz_t n; /* Modulus */
    mpz_t e; /* Public Exponent */
} public_key;

typedef struct {
    mpz_t n; /* Modulus */
    mpz_t e; /* Public Exponent */
    mpz_t d; /* Private Exponent */
    mpz_t p; /* Starting prime p */
    mpz_t q; /* Starting prime q */
} private_key;
void menu();
std::string wczytajplik(std::string sciezka);
std::string string_to_hex(const std::string &input);
std::string hex_to_string(const std::string &input);
void zapiszdopliku(std::string item, std::string sciezka);
void cleanfile();
void generate_keys(private_key* ku, public_key* kp);
#endif //RSA_FUNCTION_H
