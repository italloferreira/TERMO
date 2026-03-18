#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TOTAL_PALAVRAS 50
#define TAM_PALAVRA 5
#define MAX_TENTATIVAS 6

typedef enum {
    LETRA_ERRADA,
    LETRA_POSICAO_ERRADA,
    LETRA_CORRETA
} StatusLetra;

const char* palavra(void) {
    static const char* palavras[TOTAL_PALAVRAS] = {
        "AMIGO", "BOLSA", "CANSA", "DENTE", "ESTRE", "FALTA", "GOLPE", "HORAS", "IGUAL", "JANTA",
        "LUGAR", "MUNDO", "NORTE", "OLHOS", "POUCO", "QUASE", "RAPAZ", "SORTE", "TERRA", "UNICO",
        "VISTA", "XADRE", "ZEBRA", "ARDOR", "BANDO", "CRIME", "DONOS", "ESCOL", "FAROL", "GARFO",
        "HOTEL", "IDEIA", "JUNTO", "LEMOS", "MENTA", "NOVOS", "OSSOS", "PRAZO", "QUEDA", "RESTO",
        "SAUDA", "TEMPO", "URUBU", "VINHO", "WAFER", "YEAST", "ZULUS", "BRAVO", "CHAVE", "DIZEM"
    };

    return palavras[rand() % TOTAL_PALAVRAS];
}

void limpar_tela(void) {
    printf("\033[2J\033[H");
}

void maiusculo(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char) toupper((unsigned char) str[i]);
    }
}

void remover_quebra_linha(char* str) {
    str[strcspn(str, "\n")] = '\0';
}

int corretor(const char* tentativa) {
    if ((int) strlen(tentativa) != TAM_PALAVRA) {
        return 0;
    }

    for (int i = 0; i < TAM_PALAVRA; i++) {
        if (!isalpha((unsigned char) tentativa[i])) {
            return 0;
        }
    }

    return 1;
}

void calcular_status(const char* tentativa, const char* palavra_esc, StatusLetra status[TAM_PALAVRA]) {
    for (int i = 0; i < TAM_PALAVRA; i++) {
        if (tentativa[i] == palavra_esc[i]) {
            status[i] = LETRA_CORRETA;
        } else {
            status[i] = LETRA_ERRADA;
            for (int j = 0; j < TAM_PALAVRA; j++) {
                if (tentativa[i] == palavra_esc[j]) {
                    status[i] = LETRA_POSICAO_ERRADA;
                    break;
                }
            }
        }
    }
}

void imprimir_bloco_letra(char letra, StatusLetra status) {
    if (status == LETRA_CORRETA) {
        printf("\033[42;30m %c \033[0m", letra);
    } else if (status == LETRA_POSICAO_ERRADA) {
        printf("\033[43;30m %c \033[0m", letra);
    } else {
        printf("\033[100;97m %c \033[0m", letra);
    }
}

void desenhar_tela(char tentativas[MAX_TENTATIVAS][TAM_PALAVRA + 1], int total_tentativas, const char* mensagem) {
    limpar_tela();

    printf("=======================================\n");
    printf("         TERMO - MODO TERMINAL         \n");
    printf("=======================================\n\n");

    printf("Acerte a palavra de %d letras em %d tentativas.\n", TAM_PALAVRA, MAX_TENTATIVAS);
    printf("Legenda: \033[42;30m certo \033[0m  \033[43;30m existe \033[0m  \033[100;97m nao existe \033[0m\n\n");

    printf("Tabuleiro:\n");
    for (int i = 0; i < MAX_TENTATIVAS; i++) {
        printf("%d | ", i + 1);
        if (i < total_tentativas) {
            for (int j = 0; j < TAM_PALAVRA; j++) {
                printf(" %c ", tentativas[i][j]);
            }
        } else {
            for (int j = 0; j < TAM_PALAVRA; j++) {
                printf(" _ ");
            }
        }
        printf("\n");
    }

    if (mensagem != NULL && mensagem[0] != '\0') {
        printf("\n%s\n", mensagem);
    }

    printf("\n");
}

void desenhar_feedback(char tentativas[MAX_TENTATIVAS][TAM_PALAVRA + 1],
                       StatusLetra historico[MAX_TENTATIVAS][TAM_PALAVRA],
                       int total_tentativas,
                       const char* mensagem) {
    limpar_tela();

    printf("=======================================\n");
    printf("         TERMO - MODO TERMINAL         \n");
    printf("=======================================\n\n");

    printf("Acerte a palavra de %d letras em %d tentativas.\n", TAM_PALAVRA, MAX_TENTATIVAS);
    printf("Legenda: \033[42;30m certo \033[0m  \033[43;30m existe \033[0m  \033[100;97m nao existe \033[0m\n\n");

    printf("Tabuleiro:\n");
    for (int i = 0; i < MAX_TENTATIVAS; i++) {
        printf("%d | ", i + 1);
        if (i < total_tentativas) {
            for (int j = 0; j < TAM_PALAVRA; j++) {
                imprimir_bloco_letra(tentativas[i][j], historico[i][j]);
                printf(" ");
            }
        } else {
            for (int j = 0; j < TAM_PALAVRA; j++) {
                printf(" _ ");
            }
        }
        printf("\n");
    }

    if (mensagem != NULL && mensagem[0] != '\0') {
        printf("\n%s\n", mensagem);
    }

    printf("\n");
}

int ler_linha(char* buffer, size_t tamanho) {
    if (fgets(buffer, (int) tamanho, stdin) == NULL) {
        return 0;
    }

    remover_quebra_linha(buffer);
    return 1;
}

int ler_opcao_jogo(const char* mensagem) {
    char linha[16];

    while (1) {
        printf("%s", mensagem);
        if (!ler_linha(linha, sizeof(linha))) {
            return 0;
        }

        if (strcmp(linha, "1") == 0) {
            return 1;
        }
        if (strcmp(linha, "0") == 0) {
            return 0;
        }

        printf("Opcao invalida. Digite 1 para sim ou 0 para nao.\n");
    }
}

int main(void) {
    int jogar;
    char tentativas[MAX_TENTATIVAS][TAM_PALAVRA + 1];
    StatusLetra historico[MAX_TENTATIVAS][TAM_PALAVRA];
    char entrada[64];
    char mensagem[128];

    srand((unsigned int) time(NULL));

    limpar_tela();
    printf("Bem-vindo ao TERMO!\n\n");
    printf("Regras:\n");
    printf("- Voce tem %d tentativas para acertar a palavra.\n", MAX_TENTATIVAS);
    printf("- Apenas palavras com %d letras sao aceitas.\n", TAM_PALAVRA);
    printf("- As letras aparecem coloridas para indicar o resultado de cada jogada.\n\n");

    jogar = ler_opcao_jogo("Deseja jogar? (1 = sim / 0 = nao): ");

    while (jogar == 1) {
        const char* palavra_esc = palavra();
        int venceu = 0;
        int total_tentativas = 0;

        strcpy(mensagem, "Digite uma palavra de 5 letras para comecar.");

        while (total_tentativas < MAX_TENTATIVAS) {
            desenhar_feedback(tentativas, historico, total_tentativas, mensagem);
            printf("Tentativa %d/%d: ", total_tentativas + 1, MAX_TENTATIVAS);

            if (!ler_linha(entrada, sizeof(entrada))) {
                printf("\nErro ao ler a entrada. Encerrando o jogo.\n");
                return 1;
            }

            maiusculo(entrada);

            if (!corretor(entrada)) {
                strcpy(mensagem, "Entrada invalida! Digite apenas uma palavra com 5 letras.");
                continue;
            }

            strcpy(tentativas[total_tentativas], entrada);
            calcular_status(entrada, palavra_esc, historico[total_tentativas]);
            total_tentativas++;

            if (strcmp(entrada, palavra_esc) == 0) {
                venceu = 1;
                strcpy(mensagem, "Parabens! Voce acertou a palavra.");
                break;
            }

            snprintf(mensagem, sizeof(mensagem), "A palavra ainda nao foi encontrada. Restam %d tentativa(s).",
                     MAX_TENTATIVAS - total_tentativas);
        }

        if (!venceu) {
            snprintf(mensagem, sizeof(mensagem), "Fim de jogo! A palavra correta era %s.", palavra_esc);
        }

        desenhar_feedback(tentativas, historico, total_tentativas, mensagem);
        jogar = ler_opcao_jogo("Deseja jogar novamente? (1 = sim / 0 = nao): ");
    }

    printf("\nValeu por jogar! Ate a proxima.\n");
    return 0;
}
