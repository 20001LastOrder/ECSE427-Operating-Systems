CC=gcc
CFILES:=$(wildcard  c/*.c)
OBJS:=$(CFILES:c/%.c=%.o)

INCLUDEPATH = -Iheader
FLAGS=$(INCLUDEPATH) -l./-O3 -Wall -c -g

all:$(OBJS)
	$(CC) $(OBJS) -o mykernel
$(OBJS):$(CFILES)
	$(CC) $(FLAGS) $(CFILES)
debug:$(OBJS)
	$(CC) $(OBJS) -o mykernel
$(OBJS):$(CFILES)
	$(CC) $(FLAGS) $(CFILES)

clear:
	rm *.o