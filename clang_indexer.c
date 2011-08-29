#include <clang-c/Index.h>

enum CXChildVisitResult visitor(
        CXCursor cursor,
        CXCursor parent,
        CXClientData client_data)
{
    CXFile file;
    unsigned int line, column, offset;
    clang_getInstantiationLocation(
            clang_getCursorLocation(cursor),
            &file, &line, &column, &offset);

    CXCursor refCursor = clang_getCursorReferenced(cursor);
    CXFile refFile;
    unsigned int refLine, refColumn, refOffset;
    clang_getInstantiationLocation(
            clang_getCursorLocation(refCursor),
            &refFile, &refLine, &refColumn, &refOffset);

    if (clang_getCursorKind(cursor) == CXCursor_TypeRef) {
        printf("'%s' at %s:%d:%d refers to %s at %s:%d:%d\n",
                clang_getCString(clang_getCursorSpelling(cursor)),
                clang_getCString(clang_getFileName(file)),
                line, column,
                clang_getCString(clang_getCursorUSR(refCursor)),
                clang_getCString(clang_getFileName(refFile)),
                refLine, refColumn
                );
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

    int n = clang_getNumDiagnostics(tu);
    if (n > 0) {
        int nErrors = 0;
        for (unsigned i = 0; i != n; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(tu, i);
            CXString string = clang_formatDiagnostic(
                diag,
                clang_defaultDiagnosticDisplayOptions());
            fprintf(stderr, "%s\n", clang_getCString(string));
            clang_disposeString(string);
            if (clang_getDiagnosticSeverity(diag) == CXDiagnostic_Error
                    || clang_getDiagnosticSeverity(diag) == CXDiagnostic_Fatal)
                nErrors++;
        }
        if (nErrors) {
            clang_disposeTranslationUnit(tu);
            clang_disposeIndex(index);
            return 1;
        }
    }

    ;
    clang_visitChildren(
            clang_getTranslationUnitCursor(tu),
            &visitor,
            0);


    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
    return 0;
}
