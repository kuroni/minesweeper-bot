SRCDIR := src
OBJDIR := obj
BINDIR := bin
INCDIR := include
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))
LDFLAGS := -lgdi32
CXXFLAGS := -std=c++17 -O2 -Wall -I$(INCDIR)
CFLAGS := -std=c11 -O2 -Wall -I$(INCDIR)

.PHONY: clean

$(BINDIR)/out: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(wildcard $(INCDIR)/*.hpp)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRCDIR)/main.cpp $(LDFLAGS)

$(OBJDIR)/EasyBMP.o: $(SRCDIR)/EasyBMP.cpp $(wildcard $(INCDIR)/*.h)
	$(CXX) $(CXXFLAGS) -c -o $@ $(SRCDIR)/EasyBMP.cpp $(LDFLAGS)

clean:
	$(RM) $(OBJ)