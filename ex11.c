#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//struct do registro de elementos na árvore
typedef struct reg {
    char chave[100];
    struct reg *pai;
    struct reg *filho;
    struct reg *irmao;
} celula;
typedef celula *arvore; //define que a célula é um ponteiro para a árvore e sub-árvore
arvore constroi_arv(char *x) {
    arvore r;
    r = (arvore) malloc(sizeof(celula));
    if (r == NULL) { perror("ERRO: malloc"); exit(EXIT_FAILURE); }
    r->pai = NULL;      //define como raiz
    strncpy(r->chave, x, 99);        //coloca o identificador/valor
    r->chave[99] = '\0';
    r->filho = NULL;    
    r->irmao = NULL;    
    return r;
}

//adiciona um filho com chave x ao nó r
/* Adiciona um novo filho (no inicio da lista de filhos) ao no r.
 * Parametros: r - no pai; x - chave do filho. Retorno: nenhum. */
void cria_filho(arvore r, char *x) {
    arvore f;
    if (r != NULL) {
        f = (arvore) malloc(sizeof(celula));
        if (f == NULL) { perror("ERRO: malloc"); exit(EXIT_FAILURE); }
        f->pai = r;                 //define que ele pertence à uma sub-árvore r
        strncpy(f->chave, x, 99);        //coloca o identificador no filho
        f->chave[99] = '\0';
        f->filho = NULL;            //define que não tem filhos 
        f->irmao = r->filho;        //define o irmão do nó como o filho anterior da raiz
        r->filho = f;               //define o novo filho, ou folha, como o nó adicionado
    }
}

/* Adiciona um irmao logo apos o no dado.
 * Parametros: no - no de referencia; x - chave do irmao. Retorno: nenhum. */
void cria_irmao(arvore no, char *x){
    if (no != NULL) {
        arvore ir = (arvore)(malloc(sizeof(celula)));
        if(ir != NULL){
            ir->pai = no->pai;               //define que ele pertence à um mesmo pai
            strncpy(ir->chave, x, 99);        //coloca o identificador no irmão
            ir->chave[99] = '\0';
            ir->filho = NULL;                //define que não tem filhos 
            ir->irmao = no->irmao;           //aponta para o irmão já existente
            no->irmao = ir;                  //define o novo filho, ou folha, como o nó adicionado
        }
    }
}

//retorna o número de filhos de um nó dado
/* Retorna o numero de filhos do no r. */
int num_filhos(arvore r) {
    int k = 0;
    if (r != NULL) {
        r = r->filho;
        while (r != NULL) {
            k++;
            r = r->irmao;
        }
    }
    return k; 
}

arvore retorna_pai(arvore r) { return r->pai; }
int eh_raiz(arvore r) {
    if (retorna_pai(r) == NULL) return 1;
    else return 0; 
}

int eh_folha(arvore r) {
    if (r->filho == NULL) return 1;
    else return 0; 
}

/* Retorna o numero de nos da (sub)arvore r e de seus irmaos seguintes. */
int tamanho(arvore r) {
    if (r == NULL) return 0;
    else return 1 + tamanho(r->filho) + tamanho(r->irmao);
}

/* Retorna a altura da arvore r (vazia = -1, folha = 0). */
int altura(arvore r) {
    int alt, alt_maior;
    if (r == NULL) return -1;           //se n existir arvore
    if (r->filho == NULL) return 0;     //se encontrar nenhum filho
    r = r->filho;                       //desce um nível                       
    alt_maior = altura(r); 
    while (r->irmao != NULL) {          //busca lateralmente por irmãos
        r = r->irmao;
        alt = altura(r);
        if (alt_maior < alt) alt_maior = alt;   //registra o maior irmão
    }
    return alt_maior + 1;
}

/* Imprime a hierarquia com indentacao.
 * Parametros: r - no inicial; cont - nivel de indentacao. Retorno: nenhum. */
void mostraHier(arvore r, int cont){
    if(r == NULL){return;}  //se acha uma folha, ou a raiz é nula, retorna
    for(int i = 0; i< cont; i++){printf("-");}   //printa a identação
    printf("%s\n", r->chave);                    //printa o conteúdo
    if(r-> filho != NULL){mostraHier(r->filho,cont+2);}  //se tem filho, desce a hierarquia pra mostrar ele
    if(r->irmao != NULL){mostraHier(r->irmao,cont);}      //se n tem filho, passa pro irmão e mantem a identação  
    //se n tem irmão, retorna a hierarquia
}

/* Libera (free) todos os nos da arvore r, incluindo irmaos e descendentes.
 * Parametro: r - no inicial. Retorno: nenhum. */
void libera_arv(arvore r) {
    if (r == NULL) return;
    libera_arv(r->filho);
    libera_arv(r->irmao);
    free(r);
}

int main(){
    arvore raiz = constroi_arv("Patrao");
    cria_filho(raiz, "Gerente 1");
    cria_filho(raiz, "Gerente 2"); 
    arvore hw = raiz->filho;
    cria_filho(hw, "Funcionario 1");
    cria_filho(hw, "Funcionario 2");
    cria_irmao(hw->filho, "Funcionario 3"); 
    arvore sw = hw->irmao;
    cria_filho(sw, "Funcionario 3");
    cria_filho(sw->filho, "Estagiario 1"); //Filho do "Funcionario 3" de "Gerente 1" (3º nível)
    //6. Exibe a estrutura completa
    printf("=== HIERARQUIA DA ARVORE ===\n");
    if (raiz == NULL) {
        printf("Arvore vazia.\n");
    } else {
        mostraHier(raiz, 0);
    }
    printf("Total de nos: %d | Altura: %d\n", tamanho(raiz), altura(raiz));
    libera_arv(raiz);
    return 0;
}