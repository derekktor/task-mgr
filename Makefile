all: todos

todos: main.c
	gcc main.c -o todos && ./todos

install: todos
	cp tasks /.local/bin/todos

clean:
	rm -rf *.out todos

