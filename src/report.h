/*
 *  Created by Martin Mroz on 3/21/15.
 *
 *  The author of the original program disclaims copyright.
 *  D language support Copyright (GPL) Serge Vakulenko <serge@vak.ru>.
 *  Copyright (GPL) (c) 2015 Martin Mroz <martinmroz@gmail.com>
 *  See LICENSE for additional information.
 */

#ifndef __lemon_report_h__
#define __lemon_report_h__

#include "lemon.h"

#include "struct.h"

void Reprint(struct lemon *);
void ReportOutput(struct lemon *);
void ReportTable(struct lemon *, int);
void ReportHeader(struct lemon *);
void CompressTables(struct lemon *);
void ResortStates(struct lemon *);

#endif /* defined(__lemon_report_h__) */
