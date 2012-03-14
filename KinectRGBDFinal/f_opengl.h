#ifndef F_OPENGL_H
#define F_OPENGL_H


#include <gl/glut.h>

//GLfloat angle, fAspect;



// Inicializa par�metros de rendering
void Inicializa (void);

// Fun��o usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void);
// Fun��o callback chamada quando o tamanho da janela � alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h);

// Fun��o callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y);

void GerenciaTeclado(unsigned char key, int x, int y);

#endif