#include <iostream>
#include <fstream>
#include <string>
#include "image.h"
#include "image.cpp"

using namespace std;

int main()
{
    string nazwa;
    short int wybor=3;
    Imagecon imageConverter;
    fstream plik;
    string linia;
    while(getline(plik, linia)){
            cout<<linia<<endl;
    }

    while(1){
        if(wybor==3){       //czy wczytac nowy plik
            cout<<"Podaj nazwe pliku z rozszerzeniem\n";
            cin>>nazwa;
            imageConverter.nazwa=nazwa;
        }

        cout<<"Wybierz metode konwersji: \n1 - Entropia danych \n2 - RLE \n3 - reverse RLE\n";
        cin>>wybor;
        switch(wybor){
            case 1:
                imageConverter.entropia();
                break;
            case 2:
                imageConverter.RLE();

                break;
            case 3:
                imageConverter.reverseRLE();
                break;
        }
        cout<<"Operacja zakonczona\nZamknac program? \n1 - Tak\n2 - Nie(stary plik)\n3 - Nie(nowy plik)\n"; //zamykanie programu
        cin>>wybor;
        if(wybor==1)break;
    }
    return 0;
}
