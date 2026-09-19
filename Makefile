all: todos

todos: main.c
	gcc main.c -o todos && ./todos

clean:
	rm -rf *.out todos

