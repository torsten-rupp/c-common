# ----------------------------------------------------------------------------
#
# Contents: Makefile for C/C++ utility modules
# Systems: Unix
#
# ----------------------------------------------------------------------------

#------------------------ Specific variables/settings ------------------------

VERSION_LISTS=0.08
VERSION_CSTRINGS=4
VERSION_STRINGS=0.14
VERSION_STRINGLISTS=0.04
VERSION_RINGBUFFERS=0.01
VERSION_AUTOFREE=0.01

CC = gcc
#CFLAGS = -O2 -Wall -g3 -DPLATFORM_LINUX -DHAVE_LONG_LONG
#CFLAGS = -O0 -Wall -Wpointer-arith -Wunused-label -Wunused-parameter -Wunused-variable -Wmissing-prototypes -Wmissing-declarations -Wextra -Wfloat-equal -Wbad-function-cast -Wsign-compare -Wstrict-prototypes -g -DPLATFORM_LINUX -DHAVE_LONG_LONG -DHAVE_BACKTRACE -DHAVE_BACKTRACE_SYMBOLS -fno-builtin -rdynamic -I.
CFLAGS = -std=gnu99 -g3 -O0 -fno-pie -fPIE -fno-builtin -D_FORTIFY_SOURCE=2 -Wall -Wpointer-arith -Wunused-label -Wunused-parameter -Wunused-variable -Wmissing-prototypes -Wmissing-declarations -Wextra -Wfloat-equal -Wbad-function-cast -Wsign-compare -Wstrict-prototypes -g -D_GNU_SOURCE -DPLATFORM_LINUX -DHAVE_LONG_LONG -fno-builtin -rdynamic
CFLAGS += -I. -Ictest
#CFLAGS += -DNDEBUG

LD = gcc
#LDFLAGS = -rdynamic
LDFLAGS = -no-pie
LIBRARIES = -lc -lpthread -lrt -lpcre -lpcreposix -lm -lbfd

FTP_CMD = (stty -echo; read -p "FTP password: " password; stty echo; echo; \
           ftp-upload \
             -v \--host $(FTP) \
             --user $(FTP_USER) \
             --password $${password} \
             --binary \
             --dir $1 \
             $2; \
          )

BUILD_DIR = build

#---------------------------------- Rules ------------------------------------

$(BUILD_DIR)/%.o:common/%.c
	install -d $(BUILD_DIR)/
	$(CC) $(CFLAGS) -c common/$*.c -o $(BUILD_DIR)/$*.o

$(BUILD_DIR)/%.o:tests/%.c
	install -d $(BUILD_DIR)/
	$(CC) $(CFLAGS) -c tests/$*.c -o $(BUILD_DIR)/$*.o

$(BUILD_DIR)/%.o:demos/%.c
	install -d $(BUILD_DIR)/
	$(CC) $(CFLAGS) -c demos/$*.c -o $(BUILD_DIR)/$*.o

$(BUILD_DIR)/%.o:%.c
	install -d $(BUILD_DIR)/
	$(CC) $(CFLAGS) -c $*.c -o $(BUILD_DIR)/$*.o

#--------------------------------- Objects -----------------------------------

SOURCES =       common/arrays.c \
                common/autofree.c \
                common/bitsets.c \
                common/cstrings.c \
                common/cmdoptions.c \
                common/configvalues.c \
                common/devices.c \
                common/dictionaries.c \
                common/global.c \
                common/files.c \
                common/fragmentlists.c \
                common/hashtables.c \
                common/lists.c \
                common/misc.c \
                common/msgqueues.c \
                common/progressinfo.c \
                common/ringbuffers.c \
                common/semaphores.c \
                common/stacktraces.c \
                common/stringarrays.c \
                common/stringlists.c \
                common/stringmaps.c \
                common/strings.c \
                common/threads.c \
                common/threadpools.c \
                common/tinymemory.c \

OBJECTS =       $(foreach source,$(SOURCES),$(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(source))))

OBJECTS +=      $(BUILD_DIR)/autofree_demo.o \
                $(BUILD_DIR)/cstrings_demo.o \
                $(BUILD_DIR)/dictionaries_demo.o \
                $(BUILD_DIR)/errors.o \
                $(BUILD_DIR)/lists_demo.o \
                $(BUILD_DIR)/ringbuffers_demo.o \
                $(BUILD_DIR)/semaphores_demo.o \
                $(BUILD_DIR)/stacktrace_demo.o \
                $(BUILD_DIR)/stresstest.o \
                $(BUILD_DIR)/stringlists_demo.o \
                $(BUILD_DIR)/stringmaps_demo.o \
                $(BUILD_DIR)/strings_demo.o \
                $(BUILD_DIR)/tinymemory_demo.o \

DEMOS =         $(BUILD_DIR)/lists_demo \
                $(BUILD_DIR)/cstrings_demo \
                $(BUILD_DIR)/strings_demo \
                $(BUILD_DIR)/stringlists_demo \
                $(BUILD_DIR)/stringmaps_demo \
                $(BUILD_DIR)/ringbuffers_demo \
                $(BUILD_DIR)/autofree_demo \
                $(BUILD_DIR)/semaphores_demo \
                $(BUILD_DIR)/tinymemory_demo \
                $(BUILD_DIR)/dictionaries_demo \
                $(BUILD_DIR)/stacktrace_demo \

OBJECTS +=      $(BUILD_DIR)/arrays_tests.o \
                $(BUILD_DIR)/cmdoptions_tests.o \
                $(BUILD_DIR)/cstrings_tests.o \
                $(BUILD_DIR)/devices_tests.o \
                $(BUILD_DIR)/dictionaries_tests.o \
                $(BUILD_DIR)/files_tests.o \
                $(BUILD_DIR)/fragmentlists_tests.o \
                $(BUILD_DIR)/hashtables_tests.o \
                $(BUILD_DIR)/misc_tests.o \
                $(BUILD_DIR)/msgqueues_tests.o \
                $(BUILD_DIR)/lists_tests.o \
                $(BUILD_DIR)/patterns_tests.o \
                $(BUILD_DIR)/progressinfo_tests.o \
                $(BUILD_DIR)/ringbuffers_tests.o \
                $(BUILD_DIR)/strings_tests.o \
                $(BUILD_DIR)/stringmaps_tests.o \
                $(BUILD_DIR)/threadpools_tests.o \
                $(BUILD_DIR)/threads_tests.o \
                $(BUILD_DIR)/tinymemory_tests.o \

TESTS =         $(BUILD_DIR)/arrays_tests \
                $(BUILD_DIR)/bitsets_tests \
                $(BUILD_DIR)/cmdoptions_tests \
                $(BUILD_DIR)/cstrings_tests \
                $(BUILD_DIR)/devices_tests \
                $(BUILD_DIR)/dictionaries_tests \
                $(BUILD_DIR)/files_tests \
                $(BUILD_DIR)/fragmentlists_tests \
                $(BUILD_DIR)/hashtables_tests \
                $(BUILD_DIR)/lists_tests \
                $(BUILD_DIR)/misc_tests \
                $(BUILD_DIR)/msgqueues_tests \
                $(BUILD_DIR)/patterns_tests \
                $(BUILD_DIR)/progressinfo_tests \
                $(BUILD_DIR)/ringbuffers_tests \
                $(BUILD_DIR)/strings_tests \
                $(BUILD_DIR)/stringmaps_tests \
                $(BUILD_DIR)/threadpools_tests \
                $(BUILD_DIR)/threads_tests \
                $(BUILD_DIR)/tinymemory_tests \

TARGETS = 	$(OBJECTS) \
            $(DEMOS) \
            $(TESTS) \
            \
         	  $(BUILD_DIR)/stresstest \

#------------------------------ Link modules ---------------------------------

#------------------------------ Dependencies ---------------------------------

-include Makefile.user

all: \
  $(TARGETS)

.PHONY: clean
clean:
	$(RM) $(TARGETS)

.PHONE: help
help:
	@echo "Help:"
	@echo ""
	@echo "  all                - make all objects and test programs"
	@echo "  dist               - make distributions packages"
	@echo "  tests              - run unit tests"
	@echo ""
	@echo "  lists_dist         - lists package"
	@echo "  lists_upload       - upload lists package"
	@echo "  cstrings_dist      - cstrings package"
	@echo "  cstrings_upload    - upload cstrings package"
	@echo "  strings_dist       - strings package"
	@echo "  strings_upload     - upload strings package"
	@echo "  stringlists_dist   - string list package"
	@echo "  stringlists_upload - upload string list package"
	@echo "  stringmaps_dist    - string map package"
	@echo "  stringmaps_upload  - upload string map package"
	@echo "  ringbuffers_dist   - ringbuffers package"
	@echo "  ringbuffers_upload - upload ringbuffers package"
	@echo "  autofree_dist      - autofree package"
	@echo "  autofree_upload    - upload autofree package"

.PHONY: dist
dist: \
  lists_dist \
  strings_dist \
  stringlists_dist \
  ringbuffers_dist \
  autofree_dist

# ----------------------------------------------------------------------

errors.c errors.h: errors.def errors.pl
	./errors.pl -h errors.h -c errors.c < errors.def

$(BUILD_DIR)/errors.o: errors.c errors.h

# ----------------------------------------------------------------------

$(BUILD_DIR)/lists.o: common/lists.c common/lists.h

$(BUILD_DIR)/lists_tests.o: tests/lists_tests.c common/lists.h
$(BUILD_DIR)/lists_tests: $(BUILD_DIR)/lists_tests.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/lists_demo.o: demos/lists_demo.c common/lists.h
$(BUILD_DIR)/lists_demo: $(BUILD_DIR)/lists_demo.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

lists_dist lists-$(VERSION_LISTS).tar.bz2:
	cp lists.changelog ChangeLog
	tar cjf lists-$(VERSION_LISTS).tar.bz2 \
          common/cstrings.c common/cstrings.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          lists_demo.c \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

lists_upload:
	$(call FTP_CMD, \
          /development/c/lists, \
          lists-$(VERSION_LISTS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/cstrings.o: common/cstrings.c common/cstrings.h

$(BUILD_DIR)/cstrings_tests.o: tests/cstrings_tests.c common/cstrings.h
$(BUILD_DIR)/cstrings_tests: $(BUILD_DIR)/cstrings_tests.o $(BUILD_DIR)/cstrings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/cstrings_demo.o: demos/cstrings_demo.c common/cstrings.h
$(BUILD_DIR)/cstrings_demo: $(BUILD_DIR)/cstrings_demo.o $(BUILD_DIR)/cstrings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

cstrings_dist cstrings-$(VERSION_CSTRINGS).tar.bz2:
	cp cstrings.changelog ChangeLog
	tar cjf cstrings-$(VERSION_CSTRINGS).tar.bz2 \
          common/cstrings.c common/cstrings.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          config.h \
          cstrings_demo.c \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

cstrings_upload:
	$(call FTP_CMD, \
          /development/c/cstrings, \
          cstrings-$(VERSION_CSTRINGS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/strings.o: common/strings.c common/strings.h

$(BUILD_DIR)/strings_tests.o: tests/strings_tests.c common/strings.h
$(BUILD_DIR)/strings_tests: $(BUILD_DIR)/strings_tests.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/strings_demo.o: demos/strings_demo.c common/strings.h
$(BUILD_DIR)/strings_demo: $(BUILD_DIR)/strings_demo.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

strings_dist strings-$(VERSION_STRINGS).tar.bz2:
	cp strings.changelog ChangeLog
	tar cjf strings-$(VERSION_STRINGS).tar.bz2 \
          common/cstrings.c common/cstrings.h \
          common/strings.c common/strings.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          config.h \
          strings_demo.c \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

strings_upload:
	$(call FTP_CMD, \
          /development/c/strings, \
          strings-$(VERSION_STRINGS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/stringlists.o: common/stringlists.c common/stringlists.h

$(BUILD_DIR)/stringlists_demo.o: demos/stringlists_demo.c common/stringlists.h
$(BUILD_DIR)/stringlists_demo: $(BUILD_DIR)/stringlists_demo.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

stringlists_dist stringlists-$(VERSION_STRINGS).tar.bz2:
	cp stringlists.changelog ChangeLog
	tar cjf stringlists-$(VERSION_STRINGLISTS).tar.bz2 \
          common/strings.c common/strings.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          common/stringlists.c common/stringlists.h \
          config.h \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

stringlists_upload:
	$(call FTP_CMD, \
          /development/c/stringlists, \
          stringlists-$(VERSION_STRINGLISTS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/stringmaps.o: common/stringmaps.c common/stringmaps.h

$(BUILD_DIR)/stringmaps_tests.o: tests/stringmaps_tests.c common/stringmaps.h
$(BUILD_DIR)/stringmaps_tests: $(BUILD_DIR)/stringmaps_tests.o $(BUILD_DIR)/stringmaps.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/stringmaps_demo.o: demos/stringmaps_demo.c common/stringmaps.h
$(BUILD_DIR)/stringmaps_demo: $(BUILD_DIR)/stringmaps_demo.o $(BUILD_DIR)/stringmaps.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

stringmaps_dist stringmaps-$(VERSION_STRINGS).tar.bz2:
	cp stringmaps.changelog ChangeLog
	tar cjf stringmaps-$(VERSION_STRINGMAPS).tar.bz2 \
          common/strings.c common/strings.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          common/stringmaps.c common/stringmaps.h \
          config.h \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

stringmaps_upload:
	$(call FTP_CMD, \
          /development/c/stringmaps, \
          stringmaps-$(VERSION_STRINGMAPS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/ringbuffers.o: common/ringbuffers.c common/ringbuffers.h

$(BUILD_DIR)/ringbuffers_tests.o: tests/ringbuffers_tests.c common/ringbuffers.h
$(BUILD_DIR)/ringbuffers_tests: $(BUILD_DIR)/ringbuffers_tests.o $(BUILD_DIR)/ringbuffers.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/ringbuffers_demo: $(BUILD_DIR)/ringbuffers_demo.o $(BUILD_DIR)/ringbuffers.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

ringbuffers_dist ringbuffers-$(VERSION_ringbuffers).tar.bz2:
	cp ringbuffers.changelog ChangeLog
	tar cjf ringbuffers-$(VERSION_RINGBUFFERS).tar.bz2 \
          common/ringbuffers.c common/ringbuffers.h \
          common/global.c common/global.h \
          common/lists.c common/lists.h \
          ringbuffers_demo.c \
          config.h \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

ringbuffers_upload:
	$(call FTP_CMD, \
          /development/c/ringbuffers, \
          ringbuffers-$(VERSION_RINGBUFFERS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/arrays.o: common/arrays.c common/arrays.h common/lists.h common/strings.h

$(BUILD_DIR)/arrays_tests.o: tests/arrays_tests.c common/arrays.h
$(BUILD_DIR)/arrays_tests: $(BUILD_DIR)/arrays_tests.o $(BUILD_DIR)/arrays.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/fragmentlists.o: common/fragmentlists.c common/fragmentlists.h common/lists.h common/strings.h

$(BUILD_DIR)/fragmentlists_tests.o: tests/fragmentlists_tests.c common/fragmentlists.h
$(BUILD_DIR)/fragmentlists_tests: $(BUILD_DIR)/fragmentlists_tests.o $(BUILD_DIR)/fragmentlists.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/autofree_demo: $(BUILD_DIR)/autofree_demo.o $(BUILD_DIR)/autofree.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

autofree_dist autofree-$(VERSION_autofree).tar.bz2:
	cp autofree.changelog ChangeLog
	tar cjf autofree-$(VERSION_AUTOFREE).tar.bz2 \
          global.c global.h \
          lists.c lists.h \
          autofree.c autofree.h \
          autofree_demo.c \
          config.h \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

autofree_upload:
	$(call FTP_CMD, \
          /development/c/autofree, \
          autofree-$(VERSION_AUTOFREE).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/sempahores.o: common/semaphores.c common/semaphores.h common/threads.h common/files.h common/devices.h common/global.h common/lists.h common/strings.h common/stringlists.h common/misc.h

$(BUILD_DIR)/semaphores_demo: $(BUILD_DIR)/semaphores_demo.o $(BUILD_DIR)/semaphores.o $(BUILD_DIR)/threads.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/misc.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

semaphores_dist semaphores-$(VERSION_semaphores).tar.bz2:
	cp semaphores.changelog ChangeLog
	tar cjf semaphores-$(VERSION_semaphores).tar.bz2 \
          common/global.c common/global.h \
          common/semaphores.c common/semaphores.h \
          semaphores_demo.c \
          config.h \
          errors.c errors.h \
          errors.def \
          errors.pl \
          ChangeLog
	rm -f ChangeLog

semaphores_upload:
	$(call FTP_CMD, \
          /development/c/semaphores, \
          semaphores-$(VERSION_LISTS).tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/dictionaries.o: common/dictionaries.c common/dictionaries.h

$(BUILD_DIR)/dictionaries_tests.o: tests/dictionaries_tests.c common/dictionaries.h
$(BUILD_DIR)/dictionaries_tests: $(BUILD_DIR)/dictionaries_tests.o $(BUILD_DIR)/dictionaries.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/semaphores.o $(BUILD_DIR)/threads.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/dictionaries_demo: $(BUILD_DIR)/dictionaries_demo.o $(BUILD_DIR)/dictionaries.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/semaphores.o $(BUILD_DIR)/threads.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/misc.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/hashtables.o: common/hashtables.c common/hashtables.h

$(BUILD_DIR)/hashtables_tests.o: tests/hashtables_tests.c common/hashtables.h
$(BUILD_DIR)/hashtables_tests: $(BUILD_DIR)/hashtables_tests.o $(BUILD_DIR)/hashtables.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/cmdoptions.o: common/cmdoptions.c common/cmdoptions.h

$(BUILD_DIR)/cmdoptions_tests.o: tests/cmdoptions_tests.c common/cmdoptions.h
$(BUILD_DIR)/cmdoptions_tests: $(BUILD_DIR)/cmdoptions_tests.o $(BUILD_DIR)/cmdoptions.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/arrays.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/files.o: common/files.c common/files.h

$(BUILD_DIR)/files_tests.o: tests/files_tests.c common/files.h
$(BUILD_DIR)/files_tests: $(BUILD_DIR)/files_tests.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/devices.o: common/devices.c common/devices.h

$(BUILD_DIR)/devices_tests.o: tests/devices_tests.c common/devices.h
$(BUILD_DIR)/devices_tests: $(BUILD_DIR)/devices_tests.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/stacktrace_demo: $(BUILD_DIR)/stacktrace_demo.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/stacktraces.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

stacktrace_upload:
	$(call FTP_CMD, \
          demos, \
          stacktrace/stacktrace_demo.tar.bz2 \
         )

# ----------------------------------------------------------------------

$(BUILD_DIR)/threads.o: common/threads.c common/threads.h

$(BUILD_DIR)/threads_tests.o: $(BUILD_DIR)/threads.o
$(BUILD_DIR)/threads_tests: $(BUILD_DIR)/threads_tests.o $(BUILD_DIR)/threads.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/threadpools.o: common/threadpools.c common/threadpools.h

$(BUILD_DIR)/threadpools_tests.o: $(BUILD_DIR)/threadpools.o
$(BUILD_DIR)/threadpools_tests: $(BUILD_DIR)/threadpools_tests.o $(BUILD_DIR)/threadpools.o $(BUILD_DIR)/arrays.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/bitsets.o: common/bitsets.c common/bitsets.h

$(BUILD_DIR)/bitsets_tests.o: $(BUILD_DIR)/bitsets.o
$(BUILD_DIR)/bitsets_tests: $(BUILD_DIR)/bitsets_tests.o $(BUILD_DIR)/bitsets.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/misc.o: common/misc.c common/misc.h

$(BUILD_DIR)/misc_tests.o: tests/misc_tests.c common/misc.h
$(BUILD_DIR)/misc_tests: $(BUILD_DIR)/misc_tests.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/arrays.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/tinymemory.o: common/tinymemory.c common/tinymemory.h

$(BUILD_DIR)/tinymemory_tests.o: tests/tinymemory_tests.c common/tinymemory.h
$(BUILD_DIR)/tinymemory_tests: $(BUILD_DIR)/tinymemory_tests.o $(BUILD_DIR)/tinymemory.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

$(BUILD_DIR)/tinymemory_demo: $(BUILD_DIR)/tinymemory_demo.o $(BUILD_DIR)/tinymemory.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/patterns.o: common/patterns.c common/patterns.h

$(BUILD_DIR)/patterns_tests.o: tests/patterns_tests.c common/patterns.h
$(BUILD_DIR)/patterns_tests: $(BUILD_DIR)/patterns_tests.o $(BUILD_DIR)/patterns.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/progressinfo.o: common/progressinfo.c common/progressinfo.h

$(BUILD_DIR)/progressinfo_tests.o: tests/progressinfo_tests.c common/progressinfo.h
$(BUILD_DIR)/progressinfo_tests: $(BUILD_DIR)/progressinfo_tests.o $(BUILD_DIR)/progressinfo.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/msgqueues.o: common/msgqueues.c common/msgqueues.h

$(BUILD_DIR)/msgqueues_tests.o: tests/msgqueues_tests.c common/msgqueues.h
$(BUILD_DIR)/msgqueues_tests: $(BUILD_DIR)/msgqueues_tests.o $(BUILD_DIR)/msgqueues.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/strings.o $(BUILD_DIR)/stringlists.o $(BUILD_DIR)/misc.o $(BUILD_DIR)/files.o $(BUILD_DIR)/devices.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

$(BUILD_DIR)/stresstest: $(BUILD_DIR)/stresstest.o $(BUILD_DIR)/global.o $(BUILD_DIR)/stacktraces.o $(BUILD_DIR)/errors.o $(BUILD_DIR)/lists.o $(BUILD_DIR)/arrays.o $(BUILD_DIR)/ringbuffers.o $(BUILD_DIR)/threads.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

# ----------------------------------------------------------------------

.PHONY: tests
tests: \
  $(TESTS) \
  $(BUILD_DIR)/stresstest
	for test in $(TESTS); do \
          ./$$test; \
          valgrind --leak-check=full --error-exitcode=127 ./$$test; \
          if test $$? -ne 0; then \
            exit 1; \
          fi; \
        done;
	$(BUILD_DIR)/stresstest 5

# end of file
