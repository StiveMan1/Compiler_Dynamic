#ifndef LA_IDENTIFIER_H
#define LA_IDENTIFIER_H

#include "struct.h"


#define SpaceChar(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IdentifierStart(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || (c) == '_')
#define IdentifierChar(c) (IdentifierStart(c) || ((c) >= '0' && (c) <= '9'))

void la_identifier(struct token_st *, struct la_parser *);

#endif //LA_IDENTIFIER_H
