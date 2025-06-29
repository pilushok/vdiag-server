BINDIR=bin
INCDIR=inc
SRCDIR=src
OBJS=$(BINDIR)/uds-server.o
CXX=g++

all: $(BINDIR)/uds-server

$(BINDIR):
	@echo "Creating bin directory"
	mkdir $(BINDIR)

$(BINDIR)/uds-server: $(OBJS) | $(BINDIR) 
	@echo " LD uds-server"
	$(CXX) -o $@ $^ 

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	@echo " CXX $@"
	$(CXX) -c $< -o $@ -I $(INCDIR)

$(BINDIR)/%.o: $(SRCDIR)/%.c | $(BINDIR)
	@echo " CC $@"
	$(CXX) -c $< -o $@

clean:
	@echo "Cleaning up $(BINDIR)"
	rm -f $(BINDIR)/*
