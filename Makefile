# Add the .../EXE file to APP or TEST
# Add the .../xxx.o file to the OBJS

APP = 

DRIVERS = 

OBJS = util.o \

########### EXE file given. ############
TESTS = test/step0.exe \

CFLAGS := $(CFLAGS) -g -Wextra -Wall -Wunused-variable -iquote .


# -Wextra => Print extra (possibly unwanted) warnings.
# -Wall => Enable most warning messages.
#  Directories specified with -iquote apply only to the quote form of the directive, #include "file".

ifeq ($(shell uname),Linux)
	BASE = platform/linux
	CFLAGS := $(CFLAGS) -iquote $(BASE)
	CFLAGS := $(CFLAGS) -pthread 
endif

# -pthread Define additional macros required for using the POSIX threads library. 
# ---- You should use this option consistently for both compilation and linking. 
#---------- This option is supported on GNU/Linux targets, most other Unix derivatives, and also on x86 Cygwin and MinGW targets.

.PHONY: all clean

all: $(APPS) $(TESTS)

########### .EXE <- .o  which is Linking. #############
$(APPS): %.exe : %.o $(OBJS) $(DRIVERS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)	 

$(TESTS): %.exe : %.o $(OBJS) $(DRIVERS) # test/test.h
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)	 

# $@ The file name of the target of the rule. If the target is an archive member, then ‘$@’ is the name of the archive file.
# $^ The names of all the prerequisites, with spaces between them.
# $< The name of the first prerequisite.

############ .o <- .c  which is Compile and Assembly. #############
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(APPS) $(OBJS) $(DRIVERS) $(TESTS) $(APPS:.exe=.o) $(TESTS:.exe=.o)

# substitution reference substitutes the value of a variable with alterations that you specify. 
# It has the form ‘$(var:a=b)’ (or ‘${var:a=b}’) and its meaning is to take the value of the variable var, 
# replace every a at the end of a word with b in that value, and substitute the resulting string.