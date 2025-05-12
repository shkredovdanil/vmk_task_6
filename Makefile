X_FILE=main

compile:
	@gcc -m32 -std=c99 -c main.c -o main.o
	@nasm -f elf32 f1.asm -o f1.o
	@nasm -f elf32 f2.asm -o f2.o
	@nasm -f elf32 f3.asm -o f3.o
	@gcc -m32 main.o f1.o f2.o f3.o -o $(X_FILE) -lm

run: compile
	@./$(X_FILE)

all: run

clean:
	@rm *.o $(X_FILE) 2>/dev/null || true