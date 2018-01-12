all:
	for dir in `ls | egrep -v '^(Makefile|common.c|_)'`; do (cd $$dir && make); done

clean:
	rm -f */run-test*
