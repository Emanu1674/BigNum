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
ulst8* DS_LstCriar( void ) {
    return NULL;
}

// Insere um valor [i] no começo da lista [list]
ulst8* DS_LstInsere(ulst8* list, int i) {

    ulst8* lstNovo = (ulst8*) malloc(sizeof(ulst8));
    lstNovo->n = i;
    lstNovo->prox = list;
    return lstNovo;
}

// Retorna o final da lista [list]
ulst8* DS_LstUltimo( ulst8* list ) {

	ulst8* p = list;
	if (p != NULL)
		while ( p->prox != NULL )
			p = p->prox;
	return p;
}

// Insere um valor [i] no final da lista [list]
ulst8* DS_LstInsereFinal( ulst8* list, int i ) {

    ulst8* lstNovo = (ulst8*) malloc(sizeof(ulst8));
    lstNovo->n = i;
    lstNovo->prox = NULL;
    ulst8* ult = DS_LstUltimo(list);
    if( ult == NULL )
        list = lstNovo;
    else
        ult->prox = lstNovo;
    return list;
}

// Inverte a lista [list]
ulst8* DS_LstReverter( ulst8* list ) {

    ulst8* ant = NULL;
    ulst8* atual = list;
    while (atual != NULL) {
        ulst8* prox = atual->prox;
        atual->prox = ant;
        ant = atual;
        atual = prox;
    }
    return ant;
}

// Converte uma string de números em uma lista
ulst8* DS_LstFromString( const char* str ) {

    ulst8* list = DS_LstCriar();

    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            list = DS_LstInsere(list, str[i] - '0');
        }
    }

    return list;
}

// Imprime a lista no terminal
void DS_LstImprime( ulst8* list ){

    ulst8* p;
    for ( p = list ; p != NULL ; p = p->prox )
        printf("%d", p->n);
}

// Imprime a lista [lost] em notação científica, mostrando [dig]e*10^n
void DS_LstImprimeCientifico( ulst8* list, int digitos ) {
    ulst8* p = list;
    int digitosTotal = 0;

    // Conta os dígitos totais
    for ( ulst8* t = list; t != NULL; t = t->prox )
        digitosTotal++;

    printf("%d", p->n);  // Primeiro dígito (antes do ponto)
    p = p->prox;

    if ( digitos > 1 && p != NULL )
        printf(".");

    // Próximos dígitos após o ponto
    for ( int i = 1; i < digitos && p != NULL; ++i, p = p->prox )
        printf("%d", p->n);

    printf("×10^%d\n", digitosTotal - 1);
}

// Libera a lista quando ela não é mais necessária
void DS_LstFree( ulst8* list ) {

    ulst8* p = list;
    while ( p != NULL ) {
        ulst8* t = p->prox;
        free(p);
        p = t;
    }
}

// Retorna a soma de duas listas [a] e [b]
ulst8* DS_LstSoma( ulst8* a, ulst8* b ) {

    ulst8* resultado = NULL;
    int carry = 0;

    while ( a != NULL || b != NULL || carry != 0 ) {

        uint8_t digitoA = ( a != NULL ) ? a->n : 0;
        uint8_t digitoB = ( b != NULL ) ? b->n : 0;

        uint8_t soma = digitoA + digitoB + carry;
        carry = soma / 10;
        resultado = DS_LstInsere( resultado, soma % 10 );

        if ( a != NULL ) a = a->prox;
        if ( b != NULL ) b = b->prox;
    }

    return resultado;
}

int main() {

    srand(time(NULL));
    setlocale( LC_ALL, "en_us.UTF-8" );

    system(CLEAR);

    ulst8* a = DS_LstFromString("223971923866943488");
    ulst8* b = DS_LstFromString("269139387994865665");
    ulst8* s = DS_LstSoma(a, b);

    a = DS_LstReverter(a);
    b = DS_LstReverter(b);

    printf("Soma: ");
    DS_LstImprime(a);
    printf(" + ");
    DS_LstImprime(b);
    printf(" = ");
    DS_LstImprimeCientifico(s, 4);

    return 0;
}