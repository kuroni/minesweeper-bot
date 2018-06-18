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
	$(CXX) $(CXXFLAGS) -o $@ $? $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INC)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(OBJDIR)/EasyBMP.o: $(SRCDIR)/EasyBMP.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

clean:
	$(RM) $(OBJ)