#include <stdio.h>  //biblioteca padrão de entrada e saída
#include <stdlib.h> //para malloc() e free()
#include <string.h> //para manipular srings

//-------- ESTRUTURAS --------
// Nó da BST de pistas
typedef struct Sala { 
    char nome[50];          //nome do cômodo da mansão
    char pista[100];        //pista para cada comodo, qdo houver
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST de pistas
typedef struct Pista {
char texto[100];
struct Pista *esq;
struct Pista *dir;
} Pista;


//-------- FUNÇÃO QUE CRIA E INICIALIZA A NOVA SALA --------
Sala* criarSala(char *nome, char *pista) {                      //Qdo preencher a sala, deve ter o '"nome","pista"'
    Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));   //aloca dinamicamente memoria para um nova Sala e guarda o endereço em 'nova'
    if (nova == NULL){
        printf("Erro ao alocar memoria. Saindo...\n");
        exit(1);
    }
    strcpy(nova->nome, nome); //copia o nome passado como parâmetro (string) para o campo 'nome' da Sala

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';

    nova->esquerda = NULL;  //inicializa os ponteiros dos filhos como NULL (sem filhos ainda, por isso NULL)
    nova->direita = NULL;

    return nova;    //retorna o endereço do nova Sala criado
}

//-------- FUNÇÃO PARA AS PISTAS --------
//Inserindo na BST
Pista* inserirPista(Pista* raiz, char* texto) {
    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(texto, raiz->texto) < 0){
        raiz->esq = inserirPista(raiz->esq, texto);
    } else  {
        raiz->dir = inserirPista(raiz->dir, texto);
    }
    return raiz;
}
//Exibindo pistas em ordem
void exibirPistas(Pista* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->texto);
        exibirPistas(raiz->dir);
    }
}

//-------- FUNÇÃO PARA EXPLORAR A SALA (ESQUERDA, DIREITA, SAIR) --------
//Explorando a mansão e coletando pistas (Esquerda, Direita, Sair)
void explorarSalas(Sala *atual, Pista **arvorePistas){  //cria o ponteiro do ponteiro
    char escolha;   //para criar a interação do jogador (d, e, s)

    printf("\n|.|.|.|.|.|.|.|.|.|.|.|.|.|.|.|\n");
    printf("|.|.|.| DETECTIVE STACK |.|.|.|\n");
    printf("|.|.|.|.|.|.|.|.|.|.|.|.|.|.|.|\n");

    while (atual != NULL){
        printf("\nVocê está em %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("->Você achou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        if(atual->esquerda == NULL && atual->direita == NULL){
            printf("Aqui não tem saída.\n"); 
            printf("Digite 's' para SAIR ou volte para estudar as pistas!\n");
        }
        printf("\nPara onde deseja ir?\n");
        if(atual->esquerda != NULL){
            printf("Digite 'E' para %s\n", atual->esquerda->nome);
        }
        if(atual->direita != NULL){
            printf("Digite 'D' para %s\n", atual->direita->nome);
        }
        printf("Digite 'S' para SAIR\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL){
            atual = atual->esquerda;
        } else if((escolha == 'd' || escolha == 'D') && atual->direita != NULL){
            atual = atual->direita;
        } else if(escolha == 's' || escolha == 'S'){
            printf("Sério? Você está saindo, então até breve!!\n");
            return;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

//Criando o Mapa
Sala* mapaSalas() {
    Sala* raiz = criarSala("| Hall de Entrada |", "Porque a porta já estava aberta!");   //cria o Sala raiz da árvore, representado o ""Hall de Entrada"
    raiz->esquerda = criarSala("| Biblioteca |", "Aqui tem pegadas de sapato com lama e 01 livro faltando na estante");      //liga o Sala "Sala de Estar" como filho à esquerda da raiz: RAIZ passa pra ESQUERDA (Sala (Biblioteca))
    raiz->esquerda->esquerda = criarSala("| Quarto |", "Hum... a Janela aberta e cortina rasgada");
    raiz->esquerda->direita = criarSala("| Jardim Secreto |", "Ops! tem Terra revirada recentemente");
    raiz->direita = criarSala("| Sala de Estar |", "Uau! 2 Copos quebrados no chão");          
    raiz->direita->esquerda = criarSala("| Cozinha |", "Cuidado, tem Faca fora do bloco de facas"); 
    raiz->direita->direita = criarSala("| Sala de Jogos |", "Alguém começou um jogo e não terminou: Tabuleiro montado para ataque");

    return raiz;
}

//-------- FUNÇÃO PARA LIBERAR MEMORIA --------
void liberarSalas(struct Sala* raiz) { //libera a memoria locada para ua arvore binaria de forma recursiva
    if (raiz != NULL) { //Se o Sala atual existe
        liberarSalas(raiz->esquerda);    //Libera primeiro a subarvore esquerda
        liberarSalas(raiz->direita);     //Depois, libera primeiro a subarvore à direita
        free(raiz); //Por ultimo, libera o proprio Sala atual
    }
}

void liberarPistas(struct Pista* raiz) { //libera a memoria locada para ua arvore binaria de forma recursiva
    if (raiz != NULL) { //Se o Sala atual existe
        liberarPistas(raiz->esq);    //Libera primeiro a subarvore esquerda
        liberarPistas(raiz->dir);     //Depois, libera primeiro a subarvore à direita
        free(raiz); //Por ultimo, libera o proprio Sala atual
    }
}

//-------- FUNÇÃO PRINCIPAL --------
int main() {
    Sala* raiz = mapaSalas();    //Inicializa a arvore
    Pista* arvorePistas = NULL;  //Inicializa Pistas com o NULL

    explorarSalas(raiz, &arvorePistas);    //inicia a exploração da sala a partir da raiz

    printf("\n........................\n");
    printf(" PISTAS COLETADAS (A-Z)\n");
    printf("........................\n");
    exibirPistas(arvorePistas);


    liberarSalas(raiz);          //libera memoria alocada para a arvore
    liberarPistas(arvorePistas); //libera memoria alocada para as pistas
    
    return 0;
}