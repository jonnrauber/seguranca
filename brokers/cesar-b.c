#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_LEN 100

int main(void) {
	short chave, i, j, pos;
	char message[] = {"ROD PXQGR"};
	char alfabeto[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
	char traducao[MAX_MSG_LEN];
	
	for (chave = 0; chave < strlen(alfabeto); chave++) {
		//reseta a traducao
		memset(traducao, '\0', MAX_MSG_LEN);
		
		//itera sobre a mensagem
		for (i = 0; i < strlen(message); i++) {
			//encontra a posição do símbolo no alfabeto
			pos = -1;
			for (j = 0; j < strlen(alfabeto); j++) {
				if (message[i] == alfabeto[j]) {
					pos = j;
					break;
				}
			}
			if (pos == -1) {
				//se não é uma letra do alfabeto, só adiciona o simbolo na tradução
				traducao[i] = message[i];
			} else {
				pos = pos - chave; //desloca o valor da chave à esquerda no alfabeto
				if (pos < 0)
					pos = pos + strlen(alfabeto); //trata se der underflow
				traducao[i] = alfabeto[pos];
			}
		}
		printf("Chave=%d: %s\n", chave, traducao);
	}
		
	
	
	return 0;
}
