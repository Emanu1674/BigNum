/* I_MAIN.C
 * ----------------------------------------------------------------------------
 * Função main.
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <locale.h>

#ifdef _WIN32
#define CLEAR "cls"
#include <conio.h>
#else
#define CLEAR "clear"
#include <termios.h>
#include <unistd.h>

// --------------------------------------
// Implementação de <conio.h>
// Apenas se sistema for UNIX
// --------------------------------------

// Retorna um char imediatamente após uma tecla ser pressionada
char getch(void) {

    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
 }

#endif

// --------------------------------------
// Tratamento de exceções
// --------------------------------------

void E_UnhandledException() {

    system( CLEAR );
    puts( "O programa sofreu um erro irrecuperável e será encerrado\n"
          "prematuramente. Todos os dados não salvos foram perdidos.\n\n" );

    puts( "Presione qualquer tecla para continuar.\n" );
    puts( "ERROR: FATAL_UNHANDLED -> UnhandledException" );
    getch();
    system( CLEAR );
    exit( EXIT_FAILURE );
}

void E_MallocOutOfMemory() {

    while (1) {
        char key = '\0';
        printf( "\nAlocação de memória falhou\n" );
        printf( "Abortar, Tentar, Falhar?" );

        fflush(stdin);
        key = getch();
        if( key == 'A' || key == 'a' ) {
            exit(EXIT_FAILURE);
        } else
        if( key == 'R' || key == 'r' ) {
            return;
        } else
        if( key == 'F' || key == 'f' ) {
            return;
        }
    }
}

void E_BugCheck( uint8_t errCode ) {

    switch ( errCode ) {
        case 0x00:
            E_MallocOutOfMemory();
            break;
        
        default:
            E_UnhandledException();
            break;
        }
    }
    
    // --------------------------------------
    // Código do Programa
    // --------------------------------------
    
// Struct da lista
typedef struct S0 {

    uint8_t n;
    struct S0* prox;
} ulst8;

// Cria uma lista vazia
ulst8* DS_LstCriar( void ) {

    return NULL;
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

// Retorna o final da lista [list]
ulst8* DS_LstUltimo( ulst8* list ) {

    if (!list) return NULL;
    ulst8* p = list;
    if (p != NULL)
        while ( p->prox != NULL )
            p = p->prox;
    return p;
}

// Insere um valor [i] no começo da lista [list]
ulst8* DS_LstInsere( ulst8* list, int i ) {

    ulst8* lstNovo = (ulst8*) malloc(sizeof(ulst8));
    if (lstNovo == NULL) E_BugCheck(0x00);

    lstNovo->n = i;
    lstNovo->prox = list;
    return lstNovo;
}

// Insere um valor [i] no final da lista [list]
ulst8* DS_LstInsereFinal( ulst8* list, int i ) {

    ulst8* lstNovo = (ulst8*) malloc(sizeof(ulst8));
    if (lstNovo == NULL) E_BugCheck(0x00);

    lstNovo->n = i;
    lstNovo->prox = NULL;
    ulst8* ult = DS_LstUltimo(list);
    if( ult == NULL ) {
        list = lstNovo;
    } else {
        ult->prox = lstNovo;
    }
    return list;
}

// Inverte a lista [list] in-place (não uma cópia)
ulst8* DS_LstInverter( ulst8* list ) {

    if (!list) return NULL;
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

// Inverte e retorna uma cópia da lista [list]
ulst8* DS_LstInverterCopia( ulst8* list ) {

    if (!list) return NULL;
    ulst8* copia = DS_LstCriar();
    while (list != NULL) {
        copia = DS_LstInsere(copia, list->n);
        list = list->prox;
    }
    return copia;
}

ulst8* DS_LstInverterCopiaSafe( ulst8* list ) {
    
    ulst8* nova = DS_LstCriar();
    ulst8* p = list;
    while (p) {
        // Insert at the *start* to reverse
        ulst8* novo = (ulst8*) malloc(sizeof(ulst8));
        novo->n = p->n;
        novo->prox = nova;
        nova = novo;
        p = p->prox;
    }
    return nova;
}

// Converte uma string [str] de números em uma lista
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
void DS_LstImprime( ulst8* list ) {

    ulst8* p;
    for ( p = list ; p != NULL ; p = p->prox )
    printf("%d", p->n);
}

// Imprime a lista [list] em modo de proteção contra loop infinito
void DS_LstImprimeSafe( ulst8* list ) {
    
    ulst8* p;
    int safety = 0;
    for ( p = list ; p != NULL ; p = p->prox ) {
        printf("%d", p->n);
        if (++safety > 250) {
            printf("[POSSÍVEL LOOP INFINITO DETECTADO]");
            break;
        }
    }
}

// Imprime a lista invertida no terminal
void DS_LstImprimeInvertido( ulst8* list ) {

    ulst8* rev = DS_LstInverterCopia(list);
    DS_LstImprime(rev);
    DS_LstFree(rev);
}

// Imprime a lista [list] em notação científica, mostrando [dig]*10^n
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

// Imprime a inversão da lista [list] em notação científica, mostrando [dig]*10^n
void DS_LstImprimeCientificoInverso( ulst8* list, int digitos ) {

    ulst8* p = DS_LstInverterCopia(list);
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
    DS_LstFree(p);
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
        resultado = DS_LstInsereFinal( resultado, soma % 10 );

        if ( a != NULL ) a = a->prox;
        if ( b != NULL ) b = b->prox;
    }

    return resultado;
}

// Retorna a diferença entre duas listas [a] e [b]
ulst8* DS_LstSubtrai( ulst8* a, ulst8* b ) {

    ulst8* resultado = DS_LstCriar();
    int emprestimo = 0;

    // Subtração dígito a dígito
    while (a != NULL || b != NULL) {

        int dig_a = (a != NULL) ? a->n : 0;
        int dig_b = (b != NULL) ? b->n : 0;

        int sub = dig_a - dig_b - emprestimo;

        if (sub < 0) {
            sub += 10;
            emprestimo = 1;
        } else {
            emprestimo = 0;
        }

        resultado = DS_LstInsereFinal(resultado, sub);

        if (a) a = a->prox;
        if (b) b = b->prox;
    }

    // Caso especial: resultado inteiro é zero (ex: 100 - 100)
    // Aqui verificamos se todos os dígitos são zero
    ulst8* p = resultado;
    int todos_zeros = 1;
    while (p != NULL) {
        if (p->n != 0) {
            todos_zeros = 0;
            break;
        }
        p = p->prox;
    }

    if (todos_zeros) {
        DS_LstFree(resultado);
        return DS_LstInsereFinal(NULL, 0);
    }

    return resultado;
}

// Multiplica uma lista [a] por um único [digito]
ulst8* DS_LstMultiplicaDigito( ulst8* a, int digito ) {
    ulst8* resultado = NULL;
    int carry = 0;

    while (a != NULL || carry > 0) {
        int prod = carry + (a ? a->n : 0) * digito;
        
        // Log the current multiplication step
        printf("Multiplicando: (%d * %d) + carry = %d -> digito resultado = %d, carry = %d\n", 
               a ? a->n : 0, digito, prod, prod % 10, prod / 10);
        
        carry = prod / 10;
        
        // Insere o digito resultado na lista [resultado]
        resultado = DS_LstInsere(resultado, prod % 10);
        
        if (a) a = a->prox;
    }

    // Soma o carry final como um novo dígito (se houver)
    if (carry > 0) {
        resultado = DS_LstInsereFinal(resultado, carry);
    }
    printf("Antes de inverter: ");
    DS_LstImprime(resultado); // Não invertido
    printf("\n");

    ulst8* resultadoFinal = DS_LstInverter(resultado);
    printf("Depois de inverter: ");
    DS_LstImprime(resultadoFinal);
    printf("\n");
    return resultadoFinal;
}

// Desloca a lista para a esquerda [zeros] posições
ulst8* DS_LstShiftLeft( ulst8* list, int zeros ) {

    while (zeros-- > 0)
        list = DS_LstInsere(list, 0);
    return list;
}

// Multiplica a lista [a] por [b]
ulst8* DS_LstMultiplica( ulst8* a, ulst8* b ) {

    ulst8* resultado = NULL;
    int shift = 0;

    ulst8* bPtr = b;

    while (bPtr != NULL) {
        //printf("Multiplicando A por dígito %d com shift %d...\n", bPtr->n, shift);

        ulst8* parcial = DS_LstMultiplicaDigito(a, bPtr->n);
        //printf("Resultado parcial: ");
        //DS_LstImprimeInvertido(parcial); puts("");
        ulst8* parcialShifted = DS_LstShiftLeft(parcial, shift);
        //printf("Resultado parcial com shift: ");
        //DS_LstImprimeInvertido(parcialShifted); puts("");

        //printf("Lista Resultado (antes da soma): ");
        //DS_LstImprimeInvertido(resultado); puts("");

        ulst8* resultadoTemp = DS_LstSoma(resultado, parcialShifted);

        DS_LstFree(resultado);
        DS_LstFree(parcial);
        resultado = resultadoTemp;

        bPtr = bPtr->prox;
        shift++;
    }

    return resultado;
}

int main() {

    setlocale( LC_ALL, "en_us.UTF-8" );
    system(CLEAR);

    ulst8* a = DS_LstFromString("250");
    ulst8* b = DS_LstFromString("100");
    ulst8* add = DS_LstCriar();
           add = DS_LstSoma(a, b);
    ulst8* sub = DS_LstCriar();
           sub = DS_LstSubtrai(a, b);
    ulst8* mul = DS_LstCriar();
           mul = DS_LstMultiplica(a, b);
/*
    puts("Listas como elas estão armazenadas:");
    DS_LstImprime(a);
    puts("");
    DS_LstImprime(b);
    puts("\n");
*/
    puts("Teste de soma:");
    DS_LstImprimeInvertido(a);
    printf(" + ");
    DS_LstImprimeInvertido(b);
    printf(" = ");
    DS_LstImprimeInvertido(add);
    puts("");

    puts("Teste de subtração:");
    DS_LstImprimeInvertido(a);
    printf(" - ");
    DS_LstImprimeInvertido(b);
    printf(" = ");
    DS_LstImprimeInvertido(sub);
    puts("");

    puts("Teste de multiplicação:");
    DS_LstImprimeInvertido(a);
    printf(" * ");
    DS_LstImprimeInvertido(b);
    printf(" = ");
    DS_LstImprimeInvertido(mul);

    return 0;
}