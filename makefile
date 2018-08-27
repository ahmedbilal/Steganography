IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=lib
LIBS=-lm -lpng
_DEPS = steganography.h image_handler.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o steganography.o image_handler.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hidden: $(OBJ)
	$(CC) -g3 -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ hidden