SRCDIR := src
OBJDIR := obj
BINDIR := bin
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))
LDFLAGS := -lgdi32
CXXFLAGS := -std=c++17 -O2 -Wall -Iinclude

.PHONY: clean

$(BINDIR)/out: $(OBJ)
	g++ $(CXXFLAGS) -o $@ $? $(LDFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $? $(LDFLAGS)

clean:
	-del /s /q $(OBJDIR)