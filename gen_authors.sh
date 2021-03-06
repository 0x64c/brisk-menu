#!/bin/bash
git log --format='%aN <%aE>' | sort -f | uniq > AUTHORS

echo "#pragma once" > src/lib/authors.h
echo "#include <stdlib.h>" >> src/lib/authors.h
echo "/* Machine generated by gen_authors.sh */" >> src/lib/authors.h
echo "static const char *brisk_developers[] = {" >> src/lib/authors.h
while read -r line; do
    echo "        \"${line}\"," >> src/lib/authors.h
done < AUTHORS
echo "        NULL," >> src/lib/authors.h
echo "};" >> src/lib/authors.h
