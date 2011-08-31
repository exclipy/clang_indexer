all: clang_index_linker clang_indexer

clang_index_linker: clang_index_linker.cpp
	$(CXX) clang_index_linker.cpp -o clang_index_linker

clang_indexer: clang_indexer.cpp
	$(CXX) clang_indexer.cpp -o clang_indexer
