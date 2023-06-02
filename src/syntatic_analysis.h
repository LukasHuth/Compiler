#ifndef H_SYNTATIC_ANALYSIS
#define H_SYNTATIC_ANALYSIS

#include "AST.h"

typedef struct {
    int whilecount;
    int forcount;
    bool in_function;
    int ifcount;
    bool in_else;
    bool returned;
} SYNTATIC_ANALYSIS_STATE;

#include "symbol_table.h"

void syntatic_analysis(AST *node);

#endif