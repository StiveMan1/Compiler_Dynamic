#ifndef LA_KEYWORD_H
#define LA_KEYWORD_H

#include "struct.h"

// Not Defined Keyword
#define KeyWord_None            0x00

// Keywords length 2
#define KeyWord_IF              0x01
#define KeyWord_IN              0x02
#define KeyWord_IS              0x03
#define KeyWord_OR              0x04

// Keywords length 3
#define KeyWord_AND             0x05
#define KeyWord_END             0x06
#define KeyWord_FOR             0x07
#define KeyWord_INT             0x08
#define KeyWord_NOT             0x09
#define KeyWord_VAR             0x0a
#define KeyWord_XOR             0x0b

// Keywords length 4
#define KeyWord_BOOL            0x0c
#define KeyWord_ELSE            0x0d
#define KeyWord_FUNC            0x0e
#define KeyWord_LOOP            0x0f
#define KeyWord_REAL            0x10
#define KeyWord_THEN            0x11
#define KeyWord_TRUE            0x12

// Keywords length 5
#define KeyWord_EMPTY           0x13
#define KeyWord_FALSE           0x14
#define KeyWord_PRINT           0x15
#define KeyWord_WHILE           0x16

// Keywords length 6
#define KeyWord_RETURN          0x17
#define KeyWord_STRING          0x18


void la_keyword(struct token_st *, struct la_parser *);

#endif //LA_KEYWORD_H
