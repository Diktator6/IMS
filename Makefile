CC=g++
FLAGS = -std=c++11 -Wall -Wextra -pedantic -L. -lsimlib -lm
PROJ_NAME=ims

all:
	$(CC) $(FLAGS) *.cpp -o $(PROJ_NAME)

run:
	./ims

clean:
	rm $(PROJ_NAME)
