# TODO: debug
export BINDIR ?= bin

TARGET=std_handlers.so
TARGET_DIR=uds_handlers
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET_DIR)/dsc_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/rdbi_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/rdba_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/tp_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/wrba_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/wrbi_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/did_map.o 
TESTS_DIR=tests
TESTS_SRCDIR=src
MOCK_OBJS=../$(BINDIR)/mocks/mock_bind.o \
			../$(BINDIR)/mocks/mock_calloc.o \
			../$(BINDIR)/mocks/mock_fcntl.o \
			../$(BINDIR)/mocks/mock_if_nametoindex.o \
			../$(BINDIR)/mocks/mock_write.o \
			../$(BINDIR)/mocks/mock_read.o \
			../$(BINDIR)/mocks/mock_open.o \
			../$(BINDIR)/mocks/mock_lseek.o \
			../$(BINDIR)/mocks/mock_close.o \
			../$(BINDIR)/mocks/mock_socket.o 
TESTS_OBJS =../$(BINDIR)/$(TARGET_DIR)/$(TESTS_DIR)/test_entrypoint.o
WRAPS=-Wl,--wrap=fcntl,--wrap=socket,--wrap=bind,--wrap=if_nametoindex,--wrap=calloc,--wrap=close,--wrap=open,--wrap=read,--wrap=lseek,--wrap=write
CXX=clang++
CC=clang

.PHONY: all clean install test

all: ../$(BINDIR)/$(TARGET_DIR)/$(TARGET) test

bin:
	@echo " Creating build directory"
	mkdir -p ../$(BINDIR)/$(TARGET_DIR)

tests_dir:
	@echo " Creating tests directory"
	mkdir -p ../$(BINDIR)/$(TARGET_DIR)/$(TESTS_DIR)

../$(BINDIR)/$(TARGET_DIR)/$(TARGET): $(OBJS) | bin
	@echo " LD	std_handlers.so"
	$(CC) -o $@ $^ -fPIC -shared

../$(BINDIR)/$(TARGET_DIR)/%.o: $(SRCDIR)/%.c | bin
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR) -I ../uds_server/inc -fPIC

../$(BINDIR)/$(TARGET_DIR)/$(TESTS_DIR)/%.o: $(TESTS_DIR)/$(TESTS_SRCDIR)/%.c | tests_dir
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I ../uds_server/$(INCDIR) -I $(INCDIR) -I $(TESTS_DIR) -I ../deps/minunit -I ../mocks/inc/

test: $(OBJS) $(TESTS_OBJS) $(MOCK_OBJS) | tests_dir
	@echo " LD	test_entrypoint"
	$(CC) -o ../$(BINDIR)/$(TARGET_DIR)/$(TESTS_DIR)/test_entrypoint $^ -Wall -Wextra $(WRAPS) -I $(TESTS_DIR) -I ../deps/minunit -I ../uds_server/$(INCDIR) -I ../mocks/inc/ 

clean:
	@echo "Cleaning uds_handlers"
	rm -rf ../$(BINDIR)/$(TARGET_DIR)/*
