TARGET=uds_server
TARGET_DIR=uds_server
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET)/can.o \
		 ../$(BINDIR)/$(TARGET)/uds.o
TESTS_DIR=tests
TESTS_SRCDIR=src
MOCK_OBJS=../$(BINDIR)/mocks/mock_bind.o \
			../$(BINDIR)/mocks/mock_calloc.o \
			../$(BINDIR)/mocks/mock_fcntl.o \
			../$(BINDIR)/mocks/mock_if_nametoindex.o \
			../$(BINDIR)/mocks/mock_socket.o 
TESTS_OBJS =../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/test_entrypoint.o
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

../$(BINDIR)/$(TARGET_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/$(TESTS_SRCDIR)/%.c | tests_dir
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I ../uds_server/$(INCDIR) -I $(INCDIR) -I $(TESTS_DIR) -I ../deps/minunit -I ../mocks/inc

test: $(OBJS) $(MOCK_OBJS) $(TESTS_OBJS) | tests_dir
	@echo " LD	test_entrypoint"
	$(CC) -o ../$(BINDIR)/$(TARGET)/$(TESTS_DIR)/test_entrypoint $^ -Wall -Wextra $(WRAPS) -I $(TESTS_DIR) -I ../deps/minunit -I $(INCDIR) -I ../mocks/inc 

clean: 
	@echo " RM	../$(BINDIR)/$(TARGET)/*"
	rm -rfv ../$(BINDIR)/$(TARGET)/*
