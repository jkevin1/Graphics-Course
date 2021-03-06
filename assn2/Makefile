# simple makefile to build all .c or .cpp files and run to create a png

# list of all .o files that could be generated from .c or .cpp
# uses all source files in this directory
# typically, you would list these out by hand to say exactly what to use
OBJS = $(patsubst %.c,   %.o, $(wildcard *.c)) \
       $(patsubst %.cpp, %.o, $(wildcard *.cpp))

# set to -O for optimized, -g for debug; also can use 'make OPT=-g'
OPT = -O

# maximum warnings, good idea to catch easy bugs
CXXFLAGS += -Wall
CFLAGS += -Wall

# first rule says what to build if you say "make" instead of "make tetra-3.png"
teapot.png:

# make a .png file from a .pam file with the same name
# $@ is the current target (the png), $< is the (first) thing it depends on
%.png: %.pam
	convert $< $@

# how to make a pam from program and nff file
# $@ will be the pam, $* is the base name without the .nff or .pam
# make as trace.ppm, then rename
%.pam: %.nff trace
	./trace < $*.nff
	mv trace.pam $@

# how to build trace executable from OBJS files
# link with c++ compiler to allow c++ code
# $@ is the current target (trace)
# set OPT variable to -g or -O for debugging or optimization
# set LDFLAGS to any library directorys (-L...) to search
# set LDLIBS to any libraries to use (-l...)
# should be OK to leave all of those blank
trace: $(OBJS)
	$(CXX) $(OPT) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

# .o from .cpp, also generating dependency file
%.o: %.cpp
	$(CXX) $(OPT) -c -o $@ $< $(CXXFLAGS)
	$(CXX) -MM -o $*.d $<

# .o from .c, also generating dependency file
%.o: %.c
	$(CC) $(OPT) -c -o $@ $< $(CFLAGS)
	$(CC) -MM -o $*.d $<

# remove everything but the program and image files
clean:
	rm -f *~ *.o *.d

# remove everything including program
clobber: clean
	rm -f trace *.pam *.png

# automatic dependency tracking using generated *.d files
-include $(patsubst %.o, %.d, $(OBJS))
