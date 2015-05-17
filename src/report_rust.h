//
//  report_rust.h
//  lemon
//
//  Created by Martin Mroz on 5/10/15.
//  Copyright (c) 2015 Martin Mroz. All rights reserved.
//

#ifndef __lemon_report_rust__
#define __lemon_report_rust__

#include "lemon.h"
#include "struct.h"

/**
 * Emits the minor token type declaration to the output file in Rust.
 * @param inputFilename The name of the Lemon input file.
 * @param inputLineno The line number within the Lemon file corresponding to the token minor declaration.
 * @param lineno The current line number within the output Rust file.
 * @param fp The output file to write the declaration into.
 * @param tokenType The literal name of the token type passed into the parse() function.
 * @param types The array of data types to include within the minor token union.
 * @param typeCount The number of types.
 * @param hasErrorSymbol Indication (0 or 1) that the grammar includes an error symbol.
 * @param errorDataTypeNumber Index into `types` corresponding to the data type of the error symbol.
 * @return The new line number within the output file (lineno + number of lines written to the output file).
 */
int ReportRustWriteTokenMinorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const tokenType, char* const* types, int typeCount, int hasErrorSymbol, int errorDataTypeNumber);

/**
 * Emits the major token type declarations to the output file in Rust.
 * @param lineno The current line number within the output Rust file.
 * @param fp The output file to write the declaration into.
 * @param prefix The string to prepend to each token definition.
 * @param symbols The array of terminal and non-terminal symbols in the grammar.
 * @param terminalCount The number of terminal symbols. Symbols[0] is the ('$') EOF token.
 * @return The new line number within the output file (lineno + number of lines written to the output file).
 */
int ReportRustWriteTokenMajorDeclaration(int lineno, FILE* fp, char *const prefix, struct symbol **symbols, int const terminalCount);

#endif /* defined(__lemon_report_rust__) */
