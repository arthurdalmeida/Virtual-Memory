#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    int tabelaPag[256];
    signed char memoria[128][256];
    int proxFrame = 0;
    int tempoAtual = 0;
    int responsavelFrame[128];
    int horarioDaChegada[128];

    int achaVitima(int tempos[], int tamanho){
        int vitima = 0;

        for (int i=0; i<tamanho; i++){
            if (tempos[i] < tempos[vitima]){
                vitima = i;
            }
        }

        return vitima;
    }

    int loadPagina(int pagina, FILE *backing){
        int destino = 0;
        if (proxFrame < 128){
            destino = proxFrame++;
        }
        else{
            destino = achaVitima(horarioDaChegada, 128);
            tabelaPag[responsavelFrame[destino]] = -1;
        }

        fseek(backing, pagina*256, SEEK_SET);
        fread(memoria[destino], 1, 256, backing);
        tabelaPag[pagina] = destino;
        responsavelFrame[destino] = pagina;
        horarioDaChegada[destino] = tempoAtual;

        return destino;
    }

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

    int endereco = 0;

    
    for (int i=0; i<256; i++){
        tabelaPag[i] = -1;
    }

    int qtdFaults = 0;
    int qtdLidos = 0;

    while (fscanf(arqAddresses, "%d", &endereco) == 1){
        if (endereco > 65535 || endereco < 0){
            fprintf(stderr, "o endereco %d está fora do intervalo de 0 a 65535\n", endereco);
            exit(1);
        }

        tempoAtual++;

        int pagina = endereco / 256;
        int offset = endereco % 256;
        
        int frame = tabelaPag[pagina];

        if (tabelaPag[pagina] == -1){
            qtdFaults++;
            frame = loadPagina(pagina, arqBacking);
        }

        int conteudo = memoria[frame][offset];
        int endFisico = frame*256 + offset;
        
        fprintf(arqSaida, "Virtual address: %d TLB: %d Physical address: %d Value: %d\n", endereco, 0, endFisico, conteudo);
        qtdLidos++;
    }
    
    if (!feof(arqAddresses)){
        fprintf(stderr, "o arquivo %s esta com formato invalido\n", argv[1]);
        exit(1);
    }

    if (qtdLidos == 0){
        fprintf(stderr, "%s nao tem nenhum endereco\n", argv[1]);
        exit(1);
    }

    fprintf(arqSaida, "Number of Translated Addresses = %d\n", qtdLidos);
    fprintf(arqSaida, "Page Faults = %d\n", qtdFaults);
    fprintf(arqSaida, "Page Fault Rate = %.3f\n", (double) qtdFaults/qtdLidos);
    fprintf(arqSaida, "TLB Hits = %d\n", 0);
    fprintf(arqSaida, "TLB Hit Rate = %.3f\n", 0.0);

    fclose(arqAddresses);
    fclose(arqBacking);
    fclose(arqSaida);

    return 0;
}