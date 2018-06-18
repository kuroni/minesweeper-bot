SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))
INC := $(INCDIR)/handle.h $(INCDIR)/solve.h $(INCDIR)/declare.h
LDFLAGS := -lgdi32
CXXFLAGS := -std=c++17 -O2 -Wall -I$(INCDIR)

.PHONY: clean

$(BINDIR)/out: $(OBJ)
	g++ $(CXXFLAGS) -o $@ $? $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INC)
	g++ $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(OBJDIR)/EasyBMP.o: $(SRCDIR)/EasyBMP.cpp
	g++ $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	-del /s /q $(OBJDIR)