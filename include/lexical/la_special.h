#ifndef LA_SPECIAL_H
#define LA_SPECIAL_H

#include "struct.h"



// Special Chars
#define Special_None        0x00

// Special Chars OP
#define Special_MOD         0x11    // %
#define Special_AND         0x12    // &
#define Special_MUL         0x13    // *
#define Special_ADD         0x14    // +
#define Special_SUB         0x15    // -
#define Special_DIV         0x16    // /
#define Special_XOR         0x17    // ^
#define Special_OR          0x18    // |
#define Special_LSHIFT      0x19    // <<
#define Special_RSHIFT      0x1a    // >>


// Special Chars Brackets
#define Special_LSB         0x31    // (
#define Special_RSB         0x32    // )
#define Special_LSQB        0x33    // [
#define Special_RSQB        0x34    // ]
#define Special_LCB         0x35    // {
#define Special_RCB         0x36    // }

// Special Chars Compare
#define Special_LESS        0x41    // <
#define Special_GREATER     0x42    // >
#define Special_EQ_LESS     0x43    // <=
#define Special_EQ_GREATER  0x44    // >=
#define Special_EQ_NOT      0x45    // /=
#define Special_EQ_EQ       0x46    // =


// Special Chars Comments
#define Special_COM_LINE    0x61    // //
#define Special_COM_STR     0x62    // */
#define Special_COM_END     0x63    // /*


#define Special_COMMA       0x01    // ,
#define Special_DOT         0x02    // .
#define Special_COLON       0x03    // :
#define Special_SEMI        0x04    // ;
#define Special_EQ          0x05    // :=



void la_special(struct token_st *, struct la_parser *);

#endif //LA_SPECIAL_H
