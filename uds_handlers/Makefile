# TODO: debug
export BINDIR ?= bin

TARGET=std_handlers.so
TARGET_DIR=uds_handlers
INCDIR=inc
SRCDIR=src
OBJS=../$(BINDIR)/$(TARGET_DIR)/diag_session_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/rdbi_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/rdba_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/tp_handler.o \
		 ../$(BINDIR)/$(TARGET_DIR)/write_by_address_handler.o
CXX=clang++
CC=clang

.PHONY: all clean install

all: ../$(BINDIR)/$(TARGET_DIR)/$(TARGET)

$(BINDIR):
	@echo " Creating $(TARGET_DIR)"
	mkdir -p ../$(BINDIR)/$(TARGET_DIR)

../$(BINDIR)/$(TARGET_DIR)/$(TARGET): $(OBJS) | $(BINDIR)
	@echo " LD	std_handlers.so"
	$(CC) -o $@ $^ -fPIC -shared

../$(BINDIR)/$(TARGET_DIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo " CC	$@"
	$(CC) -c $< -o $@ -I $(INCDIR) -I ../uds_server/inc

clean:
	@echo "Cleaning uds_handlers"
	rm -f ../$(BINDIR)/$(TARGET_DIR)/*
