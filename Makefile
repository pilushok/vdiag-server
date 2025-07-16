BINDIR=bin
INCDIR=inc
SRCDIR=src
OBJS=$(BINDIR)/uds-server.o $(BINDIR)/can.o
CXX=g++
CC=clang

all: $(BINDIR)/uds-server

$(BINDIR):
	@echo "Creating bin directory"
	mkdir $(BINDIR)

$(BINDIR)/uds-server: $(OBJS) | $(BINDIR) 
	@echo " LD uds-server"
	$(CC) -o $@ $^ 

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	@echo " CXX $@"
	$(CXX) -c $< -o $@ -I $(INCDIR)

$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo " CC $@"
	$(CC) -c $< -o $@ -I $(INCDIR)

clean:
	@echo "Cleaning up $(BINDIR)"
	rm -f $(BINDIR)/*
