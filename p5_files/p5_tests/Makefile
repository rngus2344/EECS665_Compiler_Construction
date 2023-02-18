TESTFILES := $(wildcard **/*.cmm) $(wildcard *.cmm)
TESTS := $(TESTFILES:.cmm=.test)

.PHONY: all

all: $(TESTS)

%.test:
	@echo "Testing $*.cmm"
	@touch $*.err #The @ means don't show the command being invoked
	@../cmmc $*.cmm -c 2> $*.err ;\
	PROG_EXIT_CODE=$$?;\
	echo "diff error...";\
	diff $*.err $*.err.expected;\
	ERR_EXIT_CODE=$$?;\
	exit $$ERR_EXIT_CODE

clean:
	rm -f *.out *.err
