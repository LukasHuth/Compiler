
out_dir = out
src_dir = src

CC=gcc
CFLAGS=-I.
DEPS = $(wildcard $(src_dir)/*.h)
SOURCE = $(wildcard $(src_dir)/*.c)
OBJ_temp = $(SOURCE:.c=.o)
OBJ = $(subst $(src_dir),$(out_dir),$(OBJ_temp))
exec = main

$(out_dir)/%.o: $(src_dir)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(exec): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(out_dir)/* $(exec)