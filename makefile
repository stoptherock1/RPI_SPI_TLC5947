all: clean
	gcc tlc5947_controller.c -Wall -o exeTab

clean:
	rm -f exeTab