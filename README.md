CLIC: Clang Indexer for C/C++
=============================

This is an indexer for C and C++ based on the libclang library.

There are several parts to it:

* The clic_add, clic_rm, clic_clear utilities for updating a database of symbols
* The clic_update.sh bash script, which runs the aformentioned utilities as required when the
  sources change
* A Vim plugin, hosted at https://github.com/exclipy/clang_complete, which let's the user query the
  database for references to the symbol under the cursor in Vim.

For further instructions, please consult this blog post:
http://blog.wuwon.id.au/2011/10/vim-plugin-for-navigating-c-with.html

Please note that this is an early stage of a little project that I've been doing just to make my own
life easier.  It has very sharp edges and I haven't really invested much effort in making it easy to
use.  Use at your own peril, it will destroy your code, format your hard drive, eat your children,
etc.
