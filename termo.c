#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

const char* palavra() {
    const char* palavras[] = {
        "AMIGO", "BOLSA", "CANSA", "DENTE", "ESTRE", "FALTA", "GOLPE", "HORAS", "IGUAL", "JANTA",
        "LUGAR", "MUNDO", "NORTE", "OLHOS", "POUCO", "QUASE", "RAPAZ", "SORTE", "TERRA", "UNICO",
        "VISTA", "XADRE", "ZEBRA", "ARDOR", "BANDO", "CRIME", "DONOS", "ESCOL", "FAROL", "GARFO",
        "HOTEL", "IDEIA", "JUNTO", "LEMOS", "MENTA", "NOVOS", "OSSOS", "PRAZO", "QUEDA", "RESTO",
        "SAUDA", "TEMPO", "URUBU", "VINHO", "WAFER", "YEAST", "ZULUS", "BRAVO", "CHAVE", "DIZEM"
    };

    int selecionador = rand() % 50;
    return palavras[selecionador];
}

void maiusculo(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int corretor(const char* tentativa) {
    if (strlen(tentativa) != 5) {
        printf("So pode palavras com 5 letras! Tente novamente.\n");
        return 0;
    }
    return 1;
}

void feedback(const char* tentativa, const char* palavra_esc) {
    for (int i = 0; i < 5; i++) {
        if (tentativa[i] == palavra_esc[i]) {
            printf("\033[0;32m%c\033[0m", tentativa[i]);
        } else {
            int letra_lug_errado = 0;
            for (int j = 0; j < 5; j++) {
                if (tentativa[i] == palavra_esc[j]) {
                    letra_lug_errado = 1;
                    break;
                }
            }
            if (letra_lug_errado) {
                printf("\033[1;33m%c\033[0m", tentativa[i]);
            } else {
                printf("%c", tentativa[i]);
            }
        }
    }
    printf("\n");
}

int main() {
    int vida;
    char tentativa[6];
    int jogar;

    srand(time(NULL));

    printf("Antes de comecar, atente-se as regras!\n");
    printf("- Voce tem 6 vidas para acertar a palavra\n");
    printf("- So sao aceitas palavras com 5 letras\n");
    printf("- As dicas vao aparecer com cores:\n");
    printf("- \033[0;32mVERDE\033[0m = letra no lugar certo\n");
    printf("- \033[1;33mAMARELO\033[0m = letra certa no lugar errado\n\n");

    printf("Deseja jogar? (1 = sim / 0 = nao): ");
    scanf("%d", &jogar);

    while (jogar == 1) {
        const char* palavra_esc = palavra();

        printf("\nUma nova palavra foi escolhida!\n");

        for (vida = 0; vida < 6; vida++) {
            printf("\nTentativa %d: ", vida + 1);
            scanf("%s", tentativa);
            maiusculo(tentativa);

            if (!corretor(tentativa)) {
                vida--;
                continue;
            }

            if (strcmp(tentativa, palavra_esc) == 0) {
                printf("\nParabens, voce acertou a palavra!!!\n");
                break;
            } else {
                feedback(tentativa, palavra_esc);
            }
        }

        if (strcmp(tentativa, palavra_esc) != 0) {
            printf("\nVoce perdeu! A palavra era: %s\n", palavra_esc);
        }

        printf("\nDeseja jogar novamente? (1 = sim / 0 = nao): ");
        scanf("%d", &jogar);
    }

    printf("\nValeu por jogar! Ate a proxima\n");

    return 0;
}
