TARGET=uds_server
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET)/uds-server.o \
		 ../$(BINDIR)/$(TARGET)/can.o \
		 ../$(BINDIR)/$(TARGET)/uds.o
TESTS_DIR=tests
TESTS_SRCDIR=src
TESTS_OBJS=../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/mock_bind.o \
			../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/mock_calloc.o \
			../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/mock_fcntl.o \
			../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/mock_if_nametoindex.o \
			../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/mock_socket.o \
			../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/test_entrypoint.o
WRAPS=-Wl,--wrap=fcntl,--wrap=socket,--wrap=bind,--wrap=if_nametoindex,--wrap=calloc
CXX=clang++
CC=clang

# TODO: debug
export BINDIR ?= bin

.PHONY: all clean install test

all: ../$(BINDIR)/$(TARGET)/$(TARGET) test
	
bin:
	@echo " Creating build directory"
	mkdir -p ../$(BINDIR)/$(TARGET)

tests_dir:
	@echo " Creating tests directory"
	mkdir -p ../$(BINDIR)/$(TARGET)/$(TESTS_DIR)

../$(BINDIR)/$(TARGET)/$(TARGET): $(OBJS) | bin
	@echo " LD	uds_server"
	$(CC) -o $@ $^

../$(BINDIR)/$(TARGET)/%.o: $(SRCDIR)/%.c | bin
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR)

../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/$(TESTS_SRCDIR)/%.c ../$(BINDIR)/$(TARGET)/can.o | tests_dir
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR) -I $(TESTS_DIR) -I ../deps/minunit

test: ../$(BINDIR)/$(TARGET)/can.o $(TESTS_OBJS) | tests_dir
	@echo " LD	test_entrypoint"
	$(CC) -o ../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/test_entrypoint $^ -Wall -Wextra $(WRAPS) -I $(TESTS_DIR) -I ../deps/minunit -I $(INCDIR) 

clean: 
	@echo " RM	../$(BINDIR)/$(TARGET)/*"
	rm -rfv ../$(BINDIR)/$(TARGET)/*
