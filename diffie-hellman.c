/**
 * Algoritmo de Troca de Chaves por Diffie e Hellman
 * 
 * Autor: Jonathan Terhorst Rauber - Data: 06/09/2018
 * Universidade Federal da Fronteira Sul
 * Ciência da Computação
 * Segurança e Auditoria de Sistemas
 * 
 * Para compilar:
 * gcc diffie-hellman.c -o dh -Wall -lm
 * 
 * Para executar:
 * ./dh
 * 
 * Descrição:
 * Inserir os valores de A, B, X e Y quando forem pedidos.
 * Será impressa a chave PK calculada por A e por B.
 * */

#include <stdio.h>
#include <math.h>

typedef unsigned int uint;

/*
 * Função que calcula a potência base^(exp)
 */
uint power(uint base, uint exp) {
    uint i;
    uint result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}

int main (void) {    
    uint A, B, X, Y, Ra, Rb, PK_A, PK_B;
    
    //recebe a entrada
    printf("--- Troca de Chaves por Diffie e Hellman ---\n");
    printf("A: ");
    scanf("%u", &A);
    printf("B: ");
    scanf("%u", &B);
    printf("X: ");
    scanf("%u", &X);
    printf("Y: ");
    scanf("%u", &Y);
    
    //calcula os restos das divisões
    Ra = power(A, X) % B;
    Rb = power(A, Y) % B;
    
    //calcula a chave em ambos
    PK_A = power(Rb, X) % B;
    PK_B = power(Ra, Y) % B;
    
    printf("PK_A = %u, PK_B = %u\n", PK_A, PK_B);
    
    return 0;
}
