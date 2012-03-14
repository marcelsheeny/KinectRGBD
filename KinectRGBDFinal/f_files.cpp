#include <opencv2\opencv.hpp>
#include <stdio.h>

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

void guardaArquivosNumVetor (char depthFile[], char colorFile[], int*** saida) {
	int i = 0, j = 0;
	int depth[3] = {0,0,0};
	int color[5] = {0,0,0,0,0};
	FILE *fd;
	FILE *fc;


	saida = (int ***)malloc(320*sizeof(int**));
	for(i = 0; i<320; i++){
		saida[i] = (int **)malloc(240*sizeof(int*));
		for(j = 0; j<240; j++){
			saida[i][j] = (int *)malloc(4*sizeof(int));
		}
	}

	fd = fopen (depthFile,"r"); //abre o arquivo
	fc = fopen (colorFile,"r");

	//Verifica erro
	if(fd == NULL || fc == NULL) {
		printf("ERRO!\n");
		system ("pause");
		exit(1);
	}
	i = 0;
	for (int j = 0; j <240; j++) {
		for (int i = 0; i < 320; i++) {
		//while (i < 320*240 - 1) {
			//armazena o conteudo dos arquivos nas variaveis
			fscanf(fd,"%d ",&depth[0]);
			fscanf(fd,"%d ",&depth[1]);
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
}