#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "funciones.h"

using namespace std;

int main(){
    QString rutaImagenP,rutaImagenM,rutaImagenI_M,rutaImagenF; // rutas de las imagenes
    int = weightP,heightP,weightM,heightM,weightI_M,heightI_M; // anchos y largos de las imagenes
    unsigned int = nTxt;// numero de txts
    cout << "Ingrese la ruta de la imagen P: "; // ENTRADAS RUTAS
    cin >> rutaImagenP;
    unsigned char* ptrP = loadPixels(rutaImagenP, weightP, heightP); // apuntador hacia datos imagen
    cout << "Ingrese la ruta de la imagen M: ";
    cin >> rutaImagenM;
    unsigned char* ptrM = loadPixels(rutaImagenM, weightM, heightM);
    cout << "Ingrese la ruta de la imagen I_M: ";
    cin >> rutaImagenI_M;
    unsigned char* ptrI_M = loadPixels(rutaImagenI_M, weightI_M, heightI_M);
    cout << "Ingrese la cantidad de archivos txt: ";
    cin >> ntxt;
    for(int i=0;i<nTxt;i++){ //CANTIDAD DE TXTs
        QString rutaTxt;
        unsigned char transformacion; //tipo de transformacion hecha (cada una tiene su respectivo numero)
        int seed;
        int nPixeles;
        cout << "Ingrese la ruta del txt correspondiente: "; // ENTRADAS RUTAS
        cin >> rutaTxt;
        unsigned char* ptrTxt = loadSeedMasking(rutaTxt, seed, nPixeles); // apuntador hacia datos txt // (primer parametro verificar)
        restarArreglos(ptrTxt, ptrM);
        transformacion = identificarTransformacion(ptrTxt);
        transformacionInversa(ptrP,transformacion);
        delete ptrTxt;
    }
    cout << "Ingrese la ruta de la foto a guardar: ";
    cin >> rutaImagenF;
    cout << exportImage(ptrP, weightP, heightP, rutaImagenF);
    delete ptrP;
    delete ptrM;
    delete ptrI_M;
    return 0;
}
