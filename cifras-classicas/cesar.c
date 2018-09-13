/**
 * Algoritmo da Cifra de Cesar
 * 
 * Autor: Jonathan Terhorst Rauber - Data: 13/09/2018
 * Universidade Federal da Fronteira Sul
 * Ciência da Computação
 * Segurança e Auditoria de Sistemas
 * 
 * Para compilar:
 * gcc cesar.c -o cesar -Wall -lm
 * 
 * Para executar:
 * ./cesar
 * 
 * Descrição:
 * Inserir o texto claro que deseja criptografar.
 * Será exibido o texto criptografado.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 9999
#define SHIFT 3

enum Operation {Encrypt, Decrypt};

char* caesar(char* text, unsigned int shift, enum Operation op) {
    char* encrypted = NULL;
    char c;
    int i;
    
    if (op == Decrypt) shift = 0 - shift;
    
    for (i = 0; i < strlen(text); i++) {
        encrypted = (char *)realloc(encrypted, sizeof(char)*(i+1));
        c = text[i];
        
        if (c >= 'a' && c <= 'z') {
            encrypted[i] = ((c - 'a') + shift) % 26 + 'a';
            
        } else if (c >= 'A' && c <= 'Z') {
            encrypted[i] = ((c - 'A') + shift) % 26 + 'A';
            
        } else if (c == ' ' && op == Encrypt) {
            encrypted[i] = '#';
        } else if (c == '#' && op == Decrypt) {
            encrypted[i] = ' ';
        }
    }
    encrypted = (char *)realloc(encrypted, sizeof(char)*(i+1));
    encrypted[i++] = '\0';
    return encrypted;
}

void read(char* string) {
    fgets(string, MAX_LEN, stdin);
    string[strlen(string)-1] = '\0';
}

int main(void) {
    char* text = NULL;
    char cleartext[MAX_LEN];
    
    printf("--- Cifra de César ---\n");
    
    //lê a entrada do texto claro da entrada padrão
    printf("Digite o texto a ser criptografado: \n");
    read(cleartext);
    
    //criptografa com a cifra de cesar
    text = caesar(cleartext, SHIFT, Encrypt);
    
    printf("\nTexto criptografado em cifra de César: \n");
    printf("%s\n", text);
    
    //descriptografa novamente
    text = caesar(text, SHIFT, Decrypt);
    
    printf("\nTexto novamente descriptografado: \n");
    printf("%s\n", text);
    
    free(text);
    
    return 0;
}
