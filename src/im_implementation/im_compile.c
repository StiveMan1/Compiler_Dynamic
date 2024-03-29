#include "im_implementation.h"


void run_an(struct imp_parser *state, struct object_st *object) {
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
                op_block_set_position_node(new_block, node);

                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Tuple: {
                array_add_new(state->temp_memory, MAP_TYPE);
                for (size_t i = temp_array->size; i > 0; i--) {
                    // temp_array will store StmtType_Annot
                    //  created during AST build
                    array_append(code_operations, temp_array->data[i - 1]);
                }
                break;
            }
            case PrimType_Ident_get: {
                attrib = node->data->data;
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
                op_block_set_position_node(new_block, node);

                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Subscript: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Subs;
                op_block_set_position_node(new_block, node);

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
                op_block_set_position_node(new_block, node);

                array_append(code_operations, temp_array->data[0]);
                temp_array = ((struct node_st *) temp_array->data[1]->data)->next;
                for (size_t i = 0; i < temp_array->size; i++) {
                    array_append(code_operations, temp_array->data[i]);
                }
                break;
            }
            case PrimType_Is: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Is;
                new_block->subtype = ((struct node_st *)temp_array->data[1]->data)->type;
                op_block_set_position_node(new_block, node);
                array_append(code_operations, temp_array->data[0]);
                break;
            }
            case PrimType_Lambda: {
                array_append(code_operations, temp_array->data[0]);
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
            op_block_set_position_token(new_block, temp_array->data[i]->data);

            if (((struct token_st *) temp_array->data[i]->data)->type == TokenType_KeyWords) {
                if (ExprType_XorTest == node->type) new_block->subtype = Special_XOR;
                if (ExprType_AndTest == node->type) new_block->subtype = Special_AND;
                if (ExprType_OrTest == node->type) new_block->subtype = Special_OR;
                is_bool = 1;
            }

        }
        temp_array = node->next;
        for (size_t i = temp_array->size; i > 0; i--) {
            if (is_bool) {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Convert;
                new_block->subtype = Convert_Bool;
                op_block_set_position_node(new_block, temp_array->data[i - 1]->data);
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
                array_add_new(state->temp_memory, FUNC_TYPE);
                func_set_function(array_get_last(state->temp_memory)->data, node);
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
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Print;
                    new_block->count = temp_array->size;
                }
                temp_array = node->next;
                for (size_t i = temp_array->size; i > 0; i--) {
                    array_add_new(code_operations, OP_BLOCK_TYPE);
                    new_block = array_get_last(code_operations)->data;
                    new_block->type = BlockType_Convert;
                    new_block->subtype = Convert_Str;
                    op_block_set_position_node(new_block, temp_array->data[i - 1]->data);

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
            case StmtType_For: {
                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_ForNext;
                new_block->data1 = object_copy(node->next->data[2]);

                array_append(code_operations, node->next->data[0]);
                array_append(code_operations, node->next->data[1]);
                break;
            }
        }
    }
}

void run_op(struct imp_parser *state, struct object_st *object, int stream) {
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
            op_block_set_position_block(new_block, block);
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

            struct object_st *res = object_new();

            if (block->subtype == Convert_Bool) object__bool(res, state->error_obj, obj);
            else if (block->subtype == Convert_Int) object__int(res, state->error_obj, obj);
            else if (block->subtype == Convert_Float) object__float(res, state->error_obj, obj);
            else if (block->subtype == Convert_Str) object__str(res, state->error_obj, obj);

            if (state->error_obj->present) {
                state->error_obj->pos = block->pos;
                state->error_obj->line_num = block->line_num;
                state->error_obj->line_pos = block->line_pos;
            }

            array_append(state->temp_memory, res);
            object_free(res);
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

                    struct object_st *res = object_new();

                    if (block->subtype == Special_MOD) object__mod(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_AND) object__and(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_MUL) object__mul(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_ADD) object__add(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_SUB) object__sub(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_DIV) object__div(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_XOR) object__xor(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_OR) object__or(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_LSHIFT) object__ls(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == Special_RSHIFT) object__rs(res, state->error_obj, obj1, obj2);

                    if (state->error_obj->present) {
                        state->error_obj->pos = block->pos;
                        state->error_obj->line_num = block->line_num;
                        state->error_obj->line_pos = block->line_pos;
                    }

                    array_append(state->temp_memory, res);
                    object_free(res);
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

                    int cmp_res = object_cmp(state->error_obj, obj1, obj2);
                    if (state->error_obj->present) {
                        state->error_obj->pos = block->pos;
                        state->error_obj->line_num = block->line_num;
                        state->error_obj->line_pos = block->line_pos;
                    }

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
                    if (block->subtype == KeyWord_AND) object__and(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == KeyWord_XOR) object__xor(res, state->error_obj, obj1, obj2);
                    else if (block->subtype == KeyWord_OR) object__or(res, state->error_obj, obj1, obj2);

                    if (state->error_obj->present) {
                        state->error_obj->pos = block->pos;
                        state->error_obj->line_num = block->line_num;
                        state->error_obj->line_pos = block->line_pos;
                    }

                    array_append(state->temp_memory, res);
                    object_free(res);
                    break;
                }

            } else if (block->count == 1) {
                struct object_st *temp = NULL;
                struct object_st *obj = object_copy(array_get_last(state->temp_memory));
                array_remove_last(state->temp_memory);

                struct object_st *res = object_new();

                object__neg(res, state->error_obj, obj);

                if (state->error_obj->present) {
                    state->error_obj->pos = block->pos;
                    state->error_obj->line_num = block->line_num;
                    state->error_obj->line_pos = block->line_pos;
                }

                array_append(state->temp_memory, res);
                object_free(res);
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

            object_set(obj1, obj2);
            array_append(state->temp_memory, obj2);

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
            if (state->return_obj != NULL) object_free(state->return_obj);
            if (block->count != 0) {
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
            } else {
                state->return_obj = object_new();
            }
            struct object_st *current_object = NULL;
            while (code_operations->size && state->return_obj != NULL) {
                current_object = array_get_last(code_operations);
                if (current_object->type == OP_BLOCK_TYPE) {
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Temp &&
                        state->temp_memory->size > ((struct op_block *) current_object->data)->count)
                        array_remove_last(state->temp_memory);
                    if (((struct op_block *) current_object->data)->type == BlockType_Delete_Scope) {
                        if (((struct op_block *) current_object->data)->subtype == Delete_Scope_Func) {
                            array_append(state->temp_memory, state->return_obj);
                            object_free(state->return_obj);
                            state->return_obj = NULL;
                        }
                        struct darray_st *closure = array_get_last(state->stack_memory)->data;
                        for (size_t i = 0; i < closure->size; i++) {
                            attrib = closure->data[0][i]->data;
                            object_free(attrib->data);
                            attrib->data = object_copy(closure->data[1][i]);
                        }
                        array_remove_last(state->stack_memory);
                    }
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
            struct object_st *temp = map_set_elm(map->data, block->data1->data);
            object_set(temp, obj);

            object_free(temp);
            object_free(obj);
            break;
        }
        case BlockType_Attr: {
            struct object_st *obj = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            struct object_st *res = object_attrib(state->error_obj, obj, block->data1);

            if (state->error_obj->present) {
                state->error_obj->pos = block->pos;
                state->error_obj->line_num = block->line_num;
                state->error_obj->line_pos = block->line_pos;
            }
            array_append(state->temp_memory, res);

            object_free(res);
            object_free(obj);
            break;
        }
        case BlockType_Subs: {
            struct object_st *obj2 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct object_st *obj1 = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            struct object_st *res = object_subscript(state->error_obj, obj1, obj2);

            if (state->error_obj->present) {
                state->error_obj->pos = block->pos;
                state->error_obj->line_num = block->line_num;
                state->error_obj->line_pos = block->line_pos;
            }
            array_append(state->temp_memory, res);

            object_free(res);
            object_free(obj1);
            object_free(obj2);
            break;
        }
        case BlockType_Call: {
            struct object_st *res = NULL;
            struct object_st *func = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);


            {
                array_add_new(state->stack_memory, DARRAY_TYPE);

                array_add_new(code_operations, OP_BLOCK_TYPE);
                new_block = array_get_last(code_operations)->data;
                new_block->type = BlockType_Delete_Scope;
                new_block->subtype = Delete_Scope_Func;
            }

            if (func->type != FUNC_TYPE) {
                error_fill_in(state->error_obj, INTERPRETER_ERROR, "variable is not callable", block->pos, block->line_num, block->line_pos);
                goto end;
            }
            int ok = 0;
            {
                res = ((struct func_st *)func->data)->params;
                if (res != NULL && res->type != NONE_TYPE) {
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
                    } else {
                        error_fill_in(state->error_obj, INTERPRETER_ERROR, "Miss match arguments", block->pos, block->line_num, block->line_pos);
                        goto end;
                    }
                }
                res = NULL;

                res = ((struct func_st *)func->data)->closure;
                if (res != NULL && ok && res->type != NONE_TYPE) {
                    struct darray_st *temp_array = res->data;
                    if (temp_array != NULL) {
                        for (int i = 0; i < temp_array->size; i++) {
                            attrib = temp_array->data[0][i]->data;
                            darray_append(array_get_last(state->stack_memory)->data, temp_array->data[0][i], attrib->data);
                            op_attrib_set_data(attrib, temp_array->data[1][i]);
                        }
                    }
                }
                res = NULL;

                res = ((struct func_st *)func->data)->body;
                if (res != NULL && ok && res->type != NONE_TYPE) {
                    array_append(code_operations, res);
                }
                res = NULL;
            }
            end:
            object_free(res);
            object_free(func);
            break;
        }
        case BlockType_Print: {
            struct object_st *obj = NULL;
            struct string_st *str = NULL;
            for (size_t i = 0; i < block->count; i++) {
                obj = object_copy(array_get_last(state->temp_memory));
                array_remove_last(state->temp_memory);
                str = obj->data;
                write(stream, str->data, str->size);
                if (i + 1 < block->count) write(stream, " ", 0);
                object_free(obj);
            }
            write(stream, "\n", 1);
            break;
        }
        case BlockType_ForNext: {
            struct object_st *ident = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct object_st *list = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);
            struct array_st *array = list->data;

            if (array->size != 0) {
                struct object_st *next_list = object_new();
                object_set_type(next_list, ARRAY_TYPE);
                struct array_st *next_array = next_list->data;

                object_set(ident, array->data[0]);

                array_resize(next_array, array->size - 1);
                for (size_t i = 0; i < next_array->size; i++) {
                    next_array->data[i] = object_copy(array->data[i + 1]);
                }

                array_append(state->temp_memory, next_list);
                array_append(state->temp_memory, ident);

                array_append(code_operations, object);
                array_append(code_operations, block->data1);
                object_free(next_list);
            }
            object_free(list);
            object_free(ident);
            break;
        }
        case BlockType_Is: {
            struct object_st *temp = NULL;
            struct object_st *obj = object_copy(array_get_last(state->temp_memory));
            array_remove_last(state->temp_memory);

            struct object_st *res = object_new();

            if (block->subtype == Type_bool) object__bool(res, state->error_obj, obj);
            else if (block->subtype == Type_int) object__int(res, state->error_obj, obj);
            else if (block->subtype == Type_real) object__float(res, state->error_obj, obj);
            else if (block->subtype == Type_string) object__str(res, state->error_obj, obj);
            else if (block->subtype == Type_func) {
                while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;

                if (obj->type != FUNC_TYPE) {
                    state->error_obj->present = 1;
                    string_set_str(state->error_obj->type, INTERPRETER_ERROR, 17);
                    string_set_str(state->error_obj->message, "object can not be converted into func", 37);
                }
            }
            else if (block->subtype == Type_array) {
                while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;

                if (obj->type != ARRAY_TYPE) {
                    state->error_obj->present = 1;
                    string_set_str(state->error_obj->type, INTERPRETER_ERROR, 17);
                    string_set_str(state->error_obj->message, "object can not be converted into array", 38);
                }
            }
            else if (block->subtype == Type_tuple) {
                while (obj != NULL && obj->type == OBJECT_TYPE) obj = obj->data;

                if (obj->type != MAP_TYPE) {
                    state->error_obj->present = 1;
                    string_set_str(state->error_obj->type, INTERPRETER_ERROR, 17);
                    string_set_str(state->error_obj->message, "object can not be converted into tuple", 38);
                }
            }

            if (state->error_obj->present) {
                state->error_obj->pos = block->pos;
                state->error_obj->line_num = block->line_num;
                state->error_obj->line_pos = block->line_pos;
            }

            array_append(state->temp_memory, res);
            object_free(res);
            object_free(obj);
            object_free(temp);
            break;
        }
    }
}

void interpretation(struct imp_parser *parser, int stream) {
    array_append(parser->code_operations, parser->tree);
    {
        struct array_st *code_operations = parser->code_operations;
        struct object_st *current_object = NULL;
        while (code_operations->size > 0) {
            current_object = object_copy(array_get_last(code_operations));
            array_remove_last(code_operations);

            if (!parser->error_obj->present) {
                if (current_object->type == NODE_TYPE) {
                    run_an(parser, current_object);
                }
                if (current_object->type == OP_BLOCK_TYPE) {
                    run_op(parser, current_object, stream);
                }
            }
            object_free(current_object);
        }
    }
}