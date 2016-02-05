#include "image.h"
#include <iostream>
#include <fstream>
#include <string>
#include "CImg.h"
#include <math.h>

using namespace std;
using namespace cimg_library;

Imagecon::Imagecon(){
}

struct pixel{
    int r;  //red
    int g;  //green
    int b;  //blue
};

int naLiczbe(char liczba[5]){       //zmienia tablice z cyframi na liczbe
    int wynik=0;
    int temp;
    short int puste=0;
    for(int i=0; i<5; i++){
        if(liczba[i]<48 || liczba[i]>57){
            puste++;
        }
    }
    for(int i=0; i<5-puste;i++){
        temp=liczba[i]-48;
        for(int j=0; j<4-i-puste;j++)temp=temp*10;
        wynik+=temp;
    }
    return wynik;
}

char* naHex(int liczba, char* liczbaHex){   //konwersja na system szesnastkowy
    short int tab[2];
    tab[1]=liczba%16;
    liczba=liczba/16;
    tab[0]=liczba%16;
    for (int i=0; i<2; i++){
        liczbaHex[i]=tab[i]+48;
    }
    return liczbaHex;
}

int zHex(char* liczbaHex){      //konwersja na system dzisietny
    int liczba=0;
    liczba+=(liczbaHex[0]-48)*16;
    liczba+=(liczbaHex[1]-48);
    return liczba;
}

void Imagecon::entropia(){
    double entropia=0;
    CImg<unsigned char> image(nazwa.c_str());
    int picsize=image.width()*image.height();
    pixel *tab=new pixel[picsize];
    double *tab2=new double[picsize];
    for(int i=0;i<picsize;i++){
        tab2[i]=0;
    }
    int pom=0;
    double temp;
    bool flag;
    for(int j=0; j<image.height();j++){
        for(int i=0; i<image.width();i++){
                flag=0;
            for(int k=0;k<picsize;k++){
                if(tab[k].r==image(i,j,0) && tab[k].g==image(i,j,1) && tab[k].b==image(i,j,2)){ //czy jest juz taki pixel
                    tab2[k]++;      //jak tak to zwiekszam licznik pixela
                    flag=1;
                    break;
                }
            }
            if(flag==0){
                for(int k=0;k<picsize;k++){     //jak nie ma to dodaje nowy kolor
                    if(tab2[k]==0){
                        tab[k].r=image(i,j,0);
                        tab[k].g=image(i,j,1);
                        tab[k].b=image(i,j,2);
                        tab2[k]++;
                        break;
                    }
                }
            }
        }
    }
    for(int k=0;k<picsize;k++){ //ile kolorow
        if(tab2[k]==0){
            pom=k-1;
            break;
        }
    }
    double sizet=image.width()*image.height();
    for(int i=0; i<pom; i++){   //liczenie entropii
        temp=tab2[i]/sizet*1.0; //prawdopodobienstwo
        entropia-=temp*log2(temp);      //wzor
    }
    cout<<"Entropia danych wynosi "<<entropia<<endl;
    delete tab;
    delete tab2;
}

void Imagecon::RLE(){
    string nazwa2;
    char *liczbaHex=new char[2];
    cout<<"Podaj nazwe pliku wyjsciowego\n";
    cin>>nazwa2;
    nazwa2+=".rle";
    CImg<unsigned char> image(nazwa.c_str());
    int licznik=1;
    fstream plik2;
    plik2.open(nazwa2.c_str(), ios::out);
    pixel wybrany;
    wybrany.r=image(0,0,0); //wybranie zerowego pixela
    wybrany.g=image(0,0,1);
    wybrany.b=image(0,0,2);
    plik2<<image.width()<<" "<<image.height()<<" "; //info na poczatku
    for(int j=0; j<image.height();j++){
        for(int i=0; i<image.width();i++){
            if(j==0 && i==0)i=1;    //pominiecie pierwszego pixela bo juz go ustawilem
            if(wybrany.r==image(i,j,0) && wybrany.g==image(i,j,1) && wybrany.b==image(i,j,2)){ //kolejny pixel ten sam
                licznik++;
            }else{     //kolejny pixel inny
                plik2<<licznik; //wpisanie do pliku
                plik2<<" ";
                liczbaHex=naHex(wybrany.r, liczbaHex);
                plik2<<liczbaHex[0]<<liczbaHex[1];
                liczbaHex=naHex(wybrany.g, liczbaHex);
                plik2<<liczbaHex[0]<<liczbaHex[1];
                liczbaHex=naHex(wybrany.b, liczbaHex);
                plik2<<liczbaHex[0]<<liczbaHex[1];
                wybrany.r=image(i,j,0); //ustawienie kolejnego rozpatrywanego koloru
                wybrany.g=image(i,j,1);
                wybrany.b=image(i,j,2);
                licznik=1;
            }
        }
    }
    plik2<<licznik; //wpisanie do pliku
    plik2<<" ";
    liczbaHex=naHex(wybrany.r, liczbaHex);
    plik2<<liczbaHex[0]<<liczbaHex[1];
    liczbaHex=naHex(wybrany.g, liczbaHex);
    plik2<<liczbaHex[0]<<liczbaHex[1];
    liczbaHex=naHex(wybrany.b, liczbaHex);
    plik2<<liczbaHex[0]<<liczbaHex[1];
    cout<<"Zapisano jako "<<nazwa2<<endl;
    delete liczbaHex;
}



void Imagecon::reverseRLE(){
    fstream plik;
    string linia;
    char *liczbaHex=new char[2];
    pixel wybrany;
    int pom,x=0,y=0;  //pomocnicze
    unsigned int lok=0;    //lokalizacja w czytanym pliku
    short int cyfra=0;
    int szer, wys;
    char liczba[]={0,0,0,0,0};    //zakladam ze nie bedzie pod rzad jakiegos pixela wiecej niz 99999 razy
    plik.open(nazwa.c_str(), ios::in);  //otwieranie pliku
    getline(plik, linia);
    while(linia[lok]!=32){ //szerokosc obrazka
        liczba[cyfra]=linia[lok];
        lok++;
        cyfra++;
    }
    szer=naLiczbe(liczba);
    for (int i=0;i<5;i++)liczba[i]=0;
    cyfra=0;
    lok++;
    while(linia[lok]!=32){ //wysokosc obrazka
        liczba[cyfra]=linia[lok];
        lok++;
        cyfra++;
    }
    wys=naLiczbe(liczba);
    for (int i=0;i<5;i++)liczba[i]=0;
    cyfra=0;
    lok++;
    CImg<unsigned char> image(szer,wys,1,3,0);  //stworzenie obrazu

    while(lok<linia.length()){  //analiza reszty pliku
        while(linia[lok]!=32){  //ilosc powtorzen
            liczba[cyfra]=linia[lok];
            lok++;
            cyfra++;
        }
        pom=naLiczbe(liczba);
        for (int i=0;i<5;i++)liczba[i]=0;
        cyfra=0;
        lok++;
        for(int i=0; i<2;i++){  //czerwony
            liczbaHex[cyfra]=linia[lok];
            lok++;
            cyfra++;
        }
        wybrany.r=zHex(liczbaHex);
        cyfra=0;
        for(int i=0; i<2;i++){  //zielony
            liczbaHex[cyfra]=linia[lok];
            lok++;
            cyfra++;
        }
        wybrany.g=zHex(liczbaHex);
        cyfra=0;
        for(int i=0; i<2;i++){  //niebieski
            liczbaHex[cyfra]=linia[lok];
            lok++;
            cyfra++;
        }
        wybrany.b=zHex(liczbaHex);
        cyfra=0;

        for(int i=0; i<pom; i++){   //ustawianie wartosci pixeli
            image(x,y,0)=wybrany.r;
            image(x,y,1)=wybrany.g;
            image(x,y,2)=wybrany.b;
            x++;
            if(x==szer){
                x=0;
                y++;
            }
        }
    }
    CImgDisplay main_disp(image,"Obraz RLE"); //wyswietlanie
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
    delete liczbaHex;
}
