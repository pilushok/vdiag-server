TARGET=uds_server
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET)/uds-server.o \
		 ../$(BINDIR)/$(TARGET)/can.o \
		 ../$(BINDIR)/$(TARGET)/uds.o
CXX=clang++
CC=clang

# TODO: debug
export BINDIR ?= bin

.PHONY: all clean install test

all: ../$(BINDIR)/$(TARGET)/$(TARGET)
	
$(BINDIR):
	@echo " Creating build directory"
	mkdir -p ../$(BINDIR)/$(TARGET)

../$(BINDIR)/$(TARGET)/$(TARGET): $(OBJS) | $(BINDIR)
	@echo " LD	uds_server"
	$(CC) -o $@ $^

../$(BINDIR)/$(TARGET)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR)

test:
	$(CC) -Wall -Wextra -Wl,--wrap=fcntl,--wrap=calloc -D_GNU_SOURCE can.c ../src/can.c -I ../inc -I ../../deps/minunit -o test_can_socke	

clean: 
	@echo " RM	../$(BINDIR)/$(TARGET)/*"
	rm -f ../$(BINDIR)/$(TARGET)/*
