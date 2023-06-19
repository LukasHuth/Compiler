#ifndef H_PARSER
#define H_PARSER
#include <iostream>
#include "lexer.h"
#include "AST.h"
namespace Parser
{
  AST *parse(Lexer::Lexer *lexer);
}
#endif