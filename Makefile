
out_dir = out
src_dir = src

CC=gcc
CFLAGS=-I. -Wall -Wextra -Werror -Wpedantic -std=c99 -g -O3 -lm
DEPS = $(wildcard $(src_dir)/*.h)
SOURCE = $(wildcard $(src_dir)/*.c)
OBJ_temp = $(SOURCE:.c=.o)
OBJ = $(subst $(src_dir),$(out_dir),$(OBJ_temp))
exec = main

$(out_dir)/%.o: $(src_dir)/%.c $(DEPS)
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	$(CC) -c -o $@ $< $(CFLAGS)

$(exec): $(OBJ)
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	rm -f $(out_dir)/*
	rm -f $(exec)
	rm -f output output.ll output.o

.PHONY: clean

debug:
	if [ ! -d "DrMemory-Linux-2.5.0" ]; then \
		wget https://github.com/DynamoRIO/drmemory/releases/download/release_2.5.0/DrMemory-Linux-2.5.0.tar.gz ; \
	tar -xzf DrMemory-Linux-2.5.0.tar.gz ; \
		rm DrMemory-Linux-2.5.0.tar.gz ; \
	fi
	DrMemory-Linux-2.5.0/bin64/drmemory -- $(exec)

generate:
	./$(exec)
	llc -filetype=obj output.ll -o output.o
	clang output.o -o output