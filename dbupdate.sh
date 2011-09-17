#!/bin/bash

find . -name "*.cpp" | sort > files2.txt

if [ ! -f index.db -o ! -f files.txt ]
then
    echo "Creating database"
    dbclear index.db
    for i in `cat files2.txt`
    do
        echo dbadd `cat .clang_complete` indexdb $i
        dbadd indexdb `cat .clang_complete` $i
    done
    mv files2.txt files.txt
    exit
fi

echo "Updating database"
#Generate the list of files added since last time
comm -23 files2.txt files.txt > filesadded.txt

for i in `cat filesadded.txt`
do
    echo dbadd `cat .clang_complete` indexdb $i
    dbadd indexdb `cat .clang_complete` $i
done

for i in `comm -23 files.txt files2.txt` # removed files
do
    echo dbrm index.db $i.i.gz
    dbrm index.db $i.i.gz
done

# modified files (that weren't added)
for i in `comm -23 <(find . -newer files.txt -name "*.cpp"| sort) filesadded.txt`
do
    echo dbrm index.db $i.i.gz
    dbrm index.db $i.i.gz
    echo dbadd `cat .clang_complete` indexdb $i
    dbadd indexdb `cat .clang_complete` $i
done

rm filesadded.txt
mv files2.txt files.txt
