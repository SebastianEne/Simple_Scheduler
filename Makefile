CC 		= gcc
C_SRCS 	= $(wildcard src/*.c)
OBJ 	= $(patsubst src/%.c, obj/%.o, $(C_SRCS))
INCLUDES= include/
CFLAGS 	= -g -Wall
LDFLAGS = -lpthread

sched: $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o sched $(LDFLAGS)

obj/%.o: src/%.c
		$(CC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm obj/*.o sched

.PHONY: clean
