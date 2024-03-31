# ----------------------------------------------------------------------------
#
# Contents: Makefile for
# Systems :
#			   
# ----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Compiler options (gcc):
#
# basic options:
#  -ansi           support ANSI C standard programs
#  -nostdinc       don't search in host include files
#  -c              compile only (create object files)
#  -S              produce assembly source _only_
#  -g              produce debug information for debugger
#
# additional options:
#  -p              include profiler code statements (prof/gprof)
#  -a              include profiler code statements (tcov)
#
# special options:
#  -E              execute preprocessor _only_
#  -C              keep comments in preprocessor output
#  -M              generate dependency list (with system files)
#  -MM             generate dependency list (without system files)
#
# C++ options:
#  -fcheck-new     include check if new-operator returns NULL;
#                  constructors are called only if none-NULL
#
#-----------------------------------------------------------------------------

#------------------------ Specific variables/settings ------------------------

CC=gcc
CC_FLAGS=-Wall -g -std=gnu99
CFLAGS=$(CC_FLAGS)

LD=gcc
LD_FLAGS=-lm

YACC=bison
YACC_FLAGS=

RMF = rm -f
RMRF = rm -rf

#--------------------------------- Objects -----------------------------------

OBJECTS = 	sc_codescanner.o \
                sc_ir.o \
                sc_codeparser.o \
                sc_codeparser_code.o \
                \
                sc_error.o \
                sc_files.o \
                sc_datatype.o \
                sc_optimizer.o \
                sc_codegenerator.o \
                \
                sc_formatscanner.o \
                sc_format_ir.o \
                sc_formatparser.o \
                sc_formatparser_code.o \
                \
                sc_formatgenerator.o \
                \
                sc.o \
                \
                global.o \
                lists.o \
                strings.o \
                dprintf.o \
                cmdoptions.o

INTERMEDIATE =  sc_codeparser_code.h \
                sc_codeparser_code.c \
                \
                sc_formatparser_code.h \
                sc_formatparser_code.c

TARGETS = 	sc

#---------------------------------- Rules ------------------------------------

%.o:%.c
	$(CC) $(CC_FLAGS) -c $*.c -o $*.o

# disable built-in rule
%.c:%.y

#------------------------------ Dependencies ---------------------------------

all:		$(TARGETS)

clean:
		$(RMF) $(OBJECTS) $(INTERMEDIATE) $(TARGETS)

# main
sc: $(OBJECTS)
	$(LD) $(LD_FLAGS) $^ -o $@

sc.o: sc.c sc_codescanner.h sc_codeparser_code.h

# code parser
sc_codescanner.o: sc_codescanner.c sc_codescanner.h sc_codeparser_code.h

sc_codeparser.o: sc_codeparser.c sc_codeparser.h sc_codeparser_code.h

sc_codeparser_code.h sc_codeparser_code.c: sc_codeparser.y
	$(YACC) $(YACCFLAGS) -p code_ -d -o sc_codeparser_code.c sc_codeparser.y

sc_codeparser_code.o: sc_codeparser_code.c sc_codeparser_code.h sc_codescanner.h
	$(CC) $(CC_FLAGS) -Dcode_lex=scCodeScanner_lex -c sc_codeparser_code.c -o $@

# format parser
sc_formatscanner.o: sc_formatscanner.c sc_formatscanner.h sc_formatparser_code.h

sc_formatparser.o: sc_formatparser.c sc_formatparser.h sc_formatparser_code.h

sc_formatparser_code.h sc_formatparser_code.c: sc_formatparser.y
	$(YACC) $(YACCFLAGS) -p format_ -d -o sc_formatparser_code.c sc_formatparser.y

sc_formatparser_code.o: sc_formatparser_code.c sc_formatparser_code.h sc_formatscanner.h
	$(CC) $(CC_FLAGS) -Dformat_lex=scFormatScanner_lex -c sc_formatparser_code.c -o $@

# test
.PHONY: run
run: sc
	./sc demo.sc -o demo -t both -I 1 -I 2

rund: sc
	./sc --debug demo.sc -o demo -I 1 -I 2

com:
	gcc demo.c

format: sc
	$(RMF) demo.sc.tmp
	./sc --format demo.sc
	cat demo.sc.tmp

formatd: sc
	./sc --debug --format demo.sc
