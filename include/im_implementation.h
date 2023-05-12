#ifndef IM_IMPLEMENTATION_H
#define IM_IMPLEMENTATION_H

#include "ast_semantic.h"


#define Convert_Bool            0x01
#define Convert_Int             0x02
#define Convert_Float           0x03
#define Convert_Str             0x04

#define Delete_Scope_None       0x00
#define Delete_Scope_Func       0x01

#define BlockType_Convert       0x01
#define BlockType_Arithmetic    0x02
#define BlockType_Set           0x03

#define BlockType_If_not        0x11
#define BlockType_If_not_del    0x12
#define BlockType_If            0x13
#define BlockType_If_del        0x14

#define BlockType_Put           0x21
#define BlockType_Delete_Temp   0x22
#define BlockType_Delete_Scope  0x23
#define BlockType_Return        0x33

#define BlockType_Tuple         0x40
#define BlockType_List          0x41
#define BlockType_Attr          0x42
#define BlockType_Subs          0x43
#define BlockType_Call          0x44
#define BlockType_Print         0x45
#define BlockType_ForNext       0x46
#define BlockType_Is            0x47


void interpretation(struct imp_parser *parser, int stream);

#endif //IM_IMPLEMENTATION_H
