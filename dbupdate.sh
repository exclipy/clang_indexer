#!/bin/sh

find . -name "*.cpp" | sort > files2.txt

if [ ! -f index.db -o ! -f files.txt ]
then
    dbclear index.db
    xargs dbadd index.db < files2.txt
    mv files2.txt files.txt
    exit
fi

#Generate the list of files added since last time
comm -23 files2.txt files.txt > filesadded.txt

for i in `cat filesadded`
do
    dbadd index.db $i
done

for i in `comm -23 files.txt files2.txt` # removed files
do
    dbrm index.db $i.i.gz
done

for i in `comm -23 <(find . -newer files.txt | sort) filesadded.txt` # modified files (that weren't added)
do
    dbrm index.db $i.i.gz
    dbadd index.db $i
done

rm filesadded.txt
mv files2.txt files.txt
