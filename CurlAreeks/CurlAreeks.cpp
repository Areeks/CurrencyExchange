#define CURL_STATICLIB
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <conio.h>

#include "curl/curl.h"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
std::string curlReadBuffer(char link[]);
std::string GetTitle();
void getMoney(double &euro, double &usd, double &gbp);
double cinMoney(std::string waluta);
void fPause();
void Clear();




int main(void)
{   //std::cout << GetTitle(); TYTUL ZE STRONY ZAKORKUJ.PL
    double kwota, a, b, c;
    char x;
    do
    {
        Clear();
        std::cout << "-----Program Dedykowany mojemu skarbkowi - Kocham Cie Klaudia <3-----" << std::endl;
        std::cout << "-----------------Kantor Areeks - Arkadiusz Kazmierczak---------------";
        getMoney(a, b, c);
        std::cout << "1. Wymiana na EUR" << std::endl;
        std::cout << "2. Wymiana na USD" << std::endl;
        std::cout << "3. Wymiana na GBP" << std::endl;
        std::cout << "0. Wyjscie z programu" << std::endl << std::endl;
        std::cout << "Wybierz opcje: ";
        x = _getche();
        std::cout << std::endl;

        switch (x)
        {
        case '1':
            kwota = cinMoney("EUR");
            getMoney(a, b, c);
            std::cout << "Kwota po przewalutowaniu: " << std::fixed << std::setprecision(2) << kwota * a << " zl";
            fPause();
            break;

        case '2':
            kwota = cinMoney("USD");
            getMoney(a, b, c);
            std::cout << "Kwota po przewalutowaniu: " << std::fixed << std::setprecision(2) << kwota * b << " zl";
            fPause();
            break;

        case '3':
            kwota = cinMoney("GBP");
            getMoney(a, b, c);
            std::cout << "Kwota po przewalutowaniu: " << std::fixed << std::setprecision(2) << kwota * c << " zl";
            fPause();
            break;
        }
    } while (x != '0');
    return 0;
}


void Clear()
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif
}


void fPause()
{
    std::cout << "\n\nWcisnij enter...";
    std::cin.get();
    std::cin.get();
}

double cinMoney(std::string waluta)
{
    std::string x;
    double money;
    bool checkKropka = true;
    std::cout << "Podaj kwote w " << waluta << ": ";
    std::cin >> x;
    std::replace(x.begin(), x.end(), ',', '.');
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == '.')
        {
            if (checkKropka)
            {
                checkKropka = false;
            }
            else
            {
                x.erase(i, 1);
                i--;
            }
        }
        if ((x[i] < '0' || x[i] > '9') && x[i]!='.')
        {
            x.erase(i, 1);
            i--;
        }
    }
    if (x.size() == 0)
    {
        std::cout << "Wprowadzono bledna kwote!" << std::endl;
        x = "0";
    }
    money = std::stod(x);
    std::cout << std::endl;
    return money;
}



std::string curlReadBuffer(char link[])
{
    CURL* curl;
    CURLcode res;
    char url[128];
    strcpy_s(url, link);
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        return readBuffer;
    }
    else
    {
        return "BLAD!";
    }
}

void getMoney(double &euro, double &usd, double &gbp)
{
    char url[] = "https://www.nbp.pl/home.aspx?f=/statystyka/kursy.htm";
    std::string readBuffer = curlReadBuffer(url);
    std::string str1 = "<td>1 EUR</td>";
    size_t found = readBuffer.find(str1);
    std::string str2;
    if (found != std::string::npos)
    {
        str2 = readBuffer.substr(found, 25);
        str2 = str2.substr(18);
        size_t foundtwo = str2.find("<");
        str2 = str2.substr(0, foundtwo);
        std::replace(str2.begin(), str2.end(), ',', '.');
        euro = std::stod(str2);
    }

    str1 = "<td>1 USD</td>";
    found = readBuffer.find(str1);
    if (found != std::string::npos)
    {
        str2 = readBuffer.substr(found, 25);
        str2 = str2.substr(18);
        size_t foundtwo = str2.find("<");
        str2 = str2.substr(0, foundtwo);
        std::replace(str2.begin(), str2.end(), ',', '.');
        usd = std::stod(str2);
    }

    str1 = "<td>1 GBP</td>";
    found = readBuffer.find(str1);
    if (found != std::string::npos)
    {
        str2 = readBuffer.substr(found, 25);
        str2 = str2.substr(18);
        size_t foundtwo = str2.find("<");
        str2 = str2.substr(0, foundtwo);
        std::replace(str2.begin(), str2.end(), ',', '.');
        gbp = std::stod(str2);
    }

    std::cout << "\nAktualny Kurs Walut: " << std::endl;
    std::cout << std::fixed << std::setprecision(4) << "EUR: " << euro << " USD: " << usd << " GBP: " << gbp << std::endl << std::endl;
}


std::string GetTitle()
{
    char url[] = "https://zakorkuj.pl/index.php";
    std::string readBuffer = curlReadBuffer(url);
    std::string str1 = "<h1 class=\"mb-4\">";
    size_t found = readBuffer.find(str1);
    std::string str2;
    if (found != std::string::npos)
    {
        str2 = readBuffer.substr(found, 70);
        str2 = str2.substr(1);
        size_t foundone = str2.find(">");
        size_t foundtwo = str2.find("<");
        str2 = str2.substr(foundone + 1, foundtwo);
        foundtwo = str2.find("<");
        str2 = str2.substr(0, foundtwo);
        
        return str2;
    }
    else
    {
        return "Strona zmodyfikowana brak mozliwosci zaczerpniecia tytulu!";
    }
   
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}