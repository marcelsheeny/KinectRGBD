#ifndef F_FILES_H
#define F_FILES_H

#include <opencv2\opencv.hpp>

typedef struct _npontos {
	CvPoint* pt;
	int npts;
}Npontos;

IplImage* converteArquivoParaImagem (char file[], int resX, int resY) ;

IplImage* converteVetorParaImagem (int ***v) ;

void guardaArquivosNumVetor (char depthFile[], char colorFile[], int*** saida) ;

#endif