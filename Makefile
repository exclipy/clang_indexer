clang_indexer: clang_indexer.c
	g++ -I/Users/exclipy/src/llvm/tools/clang/include -L/opt/local/lib -lclang clang_indexer.c -o clang_indexer
