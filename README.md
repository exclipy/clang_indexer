clang_indexer
=============

This is an indexer for C, C++ and Objective-C using the libclang library.

There are several parts to it:

* clang_indexer, which takes a source file and produces an index.
* clang_index2db, which merges multiple index files into a Berkeley DB database
* A Vim plugin, hosted at https://github.com/exclipy/clang_complete, which let's the user query
  the database for references to the symbol under the cursor in Vim.

Note that this project is in a very early state and is definitely not ready for use yet.
