all: clang_index_linker clang_indexer

HEADERS = \
	types.hpp \
	clang_index_parser.hpp \
	clang_index_printer.hpp

INDEXER_OBJECTS = \
	clang_indexer.o \
	clang_index_printer.o

LINKER_OBJECTS = \
	clang_index_linker.o \
	clang_index_parser.o \
	clang_index_printer.o

CXXFLAGS = -g

clang_indexer: $(INDEXER_OBJECTS) $(HEADERS)
	$(CXX) -lclang -lboost_iostreams -lz $(INDEXER_OBJECTS) -o $@

clang_index_linker: $(LINKER_OBJECTS) $(HEADERS)
	$(CXX) -lboost_iostreams -lz $(LINKER_OBJECTS) -o $@

clean:
	rm *.o clang_indexer clang_index_linker
