# Virtual-Memory

- É um simulador de memória virtual
- Implementado na linguagem C

- Feito por aao@cesar.school

## Compilar

    make

## Executar

    ./vm addresses.txt <fifo|lru> <fifo|lru>

- O segundo argumento é para substituição de página
- O terceiro argumento é para substituição da TLB
- `BACKING_STORE.bin` precisa estar na mesma pasta
- Saída gravada em `correct.txt`

## Limpar

    make clean