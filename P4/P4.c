// Lucas Garcia Garcia (lucas.garcia.garcia@udc.es)
// Javier Manotas Ruiz (j.manotas@udc.es)
// Hugo Perez Gomez (h.pgomez@udc.es)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

#define WIDTH 88
#define TAM_MAX 1024

typedef int **matriz;

matriz crearMatriz(int n) {
    int i;
    matriz m;

    if ((m = malloc(n * sizeof(int*))) != NULL)
        for (i = 0; i < n; i++)
            if ((m[i] = malloc(n * sizeof(int))) == NULL)
                return NULL;

    return m;
}

void iniMatriz(matriz m, int n) {
    int i, j, valor;

    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++) {
            valor = rand() % TAM_MAX + 1;
            m[i][j] = valor;
            m[j][i] = valor;
        }

    for (i = 0; i < n; i++)
        m[i][i] = 0;
}

void liberarMatriz(matriz m, int n) {
    int i;

    for (i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

void imprimirMatriz(matriz m, int n) {
    int i, j;

    for (i = 0; i < n; i++)
    {
        printf("[");

        for (j = 0; j < n; j++)
            printf("%4d%s", m[i][j], j < n - 1 ? "," : " ]\n\n");
    }
}

void rellenarMatriz(matriz m, int n, int arrays[n][n]) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            m[i][j] = arrays[i][j];
}

void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return t.tv_usec + t.tv_sec * 1000000.0;
}

void dijkstra(matriz grafo, matriz dst, int tam) {
    int n, i, j, min, v = 0;
    int *noVisitados = malloc(tam * sizeof(int));

    for (n = 0; n < tam; n++) {
        
        for (i = 0; i < tam; i++) {
            noVisitados[i] = 1;
            dst[n][i] = grafo[n][i];
        }
        noVisitados[n] = 0;

        for (i = 0; i < tam - 2; i++) {
            min = TAM_MAX + 1;
            for (j = 0; j < tam; j++)
                if (noVisitados[j] && dst[n][j] < min) {
                    min = dst[n][j];
                    v = j;
                }

            noVisitados[v] = 0;

            for (j = 0; j < tam; j++)
                if (noVisitados[j] && dst[n][j] > dst[n][v] + grafo[v][j])
                    dst[n][j] = dst[n][v] + grafo[v][j];
        }
    }

    free(noVisitados);
}

void imprimirCabecera(char *strA, char *strB) {
    int i;
    
    printf("\n");
    for (i = 0; 2 * i < WIDTH - strlen(strA) - strlen(strB); i++)
        printf("-");
    printf(" %s %s ", strA, strB);
    
    for (i = 0; 2 * i < WIDTH - strlen(strA) - strlen(strB); i++)
        printf("-");
    printf("\n");
}

void testMatriz(int n, int tam, int grafo[tam][tam], int esperada[tam][tam]) {
    matriz m = crearMatriz(tam);
    matriz dst = crearMatriz(tam);
    matriz sol = crearMatriz(tam);

    printf("\nCaso %d:\n\n", n);

    printf("Matriz de adyacencia:\n\n");
    rellenarMatriz(m, tam, grafo);
    imprimirMatriz(m, tam);
    dijkstra(m, dst, tam);
    liberarMatriz(m, tam);

    printf("Solucion obtenida:\n\n");
    imprimirMatriz(dst, tam);
    liberarMatriz(dst, tam);
    
    printf("Solucion esperada:\n\n");
    rellenarMatriz(sol, tam, esperada);
    imprimirMatriz(sol, tam);
    liberarMatriz(sol, tam);
}

void testFuncionamiento() {
    int caso1[5][5] = {
        { 0, 1, 8, 4, 7 },
        { 1, 0, 2, 6, 5 },
        { 8, 2, 0, 9, 5 },
        { 4, 6, 9, 0, 3 },
        { 7, 5, 5, 3, 0 },
    };
    int caso1Sol[5][5] = {
        { 0, 1, 3, 4, 6 },
        { 1, 0, 2, 5, 5 },
        { 3, 2, 0, 7, 5 },
        { 4, 5, 7, 0, 3 },
        { 6, 5, 5, 3, 0 },
    };
    int caso2[4][4] = {
        { 0, 1, 4, 7 },
        { 1, 0, 2, 8 },
        { 4, 2, 0, 3 },
        { 7, 8, 3, 0 },
    };
    int caso2Sol[4][4] = {
        { 0, 1, 3, 6},
        { 1, 0, 2, 5},
        { 3, 2, 0, 3},
        { 6, 5, 3, 0},
    };

    imprimirCabecera("Comprobacion", "funcionamiento");
    testMatriz(1, 5, caso1, caso1Sol);
    testMatriz(2, 4, caso2, caso2Sol);
}

void cabeceraCotas() {
    printf("\nCota subestimada:   n^2.6\n");
    printf("Cota exacta:        n^2.75\n");
    printf("Cota sobreestimada: n^2.9\n\n");
}

void cabeceraMediciones() {
    printf("%6sn", "");
    printf("%14st(n)", "");
    printf("%10st(n) / n^2.6", "");
    printf("%10st(n) / n^2.75", "");
    printf("%10st(n) / n^2.9\n\n", "");
}

void mostrarTiempos(long n, double t) {
  
    printf("%7ld", n);    
    printf("%18.3f%s", t, (t < 500) ? " *" : "  ");
    printf("%20.7f", t / pow(n, 2.6));
    printf("%22.7f", t / pow(n, 2.75));
    printf("%22.7f\n", t / pow(n, 2.9));
}

double crono(matriz m, matriz dst, int tam) {
    double tA, tB, t, t1;
    int k;

    iniMatriz(m, tam);

    tA = microsegundos();
    dijkstra(m, dst, tam);
    tB = microsegundos();
    t = tB - tA;
    
    if (t < 500) {
        tA = microsegundos();
        for (k = 0; k < 1000; k++) {
            iniMatriz(m, tam);
            dijkstra(m, dst, tam);
        }
        tB = microsegundos();
        t1 = tB - tA;
        tA = microsegundos();
        for (k = 0; k < 1000; k++)
            iniMatriz(m, tam);
        tB = microsegundos();
        t = (t1 - (tB - tA)) / 1000;
    }
    
    return t;
}

void tablasTiempos() {
    int min = 8, max = TAM_MAX, n, i;
    matriz m = crearMatriz(max), dst = crearMatriz(max);
    double t;

    imprimirCabecera("Tablas", "tiempos");
    cabeceraCotas();
    cabeceraMediciones();

    for (i = 0; i < 3; i++) {
        for (n = min; n <= max; n *= 2) {
            t = crono(m, dst, n);
            mostrarTiempos(n, t);
        }
        printf("\n");
    }

    liberarMatriz(m, max);
    liberarMatriz(dst, max);
}

int main() {
    inicializar_semilla();
    testFuncionamiento();
    tablasTiempos();
}