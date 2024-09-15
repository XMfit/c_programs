all: bin/main

bin/main: src/main.c src/shell_interactive.c src/exec_args.c src/built_in_proc.c src/new_proc.c
	gcc -o $@ $^

clean:
	rm -rf bin/*
