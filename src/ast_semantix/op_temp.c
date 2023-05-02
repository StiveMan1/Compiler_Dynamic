#include <stdio.h>
#include "struct.h"
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


void run_an(struct op_state *state, struct object_st *object) {
    struct array_st *code_operations = state->code_operations;
    struct node_st *node = object->data;
    struct op_attrib *attrib = NULL;
    struct op_block *new_block = NULL;

    if (node->main_type == MainType_Expr) {
        struct array_st *temp_array = node->next;
        switch (node->type) {
            case PrimType_List: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_List;
                new_block->count = temp_array->size;

                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Tuple: {
                array_add_new(state->temp_memory, MAP_TYPE);
                for (size_t i = 0; i < temp_array->size; i++) {
                    // temp_array will store StmtType_Annot
                    //  created during AST build
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Ident_get: {
                attrib = node->data->data;
//                print_obj(attrib->data, 0);
                array_append(state->temp_memory, attrib->data);
                break;
            }
            case PrimType_Ident_new: {
                attrib = node->data->data;
                darray_append(array_get_last(state->stack_memory)->data, node->data, attrib->data);
                op_attrib_new_data(attrib);
                array_append(state->temp_memory, attrib->data);
                break;
            }
            case PrimType_Literal: {
                array_append(state->temp_memory, node->data);
                break;
            }
            case PrimType_Attrib: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Attr;
                new_block->data1 = object_copy(node->data);

                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Subscript: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Subs;

                for (size_t i = temp_array->size; i > 0; i--) {
                    array_append(code_operations, temp_array->data[i - 1]);
                }
                break;
            }
            case PrimType_Call: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = (((struct node_st *) temp_array->data[1]->data)->next)->size;

                array_append(code_operations, temp_array->data[0]);
                temp_array = ((struct node_st *) temp_array->data[1]->data)->next;
                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
        }
    }
    if (node->main_type == MainType_Oper) {
        struct array_st *temp_array = node->tokens;
        size_t count = 2;
        if (node->type == ExprType_U || node->type == ExprType_NotTest) count = 1;
        int is_bool = 0;
        for (size_t i = 0; i < temp_array->size; i++) {
            array_add_new(code_operations, OP_BLOCK_TYPE);
            new_block = array_get_last(code_operations)->data;
            new_block->type = BlockType_Arithmetic;
            new_block->subtype = ((struct token_st *) temp_array->data[i]->data)->subtype;
            new_block->count = count;

            if (((struct token_st *) temp_array->data[i]->data)->type == TokenType_KeyWords)
                is_bool = 1;

        }
        temp_array = node->next;
        for (size_t i = temp_array->size; i > 0; i--) {
            if (is_bool) {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Convert;
                new_block->subtype = Convert_Bool;
            }
            array_append(code_operations, temp_array->data[i - 1]);
        }
    }
    if (node->main_type == MainType_Stmt) {
        struct array_st *temp_array = node->next;
        switch (node->type) {
            case StmtType_If: {
                // else
                if (temp_array->size % 2 == 1) {
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_If_not;
                    new_block->data1 = object_copy(temp_array->data[temp_array->size - 1]);
                }
                // else if
                for (size_t i = temp_array->size - (temp_array->size % 2) - 1; i >= 2; i -= 2) {
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_If_not_del;
                    {
                        struct object_st *obj = object_new();
                        object_set_type(obj, OP_BLOCK_TYPE);
                        ((struct op_block *) obj->data)->type = BlockType_If;
                        ((struct op_block *) obj->data)->data1 = object_copy(temp_array->data[i]);
                        new_block->data1 = obj;
                    }
                    new_block->data2 = object_copy(temp_array->data[i - 1]);
                }
                // if
                {
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_If;
                    new_block->data1 = object_copy(temp_array->data[1]);
                }
                array_append(code_operations, temp_array->data[0]);
                break;
            }
            case StmtType_While: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_If_del;
                new_block->data1 = object_copy(object);
                new_block->data2 = object_copy(temp_array->data[1]);

                array_append(code_operations, temp_array->data[0]);
                break;
            }
            case StmtType_Func_Body: {
                array_add_new(state->temp_memory, OP_OBJECT_TYPE);
                op_object_set_function(array_get_last(state->temp_memory)->data, node);
                break;
            }
            case StmtType_Assign:
            case StmtType_Func:
            case StmtType_Decl: {
                temp_array = node->next;
                if (temp_array->size == 2) {
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Set;
                }
                for (size_t i = temp_array->size; i > 0; i--) {
                    array_append(code_operations, temp_array->data[i - 1]);
                }
                break;
            }
            case StmtType_Return: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Return;
                new_block->count = temp_array->size;

                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case StmtType_List: {
                {
                    array_add_new(state->stack_memory, DARRAY_TYPE);

                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Delete_Scope;
                    new_block->subtype = Delete_Scope_None;
                }

                for (size_t i = 0; i < temp_array->size; i++) {

                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Put;
                    {
                        struct object_st *obj = object_new();
                        object_set_type(obj, OP_BLOCK_TYPE);
                        ((struct op_block *) obj->data)->type = BlockType_Delete_Temp;
                        ((struct op_block *) obj->data)->count = state->temp_memory->size;
                        new_block->data1 = obj;
                    }
                    new_block->data2 = object_copy(
                            temp_array->data[temp_array->size - i - 1]);
                }
                break;
            }
            case StmtType_Print: {
                {
                    array_add_new(state->stack_memory, DARRAY_TYPE);

                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Print;
                    new_block->count = temp_array->size;
                }
                temp_array = node->next;
                for (size_t i = temp_array->size; i > 0; i--) {
                    array_append(code_operations, temp_array->data[i - 1]);
                }
                break;
            }
            case StmtType_Annot: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Tuple;
                new_block->data1 = object_copy(node->data);

                if (node->next->size == 1) {
                    array_append(code_operations, node->next->data[0]);
                } else {
                    array_add_new(state->temp_memory, NONE_TYPE);
                }

                break;
            }
        }
    }
}

void run_op(struct op_state *state, struct object_st *object) {
    struct array_st *code_operations = state->code_operations;
    struct op_block *block = object->data;
    struct op_attrib *attrib = NULL;
    struct op_block *new_block = NULL;

    if ((block->type & 0xF0) == 0x10) {
        struct object_st *temp = object_new();
        struct object_st *obj = object_copy(array_get_last(state->temp_memory));

        if (obj->type != BOOL_TYPE) {
            array_append(code_operations, object);

            array_add_new(code_operations, OP_BLOCK_TYPE);
            new_block = array_get_last(code_operations)->data;
            new_block->type = BlockType_Convert;
            new_block->subtype = Convert_Bool;
        } else {
            int res = 0;
            if ((block->type == BlockType_If_not || block->type == BlockType_If_not_del) &&
                bool_is_null(obj->data))
                res = 1;
            if ((block->type == BlockType_If || block->type == BlockType_If_del) && !bool_is_null(obj->data))
                res = 1;

            if (res) {
                if (block->type == BlockType_If_not_del || block->type == BlockType_If_del)
                    array_remove_last(state->temp_memory);
                if (block->data1 != NULL) array_append(code_operations, block->data1);
                if (block->data2 != NULL) array_append(code_operations, block->data2);
            }
        }
        object_free(obj);
        object_free(temp);
        return;
    }

    switch (block->type) {
        case BlockType_Convert: {
            struct object_st *temp = NULL;
            struct object_st *obj = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            if (obj->type == OP_OBJECT_TYPE) {
                struct string_st *ind_str = string_new();

                if (block->subtype == Convert_Bool) string_set_str(ind_str, "__bool__", 8);
                else if (block->subtype == Convert_Int) string_set_str(ind_str, "__int__", 7);
                else if (block->subtype == Convert_Float) string_set_str(ind_str, "__float__", 9);
                else if (block->subtype == Convert_Str) string_set_str(ind_str, "__str__", 7);

                temp = op_object_get_attrib(obj->data, ind_str);
                string_free(ind_str);
            }

            if (temp != NULL) {
                array_append(state->temp_memory, obj);
                array_append(state->temp_memory, temp);

                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = 1;
            } else {
                struct object_st *res = object_new();

                if (block->subtype == Convert_Bool) object__bool(res, obj);
                else if (block->subtype == Convert_Int) object__int(res, obj);
                else if (block->subtype == Convert_Float) object__float(res, obj);
                else if (block->subtype == Convert_Str) object__str(res, obj);

                array_append(state->temp_memory, res);
                object_free(res);
            }
            object_free(obj);
            object_free(temp);
            break;
        }
        case BlockType_Arithmetic: {
            if (block->count == 2) {
                if ((block->subtype & 0xF0) == 0x10) {
                    struct object_st *temp = NULL;
                    struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);

                    if (obj1->type == OP_OBJECT_TYPE) {
                        struct string_st *ind_str = string_new();

                        if (block->subtype == Special_MOD) string_set_str(ind_str, "__mod__", 7);
                        else if (block->subtype == Special_AND) string_set_str(ind_str, "__and__", 7);
                        else if (block->subtype == Special_MUL) string_set_str(ind_str, "__mul__", 7);
                        else if (block->subtype == Special_ADD) string_set_str(ind_str, "__add__", 7);
                        else if (block->subtype == Special_SUB) string_set_str(ind_str, "__sub__", 7);
                        else if (block->subtype == Special_DIV) string_set_str(ind_str, "__div__", 7);
                        else if (block->subtype == Special_XOR) string_set_str(ind_str, "__xor__", 7);
                        else if (block->subtype == Special_OR) string_set_str(ind_str, "__or__", 6);
                        else if (block->subtype == Special_LSHIFT) string_set_str(ind_str, "__ls__", 6);
                        else if (block->subtype == Special_RSHIFT) string_set_str(ind_str, "__rs__", 6);

                        temp = op_object_get_attrib(obj1->data, ind_str);
                        string_free(ind_str);
                    }

                    if (temp != NULL) {
                        array_append(state->temp_memory, obj2);
                        array_append(state->temp_memory, obj1);
                        array_append(state->temp_memory, temp);

                        array_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = array_get_last(code_operations)->data;
                        new_block->type = BlockType_Call;
                        new_block->count = 2;
                    } else {
                        struct object_st *res = object_new();

                        if (block->subtype == Special_MOD) object__mod(res, obj1, obj2);
                        else if (block->subtype == Special_AND) object__and(res, obj1, obj2);
                        else if (block->subtype == Special_MUL) object__mul(res, obj1, obj2);
                        else if (block->subtype == Special_ADD) object__add(res, obj1, obj2);
                        else if (block->subtype == Special_SUB) object__sub(res, obj1, obj2);
                        else if (block->subtype == Special_DIV) object__div(res, obj1, obj2);
                        else if (block->subtype == Special_XOR) object__xor(res, obj1, obj2);
                        else if (block->subtype == Special_OR) object__or(res, obj1, obj2);
                        else if (block->subtype == Special_LSHIFT) object__ls(res, obj1, obj2);
                        else if (block->subtype == Special_RSHIFT) object__rs(res, obj1, obj2);

                        array_append(state->temp_memory, res);
                        object_free(res);
                    }
                    object_free(temp);
                    object_free(obj2);
                    object_free(obj1);
                    break;
                }
                else if ((block->subtype & 0xF0) == 0x40) {
                    struct object_st *temp = NULL;
                    struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);

                    if (obj1->type == OP_OBJECT_TYPE) {
                        struct string_st *ind_str = string_new();


                        if (block->subtype == Special_LESS) string_set_str(ind_str, "__lt__", 6);
                        else if (block->subtype == Special_GREATER) string_set_str(ind_str, "__gt__", 6);
                        else if (block->subtype == Special_EQ_LESS) string_set_str(ind_str, "__le__", 6);
                        else if (block->subtype == Special_EQ_GREATER) string_set_str(ind_str, "__ge__", 6);
                        else if (block->subtype == Special_EQ_NOT || block->subtype == Special_EQ_EQ)
                            string_set_str(ind_str, "__eq__", 6);

                        temp = op_object_get_attrib(obj1->data, ind_str);
                        string_free(ind_str);
                    }

                    if (temp != NULL) {
                        array_append(state->temp_memory, obj2);
                        array_append(state->temp_memory, obj1);
                        array_append(state->temp_memory, temp);

                        if (block->subtype == Special_EQ_NOT) {
                            array_add_new(code_operations, OP_BLOCK_TYPE);
                            new_block = array_get_last(code_operations)->data;
                            new_block->type = BlockType_Arithmetic;
                            new_block->subtype = KeyWord_NOT;
                            new_block->count = 1;
                        }

                        array_add_new(code_operations, OP_BLOCK_TYPE);
                        new_block = array_get_last(code_operations)->data;
                        new_block->type = BlockType_Call;
                        new_block->count = 2;
                    } else {
                        int cmp_res = object_cmp(obj1, obj2);

                        struct object_st *res = object_new();
                        object_set_type(res, BOOL_TYPE);

                        if (
                                (block->subtype == Special_LESS && cmp_res < 0) ||
                                (block->subtype == Special_GREATER && cmp_res != 2 && cmp_res > 0) ||
                                (block->subtype == Special_EQ_LESS && cmp_res <= 0) ||
                                (block->subtype == Special_EQ_GREATER && cmp_res != 2 && cmp_res >= 0) ||
                                (block->subtype == Special_EQ_NOT && cmp_res != 0) ||
                                (block->subtype == Special_EQ_EQ && cmp_res == 0))
                            ((struct bool_st *)res->data)->data = 1;
                        else ((struct bool_st *)res->data)->data = 0;

                        array_append(state->temp_memory, res);
                        object_free(res);
                    }
                    object_free(temp);
                    object_free(obj2);
                    object_free(obj1);
                    break;
                }
                else if ((block->subtype & 0xF0) == 0x50) {
                    struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);
                    struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);

                    int bool1 = 1;
                    if (obj1->type == BOOL_TYPE && bool_is_null(obj1->data)) bool1 = 0;
                    int bool2 = 1;
                    if (obj2->type == BOOL_TYPE && bool_is_null(obj2->data)) bool2 = 0;
                    object_free(obj2);
                    object_free(obj1);

                    struct object_st *res = object_new();
                    object_set_type(res, BOOL_TYPE);
                    if (block->subtype == KeyWord_AND) ((struct bool_st *)res->data)->data = (bool1 && bool2);
                    if (block->subtype == KeyWord_OR) ((struct bool_st *)res->data)->data = (bool1 || bool2);

                    array_append(state->temp_memory, res);
                    object_free(res);
                    break;
                }

            } else if (block->count == 1) {
                struct object_st *temp = NULL;
                struct object_st *obj = object_copy(array_get_last(state->temp_memory));
                array_remove_last(state->temp_memory);
                if (obj->type == OP_OBJECT_TYPE) {
                    struct string_st *ind_str = string_new();

                    string_set_str(ind_str, "__ne__", 6);

                    temp = op_object_get_attrib(obj->data, ind_str);
                    string_free(ind_str);
                }

                if (temp != NULL) {
                    array_append(state->temp_memory, obj);
                    array_append(state->temp_memory, temp);

                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Call;
                    new_block->count = 1;
                } else {
                    struct object_st *res = object_new();

                    object__neg(res, obj);

                    array_append(state->temp_memory, res);
                    object_free(res);
                }
                object_free(temp);
                object_free(obj);
                break;
            }
        }
        case BlockType_Set: {
            struct object_st *temp = NULL;
            struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            if (obj1->type == OP_OBJECT_TYPE) {
                struct string_st *ind_str = string_new();
                string_set_str(ind_str, "__set__", 7);
                temp = op_object_get_attrib(obj1->data, ind_str);
                string_free(ind_str);
            }
            if (temp != NULL) {
                array_append(state->temp_memory, obj2);
                array_append(state->temp_memory, obj1);
                array_append(state->temp_memory, temp);

                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Call;
                new_block->count = 2;
            } else {
                object_set(obj1, obj2);
                array_append(state->temp_memory, obj2);
            }

            object_free(temp);
            object_free(obj2);
            object_free(obj1);
            break;
        }

        case BlockType_Put: {
            if (block->data1 != NULL) array_append(code_operations, block->data1);
            if (block->data2 != NULL) array_append(code_operations, block->data2);
            break;
        }
        case BlockType_Delete_Temp: {
            if (state->temp_memory->size > block->count) array_resize(state->temp_memory, block->count);
            break;
        }
        case BlockType_Delete_Scope: {
            if (block->subtype == Delete_Scope_Func) {
                if (state->return_obj == NULL) array_add_new(state->temp_memory, NONE_TYPE);
                else {
                    array_append(state->temp_memory, state->return_obj);
                    object_free(state->return_obj);
                    state->return_obj = NULL;
                }
            }
//            else if (block->subtype == Delete_Scope_Class) {
//                object_set(array_get_last(state->temp_memory), state->memory->top->data);
//            }
            struct darray_st *closure = array_get_last(state->stack_memory)->data;
            for (size_t i = 0; i < closure->size; i++) {
                attrib = closure->data[0][i]->data;
                object_free(attrib->data);
                attrib->data = object_copy(closure->data[1][i]);
            }
            array_remove_last(state->stack_memory);
            break;
        }

        case BlockType_Return: {
            if (block->count != 0 && state->return_obj == NULL) {
                if (block->count == 1) {
                    state->return_obj = object_copy(array_get_last(state->temp_memory));
                    array_remove_last(state->temp_memory);
                } else {
                    state->return_obj = object_new();
                    object_set_type(state->return_obj, ARRAY_TYPE);
                    for (size_t i = 0; i < block->count; i++) {
                        array_append(state->return_obj->data, array_get_last(state->temp_memory));
                        array_remove_last(state->temp_memory);
                    }
                }
            }
            struct object_st *current_object = NULL;
            while (code_operations->size) {
                current_object = array_get_last(code_operations);
                if (current_object->type == OP_BLOCK_TYPE) {
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
                        array_remove_last(state->temp_memory);
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Scope) break;
                }
                array_remove_last(code_operations);
            }
            break;
        }

        case BlockType_List: {
            struct object_st *obj = object_new();
            object_set_type(obj, ARRAY_TYPE);
            for (size_t i = 0; i < block->count; i++) {
                array_append(obj->data, array_get_last(state->temp_memory));
                array_remove_last(state->temp_memory);
            }
            array_append(state->temp_memory, obj);
            object_free(obj);
            break;
        }
        case BlockType_Tuple: {
            struct object_st *obj = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct object_st *map = array_get_last(state->temp_memory);
            struct object_st *temp = map_set_elm(map->data, ((struct string_st *)block->data1->data)->data, ((struct string_st *)block->data1->data)->size);
            object_set(temp, obj);

            object_free(temp);
            object_free(obj);
            break;
        }
        case BlockType_Attr: {
            struct string_st *ind_str = block->data1->data;
            struct object_st *obj = object_copy(array_get_last(state->temp_memory));

            if (array_get_last(code_operations)->type != OP_BLOCK_TYPE ||
                ((struct op_block *) array_get_last(code_operations)->data)->type != BlockType_Call) {
                array_remove_last(state->temp_memory);
            } else {
                new_block = array_get_last(code_operations)->data;
                new_block->count++;
            }

            if (obj->type == OP_OBJECT_TYPE) {
                struct object_st *res = op_object_set_attrib(obj->data, ind_str);
                array_append(state->temp_memory, res);
                object_free(res);
            } else if (obj->type == MAP_TYPE) {
                struct object_st *res = map_set_elm(obj->data, ind_str->data, ind_str->size);
                array_append(state->temp_memory, res);
                object_free(res);
            } else {
                array_add_new(state->temp_memory, NONE_TYPE);
            }

            object_free(obj);
            break;
        }
        case BlockType_Subs: {
            struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            if (obj1->type == OP_OBJECT_TYPE) {
                // TODO error if not string
                struct object_st *res = op_object_set_attrib(obj1->data, obj2->data);
                array_append(state->temp_memory, res);
                object_free(res);
            } else if(obj1->type == ARRAY_TYPE) {
                // TODO error if not integer
                int pos = ((struct integer_st *)obj2->data)->data;
                struct array_st *array = obj1->data;
                array_append(state->temp_memory, array->data[pos % array->size]);
            } else if(obj1->type == MAP_TYPE) {
                // TODO error if not string
                struct object_st *res = op_object_set_attrib(obj1->data, obj2->data);
                array_append(state->temp_memory, res);
                object_free(res);
            } else {
                array_add_new(state->temp_memory, NONE_TYPE);
            }

            object_free(obj1);
            object_free(obj2);
            break;
        }
        case BlockType_Call: {
            struct object_st *res = NULL;
            struct string_st *ind_str = string_new();
            struct object_st *func = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);


            {
                array_add_new(state->stack_memory, DARRAY_TYPE);

                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Delete_Scope;
                new_block->subtype = Delete_Scope_Func;
            }

            if (func->type == OP_OBJECT_TYPE) {
                string_set_str(ind_str, "__init__", 8);
                res = op_object_get_attrib(func->data, ind_str);
            }
            int ok = 0;
            if (res == NULL) {
                if (func->type == OP_OBJECT_TYPE) {
                    string_set_str(ind_str, "__params__", 10);
                    res = op_object_get_attrib(func->data, ind_str);
                }
                if (res != NULL) {
                    struct array_st *temp_array = ((struct node_st *) res->data)->next;
                    struct node_st *node = NULL;
                    if (temp_array->size == block->count) {
                        ok = 1;
                        for (int i = 0; i < temp_array->size; i++) {
                            node = temp_array->data[i]->data;

                            attrib = node->data->data;
                            darray_append(array_get_last(state->stack_memory)->data, node->data, attrib->data);
                            op_attrib_new_data(attrib);
                            object_set(attrib->data, array_get_last(state->temp_memory));
                            array_remove_last(state->temp_memory);
                        }
                    }
                }
                object_free(res);
                res = NULL;

                if (func->type == OP_OBJECT_TYPE && ok) {
                    string_set_str(ind_str, "__closure__", 11);
                    res = op_object_get_attrib(func->data, ind_str);
                }
                if (res != NULL && ok) {
                    struct darray_st *temp_array = res->data;
                    if (temp_array != NULL) {
                        for (int i = 0; i < temp_array->size; i++) {
                            attrib = temp_array->data[0][i]->data;
                            darray_append(array_get_last(state->stack_memory)->data, temp_array->data[0][i],
                                          attrib->data);
                            op_attrib_set_data(attrib, temp_array->data[1][i]);
                        }
                    }
                }
                object_free(res);
                res = NULL;

                if (func->type == OP_OBJECT_TYPE && ok) {
                    string_set_str(ind_str, "__call__", 7);
                    res = op_object_get_attrib(func->data, ind_str);
                }

                if (res != NULL && ok) {
                    array_append(code_operations, res);
                }
                object_free(res);
            }

            string_free(ind_str);
            object_free(func);
            break;
        }
        case BlockType_Print: {
            struct object_st *obj;
            for (size_t i = 0; i < block->count; i++) {
                obj = object_copy(array_get_last(state->temp_memory));
                array_remove_last(state->temp_memory);
                print_obj(obj, 0);
            }
            break;
        }

    }
}

void run_interpreter(struct object_st *expr_obj) {
    struct op_state *state = op_state_new();
    array_append(state->code_operations, expr_obj);
    {
        struct array_st *code_operations = state->code_operations;
        struct object_st *current_object = NULL;
        while (code_operations->size > 0) {
            current_object = object_copy(array_get_last(code_operations));
            array_remove_last(code_operations);

            if (current_object->type == NODE_TYPE) {
                run_an(state, current_object);
            }
            if (current_object->type == OP_BLOCK_TYPE) {
                run_op(state, current_object);
            }
            object_free(current_object);
        }
    }
    op_state_free(state);
}
