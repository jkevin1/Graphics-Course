# view assignment make file

# GNU make magic to get the right platform-dependent flags for compiling. 
# For example, on linux this will include Defs.Linux, while on mac it
# will include Defs.Darwin
include Defs.$(shell uname)

# set to -O for optimized, -g for debug; also can use 'make OPT=-g'
OPT = -O

# maximum warnings, good idea to catch easy bugs
CXXFLAGS += -Wall

# main program name
PROG  = GLdemo

# include ALL .cpp files in this directory, with corresponding .o
OBJS  = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

# rules for building -- ordered from final output to original .c for no
# particular reason other than that the first rule is the default

# program from .o files
$(PROG): $(OBJS)
	$(CXX) $(OPT) -o $(PROG) $(OBJS) $(LDFLAGS) $(LDLIBS)

# .o from .c or .cxx, also generating dependency file
%.o: %.cpp
	$(CXX) $(OPT) -c -o $@ $< $(CXXFLAGS)
	$(CXX) -MM -o $*.d $<


# remove everything but the program
clean:
	rm -f *~ *.o *.d

# remove everything including program
clobber: clean
	rm -f $(PROG)

# automatic dependency tracking using generated *.d files
-include $(patsubst %.o, %.d, $(OBJS))
