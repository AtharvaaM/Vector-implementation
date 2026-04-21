all: main_memory main_other main_int

main1: main_memory.c vectors.c
	gcc main_memory.c vectors.c -o main1

main2: main_int_functions.c vectors.c
	gcc main_int_functions.c vectors.c -o main2

main3: main_other_functions.c vectors.c
	gcc main_other_functions.c vectors.c -o main3

build:
	gcc $(SRC) vectors.c -o $(OUT)

clean: 
	rm -f main_memory main_other