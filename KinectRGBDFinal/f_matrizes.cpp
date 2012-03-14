
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <opencv2\opencv.hpp>

#include "f_pontos.h"

void imprimeMatriz (double **mat, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (j != (m-1))
				printf ("%lf ", mat[i][j]);
			else
				printf ("%lf\n", mat[i][j]);
		}
	}
}

double **liberaMatrizDouble (int m, int n, double **v) {
	int  i;  /* variavel auxiliar */
	if (v == NULL) return (NULL);
	if (m < 1 || n < 1) {  /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (v);
    }
	for (i=0; i<m; i++) free (v[i]); /* libera as linhas da matriz */
	free (v);      /* libera a matriz */
	return (NULL); /* retorna um ponteiro nulo */
}

double **alocaMatriz2DDouble (int m, int n) {
	double **v;  /* ponteiro para a matriz */
	int   i;    /* variavel auxiliar      */
	if (m < 1 || n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
    }
	/* aloca as linhas da matriz */
	v = (double **) calloc (m, sizeof(double *));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		 return (NULL);
    }
	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		  v[i] = (double*) calloc (n, sizeof(double));
		 if (v[i] == NULL) {
		    printf ("** Erro: Memoria Insuficiente **");
			return (NULL);
		}
    }
	return (v); /* retorna o ponteiro para a matriz */
}

double** transporMatriz (double **m, int x, int y) {
	double **saida = alocaMatriz2DDouble(y,x);
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			saida[j][i] = m[i][j];
		}
	}
	return saida;
}

double** multiplicaMatrizes (double** a, double** b, int ma, int na, int mb, int nb) {
	if (na == mb) {
		double** c = alocaMatriz2DDouble(ma,nb);
		for (int i = 0; i < ma; i++) {
			for (int j = 0; j < nb; j++) {
				c[i][j] = 0;
				for (int k = 0; k < na; k++) {
					c[i][j] = c[i][j] + a[i][k]*b[k][j];
				}
			}
		}
		return c;
	}
	else {
		printf("Erro na multiplicação de matrizes\n");
		//exit(1);
		return (double**) -1;
	}
}

// Method to carry out the partial-pivoting Gaussian
// elimination.  Here index[] stores pivoting order.

 void gaussian(double** a, int* index, int n) {
    double* c;
	c = (double*)malloc(sizeof(double)*n);;

	// Initialize the index
	for (int i=0; i<n; ++i) index[i] = i;

	// Find the rescaling factors, one from each row
    for (int i=0; i<n; ++i) {
		double c1 = 0;
		for (int j=0; j<n; ++j) {
			double c0 = abs(a[i][j]);
			if (c0 > c1) c1 = c0;
		}
		c[i] = c1;
    }

	// Search the pivoting element from each column
    int k = 0;
    for (int j=0; j<n-1; ++j) {
		double pi1 = 0;
		for (int i=j; i<n; ++i) {
			double pi0 = abs(a[index[i]][j]);
			pi0 /= c[index[i]];
			if (pi0 > pi1) {
				pi1 = pi0;
				k = i;
			}
		}

		// Interchange rows according to the pivoting order
		int itmp = index[j];
		index[j] = index[k];
		index[k] = itmp;
		for (int i=j+1; i<n; ++i) {
			double pj = a[index[i]][j]/a[index[j]][j];

			// Record pivoting ratios below the diagonal
			a[index[i]][j] = pj;

			// Modify other elements accordingly
			for (int l=j+1; l<n; ++l)
			a[index[i]][l] -= pj*a[index[j]][l];
		}	
	}
	free(c);
}

double **inverteMatriz (double** a, int n) {
	double **aux;
    double **x;
    double **b;
	aux = alocaMatriz2DDouble(n,n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			aux[i][j] = a[i][j];
		}
	}
    int *index;
	index = (int*) malloc(n*sizeof(int));
	x = alocaMatriz2DDouble(n,n);
	b = alocaMatriz2DDouble(n,n);
    for (int i=0; i<n; ++i) b[i][i] = 1;
	// Transform the matrix into an upper triangle
    gaussian(aux, index, n);
	 // Update the matrix b[i][j] with the ratios stored
	for (int i=0; i<n-1; ++i)
		for (int j=i+1; j<n; ++j)
			for (int k=0; k<n; ++k)
				b[index[j]][k] -= aux[index[j]][i]*b[index[i]][k];
	// Perform backward substitutions
	for (int i=0; i<n; ++i) {
		x[n-1][i] = b[index[n-1]][i]/aux[index[n-1]][n-1];
		for (int j=n-2; j>=0; --j) {
			x[j][i] = b[index[j]][i];
			for (int k=j+1; k<n; ++k) {
				x[j][i] -= aux[index[j]][k]*x[k][i];
			}
			x[j][i] /= aux[index[j]][j];
		}
	}
	free(index);
	aux = liberaMatrizDouble(n,n,aux);
	b = liberaMatrizDouble(n,n,b);
	return x;
  }

int*** alocaMatriz3Dint (int x, int y, int z) {
	int i = 0,j = 0;
	int*** saida;
	saida = (int ***)malloc(x*sizeof(int**));
	for (i = 0; i<x; i++){
		saida[i] = (int **)malloc(y*sizeof(int*));
		for (j = 0; j<y; j++){
			saida[i][j] = (int *)malloc(z*sizeof(int));
		}
	}
	return saida;
}

double** montaMatrizCvMat (CvMat* mat) {
	double** m = alocaMatriz2DDouble(3,3);
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			m[x][y] = cvGetReal2D(mat,x,y);
		}
	}

	return m;
}

double** construirMatriz (Ponto3d* pt, int npts) {
	double** m = alocaMatriz2DDouble(3*npts,12);
	int cnt = 0;
	for (int i = 0; i < 3*npts; i++) {
		m[i][0] = pt[cnt].x; m[i][1] = pt[cnt].y; m[i][2] = pt[cnt].z; m[i][3] = 1; m[i][4] = 0; m[i][5] = 0; m[i][6] = 0; m[i][7] = 0; m[i][8] = 0;m[i][9] = 0; m[i][10] = 0; m[i][11] = 0;
		m[++i][0] = 0;m[i][1] = 0; m[i][2] = 0;m[i][3] = 0; m[i][4] = pt[cnt].x; m[i][5] = pt[cnt].y; m[i][6] = pt[cnt].z; m[i][7] = 1; m[i][8] = 0;m[i][9] = 0; m[i][10] = 0; m[i][11] = 0;
		m[++i][0] = 0;m[i][1] = 0; m[i][2] = 0;m[i][3] = 0; m[i][4] = 0; m[i][5] = 0;m[i][6] = 0;m[i][7] = 0; m[i][8] = pt[cnt].x; m[i][9] = pt[cnt].y; m[i][10] = pt[cnt].z; m[i][11] = 1;
		cnt++;
	}
	return m;
}

/*
double** construirMatriz (CvPoint* pt, int npts) {
	double** m = alocaMatriz2DDouble(2*npts,6);
	int cnt = 0;
	for (int i = 0; i < 2*npts; i++) {
		m[i][0] = pt[cnt].x;m[i][1] = pt[cnt].y; m[i][2] = 1; m[i][3] = 0; m[i][4] = 0; m[i][5] = 0;
		m[++i][0] = 0;m[i][1] = 0; m[i][2] = 0; m[i][3] = pt[cnt].x; m[i][4] = pt[cnt].y; m[i][5] = 1;
		cnt++;
	}
	pt[cnt].
	return m;
}
*/
double** gerarMatrizN0 (Ponto3d* pt, int npts) {
	double** m = alocaMatriz2DDouble(3*npts,1);
	int cnt = 0;
	for (int i = 0; i < 3*npts; i++) {
		m[i][0] = pt[cnt].x;
		m[++i][0] = pt[cnt].y;
		m[++i][0] = pt[cnt].z;
		//m[++i][0] = 1;
		cnt++;
	}
	return m;
}

double** vetorParaMatriz (double** v) {
	double** m = alocaMatriz2DDouble(4,4);
	m[0][0] = v[0][0]; m[0][1] = v[1][0]; m[0][2] = v[2][0]; m[0][3] = v[3][0];
	m[1][0] = v[4][0]; m[1][1] = v[5][0]; m[1][2] = v[6][0]; m[1][3] = v[7][0];
	m[2][0] = v[8][0]; m[2][1] = v[9][0]; m[2][2] = v[10][0];m[2][3] = v[11][0];
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0;m[3][3] = 1;
	return m;
}

double** geraMatrizIdentidade (int n) {
	double** m = alocaMatriz2DDouble(n,n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if ( i == j )
				m[i][j] = 1;
			else
				m[i][j] = 0;
		}
	}
	return m;
}