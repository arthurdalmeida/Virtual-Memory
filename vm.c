#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 4){
        fprintf(stderr, "A entrada deve ser: ./vm <arq> <fifo|lru> <fifo|lru>\n");
        exit(1);
    }

    if (strcmp(argv[2], "fifo") != 0 && strcmp(argv[2], "lru") != 0){
        fprintf(stderr, "Esse algoritmo de pagina e invalido\n");
        exit(1);        
    }

    if (strcmp(argv[3], "fifo") != 0 && strcmp(argv[3], "lru") != 0){
        fprintf(stderr, "Esse algoritmo da TLB e invalido\n");
        exit(1);        
    }

    FILE *arqAddresses = fopen(argv[1], "r");

    if (arqAddresses == NULL){
        fprintf(stderr, "o arquivo %s nao abriu\n", argv[1]);
        exit(1);
    }

    FILE *arqBacking = fopen("BACKING_STORE.bin", "rb");

    if (arqBacking == NULL){
        fprintf(stderr, "o BACKING_STORE.bin nao abriu\n");
        exit(1);
    }

    FILE *arqSaida = fopen("correct.txt", "w");

    if (arqSaida == NULL){
        fprintf(stderr, "o correct.txt nao abriu\n");
        exit(1);
    }

    return 0;
}