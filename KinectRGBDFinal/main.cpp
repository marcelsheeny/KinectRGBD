/*
Detects SIFT features in two images and finds matches between them.

Copyright (C) 2006-2010  Rob Hess <hess@eecs.oregonstate.edu>

@version 1.1.2-20100521
*/
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"

#include "f_matrizes.h"
#include "f_opengl.h"
#include "f_arquivos.h"
#include "f_pontos.h"

#include <opencv2\opencv.hpp>
#include <gl\glut.h>


#define RESX 320
#define RESY 240

int eixox, eixoy,eixoz,posx,posy,posz;
GLfloat angle, fAspect;

int*** malha1;
int*** malha2;
double** matrixmq;
double** matrizidentidade;
double** matrizRansac;

Npontos pontos1;
Npontos pontos2;

// Inicializa parâmetros de rendering
void Inicializa (void)
{ 
	//Inicializa Parametros do OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle=45;
	eixox = 160;
	eixoy = 80;
	eixoz = 200;
	posx=160;
	posy=0;
	posz=0;

	malha1 = 0;
	malha2 = 0;

	//Gera Matriz Identidade
	matrizidentidade = geraMatrizIdentidade(4);

	//Inicializa pontos que irão receber o resultado do SIFT
	pontos1.pt = (Ponto3d*)malloc(sizeof(Ponto3d)*200);
	pontos2.pt = (Ponto3d*)malloc(sizeof(Ponto3d)*200);

	//Converte Arquivos gerados pelo Kinect para vetores de 3 dimensoes do tipo [x][y][z,r,g,b]
	malha1 = converteArquivosParaVetor("depth[7].txt","color[7].txt",RESX,RESY);
	malha2 = converteArquivosParaVetor("depth[8].txt","color[8].txt",RESX,RESY);

	//Faz o SIFT guardando o resultado em ponto1 e ponto2
	sift("color[7].txt","color[8].txt",&pontos1,&pontos2,RESX,RESY,malha1,malha2);

	//Atraves dos pontos 1 e 2 gera uma matriz 4x4 usando metodo dos minimos quadrados
	matrixmq = minimosQuadrados(pontos1,pontos2);

	//Atraves da matriz de minimos quadrados, gera a matriz 4x4 dessa vez usando o RANSAC
	matrizRansac = ransac(matrixmq,pontos1,pontos2,100,10);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(angle,fAspect,0.1,5000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(eixox,eixoy,eixoz, posx,posy,posz, 0,1,0);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}

void GerenciaTeclado (unsigned char key, int x, int y) {
	if (key == 'w') {
		eixoy += 20;
	}
	if (key == 's') {
		eixoy -= 20;
	}
	if (key == 'e') {
		eixox += 20;
		posx += 20;
	}
	if (key == 'q') {
		eixox -= 20;
		posx -= 20;
	}
	if (key == 'd') {
		eixox += 20;
	}
	if (key == 'a') {
		eixox -= 20;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-in
			angle -= 5;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			angle += 5;
		}
	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}



void Desenha(void) {

	
	glClear(GL_COLOR_BUFFER_BIT);
	//desenhaDeUmVetorTriangleStrip(malha1,RESX,RESY,100,1000000,matrizidentidade);
	desenhaDeUmVetorQuads(malha1,RESX,RESY,100,matrizRansac);
	//desenhaDeUmVetorQuads(malha1,RESX,RESY,100,matrixmq);
	desenhaDeUmVetorQuads(malha2,RESX,RESY,100,matrizidentidade);
	//desenhaDeUmVetorQuads(malha2,RESX,RESY,100,matrizidentidade);
	//desenhaDeUmVetorTriangleStripSemMatriz(malha1,RESX,RESY,100,350);

	//desenhaDoArquivo("depth[7].txt","color[7].txt",RESX,RESXY);

	// Executa os comandos OpenGL
	glutSwapBuffers();
 }



/********************************** Main *************************************/


int main( int argc, char** argv ) {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(350,300);
	glutCreateWindow("Visualizacao 3D");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutMouseFunc(GerenciaMouse);
	glutKeyboardFunc(GerenciaTeclado);
	Inicializa();
	glutMainLoop();


	return 0;





}
