all:
	for dir in `ls | egrep -v '(Makefile|\.c)$$'`; do (cd $$dir && make); done

clean:
	rm -f */run-test*
