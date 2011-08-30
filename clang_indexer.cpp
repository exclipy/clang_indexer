extern "C" {
#include <clang-c/Index.h>
}

#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <tr1/unordered_set>
#include <tr1/unordered_map>

struct VisitorContext {
    typedef std::tr1::unordered_map< std::string, std::tr1::unordered_set<std::string> > IndexType;
    IndexType usrToReferences;
};

enum CXChildVisitResult visitor(
        CXCursor cursor,
        CXCursor parent,
        CXClientData clientData)
{
    VisitorContext* visitorContext = (VisitorContext*)clientData;
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
            visitorContext->usrToReferences[referencedUsr].insert(location);
        }
    }
    return CXChildVisit_Recurse;
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
        if (nErrors) // quit if we found any errors
            return 1;
    }

    VisitorContext visitorContext;
    clang_visitChildren(
            clang_getTranslationUnitCursor(tu),
            &visitor,
            &visitorContext);

    BOOST_FOREACH(const VisitorContext::IndexType::value_type &it, visitorContext.usrToReferences) {
        std::cout << it.first << '\t';
        BOOST_FOREACH(const std::string& jt, it.second) {
            std::cout << jt << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
