CC = g++

FLAGS = -g 

FILES = main.o Utility.o error.o TaskQueue.o Task.o Process.o ProcessPool.o

main: $(FILES)
	$(CC) $(FLAGS) $(FILES) -o main -lpthread

Utility.o: Utility.h  Utility.cpp error.o
	$(CC) -c $(FLAGS) Utility.cpp

error.o: error.cpp apue.h
	$(CC) -c $(FLAGS) error.cpp

main.o: Utility.o TaskQueue.o main.cpp
	$(CC) -c $(FLAGS) main.cpp

TaskQueue.o: TaskQueue.h TaskQueue.cpp
	$(CC) -c $(FLAGS) TaskQueue.cpp

Task.o: Task.h Task.cpp Utility.o TaskQueue.o
	$(CC) -c $(FLAGS) Task.cpp

Process.o: Process.h Process.cpp TaskQueue.o Task.o Utility.o
	$(CC) -c $(FLAGS) Process.cpp

ProcessPool.o: ProcessPool.h ProcessPool.cpp Process.o Utility.o
	$(CC) -c $(FLAGS) ProcessPool.cpp

clean:
	-rm $(FILES) main
