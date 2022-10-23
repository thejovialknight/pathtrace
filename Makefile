# A simple Makefile for compiling small SDL projects

# set the compiler
CXX := clang++

# NOTE: ADD FLAGS FOR LIBRARIES HERE!
# set the compiler flags
CXXFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c++17 -Wall -lSDL2_image -lm

# set directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# set files with wildcards
SRCS := $(wildcard $(SRCDIR)/*.cpp)
HDRS := $(wildcard $(SRCDIR)/*.h)
OBJS := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

#$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
#	@$(CXX) $(CXXFLAGS) -c $< -o $@

# generate names of object files
OBJS := $(SRCS:.cpp=.o)

# name of executable
EXEC := build/game.exe

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
