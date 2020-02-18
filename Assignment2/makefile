CC=gcc
FLAGS=-l./-O3 -Wall -c
CFILES:=$(shell ls | grep .c)
OBJS:=$(CFILES:%.c=%.o)
all:$(OBJS)
	$(CC) $(OBJS) -o mykernel
$(OBJS):$(CFILES)
	$(CC) $(FLAGS) $(CFILES)

clear:
	rm *.o