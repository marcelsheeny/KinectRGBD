#include <opencv2\opencv.hpp>
#include <gl\glut.h>

#include "f_matrizes.h"


IplImage* converteArquivoParaImagem (char file[], int resX, int resY) {
	//Inicializa o fc (File Color)
	FILE* fc = NULL;
	//Inicializa o array que vai capturar os dados do arquivo
	int color[5] = {0,0,0,0,0};
	IplImage* img = cvCreateImage (cvSize(resX,resY), IPL_DEPTH_8U, 3);

	fc = fopen (file, "r");
	//Verifica erro ao abrir o arquivo
	if(fc == NULL) {
		printf("ERRO!\n");
		//exit(1);
	}
	else {
		int i = 0;
		while (i != resX*resY -1) {
				fscanf(fc,"%d",&color[0]);
				fscanf(fc,"%d",&color[1]);
				fscanf(fc,"%d",&color[2]);
				fscanf(fc,"%d",&color[3]);
				fscanf(fc,"%d",&color[4]);
				//Função que desenha na tela
				cvSet2D(img, color[1], color[0], cvScalar( color[2], color[3], color[4], 0 ));
				i++;
		}
	}
	return img;
}

IplImage* converteVetorParaImagem (int ***v) {

	IplImage* img = cvCreateImage (cvSize(320,240), IPL_DEPTH_8U, 3);

	
	for (int i = 0; i < 240; i++) {
		for (int j = 0; j < 320; j++) {
			/*
			img->imageData[j*3 + i*img->widthStep] =  v[j][i][1];
			img->imageData[j*3 + i*img->widthStep+1] =  v[j][i][2];
			img->imageData[j*3 + i*img->widthStep+2] =  v[j][i][3];
		*/
			cvSet2D(img,i,  j,cvScalar( v[j][i][1], v[j][i][2], v[j][i][3], 0 ));
		
		
		}
	}

	//cvShowImage("teste",img);
	//cvWaitKey(0);
	return img;
}

int*** converteArquivosParaVetor (char depthFile[], char colorFile[], int resx, int resy) {
	int i = 0, j = 0;
	int depth[3] = {0,0,0};
	int color[5] = {0,0,0,0,0};
	FILE *fd;
	FILE *fc;

	int*** saida = alocaMatriz3Dint(320,240,4);

	fd = fopen (depthFile,"r"); //abre o arquivo
	fc = fopen (colorFile,"r");

	//Verifica erro
	if(fd == NULL || fc == NULL) {
		printf("ERRO!\n");
		system ("pause");
		printf("Erro na conversão\n");
		return NULL;
		//exit(1);
	}
	for (int j = 0; j <resy; j++) {
		for (int i = 0; i < resx; i++) {
			//armazena o conteudo dos arquivos nas variaveis
			fscanf(fd,"%d",&depth[0]);
			fscanf(fd,"%d",&depth[1]);
			fscanf(fd,"%d",&depth[2]);
			fscanf(fc,"%d",&color[0]);
			fscanf(fc,"%d",&color[1]);
			fscanf(fc,"%d",&color[2]);
			fscanf(fc,"%d",&color[3]);
			fscanf(fc,"%d",&color[4]);
			saida[i][j][0] = depth[2];  //profundidade
			saida[i][j][1] = color[2];  //Red
			saida[i][j][2] = color[3];  //Green
			saida[i][j][3] = color[4];  //Blue		
		
		}
	}
	fclose(fd);
	fclose(fc);
	return saida;
}


void desenhaDoArquivo (char depthFile[], char colorFile[], int resx, int resy) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int i = 0;
	GLint depth[3] = {0,0,0};
	GLint color[5] = {0,0,0,0,0};
	FILE *fd;
	FILE *fc;
	//char arqColor[32];
	//char arqDepth[32];

	//sprintf_s (arqDepth, 32, "depth[%d].txt", n);
	//sprintf_s (arqColor, 32, "color[%d].txt", n);

	fd = fopen (depthFile,"r"); //abre o arquivo
	fc = fopen (colorFile,"r");

	//Verifica erro
	if(fd == NULL || fc == NULL) {
		printf("ERRO!\n");
		system ("pause");
		printf ("Erro no desenhoz\n");
		return;
		//exit(1);
	}

	//for (int j = 120; j > -120;j--) {
		//for (int i = -160; i < 160; i++) {
		while (i < resx*resy - 1) {
			//armazena o conteudo dos arquivos nas variaveis
			fscanf(fd,"%d",&depth[0]);
			fscanf(fd,"%d",&depth[1]);
			fscanf(fd,"%d",&depth[2]);
			fscanf(fc,"%d",&color[0]);
			fscanf(fc,"%d",&color[1]);
			fscanf(fc,"%d",&color[2]);
			fscanf(fc,"%d",&color[3]);
			fscanf(fc,"%d",&color[4]);
			//printf("%d ",depth);
			depth[2] = depth[2]/100;
			//Tamanho do ponto
			glPointSize(3);
			glBegin(GL_POINTS);
			//
			//if (depth > 100  && depth < 200) {

			//Ignora o 0, pois parte da imagem de cor nao é mapeada pela profundidade
			if (depth[2] != 0 ) {
				//Põe as cores
				glColor3f((float)color[4]/255, (float)color[3]/255, (float)color[2]/255);
					glNormal3f(0.0, 0.0, 1.0);
					//posiçao em que irá por os pontos
					glVertex3i(color[0] - 160, 120 - color[1] , -depth[2]);
				
			}
			i++;
		}
	//}
	fclose(fd);
	fclose(fc);	
}

void desenhaDeUmVetorQuads(int ***v, int resx, int resy, int divz,double** matrix) {

	glBegin(GL_QUADS);
	double** pos = alocaMatriz2DDouble(4,1);
	double** res = alocaMatriz2DDouble(4,1);


	for (int i = 0; i < resx; i++) {
		for (int j = 0; j < resy; j++) {
			if (v[i][j][0] != 0) {

				pos[0][0] = (double)i; 
				pos[1][0] = (double)j; 
				pos[2][0] = (double)v[i][j][0]; 
				pos[3][0] = 1.0;
				res = multiplicaMatrizes(matrix,pos,4,4,4,1);

				glColor3f((GLfloat)v[i][j][3]/255, (GLfloat)v[i][j][2]/255, (GLfloat)v[i][j][1]/255);
				glNormal3f(0.0, 0.0, 1.0);
				glVertex3d((GLdouble)res[0][0], (GLdouble)(-res[1][0]) , (GLdouble)(-res[2][0])/(GLdouble)divz);
				glVertex3d((GLdouble)(res[0][0])+1, (GLdouble)(-res[1][0]) ,(GLdouble) (-res[2][0]/divz));
				glVertex3d((GLdouble)(res[0][0])+1, (GLdouble)(-res[1][0])-1 , (GLdouble)(-res[2][0])/(GLdouble)divz);
				glVertex3d((GLdouble)res[0][0], (GLdouble)(-res[1][0])-1 , (GLdouble)(-res[2][0])/(GLdouble)divz);
				
			}
		}
	}
	glEnd();
	liberaMatrizDouble(4,1,pos);
	liberaMatrizDouble(4,1,res);
}

void desenhaDeUmArquivoTriangle(char vertices[],char rgb[], double** matrix) {
	FILE *fvertices = NULL;
	FILE* frgb = NULL;
	glBegin(GL_TRIANGLES);
	double** pos = alocaMatriz2DDouble(4,1);
	double** res = alocaMatriz2DDouble(4,1);

	GLint verticelido[3] = {0,0,0};
	GLint rgblido[3] = {0,0,0};

	fvertices = fopen (vertices,"r");
	frgb = fopen (rgb,"r");

	if(fvertices == NULL || frgb == NULL) {
		printf("ERRO!\n");
		system ("pause");
		printf ("Erro no desenhoz\n");
		return;
		//exit(1);
	}

	while (verticelido[0] != -1) {
		//armazena o conteudo dos arquivos nas variaveis
		fscanf(fvertices,"%f",&verticelido[0]);
		fscanf(fvertices,"%f",&verticelido[1]);
		fscanf(fvertices,"%f",&verticelido[2]);
		fscanf(frgb,"%f",&rgblido[0]);
		fscanf(frgb,"%f",&rgblido[1]);
		fscanf(frgb,"%f",&rgblido[2]);

		glColor3f((float)rgblido[0]/255, (float)rgblido[1]/255, (float)rgblido[2]/255);
		glVertex3i(verticelido[0], verticelido[1] , verticelido[2]);



		glNormal3f(0.0, 0.0, 1.0);
			
	}
	glEnd();
	liberaMatrizDouble(4,1,pos);
	liberaMatrizDouble(4,1,res);

	fclose(fvertices);
	fclose(frgb);	
}

