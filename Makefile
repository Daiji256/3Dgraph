CC = gcc
CFLAGS = -lglut -lGLU -lGL -lm -Wall

.c.o:
	$(CC) $< -c -o $@ $(CFLAGS)

3Dgraph.out: main.o eval.o wbmp.o
	$(CC) $^ -o 3Dgraph.out $(CFLAGS)

targets: 3Dgraph.out

clean:
	rm -f *.o *.out

all: clean targets

main.o: main.c eval.h wbmp.h
eval.o: eval.c eval.h
wbmp.o: wbmp.c wbmp.h
