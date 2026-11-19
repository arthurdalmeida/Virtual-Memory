CC = cc
CFLAGS = -Wall

vm: vm.c
	$(CC) $(CFLAGS) -o vm vm.c -pthread

clean:
	rm -f vm

.PHONY: clean