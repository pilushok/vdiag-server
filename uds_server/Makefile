TARGET=uds_server
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET)/uds-server.o \
		 ../$(BINDIR)/$(TARGET)/can.o \
		 ../$(BINDIR)/$(TARGET)/uds.o
TESTS_DIR=tests
TESTS_OBJS=../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/can.o
WRAPS=-Wl,--wrap=fcntl,--wrap=socket,--wrap=bind,--wrap=if_nametoindex,--wrap=calloc
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
	$(CC) -Wall -Wextra $(WRAPS) -D_GNU_SOURCE $(TESTS_DIR)/can.c $(TESTS_DIR)/mock_calloc.c $(TESTS_DIR)/mock_socket.c $(TESTS_DIR)/mock_bind.c $(TESTS_DIR)/mock_if_nametoindex.c $(TESTS_DIR)/mock_fcntl.c src/can.c -I tests -I inc -I ../deps/minunit -o ../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/test_can_socket	

clean: 
	@echo " RM	../$(BINDIR)/$(TARGET)/*"
	rm -f ../$(BINDIR)/$(TARGET)/*
