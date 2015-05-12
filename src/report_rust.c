//
//  report_rust.c
//  lemon
//
//  Created by Martin Mroz on 5/10/15.
//  Copyright (c) 2015 Martin Mroz. All rights reserved.
//

#include "report_rust.h"

#include "error.h"


int ReportRustWriteTokenMajorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const tokenType)
{
    char *localTokenTypeName = NULL;
    if (tokenType && strlen(tokenType) > 0) {
        localTokenTypeName = tokenType;
    } else {
        LogMsg(LOGLEVEL_WARNING, inputFilename, inputLineno, "No token type specified; in Rust, this is required. Defaulting to 'i32'.");
        localTokenTypeName = "i32";
    }

    fprintf(fp,"type TokenMajor = %s;\n", localTokenTypeName);
    ++lineno;
    return lineno;
}

int ReportRustWriteTokenMinorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const* types, int typeCount, int hasErrorSymbol, int errorDataTypeNumber)
{
    /* 
     * Unlike other languages, Rust has no concept of a union (due to inherent lack of safety).
     * As a result, the token minor type is a structure. 
     * TODO: Consider re-writing as an enum type, which is implemented as a discriminated union.
     */
    fprintf(fp, "pub struct TokenMinor {\n");
    ++lineno;

    /* Emit the required yy0 base token. */
    fprintf(fp, "  pub yy0: TokenMajor,\n");
    ++lineno;

    /* Emit all user defined tokens. */
    for (int i = 0; i < typeCount; ++i) {
        if (types[i] != NULL) {
            fprintf(fp, "  pub yy%d: %s,\n", i + 1, types[i]);
            ++lineno;
        }
    }

    /* Emit the error token if available. */
    if (hasErrorSymbol) {
        fprintf(fp, "  pub yy%d: i32,\n", errorDataTypeNumber);
        lineno++;
    }

    /* Close the token structure. */
    fprintf(fp, "}\n");
    ++lineno;
    return lineno;
}
