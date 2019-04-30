all: examples

EXES = \
	   ex1 \
	   ex2 \
	   ex3 \
	   ex4 \
	   ex5 \

ex1: example1.y exp1.c
	lemon example1.y
	cc -o $@ exp1.c

ex2: example2.y exp2.c
	lemon example2.y
	cc -o $@ exp2.c

ex3: example3.y exp3.c
	lemon example3.y
	cc -o $@ exp3.c

ex4: example4.y exp4.c
	lemon example4.y
	cc -o $@ exp4.c

ex5: example5.y exp5.c lexer.l
	lemon example5.y
	flex -o lexer.c lexer.l
	cc -o $@ exp5.c lexer.c

examples: $(EXES)

clean:
	rm -f $(EXES)
	rm -f example[0-9].{c,h,out}
	rm -f lexer.c
