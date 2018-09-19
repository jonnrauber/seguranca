/**
 * Algoritmo da Cifra de Transposição
 *
 * Autor: Jonathan Terhorst Rauber - Data: 13/09/2018
 * Universidade Federal da Fronteira Sul
 * Ciência da Computação
 * Segurança e Auditoria de Sistemas
 *
 * Para compilar:
 * gcc transposition.c -o trans -Wall -lm
 *
 * Para executar:
 * ./trans
 *
 * Descrição:
 * Inserir o texto claro que deseja criptografar e a chave.
 * Será exibido o texto criptografado.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 9999

int* pos_vector = NULL;

/*
 * Imprime a tabela de transposição.
 * */
void print_table(void* _table, char* key, int qt_lines) {
    char (*table)[strlen(key)] = _table;
    int i, j;

    for (i = 0; i < strlen(key); i++) {
        printf("%c ", key[i]);
    }
    printf("\n");
    for (i = 0; i < strlen(key); i++) {
        printf("- ");
    }
    printf("\n");
    for (i = 0; i < qt_lines; i++) {
        for (j = 0; j < strlen(key); j++) {
            printf("%c ", table[i][j]);
        }
        printf("\n");
    }
}

int* key_order(char* key) {
    int i, j, menor;
    char* aux = NULL;
    int* pos_vector = (int *)malloc(sizeof(int)*strlen(key));

    aux = (char *)malloc(sizeof(char)*strlen(key));
    strcpy(aux, key);
    for (i = 0; i < strlen(aux); i++) {
        for (j = 0; j < strlen(aux); j++) {
            if (j == 0)
                menor = j;
            else
                if (aux[j] < aux[menor])
                    menor = j;
        }
        pos_vector[i] = menor;
        aux[menor] = 126; //pra impedir que seja escolhido novamente como o menor
    }
    free(aux);

    return pos_vector;
}

/*
 * Cifra de transposição.
 * Recebe um texto e uma chave e criptografa dispondo na tabela.
 * */
char* transposition_encrypt(char* text, char* key) {
    int qt_lines = (int) ceil( (double)strlen(text) / strlen(key));
    char table[qt_lines][strlen(key)];
    int i, j, k = 0, len = 0;
    char alpha = 'a';
    char* encrypted = NULL;

    for (i = 0; i < qt_lines; i++) {
        for (j = 0; j < strlen(key); j++) {
            if (k < strlen(text))
                table[i][j] = text[k++];
            else
                table[i][j] = alpha++;
        }
    }

    pos_vector = key_order(key);

    print_table(table, key, qt_lines);

    for (i = 0; i < strlen(key); i++) {
        for (j = 0; j < qt_lines; j++) {
            encrypted = (char *)realloc(encrypted, sizeof(char)*(++len));
            encrypted[len - 1] = table[j][pos_vector[i]];
        }
    }

    return encrypted;
}

/*
 * Recebe um texto criptografado e a respectiva chave de criptografia.
 * Retorna uma string com o conteudo de toda a tabela de transposicao, na sequencia legível.
 * */
char* transposition_decrypt(char* text, char* key) {
    int i, j, k = 0, l = 0;
    int qt_lines = strlen(text)/strlen(key);
    char table[qt_lines][strlen(key)];
    char* decrypted = NULL;

    for (i = 0; i < strlen(key); i++) {
        for (j = 0; j < qt_lines; j++) {
            if (k < strlen(text))
                table[j][pos_vector[i]] = text[k++];
        }
    }

    decrypted = (char *)realloc(decrypted, sizeof(char) * (k + 1));
    for (i = 0; i < qt_lines; i++) {
        for (j = 0; j < strlen(key); j++) {
            if (l <= k)
                decrypted[l++] = table[i][j];
        }
    }
    decrypted[k] = '\0';

    return decrypted;
}

/*
 * Remove espaços da string passada como argumento, retornando a string resultante.
 * */
char* remove_spaces(char* string) {
    int i, count;
    char* new = NULL;
    for (i = 0, count = 0; i < strlen(string); i++) {
        if (string[i] != ' ') {
            new = (char *)realloc(new, sizeof(char)*++count);
            new[count-1] = string[i];
        }
    }
    new = (char *)realloc(new, sizeof(char)*++count);
    new[count-1] = '\0';
    return new;
}

/*
 * Lê uma linha da entrada padrão em 'string' e substitui o caracter '\n' por '\0'
 * */
void read(char* string) {
    fgets(string, MAX_LEN, stdin);
    string[strlen(string)-1] = '\0';
}

int main(void) {
    char* text = NULL;
    char cleartext[MAX_LEN];
    char key[MAX_LEN];

    printf("--- Cifra de Transposição ---\n");

    //lê o texto claro da entrada padrão
    printf("Digite o texto a ser criptografado: ");
    read(cleartext);

    //lê a chave da entrada padrão
    printf("Digite a chave para a criptografia: ");
    read(key);

    //remove os espaços do texto claro, para dispor na tabela
    text = remove_spaces(cleartext);

    //criptografa com a cifra de transposição
    text = transposition_encrypt(text, key);

    printf("\nTexto criptografado com a cifra de transposição: \n");
    printf("%s\n", text);

    //descriptografa novamente
    text = transposition_decrypt(text, key);

    printf("\nTexto novamente descriptografado: \n");
    printf("%s\n", text);

    free(text);

    return 0;
}
