CLIC: Clang Indexer for C/C++/Obj-C
===================================

This is an indexer for C, C++ and Objective-C using the libclang library.

There are several parts to it:

* The clic_add, clic_rm, clic_clear utilities for updating a database of symbols
* The dbupdate.sh bash script, which runs the aformentioned utilities as required when the sources
  change
* A Vim plugin, hosted at https://github.com/exclipy/clang_complete, which let's the user query
  the database for references to the symbol under the cursor in Vim.

Note that this project is in a very early state and is definitely not ready for use yet.
