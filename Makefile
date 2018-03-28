TESTS = \
    test_cpy \
    test_ref

CFLAGS = -Wall -Werror -g

# Control the build verbosity                                                   
ifeq ("$(VERBOSE)","1")
    Q :=
    VECHO = @true
else
    Q := @
    VECHO = @printf
endif

GIT_HOOKS := .git/hooks/applied

.PHONY: all clean

all: $(GIT_HOOKS) $(TESTS)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

OBJS_LIB = \
    tst.o

OBJS := \
    $(OBJS_LIB) \
    test_cpy.o \
    test_ref.o

deps := $(OBJS:%.o=.%.o.d)

test_%: test_%.o $(OBJS_LIB)
	$(VECHO) "  LD\t$@\n"
	$(Q)$(CC) $(LDFLAGS) -g -o $@ $^

%.o: %.c
	$(VECHO) "  CC\t$@\n"
	$(Q)$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$@.d $<

bench: test_ref.o test_cpy.o benchmark_all.txt countCycle.c
	$(info performance of test_ref)
	./test_ref --bench benchmark_all.txt
	$(info performance of test_cpy)
	./test_cpy --bench benchmark_all.txt
	gcc -o countCycle countCycle.c -lm
	./countCycle

plot:result/countCycle.txt
	gnuplot scripts/cycle.gp
	gnuplot scripts/time.gp
	
clean:
	$(RM) $(TESTS) $(OBJS)
	$(RM) $(deps) *.png
	rm -rf result
	mkdir result

-include $(deps)
