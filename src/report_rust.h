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

int ReportRustWriteTokenMajorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const tokenType);
int ReportRustWriteTokenMinorDeclaration(char *const inputFilename, int inputLineno, int lineno, FILE* fp, char* const* types, int typeCount, int hasErrorSymbol, int errorDataTypeNumber);

#endif /* defined(__lemon_report_rust__) */
