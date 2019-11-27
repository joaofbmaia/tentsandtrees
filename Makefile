#
# In order to execute this "Makefile" just type "make"
#

OBJS	= main.o io.o map.o solver.o
SOURCE	= main.c io.c map.c solver.c
HEADER	= io.h map.h solver.h
OUT	= tentsandtrees
TESTFILE = testfiles/enunciado01.camp
CC	 = gcc
FLAGS	 = -g3 -c -Wall
LFLAGS	 = 
# -g option enables debugging mode 
# -c flag generates object code for separate files


all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)


# create/compile the individual files >>separately<<
main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

io.o: io.c
	$(CC) $(FLAGS) io.c -std=c99

map.o: map.c
	$(CC) $(FLAGS) map.c -std=c99

solver.o: solver.c
	$(CC) $(FLAGS) solver.c -std=c99


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# run the program
run: $(OUT) $(TESTFILE)
	./$(OUT) $(TESTFILE)

# compile program with debugging information
debug: $(OUT) $(TESTFILE)
	valgrind ./$(OUT) $(TESTFILE)

# run program with valgrind for errors
valgrind: $(OUT) $(TESTFILE)
	valgrind ./$(OUT) $(TESTFILE)

# run program with valgrind for leak checks
valgrind_leakcheck: $(OUT) $(TESTFILE)
	valgrind --leak-check=full ./$(OUT) $(TESTFILE)

# run program with valgrind for leak checks (extreme)
valgrind_extreme: $(OUT) $(TESTFILE)
	valgrind --leak-check=full --show-leak-kinds=all --leak-resolution=high --track-origins=yes --vgdb=yes ./$(OUT) $(TESTFILE)