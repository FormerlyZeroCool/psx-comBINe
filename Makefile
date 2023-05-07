# compiler
CXX := g++
# flags
CXXFLAGS := -std=c++17 -Wall -O3
# includes
INCLUDES := -I./include
# source directory
SRCDIR := ./src
# object directory
OBJDIR := ./obj
# executable name
EXENAME := comBINe

# source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)
# object files
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# header files
HEADERS := $(wildcard $(INCLUDES)/*.h)

# rule for building object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# rule for building the executable
$(EXENAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(EXENAME)
