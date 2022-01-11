default:
	gcc Q1_a.c -o Q1_a
	gcc Q1_b.c -o Q1_b -lpthread
preprocess:
	gcc -E Q1_a.c -o Q1_a.i
	gcc -E Q1_b.c -o Q1_b.i -lpthread
compile:
	gcc -S Q1_a.i -o Q1_a.s
	gcc -S Q1_b.i -o Q1_b.s -lpthread
assemble:
	gcc -c Q1_a.s -o Q1_a.o
	gcc -c Q1_b.s -o Q1_b.o -lpthread
link:
	gcc Q1_a.o -o Q1_a
	gcc Q1_b.o -o Q1_b -lpthread
run:
	./Q1_a
	./Q1_b