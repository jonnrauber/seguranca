/**
 * Algoritmo da Cifra de Substituição
 *
 * Autor: Jonathan Terhorst Rauber - Data: 13/09/2018
 * Universidade Federal da Fronteira Sul
 * Ciência da Computação
 * Segurança e Auditoria de Sistemas
 *
 * Para compilar:
 * gcc substitution.c -o subs -Wall -lm
 *
 * Para executar:
 * ./subs
 *
 * Descrição:
 * Inserir o texto claro que deseja criptografar e a chave.
 * Será exibido o texto criptografado.
 * Observação: somente caracteres em minúsculo são decodificados.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 9999

/*
 * Altera um caractere dada sua posição no alfabeto de origem e a correspondência
 * com o símbolo criptografado.
 * */
char change_character(char c_text, char* alphabet, char* new_alphabet) {
    int index_of_c_text = -1, i;
    for (i = 0; i < strlen(alphabet); i++) {
        if (c_text == alphabet[i]) {
            index_of_c_text = i;
            break;
        }
    }
    if (index_of_c_text == -1) return c_text;
    return new_alphabet[index_of_c_text];
}

/*
 * Retorna 1 se o caractere 'c' está contido no alfabeto 'alphabet'.
 * Retorna 0, caso contrário.
 * */
int in_alphabet(char c, char* alphabet) {
    int i;
    for (i = 0; i < strlen(alphabet); i++)
        if (c == alphabet[i])
            return 1;
    return 0;
}

/*
 * Imprime em tela a correspondência direta de substituição.
 * */
void print_alphabets(char* alphabet, char* new_alphabet) {
    int i;
    printf("\nAlfabeto de substituição:\n");
    for (i = 0; i < strlen(new_alphabet); i++)
        printf("%c ", new_alphabet[i]);
    printf("\n");
    for (i = 0; i < strlen(alphabet); i++)
        printf("^ ");
    printf("\n");
    for (i = 0; i < strlen(alphabet); i++)
        printf("%c ", alphabet[i]);
    printf("\n");
}

/*
 * Cifra de substituição.
 * Recebe um texto e uma chave e criptografa, fazendo a substituição caractere
 * por caractere.
 * */
char* substitution(char* text, char* key) {
    char alphabet[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\0'};
    char* new_alphabet = NULL;
    char* encrypted = NULL;
    int i, len = 0;

    for (i = 0; i < strlen(key); i++) {
        new_alphabet = (char *)realloc(new_alphabet, sizeof(char)*(++len));
        new_alphabet[len-1] = key[i];
    }
    new_alphabet = (char *)realloc(new_alphabet, sizeof(char)*(++len));
    new_alphabet[len-1] = '\0';

    while ((len - 1) < strlen(alphabet)) {
        for (i = 0; i < strlen(alphabet); i++) {
            if (! in_alphabet(alphabet[i], new_alphabet)) {
                if (new_alphabet[len-1] != '\0')
                    new_alphabet = (char *)realloc(new_alphabet, sizeof(char)*(++len));
                new_alphabet[len-1] = alphabet[i];
                break;
            }
        }
        for (i = strlen(alphabet)-1; i >= 0; i--) {
            if (! in_alphabet(alphabet[i], new_alphabet)) {
                if (new_alphabet[len-1] != '\0')
                    new_alphabet = (char *)realloc(new_alphabet, sizeof(char)*(++len));
                new_alphabet[len-1] = alphabet[i];
                break;
            }
        }
        new_alphabet = (char *)realloc(new_alphabet, sizeof(char)*(++len));
        new_alphabet[len-1] = '\0';
    }

    print_alphabets(alphabet, new_alphabet);

    encrypted = (char *)realloc(encrypted, sizeof(char)*strlen(text));
    for (i = 0; i < strlen(text); i++) {
        encrypted[i] = change_character(text[i], alphabet, new_alphabet);
    }

    free(new_alphabet);
    return encrypted;
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

    printf("--- Cifra de Substituição ---\n");

    //lê o texto claro da entrada padrão
    printf("Digite o texto a ser criptografado: ");
    read(cleartext);

    //lê a chave da entrada padrão
    printf("Digite a chave para a criptografia: ");
    read(key);

    //criptografa com a cifra de transposição
    text = substitution(cleartext, key);

    printf("\n-- Texto criptografado com a cifra de substituição: --\n");
    printf("%s\n", text);

    free(text);

    return 0;
}
