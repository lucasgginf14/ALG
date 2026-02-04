// Lucas Garcia Garcia (lucas.garcia.garcia@udc.es)
// Javier Manotas Ruiz (j.manotas@udc.es)
// Hugo Perez Gomez (h.pgomez@udc.es)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return t.tv_usec + t.tv_sec * 1000000.0;
}

int sumaSubMax1(int v[], int n) {
    int sumaMax = 0, estaSuma, i, j;
    
    for (i = 0; i < n; i++) {
        estaSuma = 0;
        for (j = i; j < n; j++) {
            estaSuma += v[j];
            if (estaSuma > sumaMax)
                sumaMax = estaSuma;
        }
    }
    return sumaMax;
}

int sumaSubMax2(int v[], int n) {
    int estaSuma = 0, sumaMax = 0, j;
    
    for (j = 0; j < n; j++) {
        estaSuma += v[j];
        if (estaSuma > sumaMax)
            sumaMax = estaSuma;
        else if (estaSuma < 0)
            estaSuma = 0;
    }
    return sumaMax;
}

void listar_vector(int v[], int n) {
    int i;
    
    printf("[");
    for (i = 0; i < n; i++)
        printf("%3d%s", v[i], i < n - 1 ? " " : " ]");
}

void mostrarComprobacion(int v[], int n) {
    listar_vector(v, n);
    printf("%15s%2d", "", sumaSubMax1(v, n));
    printf("%15s%2d\n", "", sumaSubMax2(v, n));
}

void test1() {
    int i;
    int arrays[6][5] = {
        {-9, 2, -5, -4, 6},
        {4, 0, 9, 2, 5},
        {-2, -1, -9, -7, -1},
        {9, -2, 1, -7, -8},
        {15, -2, -5, -4, 16},
        {7, -5, 6, 7, -7},
    };

    printf("\nTest1:%34s%17s\n\n", "subSumaMax1", "subSumaMax2");
    for (i = 0; i < 6; i++)
        mostrarComprobacion(arrays[i], 5);
    printf("\n");
}

void aleatorio(int v[], int n) {
    int i, m = 2 * n + 1;
    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void test2() {
    int i, v[9];
    printf("\nTest2:%49s%17s\n\n", "subSumaMax1", "subSumaMax2");
    for (i = 0; i < 9; i++) {
        aleatorio(v, 9);
        mostrarComprobacion(v, 9);
    }
    printf("\n");
}

void cabeceraTiempos(int ntest, double cota, double power)
{
    printf("\n\nSumaSubMax %d%25s", ntest, "");
    printf("Cota subestimada%11s", "");
    printf("Cota exacta%4s", "");
    printf("Cota sobreestimada\n\n");

    printf("%5sn", "");
    printf("%18st(n)", "");
    printf("%13st(n) / n^%.1f", "", power - cota);
    printf("%12st(n) / n^%.f", "", power);
    printf("%10st(n) / n^%.1f\n", "", power + cota);
}

void testTiempos(int ntest, int (*algo) (int*,  int)) {
    int *v, n, k = 500, i, j, max = (ntest == 1 ? 32000 : 256000);
    double tA, tB, t, cota = 0.2, power = 3 - ntest;
    bool media;

    cabeceraTiempos(ntest, cota, power);
    v = malloc(sizeof(int) * max);

    for (i = 0; i < 3; i++) {
        printf("\nMedicion %d\n\n", i + 1);
        for (n = 500; n <= max; n *= 2) {
            aleatorio(v, n);
            tA = microsegundos();
            algo(v, n);
            tB = microsegundos();
            t = tB - tA;
            
            if ((media = t < 500))
            {
                tA = microsegundos();
                for (j = 0; j < k; j++)
                    algo(v, n);
                tB = microsegundos();
                t = (tB - tA) / k;
            }
            
            printf("%6d", n);
            printf("%22.3f%s", t, media ? " * " : "   ");
            printf("%22.7f", t / pow(n, power - cota));
            printf("%22.7f", t / pow(n, power));
            printf("%22.7f\n", t / pow(n, power + cota));
        }
    }
    free(v);
    printf("\n");
}

int main()
{
    inicializar_semilla();
    test1();
    test2();
    testTiempos(1, &sumaSubMax1);
    testTiempos(2, &sumaSubMax2);
    return 0;
}
