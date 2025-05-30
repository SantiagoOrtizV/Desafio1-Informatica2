#ifndef FUNCIONES_H
#define FUNCIONES_H\

#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QDir>
#include <QDebug>


using namespace std;

QString construirRutaArchivo(const QString& carpeta, const QString& nombreArchivo) {
    /*
 * @brief Busca una archivo en una carpeta y devuelve su ruta.
 *
 * busca un archivo en una carpeta determinada y devuelve su ruta, a partir dse su nombre.
 *
 * @param carpeta ruta de la carpeta (tipo const QString&).
 * @param nombreArchivo nombre del archivo junto con su extension(tipo const QString&).
 * @return ruta del archivo en especifico (tipo QString).
 */
    QDir dir(carpeta);
    return dir.filePath(nombreArchivo);
}

unsigned char rotate(unsigned char ent, unsigned int n) {
    /*
 * @brief Rotacion a nivel de bits de un numero.
 *
 * Rota los binarios de un numero hacia la derecha, es decir, aquellos que se desbordan por la
 * derecha vuelven por la izquierda.
 *
 * @param ent numero al cual hacerle la rotacion (tipo unsigned char).
 * @param n cantidad de rotaciones a hacer (tipo unsigned int).
 * @return nu8mero resultado de las rotaciones (tipo unsigned char).
 */
    return (ent >> n) | (ent << (8 - n));
}

unsigned char* loadPixels(QString input, int &width, int &height){
    /*
 * @brief Carga una imagen BMP desde un archivo y extrae los datos de píxeles en formato RGB.
 *
 * Esta función utiliza la clase QImage de Qt para abrir una imagen en formato BMP, convertirla al
 * formato RGB888 (24 bits: 8 bits por canal), y copiar sus datos de píxeles a un arreglo dinámico
 * de tipo unsigned char. El arreglo contendrá los valores de los canales Rojo, Verde y Azul (R, G, B)
 * de cada píxel de la imagen, sin rellenos (padding).
 *
 * @param input Ruta del archivo de imagen BMP a cargar (tipo QString).
 * @param width Parámetro de salida que contendrá el ancho de la imagen cargada (en píxeles).
 * @param height Parámetro de salida que contendrá la altura de la imagen cargada (en píxeles).
 * @return Puntero a un arreglo dinámico que contiene los datos de los píxeles en formato RGB.
 *         Devuelve nullptr si la imagen no pudo cargarse.
 *
 * @note Es responsabilidad del usuario liberar la memoria asignada al arreglo devuelto usando `delete[]`.
 */

    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << std::endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
    height = imagen.height();
    int dataSize = width * height * 3;
    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);
        unsigned char* dstLine = pixelData + y * width * 3;
        memcpy(dstLine, srcLine, width * 3);
    }
    return pixelData;
}


unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels){
    /*
 * @brief Carga la semilla y los resultados del enmascaramiento desde un archivo de texto.
 *
 * Esta función abre un archivo de texto que contiene una semilla en la primera línea y,
 * a continuación, una lista de valores RGB resultantes del proceso de enmascaramiento.
 * Primero cuenta cuántos tripletes de píxeles hay, luego reserva memoria dinámica
 * y finalmente carga los valores en un arreglo de enteros.
 *
 * @param nombreArchivo Ruta del archivo de texto que contiene la semilla y los valores RGB.
 * @param seed Variable de referencia donde se almacenará el valor entero de la semilla.
 * @param n_pixels Variable de referencia donde se almacenará la cantidad de píxeles leídos
 *                 (equivalente al número de líneas después de la semilla).
 *
 * @return Puntero a un arreglo dinámico de enteros que contiene los valores RGB
 *         en orden secuencial (R, G, B, R, G, B, ...). Devuelve nullptr si ocurre un error al abrir el archivo.
 *
 * @note Es responsabilidad del usuario liberar la memoria reservada con delete[].
 */
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
    }
    archivo >> seed;
    int r, g, b;
    while (archivo >> r >> g >> b) {
        n_pixels++;
    }
    archivo.close();
    archivo.open(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }
    unsigned int* RGB = new unsigned int[n_pixels * 3];
    archivo >> seed;
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }
    archivo.close();
    return RGB;
}

void restarArreglos(unsigned int*ptrTxt, unsigned char*ptrM, int nPixeles){
    /*
 * @brief Resta la mascara de los datos de un txt.
 *
 * Esta función resta el arreglo lineal de la mascara (dado por la funcion loadPixels) del
 * arreglo lineal de un txt (dado por la funcion loadSeedMasking).
 *
 * @param ptrTxt puntero del arreglo lineal del txt (unsigned int).
 * @param ptrM puntero del arreglo lineal de la mascara (unsigned char).
 * @param nPixeles entero que representa el numero de pixeles (conjunto R,G,B) representados en el txt.
 *
 * @return vacio. (nomas modifica el arreglo dinamico de txt)
 *
 * @note
 */
    for (int k = 0; k < nPixeles * 3; k++) {
        ptrTxt[k] = ptrTxt[k] - ptrM[k];
    }



}

unsigned char doTransformation(unsigned char transformacion, unsigned char n, unsigned char nXor){
    /*
 * @brief Aplica una transformacion determinada a un numero.
 *
 * @param transformacion identificador de la transformacion a hacer (tipo unsigned char).
 * @param n numero al que hacerle la transformacion(tipo unsigned char).
 * @param nXor numero con el que hacerle xor en caso de que sea ese tipo de transformacion (tipo unsigned char).
 * @return resultado de la transformacion (tipo unsigned char).
 *
 * @note
 */
    switch(transformacion/10){
    case 0:{
        n = n^nXor;
        break;
    }
    case 1:{
        n = n>>transformacion%10;
        break;
    }
    case 2:{
        n = n<<transformacion%10;
        break;
    }
    case 3:{
        n = rotate(n,transformacion%10);
        break;
    }
    }
    return n;
}

void identificarTransformacion(unsigned int* ptrTxt,unsigned char* ptrI_D,int seed,unsigned char* ptrI_M,unsigned char* transformaciones,unsigned char contArr){
    /*
 * @brief identifica la transformacion hecha anteriormente
 *
 *
 *
 * @param ptrTxt apuntador a la lista de informacion de los pixeles en un archivo del tipo M#.txt determinado. (tipo unsigned int*)
 * @param ptrI_D apuntador a la lista de informacion de los pixeles en el archivo I_D.bmp. (tipo unsigned char* ptrI_D)
 * @param seed referencia del indice en del primer valor en el cual se aplica la mascara. (tipo int)
 * @param ptrI_M apuntador a la lista de informacion de los pixeles en el archivo I_M.bmp. (tipo unsigned char* ptrI_M)
 * @param transformaciones apuntador al array donde se guardan las transformaciones hechas. (tipo unsigned char)
 * @param contArr indice de control multiproposito referente al array transformaciones (tipo unsigned char)
 *
 * @return void
 *
 * @note
 */
    int con = 0;
    unsigned char transformacion;
    for (int i=0; con!=1; i++){
        con = 0;
        unsigned char n = ptrTxt[i];
        n = n^ptrI_M[seed+i];
        for (int j=contArr-1;j>=0&&contArr!=0; j--){
            n = doTransformation(transformaciones[j],n,ptrI_M[seed+i]);
        }
        if(n==ptrI_D[seed+i]){ //xor
            con++;
            transformacion = 0;
        }
        for(int k=1;k<=8;k++){
            unsigned char n = ptrTxt[i];
            n = n>>k;
            for (int j=contArr-1;j>=0&&contArr!=0; j--){
                n = doTransformation(transformaciones[j],n,ptrI_M[seed+i]);
            }
            if(n==ptrI_D[seed+i]){ //despl. der.
                con++;
                transformacion = 10 + k;
            }
        }
        for(int k=1;k<=7;k++){
            unsigned char n = ptrTxt[i];
            n = n<<k;
            for (int j=contArr-1;j>=0&&contArr!=0; j--){
                n = doTransformation(transformaciones[j],n,ptrI_M[seed+i]);
            }
            if(n==ptrI_D[seed+i]){ //despl. izq.
                con++;
                transformacion = 20 + k;
            }
        }
        for(unsigned int k=0;k<=7;k++){
            unsigned char n = ptrTxt[i];
            n = rotate(n,k);
            for (int j=contArr-1;j>=0&&contArr!=0; j--){
                n = doTransformation(transformaciones[j],n,ptrI_M[seed+i]);
            }
            if(n==ptrI_D[seed+i]){ //rot
                con++;
                transformacion = 30 + k;
            }
        }
    }
    transformaciones[contArr] = transformacion;
}

void PrintTransformaciones(unsigned char* transformaciones, unsigned int nTxt){
    /*
 * @brief imprime las transformaciones
 *
 * imprime las transformaciones con sus respectivos nombres y orden a partir de una
 * lista.
 *
 * @param transformaciones apuntador al array de transformaciones. (tipo unsigned char*)
 * @param nTxt cantidad de archivos tipo M#.txt. (tipo unsigned int)
 *
 * @return void
 *
 * @note
 */
    for (unsigned int i = 0 ; i < nTxt ; i++){
        switch(transformaciones[i]/10){
            case 0:
                cout << "la tranformacion #" << i+1 << " es xOR" << endl;
                break;
            case 1:
                cout << "la tranformacion #" << i+1 << " es desplazamiento a la izquierda de: " << transformaciones[nTxt-1-i]%10 << endl;
                break;
            case 2:
                cout << "la tranformacion #" << i+1 << " es desplazamiento a la derecha de: " << transformaciones[nTxt-1-i]%10 << endl;
                break;
            case 3:
                cout << "la tranformacion #" << i+1 << " es Rotacion a la derecha de: " << transformaciones[nTxt-1-i]%10 << endl;
                break;
        }
    }

}

#endif
