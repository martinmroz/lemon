//
//  report_rust.c
//  lemon
//
//  Created by Martin Mroz on 5/10/15.
//  Copyright (c) 2015 Martin Mroz. All rights reserved.
//

#include "report_rust.h"

#include "error.h"


int ReportRustWriteTokenMinorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const tokenType, char* const* types, int typeCount, int hasErrorSymbol, int errorDataTypeNumber)
{
    char *localTokenTypeName = NULL;
    if (tokenType && strlen(tokenType) > 0) {
        localTokenTypeName = tokenType;
    } else {
        LogMsg(LOGLEVEL_WARNING, inputFilename, inputLineno, "No token type specified; in Rust, this is required.");
        LogMsg(LOGLEVEL_WARNING, inputFilename, inputLineno, "Use the %token_type directive to do so. Defaulting to 'i32'.");
        localTokenTypeName = "i32";
    }
    
    fprintf(fp, "pub type ParseTokenType = %s;\n", localTokenTypeName);
    ++lineno;
    
    /* 
     * Unlike C-derived languages, Rust has no concept of a union (due to inherent lack of safety).
     * As a result, the token minor type is an enumeration with associated values, which are 
     * destructured as part of the reduce step.
     *
     * YYInvalidToken is used when no reasonable minor value exists. 
     * YY0 is used to store the value passed into parse().
     */
    fprintf(fp, "enum TokenMinor {\n");
    fprintf(fp, "  YYInvalidToken,");
    fprintf(fp, "  YY0(ParseTokenType),\n");
    lineno += 3;

    /* Emit all user defined tokens. */
    for (int i = 0; i < typeCount; ++i) {
        if (types[i] != NULL) {
            fprintf(fp, "  YY%d(%s),\n", i + 1, types[i]);
            ++lineno;
        }
    }

    /* Emit the error token if available. */
    if (hasErrorSymbol) {
        fprintf(fp, "  YY%d(i32),\n", errorDataTypeNumber);
        ++lineno;
    }

    /* Close the token structure. */
    fprintf(fp, "}\n");
    ++lineno;
    return lineno;
}

int ReportRustWriteTokenMajorDeclaration(int lineno, FILE* fp, char *const prefix, struct symbol **symbols, int const terminalCount)
{
    /* The major tokens in the Rust parser are declared as CodeNumbers stored in the token module. */
    fprintf(fp, "pub type TokenMajor = CodeNumber;\n");
    fprintf(fp, "pub mod token {\n");
    fprintf(fp, "  use super::TokenMajor;\n");
    fprintf(fp, "\n");
    fprintf(fp, "  /* Numerical value of each major terminal token. */\n");
    lineno += 5;
    
    /* The first terminal symbol ('$') is the reserved EOF token. */
    fprintf(fp, "  pub const __RESERVED_EOF__: TokenMajor = 0;\n");
    ++lineno;

    /* The remaining terminals are in the symbols list. */
    for (int i = 1; i < terminalCount; ++i) {
        fprintf(fp, "  pub const %s%-30s: TokenMajor = %2d;\n", prefix, symbols[i]->name, i);
        ++lineno;
    }
    
    /* Close the module. */
    fprintf(fp, "}\n");
    ++lineno;
    return lineno;
}
