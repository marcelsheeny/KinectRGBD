#include <gl/glut.h>
#include <stdio.h>

GLfloat angle, fAspect;
int eixox, eixoy,eixoz,posx,posy,posz;


// Inicializa par�metros de rendering
void Inicializa (void)
{ 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    angle=45;
	eixox = 160;
	eixoy = 80;
	eixoz = 200;
	posx=160;
	posy=0;
	posz=0;
}

// Fun��o usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de proje��o
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de proje��o
	glLoadIdentity();

	// Especifica a proje��o perspectiva
	gluPerspective(angle,fAspect,0.1,5000);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posi��o do observador e do alvo
	gluLookAt(eixox,eixoy,eixoz, posx,posy,posz, 0,1,0);
}

// Fun��o callback chamada quando o tamanho da janela � alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Para previnir uma divis�o por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);
 
	// Calcula a corre��o de aspecto
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

// Fun��o callback chamada para gerenciar eventos do mouse
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

