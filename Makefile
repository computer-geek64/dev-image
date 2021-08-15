# Makefile
# Usage:
# make          # Compile binaries
# make clean    # Remove all binaries and objects

CC=cc


all: setup

setup: setup.o python_setup.o apt.o
	$(CC) $? -o $@

setup.o: src/setup.c src/setup.h
	$(CC) -c $< -o $@

python_setup.o: src/python_setup.c src/python_setup.h
	$(CC) -c $< -o $@

apt.o: src/apt.c src/apt.h
	$(CC) -c $< -o $@

clean:
	@rm -rvf setup setup.o python_setup.o apt.o

