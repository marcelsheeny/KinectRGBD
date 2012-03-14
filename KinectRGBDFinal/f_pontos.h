#ifndef F_PONTOS_H
#define F_PONTOS_H

typedef struct ponto3d {
    double x;
    double y;
    double z;
}Ponto3d;

typedef struct _npontos {
	Ponto3d* pt;
	int npts;
}Npontos;

void desenhaDeUmVetorTriangleStrip(int ***v, int resx, int resy, int divz,double dmax, double **matrix);

void desenhaDeUmVetorQuads(int ***v, int resx, int resy, int divz,double** matrix);

double distancia (Ponto3d p1, Ponto3d p2);

double** ransac(double** mq, Npontos ptsimg1, Npontos ptsimg2, int nIteracoes, int nPtsRansac);

double** minimosQuadrados (Npontos pontos1, Npontos pontos2);

void sift (char colorFile1[],char colorFile2[], Npontos* pontos1, Npontos* pontos2,int resx, int resy, int*** malha1, int*** malha2);

void desenhaDeUmVetorTriangleStripSemMatriz(int ***v, int resx, int resy, int divz, double dmax);

#endif