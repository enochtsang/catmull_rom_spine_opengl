#!/usr/bin/make

##############################################
# A simple but flexible Makefile for small-to-
#  medium C++ projects. Written by Haysn
#  Hornbeck.

##############################################
# Note: Header dependencies lag compilation.
#  If you've edited a source file to add a new
#  "include" and suddenly get compiler errors,
#  try compiling again.

##############################################
# Set up the project-specific options
EXEC=image-manipulation
LIBS=`pkg-config --libs glfw3 gl`

##############################################
# Tweak the compiler/debugger/profiler options
CXX=g++		# or nvcc, or clang++

# DEBUG=-ggdb -Og	# (make debugging easier)
DEBUG=-O2 -g
DEBUG_CMDLINE=

# PROFILE=-pg -fprofile-arcs -ftest-coverage
PROFILE=		# (disable profiling)
PROFILE_CMDLINE=	# (any parameters needed?)

#############################################
# Set up the source directories 
INCDIR=inc
SRCDIR=src
OBJDIR=obj
BINDIR=bin

#############################################
# The rest should only need to be modified if
#  you're not using g++
INCLUDES=-I$(INCDIR)
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
DEPS=$(OBJECTS:.o=.d)
PROFS=$(OBJECTS:.o=.gcda) $(OBJECTS:.o=.gcno)

LDFLAGS=$(DEBUG) $(PROFILE) $(LIBS) $(LIBDIRS)
CXX_FLAGS=$(DEBUG) $(PROFILE) -c -std=c++11 $(LIBDIRS) $(INCLUDES) -MMD -Wall -pipe
EXECUTABLE=$(BINDIR)/$(EXEC)

all:	$(EXECUTABLE)
clean:
	rm -f $(OBJECTS) $(DEPS) $(PROFS) $(EXECUTABLE) $(OBJDIR)/gmon.out
run: $(EXECUTABLE)
	$(EXECUTABLE) $(DEBUG_CMDLINE)
debug: $(EXECUTABLE)
	gdb --args $(EXECUTABLE) $(DEBUG_CMDLINE)
profile: $(OBJDIR)/gmon.out
	gprof $(EXECUTABLE) $(OBJDIR)/gmon.out
$(OBJDIR)/gmon.out: $(EXECUTABLE)
	$(EXECUTABLE) $(PROFILE_CMDLINE)
	mv gmon.out $(OBJDIR)

-include $(DEPS)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	cp $(EXECUTABLE) .

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $< -o $@

