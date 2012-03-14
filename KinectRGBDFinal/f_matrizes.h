
#ifndef F_MATRIZES_H
#define F_MATRIZES_H

#include "f_pontos.h"


void imprimeMatriz (double **mat, int n, int m);

double **liberaMatrizDouble (int m, int n, double **v);

double **alocaMatriz2DDouble (int m, int n);

double** transporMatriz (double **m, int x, int y);

double** multiplicaMatrizes (double** a, double** b, int ma, int na, int mb, int nb);

void gaussian(double** a, int* index, int n) ;

double **inverteMatriz(double** a, int n);

int*** alocaMatriz3Dint (int x, int y, int z);

double** construirMatriz (Ponto3d* pt, int npts);

double** gerarMatrizN0 (Ponto3d* pt, int npts);

double** vetorParaMatriz (double** v);

double** geraMatrizIdentidade (int n);

#endif