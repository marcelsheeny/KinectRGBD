#ifndef F_ARQUIVOS_H
#define F_ARQUIVOS_H

#include <opencv2\opencv.hpp>


IplImage* converteArquivoParaImagem (char file[], int resX, int resY) ;

int*** converteArquivosParaVetor (char depthFile[], char colorFile[],int resx, int resy) ;

void desenhaDoArquivo (char depthFile[], char colorFile[],int resx, int resy);


#endif
