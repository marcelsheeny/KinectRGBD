#ifndef F_OPENGL_H
#define F_OPENGL_H


#include <gl/glut.h>

//GLfloat angle, fAspect;



// Inicializa parâmetros de rendering
void Inicializa (void);

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void);
// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h);

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y);

void GerenciaTeclado(unsigned char key, int x, int y);

#endif