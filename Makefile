SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include
OBJ := $(OBJDIR)/main.o $(OBJDIR)/qdbmp.o
INC := $(INCDIR)/handle.h $(INCDIR)/solve.h $(INCDIR)/declare.h
LDFLAGS := -lgdi32
CXXFLAGS := -std=c++17 -O2 -Wall -I$(INCDIR)
CFLAGS := -std=c11 -O2 -Wall -I$(INCDIR)

.PHONY: clean

$(BINDIR)/out: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $? $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INC)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(OBJDIR)/qdbmp.o: $(SRCDIR)/qdbmp.c $(INCDIR)/qdbmp.h
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	$(RM) $(OBJ)