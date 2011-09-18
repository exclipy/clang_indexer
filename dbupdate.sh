#!/bin/bash

add_to_index() {
    echo dbadd index.db `cat .clang_complete` $1
    dbadd index.db `cat .clang_complete` $1
}

remove_from_index() {
    echo dbrm index.db $1.i.gz
    dbrm index.db $1.i.gz
}

find . -name "*.cpp" | sort > files2.txt

if [ ! -f index.db -o ! -f files.txt ]
then
    echo "Creating database"
    dbclear index.db
    for i in `cat files2.txt`
    do
        add_to_index $i
    done
    mv files2.txt files.txt
    exit
fi

echo "Updating database"
#Generate the list of files added since last time
comm -23 files2.txt files.txt > filesadded.txt

for i in `cat filesadded.txt`
do
    add_to_index $i
done

for i in `comm -23 files.txt files2.txt` # removed files
do
    remove_from_index $i
done

# modified files (that weren't added)
for i in `comm -23 <(find . -newer files.txt -name "*.cpp"| sort) filesadded.txt`
do
    remove_from_index $i
    add_to_index $i
done

rm filesadded.txt
mv files2.txt files.txt
