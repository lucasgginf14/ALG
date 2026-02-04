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

void inicializar_semilla() {
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return t.tv_usec + t.tv_sec * 1000000.0;
}

void ord_ins(int v[], int n) {
    int i, x, j;

    for (i = 1; i < n; i++) {
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x) {
            v[j + 1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}

void ord_shell(int v[], int n) {
    int incremento = n, i, j, seguir, tmp;

    do {
        incremento /= 2;
        for (i = incremento + 1; i < n; i++) {
            tmp = v[i];
            j = i;
            seguir = 1;

            while (j - incremento >= 0 && seguir) {
                if (tmp < v[j - incremento]) {
                    v[j] = v[j - incremento];
                    j = j - incremento;
                }
                else
                    seguir = 0;
            }
            v[j] = tmp;
        }
    } while (incremento != 1);    
}

int estaOrdenado(int v[], int n) {
    int i;

    for (i = 1; i < n; i++)
        if (v[i] < v[i - 1])
            return 0;
    return 1;
}

void imprimirCabecera(char *str, char *modo) {
    int i;
    
    for (i = 0; 2 * i < 88 - strlen(str) - strlen(modo); i++)
        printf("-");
    printf(" %s %s ", str, modo);
    
    for (i = 0; 2 * i < 88 - strlen(str) - strlen(modo); i++)
        printf("-");
    printf("\n");
}

void listar_vector(int v[], int n) {
    int i;
    
    printf("[");
    for (i = 0; i < n; i++)
        printf("%3d%s", v[i], i < n - 1 ? ", " : " ]");
}

void aleatorio(int v[], int n) {
    int i, m = 2 * n + 1;

    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void descendente(int v[], int n) {
    int i;

    for (i = 0; i < n; i++)
        v[i] = n - i;
}

void ascendente(int v[], int n) {
    int i;

    for (i = 0; i < n; i++)
        v[i] = 1 + i;
}

void comprobacionOrdenado(int v[], int n) {
    printf("\nordenado? %s\n", estaOrdenado(v, n) ? "Si" : "No");
}

void test_implementation(void (*algo)(int*, int), char *algoNombre) {
    int n = 18, v[n];

    printf("\nIncicializacion aleatoria\n");
    aleatorio(v, n);
    listar_vector(v, n);
    comprobacionOrdenado(v, n);
    
    printf("\nOrdenacion por %s\n", algoNombre);
    algo(v, n);
    listar_vector(v, n);
    comprobacionOrdenado(v, n);


    printf("\nInicializacion descendente\n");
    descendente(v, n);
    listar_vector(v, n);
    comprobacionOrdenado(v, n);

    printf("\nOrdenacion por %s\n", algoNombre);
    algo(v, n);
    listar_vector(v, n);
    comprobacionOrdenado(v, n);

    printf("\n");
}

double test(void (*f) (int*, int), int v[], int n, void (*ini) (int*, int)) {
    double tA, tB, t, t1;
    int k;

    ini(v, n);
    tA = microsegundos();
    f(v, n);
    tB = microsegundos();
    t = tB - tA;
    
    if (t < 500) {
        tA = microsegundos();
        for (k = 0; k < 500; k++) {
            ini(v, n);
            f(v, n);
        }
        tB = microsegundos();
        t1 = tB - tA;
        tA = microsegundos();
        for (k = 0; k < 500; k++)
            ini(v, n);
        tB = microsegundos();
        t = (t1 - (tB - tA)) / 500;
    }
    return t;
}

void cabeceraCotas(char *str, char *modo) {
    double cota;

    printf("\n");
    if (!strcmp(str, "Insercion")) {
        cota = !strcmp(modo, "Ascendente") ? 1 : 2;
        printf("Cota subestimada: %2sn^%.1f\n", "", cota - 0.2);
        printf("Cota exacta: %7sn^%.1f\n", "", cota);
        printf("Cota sobreestimada: n^%.1f\n\n", cota + 0.2);
    }
    else {
        if(!strcmp(modo, "Aleatorio")) {
            printf("Cota subestimada:   n^1.0\n");
            printf("Cota exacta:        n^1.2\n");
            printf("Cota sobreestimada: n^1.4\n\n");
        }
        else {
            printf("Cota subestimada:    n\n");
            printf("Cota exacta:         n*log(n)\n");
            printf("Cota sobreestimada:  n^1.2\n\n");
        }
    }
}

void cabeceraMediciones(char *str, char *modo) {
    double cota;
    
    printf("%6sn%14st(n)", "", "");
    if (!strcmp(str, "Insercion")) {
        cota = !strcmp(modo, "Ascendente") ? 1 : 2;
        printf("%10st(n) / n^%.1f", "", cota - 0.2);
        printf("%10st(n) / n^%.1f", "", cota);
        printf("%10st(n) / n^%.1f\n\n", "", cota + 0.2);
    }
    else {
        if (!strcmp(modo, "Aleatorio")) {
            printf("%9st(n) /  n^1.0", "");
            printf("%9st(n) /  n^1.2", "");
            printf("%9st(n) /  n^1.4\n\n", "");
        }
        else {
            printf("%14st(n) / n", "");
            printf("%7st(n) / n*log(n)", "");
            printf("%11st(n) / n^1.2\n\n", "");
        }
    }
}

void mostrarTiempos(char *str, char *modo, int n, double t) {
    double cota;
  
    printf("%7d", n);    
    printf("%18.3f%s", t, (t < 500) ? " *" : "  ");
    if (!strcmp(str, "Insercion")) {
        cota = !strcmp(modo, "Ascendente") ? 1 : 2;
        printf("%20.7f", t / pow(n, cota - 0.2));
        printf("%22.7f", t / pow(n, cota));
        printf("%22.7f\n", t / pow(n, cota + 0.2));
    }
    else {
        if (!strcmp(modo, "Aleatorio")) {
            printf("%20.7f", t / pow(n, 1.0));
            printf("%22.7f", t / pow(n, 1.2));
            printf("%22.7f\n", t / pow(n, 1.4));
        }
        else {
            printf("%20.7f", t / n);
            printf("%22.7f", t / (n * log2(n)));
            printf("%22.7f\n", t / pow(n, 1.2));
        }
    }
}

void testear(void (*algo) (int*, int), char *str, char *modo) {
    int min = 500, max = 64000, v[max], i, n;
    double t;

    imprimirCabecera(str, modo);
    cabeceraCotas(str, modo);

    for (i = 0; i < 3; i++) {
        cabeceraMediciones(str, modo);
        for (n = min; n <= max; n *= 2) {
            if (!strcmp(modo, "Aleatorio"))
                t = test(algo, v, n, aleatorio);
            else if (!strcmp(modo, "Descendente"))
                t = test(algo, v, n, descendente);
            else if (!strcmp(modo, "Ascendente"))
                t = test(algo, v, n, ascendente);
            
            mostrarTiempos(str, modo, n, t);
        }
        printf("\n");
    }
}

void testTiempos(void (*algo) (int*,  int), char *nombreAlgo) {
    testear(algo, nombreAlgo, "Aleatorio");
    testear(algo, nombreAlgo, "Descendente");
    testear(algo, nombreAlgo, "Ascendente");
    printf("\n");
}

int main() {
    inicializar_semilla();
    printf("\n");
    imprimirCabecera("Comprobacion", "Funcionamiento");
    test_implementation(&ord_ins, "Insercion");
    test_implementation(&ord_shell, "Shell");
    
    testTiempos(&ord_ins, "Insercion");
    testTiempos(&ord_shell, "Shell");
    return 0;
}