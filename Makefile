OBJS = scheduling.o io.o glpk.o

LIBS = -lglpk -lm

CFLAGS = -Wall -Wextra -Werror -O2 -Wno-strict-aliasing
CC = gcc

scheduling: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o scheduling $(LIBS)

$(OBJS): %.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o scheduling scheduling.lp *~
