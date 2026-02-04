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

#define WIDTH 88

#define TAM 256000

struct monticulo {
    int ultimo;
    int vector[TAM];
};

typedef struct monticulo *pmonticulo;

void inicializarMonticulo(pmonticulo m) {
    m->ultimo = -1;
}

void hundir(pmonticulo m, int i) {
    int hijoIzq, hijoDer, j, temp;

    do {
        hijoIzq = 2 * i + 1;
        hijoDer = 2 * i + 2;
        j = i;

        if (hijoDer <= m->ultimo && m->vector[hijoDer] < m->vector[i])
            i = hijoDer;

        if (hijoIzq <= m->ultimo && m->vector[hijoIzq] < m->vector[i])
            i = hijoIzq;

        temp = m->vector[i];
        m->vector[i] = m->vector[j];
        m->vector[j] = temp;

    } while (j != i);
}

void crearMonticulo(int v[], int n, pmonticulo m) {
    int i;

    m->ultimo = n - 1;
    for (i = 0; i < n; i++)
        m->vector[i] = v[i];

    for (i = n / 2; i >= 0; i--)
        hundir(m, i);
}

int quitarMenor(pmonticulo m) {
    int x;
    
    if (m->ultimo < 0) {
        printf("No se puede eliminar un elemento de un monticulo vacio\n");
        exit(1);
    }

    x = m->vector[0];
    m->vector[0] = m->vector[m->ultimo];
    m->ultimo--;
    
    if (m->ultimo >= 0)
        hundir(m, 0);
    
    return x;
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

void listar_vector(int v[], int n) {
    int i;
    
    printf("[");
    for (i = 0; i < n; i++) {
        printf("%3d", v[i]);
        if (i < n - 1)
            printf(", ");
    }
    printf(" ]");
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

bool cumpleOrdMonticulo(int v[], int n) {
    int i, indiceHijoIzq, indiceHijoDer;

    for (i = 0; i < n; i++) {
        indiceHijoIzq = 2 * i + 1;
        indiceHijoDer = 2 * i + 2;

        if ((indiceHijoIzq < n && v[i] > v[indiceHijoIzq]) ||
            (indiceHijoDer < n && v[i] > v[indiceHijoDer]))
            return false;
    }

    return true;
}

bool esOrdenado(int v[], int n) {
    int i;
    
    for (i = 0; i < n - 1; i++)
        if (v[i] > v[i + 1])
            return false;

    return true;
}

void ordenarPorMonticulos(int v[], int n) {
    int i;
    struct monticulo mon;
    pmonticulo m = &mon;
    
    inicializarMonticulo(m);
    crearMonticulo(v, n, m);

    for (i = 0; i < n; i++)
        v[i] = quitarMenor(m);
}

void comprobacionMonticulos() {
    int n = 15, v[n], i;

    imprimirCabecera("Comprobacion", "monticulos");
    aleatorio(v, n);
    
    printf("\nArray:        ");
    listar_vector(v, n);

    struct monticulo m;
    pmonticulo pm = &m;
    crearMonticulo(v, n, pm);

    printf("\n\nMonticulo:    ");
    listar_vector(pm->vector, pm->ultimo + 1);
    char *str = cumpleOrdMonticulo(pm->vector, pm->ultimo + 1) ? "Si" : "No";
    printf("\nTiene orden de monticulo? %s\n\n", str);

    printf("Operacion quitar menor:\n\n");

    for (i = 0; i < n; i++) {
        printf("%3d << ", quitarMenor(pm));
        listar_vector(pm->vector, pm->ultimo + 1);
        printf("\n");
    }
}

double cronoMonticulos(int v[], int n) {
    double tA, tB, t, t1;
    int k;
    struct monticulo m;
    pmonticulo p = &m;

    aleatorio(v, n);
    tA = microsegundos();
    crearMonticulo(v, n, p);
    tB = microsegundos();
    t = tB - tA;
    
    if (t < 500) {
        tA = microsegundos();
        for (k = 0; k < 1000; k++) {
            aleatorio(v, n);
            crearMonticulo(v, n, p);
        }
        tB = microsegundos();
        t1 = tB - tA;

        tA = microsegundos();
        for (k = 0; k < 1000; k++)
            aleatorio(v, n);
        tB = microsegundos();
        t = (t1 - (tB - tA)) / 1000;
    }
    return t;
}

void cabeceraCotas(char *str) {
    printf("\n");
    if (!strcmp(str, "Creacion")) {
        printf("Cota subestimada:   n^0.7\n");
        printf("Cota exacta:        n^1.0\n");
        printf("Cota sobreestimada: n^1.3\n\n");
    }
    else {
        printf("Cota subestimada:   n\n");
        printf("Cota exacta:        nlogn\n");
        printf("Cota sobreestimada: n^2\n\n");
    }
}

void cabeceraMediciones(char *str) {
    printf("%6sn%14st(n)", "", "");
    if (!strcmp(str, "Creacion")) {
        printf("%10st(n) / n^0.7", "");
        printf("%10st(n) / n^1.0", "");
        printf("%10st(n) / n^1.3\n\n", "");
    }
    else {
        printf("%14st(n) / n", "");
        printf("%10st(n) / nlogn", "");
        printf("%12st(n) / n^2\n\n", "");   
    }
}

void mostrarTiempos(char *str, long n, double t) {
  
    printf("%7ld", n);    
    printf("%18.3f%s", t, (t < 500) ? " *" : "  ");
    if (!strcmp(str, "Creacion")) {
        printf("%20.7f", t / pow(n, 0.7));
        printf("%22.7f", t / n);
        printf("%22.7f\n", t / pow(n, 1.3));
    }
    else {
        printf("%20.7f", t / n);
        printf("%22.7f", t / (n * log(n)));
        printf("%22.7lf\n", t / (n * n));
    }
}

void tiemposCrearMonticulo() {
    int min = 500, max = 64000, v[max], i, n;
    double t;

    imprimirCabecera("Tiempos creacion", "de monticulos");
    cabeceraCotas("Creacion");

    for (i = 0; i < 3; i++) {
        cabeceraMediciones("Creacion");
        for (n = min; n <= max; n *= 2) {
            t = cronoMonticulos(v, n);
            mostrarTiempos("Creacion", n, t);
        }
        printf("\n");
    }
}

void comprobacionOrdMonticulos() {
    int n = 15, v[n];

    imprimirCabecera("Comprobacion", "ordenacion por monticulos");
    aleatorio(v, n);
    
    printf("\nArray:        ");
    listar_vector(v, n);

    ordenarPorMonticulos(v, n);
    printf("\n\nOrdenacion:   ");
    listar_vector(v, 15);
    printf("\nOrdenado? %s\n", esOrdenado(v, 15) ? "Si" : "No");
    printf("\n");
}

double crono(int v[], int n, void (*ini) (int*, int)) {
    double tA, tB, t, t1;
    int k;

    ini(v, n);
    tA = microsegundos();
    ordenarPorMonticulos(v, n);
    tB = microsegundos();
    t = tB - tA;
    
    if (t < 500) {
        tA = microsegundos();
        for (k = 0; k < 1000; k++) {
            ini(v, n);
            ordenarPorMonticulos(v, n);
        }
        tB = microsegundos();
        t1 = tB - tA;
        tA = microsegundos();
        for (k = 0; k < 1000; k++)
            ini(v, n);
        tB = microsegundos();
        t = (t1 - (tB - tA)) / 1000;
    }
    return t;
}

void tiemposOrd(char *modo) {
    int min = 500, max = 256000, v[max], n, i;
    double t;

    imprimirCabecera("Ordenacion monticulos", modo);
    cabeceraCotas(modo);

    for (i = 0; i < 3; i++) {
        cabeceraMediciones(modo);
        for (n = min; n <= max; n *= 2) {
            if (!strcmp(modo, "Aleatorio"))
                t = crono(v, n, aleatorio);
            else if (!strcmp(modo, "Descendente"))
                t = crono(v, n, descendente);
            else if (!strcmp(modo, "Ascendente"))
                t = crono(v, n, ascendente);
            
            mostrarTiempos(modo, n, t);
        }
        printf("\n");
    }
}

void tiemposOrdMonticulos() {
    tiemposOrd("Ascendente");
    tiemposOrd("Descendente");
    tiemposOrd("Aleatorio");
}

int main() {
    inicializar_semilla();

    comprobacionMonticulos();
    tiemposCrearMonticulo();
    
    comprobacionOrdMonticulos();
    tiemposOrdMonticulos();
    return 0;
}