TARGET=mocks
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET)/mock_bind.o \
			../$(BINDIR)/$(TARGET)/mock_calloc.o \
			../$(BINDIR)/$(TARGET)/mock_fcntl.o \
			../$(BINDIR)/$(TARGET)/mock_if_nametoindex.o \
			../$(BINDIR)/$(TARGET)/mock_socket.o 
WRAPS=-Wl,--wrap=fcntl,--wrap=socket,--wrap=bind,--wrap=if_nametoindex,--wrap=calloc
CXX=clang++
CC=clang

# TODO: debug
export BINDIR ?= bin

.PHONY: all clean install

bin:
	@echo " Creating build directory"
	mkdir -p ../$(BINDIR)/$(TARGET)

all: $(OBJS) | bin

../$(BINDIR)/$(TARGET)/%.o: $(SRCDIR)/%.c | bin
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR) -I ../deps/minunit

clean: 
	@echo " RM	../$(BINDIR)/$(TARGET)/*"
	rm -rfv ../$(BINDIR)/$(TARGET)/*
