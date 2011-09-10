extern "C" {
#include <clang-c/Index.h>
}

#include "clang_index_printer.hpp"
#include "types.hpp"

#include <boost/foreach.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <iostream>
#include <string>
#include <sstream>

// This code intentionally leaks memory like a sieve because the program is shortlived.

class IVisitor {
public:
    virtual enum CXChildVisitResult visit(CXCursor cursor, CXCursor parent) = 0;
};

class EverythingIndexer : public IVisitor {
public:
    virtual enum CXChildVisitResult visit(CXCursor cursor, CXCursor parent) {
        CXCursor refCursor = clang_getCursorReferenced(cursor);
        if (!clang_equalCursors(refCursor, clang_getNullCursor())) {
            CXFile file;
            unsigned int line, column, offset;
            clang_getInstantiationLocation(
                    clang_getCursorLocation(cursor),
                    &file, &line, &column, &offset);

            CXFile refFile;
            unsigned int refLine, refColumn, refOffset;
            clang_getInstantiationLocation(
                    clang_getCursorLocation(refCursor),
                    &refFile, &refLine, &refColumn, &refOffset);

            if (clang_getFileName(file).data && clang_getFileName(refFile).data) {
                std::string referencedUsr(clang_getCString(clang_getCursorUSR(refCursor)));
                std::stringstream ss;
                ss << clang_getCString(clang_getFileName(file))
                   << ":" << line;
                std::string location(ss.str());
                usrToReferences[referencedUsr].insert(location);
            }
        }
        return CXChildVisit_Recurse;
    }

    Index usrToReferences;
};

class DefinitionIndexer : public EverythingIndexer {
public:
    virtual enum CXChildVisitResult visit(CXCursor cursor, CXCursor parent) {
        if (!clang_isCursorDefinition(cursor))
            return CXChildVisit_Recurse;
        return EverythingIndexer::visit(cursor, parent);
    }
};

enum CXChildVisitResult visitorFunction(
        CXCursor cursor,
        CXCursor parent,
        CXClientData clientData)
{
    IVisitor* visitor = (IVisitor*)clientData;
    return visitor->visit(cursor, parent);
}

int main(int argc, const char* argv[]) {
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(
        index, 0,
        argv, argc,
        0, 0,
        CXTranslationUnit_None);

    // Print any errors or warnings
    int n = clang_getNumDiagnostics(tu);
    if (n > 0) {
        int nErrors = 0;
        for (unsigned i = 0; i != n; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(tu, i);
            CXString string = clang_formatDiagnostic(
                diag,
                clang_defaultDiagnosticDisplayOptions());
            fprintf(stderr, "%s\n", clang_getCString(string));
            if (clang_getDiagnosticSeverity(diag) == CXDiagnostic_Error
                    || clang_getDiagnosticSeverity(diag) == CXDiagnostic_Fatal)
                nErrors++;
        }
    }

    // Pick how much to index here:
    // DefinitionIndexer visitor;
    EverythingIndexer visitor;

    clang_visitChildren(
            clang_getTranslationUnitCursor(tu),
            &visitorFunction,
            &visitor);

    boost::iostreams::filtering_stream<boost::iostreams::output> zout;
    zout.push(boost::iostreams::gzip_compressor());
    zout.push(std::cout);

    printIndex(zout, visitor.usrToReferences);

    return 0;
}
