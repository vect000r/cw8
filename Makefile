FC = gcc
CFLAGS = -Wall -std=c99 -pedantic
LFLAGS = -Wall -std=c99 -pedantic
LINK = -lrt

NAME1 = client
NAME2 = server
LIB = queues

EXEC1 = $(NAME1).x
OBJS1 = $(NAME1).o

EXEC2 = $(NAME2).x
OBJS2 = $(NAME2).o

LIB1 = $(LIB).c


%.o: %.c
	$(FC) $(CFLAGS) -c $<

%.x: %.o
	$(FC) $(CFLAGS) -o $@ $^
	chmod +x $@

.PHONY: all
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(LIB1)
	$(FC) -o $@ $(LFLAGS) $^ $(LINK)
	chmod +x $@

$(EXEC2): $(OBJS2) $(LIB1)
	$(FC) -o $@ $(LFLAGS) $^ $(LINK)
	chmod +x $@

.PHONY: run-cw8
runclient: $(EXEC1)
	./$(EXEC1)

run-cw8: $(EXEC1) $(EXEC2)
	xterm -hold -title SERVER -bg red -fg black -e ./$(EXEC2) &
	xterm -hold -title CLIENT1 -bg green -fg black -e ./$(EXEC1) &
	xterm -hold -title CLIENT2 -bg green -fg black -e ./$(EXEC1) &
	xterm -hold -title CLIENT3 -bg green -fg black -e ./$(EXEC1) 

help:
	@echo "make run-cw7		Kompiluje i uruchamia programy powielacz.c i wykluczanie.c"
	@echo "make all			Kompiluje programy powielacz.c i wykluczanie.c"
	@echo "make clean  		Usuwa pliki .x, .o"

.PHONY: clean
clean:
	@-rm *.x *.o  