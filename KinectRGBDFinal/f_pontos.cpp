#include <stdlib.h>

#include <opencv2\opencv.hpp>
#include <gl/glut.h>
#include <time.h>

#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include "f_matrizes.h"
#include "f_pontos.h"
#include "f_arquivos.h"

/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49

double distancia (Ponto3d p1, Ponto3d p2) {
	return sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2)+pow(p2.z-p1.z,2));
}

double** minimosQuadrados (Npontos pontos1, Npontos pontos2) {

	double** m1 = construirMatriz(pontos1.pt,pontos1.npts);
	double** m2 = gerarMatrizN0(pontos2.pt,pontos2.npts);

	double** mt = transporMatriz(m1,3*pontos1.npts,12);
	double** mm = multiplicaMatrizes(mt,m1,12,3*pontos1.npts,3*pontos1.npts,12);
	double** mi = inverteMatriz(mm,12);
	double** mm2 = multiplicaMatrizes(mi,mt,12,12,12,3*pontos1.npts);
	double** mq = multiplicaMatrizes(mm2,m2,12,3*pontos1.npts,3*pontos2.npts,1);

	//double** mq = multiplicaMatrizes(multiplicaMatrizes(inverteMatriz(multiplicaMatrizes(transporMatriz(m1,3*pontos1.npts,12),m1,12,3*pontos1.npts,3*pontos1.npts,12),12),transporMatriz(m1,3*pontos1.npts,12),12,12,12,3*pontos1.npts),m2,12,3*pontos1.npts,3*pontos2.npts,1);
	double** matrixmq =  vetorParaMatriz(mq);

	liberaMatrizDouble(12,1,mq);

	liberaMatrizDouble(12,3*pontos1.npts,mt);
	liberaMatrizDouble(12,12,mm);
	liberaMatrizDouble(12,12,mi);
	liberaMatrizDouble(12,3*pontos1.npts,mm2);
	liberaMatrizDouble(3*pontos1.npts,12,m1);
	liberaMatrizDouble(3*pontos1.npts,1,m2);

	return matrixmq;
}
void desenhaDeUmVetorTriangleStrip(int ***v, int resx, int resy, int divz, double dmax, double** matrix) {

	glBegin(GL_TRIANGLE_STRIP);
	Ponto3d p1, p2;
	int i = 0, j = 0;

	double** pos1 = alocaMatriz2DDouble(4,1);
	double** res1 = alocaMatriz2DDouble(4,1);

	double** pos2 = alocaMatriz2DDouble(4,1);
	double** res2 = alocaMatriz2DDouble(4,1);

	p1.x = i; p1.y = j; p1.z = v[i][j][0];
	pos1[0][0] = (double)i; pos1[1][0] = (double)j; pos1[2][0] = (double)v[i][j][0]; pos1[3][0] = 1.0;
	res1 = multiplicaMatrizes(matrix,pos1,4,4,4,1);
	//printf ("%lf %lf %lf %lf\n",res1[0][0],res1[1][0],res1[2][0],res1[3][0]);

	for (int i = 0; i < resx-1; i++) {
		for (int j = 0; j < resy; j++) {
			if (v[i][j][0] != 0 && v[i+1][j][0] != 0 ) {

				p2.x = i+1; p2.y = j; p2.z = v[i+1][j][0];
				pos2[0][0] = (double)i+1; pos2[1][0] = (double)j; pos2[2][0] = (double)v[i+1][j][0]; pos2[3][0] = 1.0;
				res2 = multiplicaMatrizes(matrix,pos2,4,4,4,1);
				//printf ("%lf %lf %lf %lf\n",res2[0][0],res2[1][0],res2[2][0],res2[3][0]);

				if (distancia(p1,p2) < dmax) {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f((GLfloat)v[i][j][1]/255, (GLfloat)v[i][j][2]/255, (GLfloat)v[i][j][3]/255);
					glNormal3f(0.0, 0.0, 1.0);
					//printf ("%lf %lf %lf %lf\n",res1[0][0],res1[1][0],res1[2][0],res1[3][0]);
					//printf ("%lf %lf %lf %lf\n",res2[0][0],res2[1][0],res2[2][0],res2[3][0]);
					glVertex3i(res1[0][0], -res1[1][0] , -res1[3][0]/divz);
					glVertex3i(res2[0][0], -res2[1][0] , -res2[3][0]/divz);
				}
				else {
					glEnd();
				}
			}
			p1.x = i+1; p1.y = j; p1.z = v[i+1][j][0];
			pos1[0][0] =(double) i+1; pos1[1][0] =(double) j; pos1[2][0] =(double) v[i+1][j][0]; pos1[3][0] = 1.0;
			res1 = multiplicaMatrizes(matrix,pos1,4,4,4,1);
			//printf ("%lf %lf %lf %lf\n",res1[0][0],res1[1][0],res1[2][0],res1[3][0]);
				
		}
	}
	glEnd();
}

void desenhaDeUmVetorTriangleStripSemMatriz(int ***v, int resx, int resy, int divz, double dmax) {

	glBegin(GL_TRIANGLE_STRIP);
	Ponto3d p1, p2;
	int i = 0, j = 0;

	p1.x = i; p1.y = j; p1.z = v[i][j][0];

	for (int i = 0; i < resx-1; i++) {
		for (int j = 0; j < resy; j++) {
			if (v[i][j][0] != 0 && v[i+1][j][0] != 0 ) {

				p2.x = i+1; p2.y = j; p2.z = v[i+1][j][0];


				if (distancia(p1,p2) < dmax) {
					glBegin(GL_TRIANGLE_STRIP);
					glColor3f((GLfloat)v[i][j][1]/255, (GLfloat)v[i][j][2]/255, (GLfloat)v[i][j][3]/255);
					glNormal3f(0.0, 0.0, 1.0);

					glVertex3i(i, -j , -v[i][j][0]/divz);
					glVertex3i(i+1, -j , -v[i+1][j][0]/divz);
				}
				else {
					glEnd();
				}
			}
			p1.x = i+1; p1.y = j; p1.z = v[i+1][j][0];

				
		}
	}
	glEnd();
}




double** ransac (double** mq, Npontos ptsimg1, Npontos ptsimg2, int nIteracoes, int nPtsRansac) {
	if (ptsimg2.npts > 15) {
		double erro = 0;
		double menorErro = 999999999999;
		Npontos ptRansac;
		ptRansac.npts = nPtsRansac;
		ptRansac.pt = (Ponto3d*)malloc(sizeof(Ponto3d)*nPtsRansac);

		Npontos ptRansacImg1;
		ptRansacImg1.npts = nPtsRansac;
		ptRansacImg1.pt = (Ponto3d*)malloc(sizeof(Ponto3d)*nPtsRansac);

		double** posRansac = alocaMatriz2DDouble(4,1);
		double** posMq = alocaMatriz2DDouble(4,1);
		srand ( time(NULL) );
		int random;

		double** melhorMatriz = alocaMatriz2DDouble(4,4);

		double** matrizRansac = alocaMatriz2DDouble(4,4);

		double** respRansac = alocaMatriz2DDouble(4,1);
		double** respMq = alocaMatriz2DDouble(4,1);

		double dif1, dif2, dif3;
		for (int i = 0; i < nIteracoes; i++) {
			for (int n = 0; n < nPtsRansac; n++) {
				random = rand()%(ptsimg2.npts+1);

				ptRansacImg1.pt[n].x = ptsimg1.pt[random].x;
				ptRansacImg1.pt[n].y = ptsimg1.pt[random].y;
				ptRansacImg1.pt[n].z = ptsimg1.pt[random].z;

				ptRansac.pt[n].x = ptsimg2.pt[random].x;
				ptRansac.pt[n].y = ptsimg2.pt[random].y;
				ptRansac.pt[n].z = ptsimg2.pt[random].z;
			}

			matrizRansac = minimosQuadrados(ptRansacImg1,ptRansac);

			for (int cnt = 0; cnt < nPtsRansac; cnt++) {
				posRansac[0][0] = ptRansac.pt[cnt].x;
				posRansac[1][0] = ptRansac.pt[cnt].y;
				posRansac[2][0] = ptRansac.pt[cnt].z;
				posRansac[3][0] = 1;

				respRansac = multiplicaMatrizes(matrizRansac,posRansac,4,4,4,1);
				respMq = multiplicaMatrizes(mq,posRansac,4,4,4,1);

				dif1 = respMq[0][0] - respRansac[0][0];
				dif2 = respMq[1][0] - respRansac[1][0];
				dif3 = respMq[2][0] - respRansac[2][0];
				erro += sqrt((dif1)*(dif1)+
					         (dif2)*(dif2)+
							 (dif3)*(dif3));
			}
			
			if (erro < menorErro) {
				menorErro = erro;
				melhorMatriz = matrizRansac;
			}

			erro = 0;
		}

		free(ptRansac.pt);
		free(ptRansacImg1.pt);
		liberaMatrizDouble(4,1,posRansac);
		liberaMatrizDouble(4,1,posMq);
		liberaMatrizDouble(4,1,respRansac);
		liberaMatrizDouble(4,1,respMq);
		liberaMatrizDouble(4,4,matrizRansac);

		return melhorMatriz;
	}

	else {
		printf ("Numero de pontos do SIFT insuficiente\n");
		return NULL;
	}
}

void sift (char colorFile1[], char colorFile2[], Npontos* pontos1, Npontos* pontos2,int resx, int resy, int*** malha1, int*** malha2) { 
	//Parte do programa que gera os pontos do Sift
	IplImage* img1, * img2, * stacked;
	struct feature* feat1, * feat2, * feat;
	struct feature** nbrs;
	struct kd_node* kd_root;
	CvPoint pt1, pt2;
	double d0, d1;
	int n1, n2, k, i, m = 0;

	img1 = converteArquivoParaImagem(colorFile1,resx,resy);
	img2 = converteArquivoParaImagem(colorFile2,resx,resy);
	stacked = stack_imgs( img1, img2 );

	n1 = sift_features( img1, &feat1 );
	n2 = sift_features( img2, &feat2 );
	kd_root = kdtree_build( feat2, n2 );
	for( i = 0; i < n1; i++ )
	{
		feat = feat1 + i;
		k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
		if( k == 2 )
		{
			d0 = descr_dist_sq( feat, nbrs[0] );
			d1 = descr_dist_sq( feat, nbrs[1] );
			if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
			{
				pt1 = cvPoint( cvRound( feat->x ), cvRound( feat->y ) );
				pt2 = cvPoint( cvRound( nbrs[0]->x ), cvRound( nbrs[0]->y ) );


				(*pontos1).pt[m].x = pt1.x;(*pontos1).pt[m].y = pt1.y;(*pontos1).pt[m].z = malha1[pt1.x][pt1.y][0];
				(*pontos2).pt[m].x = pt2.x;(*pontos2).pt[m].y = pt2.y;(*pontos2).pt[m].z = malha2[pt2.x][pt2.y][0];


				//(*pontos2).pt[m].x = pt1.x;  (*pontos2).pt[m].y = pt1.y;  (*pontos2).pt[m].z = malha1[pt1.x][pt1.y][0];
				//(*pontos1).pt[m].x = pt2.x;  (*pontos1).pt[m].y = pt2.y;  (*pontos1).pt[m].z = malha2[pt2.x][pt2.y][0];

				pt2.y += img1->height;
				cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 );
				m++;
				feat1[i].fwd_match = nbrs[0];
			}
		}
		free( nbrs );
	}

	(*pontos1).npts = m;
	(*pontos2).npts = m;
	//cvShowImage("SIFT", stacked);
}

	

/*
void desenhaDeUmVetorQuads(int ***v, int resx, int resy, int divz, double dmax) {

	glBegin(GL_QUADS);

	for (int i = 0; i < resx; i++) {
		for (int j = 0; j < resy; j++) {
			if (v[i][j][0] != 0) {
				glColor3f((GLfloat)v[i][j][1]/255, (GLfloat)v[i][j][2]/255, (GLfloat)v[i][j][3]/255);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3i(i, -j , -v[i][j][0]/divz);
				glVertex3i(i+1, -j , -v[i][j][0]/divz);
				glVertex3i(i+1, -j-1 , -v[i][j][0]/divz);
				glVertex3i(i, -j-1 , -v[i][j][0]/divz);
				
			}
		}
	}
	glEnd();
}
*/