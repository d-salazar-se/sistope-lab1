all:
	gcc ./src/lab1.c ./src/lab.c -o lab1.o -Wall
# 	gcc ./src/vis.c -o vis.o -Wall

clean:
	-rm -f lab1.o

run: 
	./lab1.o