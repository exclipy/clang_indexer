all: clang_index_linker clang_indexer

clang_index_linker: clang_index_linker.o clang_index_parser.o
	$(CXX) clang_index_linker.o clang_index_parser.o -o clang_index_linker

clang_indexer: clang_indexer.cpp
	$(CXX) -lclang clang_indexer.cpp -o clang_indexer
