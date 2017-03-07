CC 		= gcc
C_SRCS 	= $(wildcard src/*.c)
OBJ 	= $(patsubst src/%.c, obj/%.o, $(C_SRCS))
INCLUDES= include/
CFLAGS 	= -g -Wall

sched: $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o sched

obj/%.o: src/%.c
		$(CC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm obj/*.o sched

.PHONY: clean
