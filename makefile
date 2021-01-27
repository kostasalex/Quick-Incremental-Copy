EXECUTABLE = quic

CC = gcc
CFLAGS = -c -g -Wall

$(EXECUTABLE): main.o edit.o
	@echo " Compile quic ...";
	$(CC) -o $(EXECUTABLE) main.o edit.o
	
main.o: main.c edit.h
	$(CC) $(CFLAGS) main.c

school.o: edit.c edit.h
	$(CC) $(CFLAGS) edit.c
	
.phony: clean

clean:
	@echo " Cleaning . . ."
	rm -f *.o $(EXECUTABLE)
