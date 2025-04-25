#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "funciones.h"
#include <string>

using namespace std;

int main(){
    int weightI_D,heightI_D,weightM,heightM,weightI_M,heightI_M;    //alto y ancho de las imagenes(necesario para funciones)
    string rutaCarpeta; //ruta carpeto donde se guardan todos los archivos necesarios
    cout << "Ingrese la ruta de la carpeta: ";
    getline(cin,rutaCarpeta);
    QString parCarpeta = QString::fromStdString(rutaCarpeta);
    unsigned int nTxt;  //cantidad de archivos del tipo M#.txt
    const QString rutaI_D = construirRutaArchivo(parCarpeta,"I_D.bmp");
    unsigned char* ptrI_D = loadPixels(rutaI_D, weightI_D, heightI_D); // apuntador hacia datos imagen
    const QString rutaM = construirRutaArchivo(parCarpeta,"M.bmp");
    unsigned char* ptrM = loadPixels(rutaM, weightM, heightM);
    const QString rutaI_M = construirRutaArchivo(parCarpeta,"I_M.bmp");
    unsigned char* ptrI_M = loadPixels(rutaI_M, weightI_M, heightI_M);
    cout << "Ingrese la cantidad de archivos txt: ";
    cin >> nTxt;
    unsigned char* transformaciones = new unsigned char[nTxt];  //array donde se guarda los tipos de transformaciones hechos
    cin.ignore();
    for(unsigned char i=0;i<nTxt;i++){ //CANTIDAD DE TXTs
        int seed; //semilla o ubicacion de la mascara
        int nPixeles = 0;
        QString rutaInputTxt = construirRutaArchivo(parCarpeta,QString("M%1.txt").arg(nTxt-i-1));
        string InputTxt = rutaInputTxt.toStdString();
        unsigned int* ptrTxt = loadSeedMasking(InputTxt.c_str(), seed, nPixeles); // apuntador hacia datos txt // (primer parametro verificar)
        restarArreglos(ptrTxt, ptrM, nPixeles);
        identificarTransformacion(ptrTxt,ptrI_D,seed,ptrI_M,transformaciones,i);
        delete [] ptrTxt;
        ptrTxt = nullptr;
    }
    PrintTransformaciones(transformaciones,nTxt);
    delete [] ptrI_D;
    ptrI_D = nullptr;
    delete [] ptrM;
    ptrM = nullptr;
    delete [] ptrI_M;
    ptrI_M = nullptr;
    delete [] transformaciones;
    transformaciones = nullptr;
    return 0;
}
