clang_indexer: clang_indexer.cpp
	g++ -I/Users/exclipy/src/llvm/tools/clang/include -L/opt/local/lib -lclang clang_indexer.cpp -o clang_indexer
