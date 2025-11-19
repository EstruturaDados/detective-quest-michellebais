#include <stdio.h>  //biblioteca padrão de entrada e saída
#include <stdlib.h> //para malloc() e free()
#include <string.h> //para manipular srings
 
typedef struct Sala { 
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

//FUNÇÃO QUE CRIA E INICIALIZA A NOVA SALA
Sala* criarSala(char *nome) {
    Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));   //aloca dinamicamente memoria para um nova Sala e guarda o endereço em 'nova'
    if (nova == NULL){
        printf("Erro ao alocar memoria. Saindo...\n");
        exit(1);
    }
    strcpy(nova->nome, nome); //copia o nome passado como parâmetro (string) para o campo 'nome' da Sala

    nova->esquerda = NULL;  //inicializa os ponteiros dos filhos como NULL (sem filhos ainda, por isso NULL)
    nova->direita = NULL;

    return nova;    //retorna o endereço do nova Sala criado
}

//FUNÇÃO PARA EXPLORAR A SALA (ESQUERDA, DIREITA, SAIR)
void explorarSalas(Sala *atual){
    char escolha;   //para criar a interação do jogador (d, e, s)

    while (atual != NULL){
        printf("Você está aqui -> %s\n", atual->nome);

        if(atual->esquerda == NULL && atual->direita == NULL){
            printf("Aqui não tem saída, chegou ao final da jornada!!\n", atual->nome);
            return;
        }

        printf("Para onde deseja ir?\n");
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

Sala* mapaSalas(){
    Sala* raiz = criarSala("| Hall de Entrada |");   //cria o Sala raiz da árvore, representado o ""Hall de Entrada"
    raiz->esquerda = criarSala("| Biblioteca |");      //liga o Sala "Sala de Estar" como filho à esquerda da raiz: RAIZ passa pra ESQUERDA (Sala (Biblioteca))
    raiz->esquerda->esquerda = criarSala("| Quarto |");
    raiz->esquerda->direita = criarSala("| Jardim Secreto |");
    raiz->direita = criarSala("| Sala de Estar |");          
    raiz->direita->esquerda = criarSala("| Cozinha |"); 
    raiz->direita->direita = criarSala("| Sala de Jogos |");

    return raiz;
}

void liberar(struct Sala* raiz) { //libera a memoria locada para ua arvore binaria de forma recursiva
    if (raiz != NULL) { //Se o Sala atual existe
        liberar(raiz->esquerda);    //Libera primeiro a subarvore esquerda
        liberar(raiz->direita);     //Depois, libera primeiro a subarvore à direita
        free(raiz); //Por ultimo, libera o proprio Sala atual
    }
}

int main() {
    Sala* raiz = mapaSalas();    //Inicializa a arvore

    explorarSalas(raiz);    //inicia a exploração da sala a partir da raiz
 
    liberar(raiz);      //libera memoria alocada para a arvore
    
    return 0;
}
