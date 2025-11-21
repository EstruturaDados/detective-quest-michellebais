#include <stdio.h>  //biblioteca padrão de entrada e saída
#include <stdlib.h> //para malloc() e free()
#include <string.h> //para manipular srings
#include <ctype.h>  //verifica se é letra, número, espaço e converte maiúsculas em minúsculas 

#define TAM_HASH 31 //define tamanho da tabela

//--------------------------- ENUM SUSPEITOS ---------------------------
typedef enum {
    SR_ROBERTO,
    SRTA_HELENA,
    JARDINEIRO_LUIS,
    COZINHEIRO_PAULO,
    SUSPEITO_DESCONHECIDO,
    NUM_SUSPEITOS
} Suspeito;

const char* SuspeitoNome[NUM_SUSPEITOS] = {
    "Mordomo Roberto",
    "Viuva Helena",
    "Jardineiro Luis",
    "Cozinheiro Paulo",
    "Desconhecido"
};

//--------------------------- ESTRUTURAS ---------------------------
// Nó da BST de salas (mapa)
typedef struct Sala {
    char nome[50];          //nome do cômodo da mansão
    char pista[100];        //pista para cada comodo, qdo houver
    struct Sala* esquerda;      //monta a struct da esquerda
    struct Sala* direita;       //monta a struct da direita
} Sala;

// Nó da BST de pistas coletadas
typedef struct Pista {
    char texto[100];
    struct Pista* esq;      //vai pra esquerda
    struct Pista* dir;      //vai pra direita
} Pista;

//Itens da Tabela Hash (encadeamento)
typedef struct HashItem {
    char pista[100];
    Suspeito suspeito;          //chama a struct 'Suspeito' e define uma variável 'suspeito' pra ela
    struct HashItem* prox;      //define o ponteiro 'prox' para a struct 'HashItem*'
} HashItem;

HashItem* tabelaHash[TAM_HASH]; //define que o tamanho do da struct 'tabelaHash' do tipo 'HashItem'é '[TAM_HASH]'

//--------------------------- FUNÇÕES AUXILIARES ---------------------------
unsigned int hashFunction(const char* str) {        //ponteiro 'str' como constante (string de entrada). Retorna um inteiro sem sinal 'unsigned int'
    unsigned int hash = 0;                          //o loop percorre cada caractere da string 'str' até encontrar o terminador nulo (\0)
    for (int i = 0; str[i]; i++)                    //valor de 'hash' é multiplicado por 31 (número primo, pra evitar colisões)
        hash = 31 * hash + (unsigned char)str[i];
    return hash % TAM_HASH;         //retorna o resto '%' de 'hash' divido pelo tamanho da string 'TAM_HASH' pra saber qual posição (ID) da tabela vai o valor
}
//copiando uma string (src) para uma string temporaria (dst) e letras minusculas
void str_to_lower_copy(const char* src, char* dst, size_t size) {       //'const' para não modificar o valor nem da original nem da 'dst'
    size_t i;                                   //variável para o contador
    for (i = 0; i + 1 < size && src[i]; i++)    //'i + 1 < size' para começar 1 depois e terminar 1 antes do espaço total da string (o nulo tem espaço)
        dst[i] = tolower((unsigned char)src[i]);    //'tolower' converte letras maiúsculas em minúsculas e deixa outros caracteres inalterados
    dst[i] = '\0';      // adiciona o caractere nulo (\0) na posição i, para mostrar que a string terminou
}

int str_icmp(const char* a, const char* b) {    //compara as 2 strings 'a' e 'b'
    char x[200], y[200];                    //declara strings 'x' e 'y' temporarias pra receber os valores a serem comparados
    str_to_lower_copy(a, x, sizeof(x));     //copia o conteúdo de 'a' para 'x', em minúsculos sem passar de 200 caracteres
    str_to_lower_copy(b, y, sizeof(y));     //copia o conteúdo de 'b' para 'y', em minúsculos sem passar de 200 caracteres
    return strcmp(x, y);
}

//--------------------------- TABELA HASH ---------------------------
void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++)  //for para percorrer todas as strings da tabela hash
        tabelaHash[i] = NULL;
}

void inserirNaHash(const char* pista, Suspeito suspeito) {  //adiciona item na tabela hash, usando 'pista' como chave pra determinar onde o item será armazenado
    unsigned int idx = hashFunction(pista);     //chama a função 'hashFunction' para pista ser igual a um índice numérico válido (idx) na tabela hash
    HashItem* novo = malloc(sizeof(HashItem));  //alocação dinamica de memoria

    strncpy(novo->pista, pista, sizeof(novo->pista));   //copia a string 'pista' para o campo 'pista' dentro do novo nó
    novo->pista[sizeof(novo->pista)-1] = '\0';          //termina em nulo a string

    novo->suspeito = suspeito;      //os dados são copiados para o novo nó
    novo->prox = tabelaHash[idx];   //o ponteiro 'prox' do 'novo' item aponta para o que antes era o primeiro item naquela posição
    tabelaHash[idx] = novo;         //a posição da lista (tabelaHash[idx]) é atualizada para apontar para o novo item, fazendo dele o primeiro da lista
}

Suspeito encontrarSuspeitoIndex(const char* pista) {    //busca (lookup) por um item dentro da tabela hash. Tem 'pista' como chave de busca, e retorna um objeto do tipo 'Suspeito'
    unsigned int idx = hashFunction(pista);             //chama a função 'hashFunction' para pista ser igual a um índice numérico válido (idx) na tabela hash
    HashItem* atual = tabelaHash[idx];                  //ponteiro 'atual' aponta para o primeiro item na lista ligada localizada naquele índice 'tabelaHash[idx]'
    while (atual) {                                     //percorre todos os itens presentes naquela posição da tabela hash
        if (str_icmp(atual->pista, pista) == 0)         //loop, a chave no item atual 'atual->pista' é comparada com a chave de busca 'pista'
            return atual->suspeito;
        atual = atual->prox;
    }
    return SUSPEITO_DESCONHECIDO;           //se a pista nao for encontrada na tabela hash
}

void limparHash() {        //limpa memoria da tabela hash
    for (int i = 0; i < TAM_HASH; i++) {
        HashItem* a = tabelaHash[i];
        while (a) {
            HashItem* t = a;
            a = a->prox;
            free(t);
        }
        tabelaHash[i] = NULL;
    }
}

void exibirAssociacoesHash() {      //item 3 do Menu (criado para confirmar que estão cadastradas pistas e seus suspeitos)
    printf("\nASSOCIAÇÕES PISTA → SUSPEITO:\n");
    for (int i = 0; i < TAM_HASH; i++) {
        HashItem* a = tabelaHash[i];
        while (a) {
            printf(" - \"%s\" → %s\n", a->pista, SuspeitoNome[a->suspeito]);
            a = a->prox;
        }
    }
}

//--------------------------- ÁRVORE DE SALAS ---------------------------
//cria e inicializa a nova Sala
Sala* criarSala(char* nome, char* pista) {      //Qdo preencher a sala, deve ter o '"nome","pista"'
    Sala* nova = malloc(sizeof(Sala));          //aloca dinamicamente memoria para um nova Sala e guarda o endereço em 'nova'
    if (nova == NULL){
        printf("Erro ao alocar memoria. Saindo...\n");
        exit(1);
    }
    strcpy(nova->nome, nome);                   //copia o nome passado como parâmetro (string) para o campo 'nome' da Sala
    //strcpy(nova->pista, pista ? pista : "");*********************************************************************************************************************************
    if (pista != NULL)
        strcpy(nova->pista, pista);         //Se pista não for nulo, a expressão inteira retorna o próprio ponteiro pista
    else
        nova->pista[0] = '\0';              //Senão, a expressão retorna um ponteiro para uma string vazia terminada em '\0'

    nova->esquerda = NULL;                      //inicializa os ponteiros dos filhos como NULL (sem filhos ainda, por isso NULL)
    nova->direita = NULL;
    return nova;
}
//Criando o Mapa
Sala* mapaSalas() {
    Sala* raiz = criarSala("Hall de Entrada", "a porta já estava aberta!");
    raiz->esquerda = criarSala("Biblioteca", "vejo pegadas de sapato com lama");
    raiz->esquerda->esquerda = criarSala("Quarto", "a janela esta aberta e a cortina rasgada");
    raiz->esquerda->direita = criarSala("Jardim Secreto", "tem Terra revirada recentemente");
    raiz->direita = criarSala("Sala de Estar", "copos quebrados no chão");
    raiz->direita->esquerda = criarSala("Cozinha", "tem faca fora do bloco de facas");
    raiz->direita->direita = criarSala("Sala de Jogos", "alguém começou um jogo e não terminou");
    return raiz;
}
//Liberando memória
void liberarSalas(Sala* r) {
    if (r) {
        liberarSalas(r->esquerda);
        liberarSalas(r->direita);
        free(r);
    }
}

//--------------------------- ÁRVORE DE PISTAS ---------------------------
//Inserindo na BST
Pista* inserirPista(Pista* r, const char* t) {  //ponteiro para o nó raiz atual (r) e a string de texto a ser inserida (t), e retorna o ponteiro para a raiz da subárvore
    if (!r) {           //se for diferente de Nulo
        Pista* n = malloc(sizeof(Pista));   //aloca memoria
        strcpy(n->texto, t);        //copia o texto 't' para o campo 'texto' do novo nó
        n->esq = n->dir = NULL;     //Nó folha: ponteiros esquerdo e direito são inicializados como NULL
        return n;
    }
    if (strcmp(t, r->texto) < 0)    //o novo texto 't' é lexicograficamente menor que o texto no nó atual (r->texto)
        r->esq = inserirPista(r->esq, t);   //função chama recursivamente a si mesma para a subárvore esquerda 'r->esq'
    else if (strcmp(t, r->texto) > 0) // evita duplicata: ignora casos onde strcmp retorna 0 (textos idênticos)
        r->dir = inserirPista(r->dir, t);   //função chama recursivamente a si mesma para a subárvore direita 'r->esq'
    return r;
}

//Exibindo pistas em ordem
void exibirPistas(Pista* r) {
    if (r) {
        exibirPistas(r->esq);       //chama a função 'exibirPistas' para os campos à esquerda
        printf("- %s\n", r->texto); //lista as pistas encontradas
        exibirPistas(r->dir);       //chama a função 'exibirPistas' para os campos à direita
    }
    //***********************************printf("Indique seu suspeito:\n");
}

//Liberando memória
void liberarPistas(Pista* r) {
    if (r) {
        liberarPistas(r->esq);
        liberarPistas(r->dir);
        free(r);
    }
}

//--------------------------- EXPLORAÇÃO ---------------------------
//Explorando a mansão e coletando pistas (Esquerda, Direita, Sair)
void explorarSalas(Sala* atual, Pista** arvore) {   //cria o ponteiro do ponteiro
    char op;    //para criar a interação do jogador (d, e, s)

    while (atual) {
        printf("\nVocê está em %s\n", atual->nome);
        printf("→ Pista encontrada: %s\n", atual->pista);

        *arvore = inserirPista(*arvore, atual->pista);      //ponteiro '*arvore' recebe da função 'inserirPista' as informações

        printf("\nPara onde deseja ir?\n");
        if (atual->esquerda) printf("E - %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("D - %s\n", atual->direita->nome);
        printf("S - Sair da exploração\n");
        printf("Opção: ");

        scanf(" %c", &op);

        switch (toupper(op)) {  //converte a opção do usuário 'op' para letra maiúscula
            case 'E': 
                if (atual->esquerda) {
                    atual = atual->esquerda;
                }
                break;

            case 'D': 
                if (atual->direita) {
                    atual = atual->direita; 
                }
                break;

            case 'S': 
                return;

            default: 
                printf("Opção inválida!\n");
        }
    }
}

//--------------------------- CONTAGEM E JULGAMENTO ---------------------------
int contarPistasDoSuspeito(Pista* r, Suspeito s) {
    if (!r) return 0;
    int c = (encontrarSuspeitoIndex(r->texto) == s);
    return c + contarPistasDoSuspeito(r->esq, s) + contarPistasDoSuspeito(r->dir, s);
}

void limparBuffer() {   //para limpar antes do fgets
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void verificarSuspeitoFinal(Pista* arvore) {    //*********************************************************************************
    char nome[100];
    printf("\nDigite o nome do suspeito: ");
    limparBuffer();
    fgets(nome, sizeof(nome), stdin);   //Le o maximo de caracter permitido + nulo
    nome[strcspn(nome, "\n")] = 0;      //acha o '\n' e substitui por '0'

    int idx = -1;
    for (int i = 0; i < NUM_SUSPEITOS; i++) {
        if (str_icmp(nome, SuspeitoNome[i]) == 0) { ////loop, a chave 'nome' é comparada com a chave 'SuspeitoNome[i]'
            idx = i;    //recebe o valor 'suspeito'
            break;
        }        
    }

    if (idx < 0) {  //se for maior que 0  não tem na lista
        printf("Suspeito desconhecido!\n");
        return;
    }

    int total = contarPistasDoSuspeito(arvore, idx);    //recebe o resultado da função chamada

    printf("\nResultado da investigação:\n");
    if (total >= 2) {
        printf(">> Você acertou! %s é o culpado (%d pistas).\n",
            SuspeitoNome[idx], total);
    } else {
        printf(">> Você errou. Apenas %d pista(s).\n", total);
    }
}

//--------------------------- HASH PADRÃO ---------------------------
// Preencher associações conhecidas pista -> suspeito
void preencherHashSuspeitosPadrao() {
    inserirNaHash("a porta já estava aberta!", SR_ROBERTO);
    inserirNaHash("vejo pegadas de sapato com lama", SRTA_HELENA);
    inserirNaHash("a janela esta aberta e a cortina rasgada", SR_ROBERTO);
    inserirNaHash("tem Terra revirada recentemente", JARDINEIRO_LUIS);
    inserirNaHash("copos quebrados no chão", SRTA_HELENA);
    inserirNaHash("tem faca fora do bloco de facas", COZINHEIRO_PAULO);
    inserirNaHash("alguém começou um jogo e não terminou", SR_ROBERTO);
}

//--------------------------- MENU PRINCIPAL ---------------------------
//Listando os suspeitos
void mostrarSuspeitos() {
    printf("\nSuspeitos:\n");
    for (int i = 0; i < NUM_SUSPEITOS - 1; i++)
        printf(" - %s\n", SuspeitoNome[i]);
}

//--------------------------- SUSPEITO MAIS PROVÁVEL ---------------------------
// Função recursiva auxiliar para contar pistas por suspeito
void contarPistasPorSuspeito(Pista* r, int cont[]) {
    if (!r) return;

    // Verifica quem é o suspeito associado a esta pista
    Suspeito s = encontrarSuspeitoIndex(r->texto);
    if (s != SUSPEITO_DESCONHECIDO)
        cont[s]++;

    contarPistasPorSuspeito(r->esq, cont);
    contarPistasPorSuspeito(r->dir, cont);
}

// Retorna o suspeito com maior quantidade de pistas associadas
Suspeito suspeitoMaisProvavel(Pista* raiz) {
    int cont[NUM_SUSPEITOS] = {0};

    contarPistasPorSuspeito(raiz, cont);

    int max = -1;
    Suspeito melhor = SUSPEITO_DESCONHECIDO;

    for (int i = 0; i < NUM_SUSPEITOS; i++) {
        if (cont[i] > max) {
            max = cont[i];
            melhor = i;
        }
    }
    return melhor;
}

//-------- FUNÇÃO PRINCIPAL --------
int main() {

    Sala* mapa = mapaSalas();
    Pista* arvore = NULL;

    int opcao;

    while (1) {             //menu do jogo
        printf("\n|.|.|.|.|.|.|.|.|.|.|.|.|.|.|.|\n");
        printf("|.|.|.| DETECTIVE STACK |.|.|.|\n");
        printf("|.|.|.|.|.|.|.|.|.|.|.|.|.|.|.|\n");
        printf("1 - Iniciar investigação\n");
        printf("2 - Mostrar suspeitos\n");
        printf("3 - Mostrar associações da hash\n");
        printf("4 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar();          // limpa '\n'

        switch (opcao) {
            case 1:
                inicializarHash();                  // Inicializa estruturas do jogo
                preencherHashSuspeitosPadrao();
                explorarSalas(mapa, &arvore);

                printf("\nPistas coletadas:\n");    // Exibir pistas coletadas
                exibirPistas(arvore);

                verificarSuspeitoFinal(arvore);     // Fase final
                                
                Suspeito s = suspeitoMaisProvavel(arvore);  // Mostra automaticamente o culpado mais provável
                printf("\n\n=== Análise automática ===\n");
                printf("Com base nas pistas coletadas, o suspeito mais provável é:\n");
                printf(">> %s\n", SuspeitoNome[s]);
                break;

            case 2:
                mostrarSuspeitos();
                break;

            case 3:
                inicializarHash();
                preencherHashSuspeitosPadrao();
                exibirAssociacoesHash();
                limparHash();
                break;

            case 4:
                printf("Saindo... até a próxima, detetive!\n");
                liberarSalas(mapa);
                liberarPistas(arvore);
                limparHash();
                return 0;

            default:
                printf("Opção inválida! Escolha 1-4.\n");
        }
    }
}
