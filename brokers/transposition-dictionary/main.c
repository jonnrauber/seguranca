/**

@author Jonathan Terhorst Rauber - Universidade Federal da Fronteira Sul
@since 29/11/2018

# Descrição:
Algoritmo de quebra da cifra de transposição por dicionário.
O dicionário está presente no arquivo dicionario.dat.
Para mudar a mensagem de entrada, alterar o conteúdo da variável msg[],
no método main.

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 255
#define DIC_SIZE 10

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
    char* encrypted = NULL;

    for (i = 0; i < qt_lines; i++) {
        for (j = 0; j < strlen(key); j++) {
            if (k < strlen(text))
                table[i][j] = text[k++];
            else
                table[i][j] = ' ';
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

char* read_file(FILE *file) {
  char* buffer = 0;
	int length;

	if (file == NULL) {
		printf("Erro ao abrir arquivo.\n");
		exit(0);
	} else {
		fseek(file, 0, SEEK_END);
		length = ftell(file); //pega o tamanho do arquivo
		fseek(file, 0, SEEK_SET);
		buffer = malloc(length); //alloca o buffer para a string
		if (buffer) {
			fread(buffer, 1, length, file); //lê o arquivo para a string
		}
		fclose(file);
	}
	buffer[length] = '\0'; //insere fim de string
  return buffer;
}

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

void transposition_breaker(char* msg, char* encrypt) {
	char* dictionary[DIC_SIZE];
	char* aux;
	char* sdecrypt = (char *)malloc(strlen(encrypt));
	strcpy(sdecrypt, encrypt);
	int i, j, k;
	char* token;
  char* msg_wo_spaces = remove_spaces(msg);

	FILE * dic = fopen("dicionario.dat", "r");

	if (dic == NULL) {
		printf("Erro ao abrir arquivo dicionario.dat!\n");
		exit(0);
	} else {
		aux = read_file(dic);
		token = strtok(aux, " ");
    for (i = 0; token != NULL; i++) {
			dictionary[i] = token;
      if (dictionary[i][strlen(dictionary[i])-1] == '\n')
        dictionary[i][strlen(dictionary[i])-1] = '\0';
			token = strtok(NULL, " ");
    }
    int usable_size = i; //in the dictionary

		int qt_lines; //nuber of lines that de matrix of should have
		int auxi = 0;
		int l;
		int* keypos;

    for (l = 0; l < usable_size; l++) { //we should use the words on dictionary as possible keys and try that key to decrypt the message
      printf("Tentativa %d/%d - chave: %s\n", l+1, usable_size, dictionary[l]);
    	qt_lines = (int) ceil( (double)strlen(encrypt) / strlen(dictionary[l])); //number of lines that de matrix of should have
    	keypos = key_order(dictionary[l]);
    	auxi = 0;
    	for (i = 0; i < strlen(dictionary[l]); i++) { //key ordenately
				j = keypos[i];
				for (k = 0; k < qt_lines; k++) {
					sdecrypt[j + (strlen(dictionary[l])*k)] = encrypt[auxi];
					auxi++;
				}
			}
			sdecrypt[strlen(encrypt)] = '\0';
			printf("Mensagem decriptada: %s\n", sdecrypt);
      if (strcmp(msg_wo_spaces, remove_spaces(sdecrypt)) == 0) {
        printf("SUCESSO NA QUEBRA! CHAVE = %s\n", dictionary[l]);
        return;
	    }
    }
	}
  printf("O PROCESSO DE QUEBRA ENCERROU SEM SUCESSO.\n");
	return;
}

int main (void) {
	char msg[] = {"A CHAPECOENSE ESTA EM DECIMO LUGAR NO CAMPEONATO"};
	char auxkey[MAX_LEN] = "";
	char* encrypt;

  printf("-- QUEBRADOR DA CIFRA DE TRANSPOSIÇÃO POR DICIONÁRIO --\n\n");

  printf("Mensagem a ser criptografada:\n%s\n\n", msg);
	printf("Escreva a chave de criptografia: ");
	fgets(auxkey, MAX_LEN, stdin);
	char* key = strtok(auxkey, "\n");

  //criptografa com a cifra de transposição
  encrypt = transposition_encrypt(msg, key);

  printf("\nTexto criptografado:\n%s\n", encrypt);

  printf("\n## Início da quebra por dicionário para descobrir a chave ##\n");
	transposition_breaker(msg, encrypt);

	return 0;
}
