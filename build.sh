for dir in `ls | egrep -v '\.(sh|c)$'`; do
	(cd $dir && make)
done
