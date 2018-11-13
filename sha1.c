#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Algoritmo SHA-1
 * @author Jonathan Terhorst Rauber
 * @since 05/11/2018
 * Universidade Federal da Fronteira Sul - Campus Chapecó
 * 
 * Para compilar, basta executar o comando:
 * gcc -g -Wall -o sha1 sha1.c
 * ./sha1
 * 
 * Exemplos de hashes SHA-1:
 * "The quick brown fox jumps over the lazy dog" = 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12
 * "The quick brown fox jumps over the lazy cog" = de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3
 * "" = da39a3ee5e6b4b0d3255bfef95601890afd80709
 */

#define ROTACIONA_ESQUERDA(a, b) ((a << b) | (a >> (32 - b)))
#define SHA1_HASH_SIZE 20

typedef struct {
	unsigned char data[64];
	unsigned int datalen;
	unsigned long long bitlen;
	unsigned int h[5];
	unsigned int k[4];
} SHA1_STRUCT;

typedef struct {
	ssize_t len;
	int type;
	char *data;
} message;

/**
 *  Faz a transformação descrita pelo algoritmo SHA-1. 
 * */
void transforma_sha1(SHA1_STRUCT *sha1, unsigned char data[]) {
	unsigned int a, b, c, d, e, i, j, t, m[80];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) + (data[j + 1] << 16) + (data[j + 2] << 8) + (data[j + 3]);
	for ( ; i < 80; ++i) {
		m[i] = (m[i - 3] ^ m[i - 8] ^ m[i - 14] ^ m[i - 16]);
		m[i] = (m[i] << 1) | (m[i] >> 31);
	}

	a = sha1->h[0];
	b = sha1->h[1];
	c = sha1->h[2];
	d = sha1->h[3];
	e = sha1->h[4];

	for (i = 0; i < 20; ++i) {
		t = ROTACIONA_ESQUERDA(a, 5) + ((b & c) ^ (~b & d)) + e + sha1->k[0] + m[i];
		e = d;
		d = c;
		c = ROTACIONA_ESQUERDA(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 40; ++i) {
		t = ROTACIONA_ESQUERDA(a, 5) + (b ^ c ^ d) + e + sha1->k[1] + m[i];
		e = d;
		d = c;
		c = ROTACIONA_ESQUERDA(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 60; ++i) {
		t = ROTACIONA_ESQUERDA(a, 5) + ((b & c) ^ (b & d) ^ (c & d))  + e + sha1->k[2] + m[i];
		e = d;
		d = c;
		c = ROTACIONA_ESQUERDA(b, 30);
		b = a;
		a = t;
	}
	for ( ; i < 80; ++i) {
		t = ROTACIONA_ESQUERDA(a, 5) + (b ^ c ^ d) + e + sha1->k[3] + m[i];
		e = d;
		d = c;
		c = ROTACIONA_ESQUERDA(b, 30);
		b = a;
		a = t;
	}

	sha1->h[0] += a;
	sha1->h[1] += b;
	sha1->h[2] += c;
	sha1->h[3] += d;
	sha1->h[4] += e;
}

void init_sha1(SHA1_STRUCT *sha1) {
	sha1->datalen = 0;
	sha1->bitlen = 0;
	//inicializa variáveis de estado
	sha1->h[0] = 0x67452301;
	sha1->h[1] = 0xEFCDAB89;
	sha1->h[2] = 0x98BADCFE;
	sha1->h[3] = 0x10325476;
	sha1->h[4] = 0xC3D2E1F0;
	//inicializa key
	sha1->k[0] = 0x5A827999;
	sha1->k[1] = 0x6ED9EBA1;
	sha1->k[2] = 0x8F1BBCDC;
	sha1->k[3] = 0xCA62C1D6;
}

/**
 * Divide a mensagem de entrada em 'pedaços' de 512 bits.
 * */
void divide_sha1(SHA1_STRUCT *sha1, unsigned char data[], size_t len) {
	size_t i;
	
	for (i = 0; i < len; i++) {
		sha1->data[sha1->datalen++] = data[i];
		if (sha1->datalen == 64) {
			transforma_sha1(sha1, sha1->data);
			sha1->bitlen += 512;
			sha1->datalen = 0;
		}
	}
}

/**
 * Faz as adições necessárias para gerar o hash SHA-1.
 * Adiciona 0x80's e 0x00's. Apresenta o hash em big endian.
 * */
void gera_hash_sha1(SHA1_STRUCT *sha1, unsigned char hash[]) {
	unsigned int i;
	
	i = sha1->datalen;
	
	if (sha1->datalen < 56) {
		sha1->data[i++] = 0x80;
		while (i < 56)
			sha1->data[i++] = 0x00;
			
	} else {
		sha1->data[i++] = 0x80;
		while (i < 64) 
			sha1->data[i++] = 0x00;
		transforma_sha1(sha1, sha1->data);
		memset(sha1->data, 0, 56);
	}
	
	//adiciona o comprimento da mensagem e transforma
	sha1->bitlen += sha1->datalen * 8;
	for (short j = 63, k = 0; j >= 56; j--, k += 8)
		sha1->data[j] = sha1->bitlen >> k;
	transforma_sha1(sha1, sha1->data);
	
	//converte pra big endian
	for (i = 0; i < 4; ++i) {
		hash[i]    = (sha1->h[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i+4]  = (sha1->h[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i+8]  = (sha1->h[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i+12] = (sha1->h[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i+16] = (sha1->h[4] >> (24 - i * 8)) & 0x000000ff;
	}
}

/**
 * Recebe uma mensagem e executa o SHA-1, gerando o hash correspondente.
 * */
void executa_sha1(char message[]) {
	unsigned char *message_bytes;
	SHA1_STRUCT sha1;
	unsigned char buffer[SHA1_HASH_SIZE];
	
	message_bytes = malloc(strlen(message) * sizeof(unsigned char));
	strncpy((char *)message_bytes, message, strlen(message));
	
	for(int i=0; i<20; i++) printf("-");
	printf("\n");
	
	printf("Mensagem: \"%s\"\n", message);
	
	//inicializa estrutura para o algoritmo sha1
	init_sha1(&sha1);
	
	//divide a mensagem em pedaços de 512 bits (64 bytes)
	divide_sha1(&sha1, message_bytes, strlen(message));

	//gera o hash SHA1
	gera_hash_sha1(&sha1, buffer);
	
	printf("Hash produzido: ");
	for (int i = 0; i < SHA1_HASH_SIZE; i++) {
		printf("%02x", buffer[i]);
	}
	printf("\n");
	
	for(int i=0; i<20; i++) printf("-");
	printf("\n");
	
	free(message_bytes);
}

/**
 * Main para testes. O loop while(1) recebe frases do usuário.
 * */
int main (int argc, char* args[]) {
	char message1[] = "The quick brown fox jumps over the lazy dog";
	char message2[] = "The quick brown fox jumps over the lazy cog";
	char message3[] = "";
	
	printf("GERADOR DE HASH SHA-1 ---- por Jonathan Terhorst Rauber (UFFS-Chapecó)\n\n");
	printf("Alguns exemplos de frases e hashes:\n");
	
	executa_sha1(message1);
	executa_sha1(message2);
	executa_sha1(message3);	
	
	printf("\n");
	
	while(1) {
		message *m;
		m = malloc(sizeof(message));
		if (!m) { perror("malloc"); exit(EXIT_FAILURE); };
		m->len = 0;
		m->type = 6;
		m->data = NULL;
		printf("Escreva a mensagem para gerar o hash: ");
		fflush(stdout);
		size_t message_size = 0;
		m->len = getline(&m->data, &message_size, stdin);
		
		m->data[strlen(m->data)-1] = '\0';
		
		executa_sha1(m->data);
		
		free(m);
	}
	
	return 0;
}
