#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "funciones.h"
#include <string>

using namespace std;

int main(){
    string rutaInputI_D,rutaInputM,rutaInputI_M,rutaInputI_O; // rutas de las imagenes
    int weightI_D,heightI_D,weightM,heightM,weightI_M,heightI_M; // anchos y largos de las imagenes
    unsigned int nTxt;// numero de txts
    cout << "Ingrese la ruta de la imagen I_D: "; // ENTRADAS RUTAS
    getline(cin, rutaInputI_D);
    QString rutaI_D = QString::fromStdString(rutaInputI_D);
    unsigned char* ptrI_D = loadPixels(rutaI_D, weightI_D, heightI_D); // apuntador hacia datos imagen
    cout << "Ingrese la ruta de la imagen M: ";
    getline(cin, rutaInputM);
    QString rutaM = QString::fromStdString(rutaInputM);
    unsigned char* ptrM = loadPixels(rutaM, weightM, heightM);
    cout << "Ingrese la ruta de la imagen I_M: ";
    getline(cin, rutaInputI_M);
    QString rutaI_M = QString::fromStdString(rutaInputI_M);
    unsigned char* ptrI_M = loadPixels(rutaI_M, weightI_M, heightI_M);
    cout << "Ingrese la cantidad de archivos txt: ";
    cin >> nTxt;
    cin.ignore();
    for(unsigned char i=0;i<nTxt;i++){ //CANTIDAD DE TXTs
        unsigned char transformacion; //tipo de transformacion hecha (cada una tiene su respectivo numero)
        int seed;
        int nPixeles = 0;
        string rutaInputTxt;
        cout << "Ingrese la ruta del txt correspondiente: "; // ENTRADAS RUTAS
        getline(cin, rutaInputTxt);
        unsigned int* ptrTxt = loadSeedMasking(rutaInputTxt.c_str(), seed, nPixeles); // apuntador hacia datos txt // (primer parametro verificar)
        //seed *= 3;
        restarArreglos(ptrTxt, ptrM, nPixeles);
        transformacion = identificarTransformacion(ptrTxt,ptrI_D,seed,ptrI_M);
        int trans = transformacion;
        cout << trans << endl;
        transformacionInversa(ptrI_D,transformacion,weightI_D,heightI_D,ptrI_M);
        delete [] ptrTxt;
        ptrTxt = nullptr;
    }
    cout << "Ingrese la ruta de la foto a guardar: ";
    getline(cin, rutaInputI_O);
    QString rutaI_O = QString::fromStdString(rutaInputI_O);
    cout << exportImage(ptrI_D, weightI_D, heightI_D, rutaI_O);
    delete [] ptrI_D;
    ptrI_D = nullptr;
    delete [] ptrM;
    ptrM = nullptr;
    delete [] ptrI_M;
    ptrI_M = nullptr;
    return 0;
}
