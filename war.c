#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes Globais ---
#define MAX_LIVROS 50
#define MAX_EMPRESTIMOS 100
#define TAM_STRING 100

// --- Definição das Estruturas ---
/**
 * @brief Estrutura que representa um Livro no sistema da biblioteca.
 * Armazena as informações básicas de um livro e seu status de disponibilidade.
 */
struct Livro {
    char nome[TAM_STRING];      /**< Título do livro. */
    char autor[TAM_STRING];     /**< Nome do autor do livro. */
    char editora[TAM_STRING];   /**< Nome da editora responsável pela publicação. */
    int edicao;                 /**< Número ou ano da edição. */
    int disponivel;             /**< Flag de status de disponibilidade (1: Disponível, 0: Emprestado). */
};

/**
 * @brief Estrutura que representa um empréstimo de livro.
 * Nota: Esta estrutura é inferida a partir dos protótipos e da função main.
 */
struct Emprestimo {
    int idLivro;                /**< Índice do livro emprestado no array 'biblioteca'. */
    char leitor[TAM_STRING];    /**< Nome do leitor. (Inferido) */
    // Campos adicionais como data poderiam ser adicionados aqui.
};

// --- Protótipos das Funções ---
// Declarar as funções aqui permite que a 'main' as chame antes de suas definições.
void limparBufferEntrada();
void exibirMenu();
void cadastrarLivro(struct Livro *biblioteca, int *totallivros);
void listarLivros(const struct Livro *biblioteca, int totallivros); // 'const' indica que a função só lê.
void realizarEmprestimo(struct Livro *biblioteca, int totallivros, struct Emprestimo *emprestimos, int *totalEmprestimos);
void listarEmprestimos(const struct Livro *biblioteca, const struct Emprestimo *emprestimos, int totalEmprestimos);
void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos);


int main() {
    // Alocação dinâmica de memória para os arrays de estruturas
    struct Livro *biblioteca = (struct Livro *) calloc(MAX_LIVROS, sizeof(struct Livro));
    // Note: A alocação de 'emprestimos' estava cortada, foi completada com base na lógica.
    struct Emprestimo *emprestimos = (struct Emprestimo *) malloc(MAX_EMPRESTIMOS * sizeof(struct Emprestimo));

    if (biblioteca == NULL || emprestimos == NULL) {
        printf("Erro: Falha ao alocar memoria.\n");
        return 1;
    }

    int totallivros = 0;
    int totalEmprestimos = 0;
    int opcao;

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) { // Verifica se a leitura falhou (não é um número)
             // Tenta limpar o buffer se a leitura não for um número
            limparBufferEntrada();
            opcao = -1; // Define uma opção inválida para cair no default
        } else {
            limparBufferEntrada(); // Limpa o buffer após scanf ter sucesso
        }


        switch (opcao) {
            case 1:
                // Passamos o endereço de 'totallivros' (&) para que a função possa MODIFICAR seu valor.
                cadastrarLivro(biblioteca, &totallivros);
                break;
            case 2:
                // Passamos apenas o valor de 'totallivros', pois a função só precisa LER.
                listarLivros(biblioteca, totallivros);
                break;
            case 3:
                realizarEmprestimo(biblioteca, totallivros, emprestimos, &totalEmprestimos);
                break;
            case 4:
                listarEmprestimos(biblioteca, emprestimos, totalEmprestimos);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter para tentar novamente.");
                getchar(); // Espera o usuário pressionar Enter
                break;
        }
    } while (opcao != 0);

    // Chama a função dedicada para liberar a memória alocada.
    liberarMemoria(biblioteca, emprestimos);

    return 0;
}

// --- Implementação das Funções ---

void limparBufferEntrada() {
    int c;
    while (((c = getchar()) != '\n' && c != EOF));
}

/**
 * @brief Exibe o menu principal de opções para o usuário.
 */
void exibirMenu() {
    printf("======================================\n");
    printf("     BIBLIOTECA - PARTE 3 (MESTRE)\n");
    printf("======================================\n");
    printf("1 - Cadastrar novo livro\n");
    printf("2 - Listar todos os livros\n");
    printf("3 - Realizar emprestimo\n");
    printf("4 - Listar emprestimos\n");
    printf("0 - Sair\n");
    printf("--------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Adiciona um novo registro de livro à biblioteca.
 *
 * Solicita ao usuário os dados do livro e os armazena na próxima posição
 * disponível do array 'biblioteca'. O contador de livros é atualizado.
 *
 * @param biblioteca Ponteiro para o array de estruturas struct Livro.
 * @param totallivros Ponteiro para a variável que armazena o número
 * de livros cadastrados.
 */
void cadastrarLivro(struct Livro *biblioteca, int *totallivros) {
    printf("--- Cadastro de Novo Livro ---\n\n");

    if (*totallivros < MAX_LIVROS) {
        int indice = *totallivros; // Usa o valor apontado pelo ponteiro para o índice.

        printf("Digite o nome do livro: ");
        fgets(biblioteca[indice].nome, TAM_STRING, stdin);
        biblioteca[indice].nome[strcspn(biblioteca[indice].nome, "\n")] = 0; // Remove newline

        printf("Digite o autor: ");
        fgets(biblioteca[indice].autor, TAM_STRING, stdin);
        biblioteca[indice].autor[strcspn(biblioteca[indice].autor, "\n")] = 0; // Remove newline

        printf("Digite a editora: ");
        fgets(biblioteca[indice].editora, TAM_STRING, stdin);
        biblioteca[indice].editora[strcspn(biblioteca[indice].editora, "\n")] = 0; // Remove newline

        printf("Digite a edicao (numero inteiro): ");
        if (scanf("%d", &biblioteca[indice].edicao) != 1) {
            printf("Entrada invalida para edicao. Definindo para 0.\n");
            biblioteca[indice].edicao = 0;
            limparBufferEntrada();
        } else {
             limparBufferEntrada();
        }


        biblioteca[indice].disponivel = 1; // Novo livro sempre começa como disponível.

        (*totallivros)++; // Incrementa o valor da variável original na main.
        printf("\nLivro cadastrado com sucesso!\n");
    } else {
        printf("Biblioteca cheia!\n");
    }

    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Lista todos os livros cadastrados na biblioteca.
 *
 * @param biblioteca O array (ponteiro constante, apenas para leitura) de livros.
 * @param totallivros O número de livros (passagem por valor).
 */
void listarLivros(const struct Livro *biblioteca, int totallivros) {
    printf("--- Lista de Livros Cadastrados ---\n\n");

    if (totallivros == 0) {
        printf("Nenhum livro cadastrado ainda.\n");
    } else {
        for (int i = 0; i < totallivros; i++) {
            printf("--------------------------------------\n");
            printf("LIVRO %d\n", i + 1);
            printf("Nome: %s\n", biblioteca[i].nome);
            printf("Autor: %s\n", biblioteca[i].autor);
            // Editora e Edicao não estavam no print, mas seriam adicionados aqui
            // printf("Editora: %s\n", biblioteca[i].editora);
            // printf("Edicao: %d\n", biblioteca[i].edicao);
            printf("Status: %s\n", biblioteca[i].disponivel ? "Disponivel" : "Emprestado");
        }
    }
    printf("--------------------------------------\n");
    printf("\nPressione Enter para continuar...");
    getchar();
}

// --- Funções com protótipos mas sem implementação nos trechos fornecidos ---

/**
 * @brief Função placeholder para realizar o empréstimo de um livro.
 */
void realizarEmprestimo(struct Livro *biblioteca, int totallivros, struct Emprestimo *emprestimos, int *totalEmprestimos) {
    printf("\n[Funcao 'realizarEmprestimo' nao implementada.]\n");
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Função placeholder para listar todos os empréstimos realizados.
 */
void listarEmprestimos(const struct Livro *biblioteca, const struct Emprestimo *emprestimos, int totalEmprestimos) {
    printf("\n[Funcao 'listarEmprestimos' nao implementada.]\n");
    printf("\nPressione Enter para continuar...");
    getchar();
}

/**
 * @brief Libera a memória alocada dinamicamente para os arrays de livros e empréstimos.
 *
 * @param biblioteca Ponteiro para o array de livros a ser liberado.
 * @param emprestimos Ponteiro para o array de empréstimos a ser liberado.
 */
void liberarMemoria(struct Livro *biblioteca, struct Emprestimo *emprestimos) {
    free(biblioteca);
    free(emprestimos);
    printf("Memoria liberada com sucesso.\n");
}
