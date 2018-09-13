/**
 * Algoritmo da Cifra de Vigenere
 *
 * Autor: Jonathan Terhorst Rauber - Data: 13/09/2018
 * Universidade Federal da Fronteira Sul
 * Ciência da Computação
 * Segurança e Auditoria de Sistemas
 *
 * Para compilar:
 * gcc vigenere.c -o vig -Wall -lm
 *
 * Para executar:
 * ./vig
 *
 * Descrição:
 * Inserir o texto claro que deseja criptografar e a chave.
 * Será exibido o texto criptografado.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 9999

/*
 * Retorna 1 caso o caractere seja minúsculo, 0 caso seja maiúsculo e -1 caso deseja
 * outro caractere não contido no alfabeto.
 * */
int is_lower_case(char c) {
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 0;
    return -1;
}

/*
 * Usa a "tabula recta", ou quadrado de Vigenere para substituir um caractere no
 * texto e produzir a criptografia.
 * */
char change_character(char c_text, char c_key, int* key_pos) {
  if (is_lower_case(c_text) == -1)
        return c_text;
    *key_pos += 1;
    if (is_lower_case(c_text) && is_lower_case(c_key))
        return ((c_text - 'a') + (c_key - 'a')) % 26 + 'a';
    if (! is_lower_case(c_text) && ! is_lower_case(c_key))
        return ((c_text - 'A') + (c_key - 'A')) % 26 + 'A';
    if (is_lower_case(c_text) && ! is_lower_case(c_key))
        return ((c_text - 'a') + (c_key - 'A')) % 26 + 'a';
    if (! is_lower_case(c_text) && is_lower_case(c_key))
        return ((c_text - 'A') + (c_key - 'a')) % 26 + 'A';
}

/*
 * Criptografa com a cifra de Vigenere.
 * Recebe um texto claro e uma chave e retorna o texto criptografado.
 * */
char* vigenere(char* text, char* key) {
    char* encrypted = NULL;
    int i, j;

    for (i = 0, j = 0; i < strlen(text); i++) {
        encrypted = (char *)realloc(encrypted, sizeof(char)*(i+1));
        encrypted[i] = change_character(text[i], key[j % strlen(key)], &j);
    }
    encrypted = (char *)realloc(encrypted, sizeof(char)*(i+1));
    encrypted[i] = '\0';
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

    printf("--- Cifra de Vigenere ---\n");

    //lê o texto claro da entrada padrão
    printf("Digite o texto a ser criptografado: ");
    read(cleartext);

    //lê a chave da entrada padrão
    printf("Digite a chave para a criptografia: ");
    read(key);

    //criptografa com a cifra de vigenere
    text = vigenere(cleartext, key);

    printf("-- Texto criptografado em cifra de Vigenere: --\n");
    printf("%s\n", text);

    free(text);

    return 0;
}
