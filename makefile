all: clean
	gcc tab.c -Wall -o exeTab

clean:
	rm -f exeTab