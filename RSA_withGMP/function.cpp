//
// Created by Bartłomiej Jencz on 28.11.2018.
//

#include "function.h"
using namespace std;

//You have to change to your own
std::string working_directory="C:\\Users\\barte\\CLionProjects\\temp\\";


void menu() {
    std::cout<<"*****************************************\n";
    std::cout<<"*                                       *\n";
    std::cout<<"*             Program RSA               *\n";
    std::cout<<"*     1) Generowanie kluczy             *\n";
    std::cout<<"*     2) Szyfrowanie                    *\n";
    std::cout<<"*     3) Deszyfrowanie                  *\n";
    std::cout<<"*     4) Czyszczenie plikow i kluczy    *\n";
    std::cout<<"*     0) Zakonczenie programu           *\n";
    std::cout<<"*                                       *\n";
    std::cout<<"*****************************************\n";

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

std::string wczytajplik(std::string sciezka)
{
    std::fstream plik;
    std::string temp1=working_directory+"files\\";
    sciezka.insert(0,temp1);
    plik.open(sciezka);
    std::stringstream s1;
    std::string napis;

    if( plik.good() )
    {
       // std::cout << "Zawartosc pliku:" << std::endl;
        while( !plik.eof() )
        {
            getline( plik, napis );
            s1 << napis;
        }
        plik.close();
    }
    else
        std::cout << "Error! Nie udalo otworzyc sie pliku!" << std::endl;
//cout<<s1.str();
    return s1.str();
}
void zapiszdopliku(std::string item, std::string sciezka)
{
    std::fstream plik;
    std::string temp1=working_directory+"files\\";
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

void cleanfile() {
    std::ofstream plik;
    std::ofstream plik1;
    std::ofstream plik2;
    plik.open(working_directory+"files\\key",std::ofstream::out|std::ofstream::trunc);
    plik1.open(working_directory+"files\\cipher",std::ofstream::out|std::ofstream::trunc);
    plik2.open(working_directory+"files\\decipher",std::ofstream::out|std::ofstream::trunc);
    plik.close();
    plik1.close();
    plik2.close();
    plik.open(working_directory+"files\\kpn",std::ofstream::out|std::ofstream::trunc);
    plik1.open(working_directory+"files\\kpe",std::ofstream::out|std::ofstream::trunc);
    plik2.open(working_directory+"files\\kue",std::ofstream::out|std::ofstream::trunc);
    plik.close();
    plik1.close();
    plik2.close();
    plik.open(working_directory+"files\\kud",std::ofstream::out|std::ofstream::trunc);
    plik.close();
    plik.open(working_directory+"files\\cipher16",std::ofstream::out|std::ofstream::trunc);
    plik.close();
    plik.open(working_directory+"files\\plain160",std::ofstream::out|std::ofstream::trunc);
    plik.close();


}

/* NOTE: Assumes mpz_t's are initted in ku and kp */
void generate_keys(private_key* ku, public_key* kp)
{
    char buf[BUFFER_SIZE];
    int i;
    mpz_t phi; mpz_init(phi);
    mpz_t tmp1; mpz_init(tmp1);
    mpz_t tmp2; mpz_init(tmp2);

    srand(time(NULL));

    /* Instead of selecting e st. gcd(phi, e) = 1; 1 < e < phi, lets choose e
     * first then pick p,q st. gcd(e, p-1) = gcd(e, q-1) = 1 */
    // We'll set e globally.  I've seen suggestions to use primes like 3, 17 or
    // 65537, as they make coming calculations faster.
    mpz_set_ui(ku->e, 65537);
    cout<<"ku->e: "<<ku->e<<endl;

    /* Wybierz p i q */
    /* Zacznij od p */
    // Ustawiamy bity losowo
    for(i = 0; i < BUFFER_SIZE; i++)
        buf[i] = rand() % 0xFF;
    // Ustaw 2 najwyższe bity na 1 żeby int(tmp) był wystarczająca wielki
    buf[0] |= 0xC0;
    // Ustaw ostatni bit na 1 aby liczba była nieparzysta(lepsze dla znajdowania liczb pierwszych)
    buf[BUFFER_SIZE - 1] |= 0x01;
    // tablica char -> int
    mpz_import(tmp1, BUFFER_SIZE, 1, sizeof(buf[0]), 0, 0, buf);
    // Wybierz nastepna liczbe pierwsza wieksza od losowej liczby
    mpz_nextprime(ku->p, tmp1);
    /* Upewnij sie ze wynik bedzie dobry*/
    mpz_mod(tmp2, ku->p, ku->e);        /* Jezeli p mod e == 1, gcd(phi, e) != 1 */
    while(!mpz_cmp_ui(tmp2, 1))
    {
        mpz_nextprime(ku->p, ku->p);    /* To wybierz nastepna liczbe i testy milera rabina 15 */
        mpz_mod(tmp2, ku->p, ku->e);
    }

    /* Teraz wybieramy q */
    do {
        for(i = 0; i < BUFFER_SIZE; i++)
            buf[i] = rand() % 0xFF;
        // Ustaw 2 najwyższe bity na 1 żeby int(tmp) był wystarczająca wielki
        buf[0] |= 0xC0;
        // Ustaw ostatni bit na 1 aby liczba była nieparzysta(lepsze dla znajdowania liczb pierwszych)
        buf[BUFFER_SIZE - 1] |= 0x01;
        mpz_import(tmp1, (BUFFER_SIZE), 1, sizeof(buf[0]), 0, 0, buf);
        mpz_nextprime(ku->q, tmp1);
        mpz_mod(tmp2, ku->q, ku->e);
        while(!mpz_cmp_ui(tmp2, 1))
        {
            mpz_nextprime(ku->q, ku->q);
            mpz_mod(tmp2, ku->q, ku->e);
        }
    } while(mpz_cmp(ku->p, ku->q) == 0); /* Jeżeli mamy takie same liczby pierwsze, sproboj ponownie */

    /* Oblicz n = p x q */
    mpz_mul(ku->n, ku->p, ku->q);

    /* Wylicz phi(n) = (p-1)(q-1) */
    mpz_sub_ui(tmp1, ku->p, 1);
    mpz_sub_ui(tmp2, ku->q, 1);
    mpz_mul(phi, tmp1, tmp2);

    /* Oblicz d (multiplicative inverse of e mod phi) */
    if(mpz_invert(ku->d, ku->e, phi) == 0)
    {
        mpz_gcd(tmp1, ku->e, phi);
        printf("gcd(e, phi) = [%s]\n", mpz_get_str(NULL, 16, tmp1));
        printf("Invert failed\n");
    }

    /* Ustawianie klucza publicznego*/
    mpz_set(kp->e, ku->e);
    mpz_set(kp->n, ku->n);

    return;
}
