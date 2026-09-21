all: todos

todos: main.c
	gcc main.c -o todos && ./todos

install: todos
	cp todos ~/.local/bin/todos

clean:
	rm -rf *.out todos

