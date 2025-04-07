/* I_MAIN.C
 * ----------------------------------------------------------------------------
 * 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#include <windows.h>
#else
#define CLEAR "clear"
#endif

// Struct da lista
typedef struct S0 {

    uint8_t n;
    struct S0* prox;
} ulst8;

// Cria uma lista vazia
ulst8* DS_LstCriar ( void ) {
    return NULL;
}

// Insere um valor [i] na lista [list]
ulst8* DS_LstInsere (ulst8* list, int i) {

    ulst8* lstNovo = (ulst8*) malloc(sizeof(ulst8));
    lstNovo->n = i;
    lstNovo->prox = list;
    return lstNovo;
}

// Imprime a lista no terminal
void DS_LstImprime (ulst8* list){

    ulst8* p;
    for ( p = list ; p != NULL ; p = p->prox )
        printf("%d", p->n);
}

// Libera a lista quando ela não é mais necessária
void DS_LstFree ( ulst8* list ) {

    ulst8* p = list;
    while ( p != NULL ) {
        ulst8* t = p->prox;
        free(p);
        p = t;
    }
}

// Retorna a soma de duas listas [a] e [b]
ulst8* DS_LstSoma ( ulst8* a, ulst8* b ) {

    ulst8* resultado = NULL;
    int carry = 0;

    while ( a != NULL || b != NULL || carry != 0 ) {

        uint8_t digitoA = (a != NULL) ? a->n : 0;
        uint8_t digitoB = (b != NULL) ? b->n : 0;

        uint8_t soma = digitoA + digitoB + carry;
        carry = soma / 10;
        resultado = DS_LstInsere( resultado, soma % 10 );

        if (a != NULL) a = a->prox;
        if (b != NULL) b = b->prox;
    }

    return resultado;
}

int main() {

    srand(time(NULL));
    setlocale( LC_ALL, "en_us.UTF-8" );

    system(CLEAR);

    ulst8*    a;
    ulst8*    b;

    a = DS_LstCriar();
    b = DS_LstCriar();

    a = DS_LstInsere(a, 9);
    a = DS_LstInsere(a, 9);
    a = DS_LstInsere(a, 9);

    b = DS_LstInsere(b, 1);

    printf("Soma: ");
    DS_LstImprime(a);
    printf(" + ");
    DS_LstImprime(b);
    printf(" = ");
    DS_LstImprime(DS_LstSoma(a, b));
    return 0;
}