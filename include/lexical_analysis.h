#ifndef TK_TOKENIZE_H
#define TK_TOKENIZE_H

#include "lexical/la_identifier.h"
#include "lexical/la_integer.h"
#include "lexical/la_keyword.h"
#include "lexical/la_special.h"
#include "lexical/la_string.h"

void tokenize_parse(struct token_st *token, struct la_parser *parser);
void tokenize(struct la_parser *parser);

#endif //TK_TOKENIZE_H
