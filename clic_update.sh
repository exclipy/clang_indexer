#!/bin/bash

# Specify files to index here
find . -name "*.cpp" | sort > files2.txt

add_to_index() {
    echo clic_add index.db `cat .clang_complete` $1
    clic_add index.db `cat .clang_complete` $1
}

remove_from_index() {
    echo clic_rm index.db $1.i.gz
    clic_rm index.db $1.i.gz
    echo rm $1.i.gz
    rm $1.i.gz
}

if [ ! -f index.db -o ! -f files.txt ]; then
    echo "Creating database"
    clic_clear index.db
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

for i in `cat filesadded.txt`; do
    add_to_index $i
done

# Remove removed files
for i in `comm -23 files.txt files2.txt`; do
    remove_from_index $i
done

# Update modified files
for i in `cat files.txt`; do
    if [ -f $i -a $i -nt files.txt ]; then
        remove_from_index $i
        add_to_index $i
    fi
done

rm filesadded.txt
mv files2.txt files.txt
