#include "ast_analyze.h"

#define expr_cast(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
node_set(obj->data, expr); node_clear(expr); array_append((expr)->next, obj); object_free(obj); }
#define expr_add(expr) { struct object_st *obj = object_new(); object_set_type(obj, NODE_TYPE); \
array_append((expr)->next, obj); expr_next = obj->data; object_free(obj);}
#define parser_end if (parser->list->size <= parser->position)
#define parser_get token_ptr = parser->list->data[parser->position];token = parser->list->data[parser->position]->data;
#define analyze_start size_t current_pointing = parser->position;struct node_st *expr_next = expr; struct token_st *token = NULL;struct object_st *token_ptr = NULL;int result = SN_Status_Nothing, sub_result;
#define analyze_end \
end:    if (result != SN_Status_Success) {node_clear(expr);parser->position = current_pointing;} return result; \
sub:    result = sub_result; goto end; \
eof:    result = SN_Status_EOF; set_error(parser, parser->position - 1, ""); goto end; \
err:    result = SN_Status_Error; set_error(parser, parser->position, "Unexpected end of file"); goto end;

#define check_call(call, check) {sub_result = call; if (sub_result == SN_Status_Nothing) check if (sub_result != SN_Status_Success) goto sub;}

// Cheking type of operation 
// Union operation
int u_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_ADD && token->subtype != Special_SUB))
                break;
            parser->position++;

            if (token->subtype == Special_ADD) continue;
            array_append(expr->tokens, token_ptr);
        }
        if (expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_U;
            expr->main_type = MainType_Oper;
            node_set_position(expr, parser->list->data[parser->position - 1]->data);
        } else node_clear(expr);
        check_call(primary_expr(parser, expr_next), {
            if (expr_next == expr) goto end;
            else goto err;
        })
        result = SN_Status_Success;
    }
analyze_end
}
// Multiplication and division operations
int m_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(u_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_MUL && token->subtype != Special_DIV &&
                    token->subtype != Special_MOD))
                break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_M;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Addision and Substraction operation
int a_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(m_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_ADD && token->subtype != Special_SUB))
                break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_A;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Shift operation
int shift_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(a_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special ||
                (token->subtype != Special_LSHIFT && token->subtype != Special_RSHIFT))
                break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Shift;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// And operation
int and_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(shift_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_AND) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_And;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// XOR operation
int xor_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(and_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_XOR) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Xor;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Or operation
int or_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(xor_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || token->subtype != Special_OR) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Or;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Сomparison operations
int comp_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(or_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_Special || (
                    token->subtype != Special_EQ_NOT && token->subtype != Special_EQ_EQ &&
                    token->subtype != Special_EQ_LESS && token->subtype != Special_EQ_GREATER &&
                    token->subtype != Special_LESS && token->subtype != Special_GREATER))
                break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_Comp;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Not test operations
int not_test_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        while (parser->position < parser->list->size) {
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_NOT) break;
            parser->position++;

            array_append(expr->tokens, token_ptr);
        }
        if (expr->tokens->size % 2 != 0) {
            expr_add(expr)
            expr->type = ExprType_NotTest;
            expr->main_type = MainType_Oper;
            node_set_position(expr, parser->list->data[parser->position - 1]->data);
        } else node_clear(expr);
        check_call(comp_oper(parser, expr_next), {
            if (expr_next == expr) goto end;
            else goto err;
        })
        result = SN_Status_Success;
    }
analyze_end
}
// And test operations
int and_test_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(not_test_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_AND) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_AndTest;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// XOR test operations
int xor_test_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(and_test_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_XOR) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_XorTest;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// Or test operations
int or_test_oper(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        int times = 0;
        while (parser->position < parser->list->size) {
            check_call(xor_test_oper(parser, expr_next), {
                if (times == 0) goto end;
                else goto err;
            })

            parser_end break;
            parser_get
            if (token->type != TokenType_KeyWords || token->subtype != KeyWord_OR) break;
            parser->position++;

            parser_end goto eof;
            if (times == 0) {
                expr_cast(expr)
                expr->type = ExprType_OrTest;
                expr->main_type = MainType_Oper;
                node_set_position(expr, parser->list->data[parser->position - 1]->data);
                times = 1;
            }
            expr_add(expr)
            array_append(expr->tokens, token_ptr);
        }
        result = SN_Status_Success;
    }
analyze_end
}
// List operations
int list_oper(struct ast_parser *parser, struct node_st *expr, short start, short end) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != start) goto end;
        parser->position++;

        expr->main_type = MainType_Expr;
        node_set_position(expr, parser->list->data[parser->position - 1]->data);
        expr->type = PrimType_List;

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->subtype == end) {
            parser->position++;
            result = SN_Status_Success;
            goto end;
        }

        while (parser->position < parser->list->size) {
            expr_add(expr)
            check_call(or_test_oper(parser, expr_next), goto err;)

            parser_end goto eof;
            parser_get
            if (token->type == TokenType_Special && token->subtype == end) {
                parser->position++;
                result = SN_Status_Success;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;
        }
        goto err;
    }
analyze_end
}
// List identifier
int list_ident(struct ast_parser *parser, struct node_st *expr) {
    analyze_start
    {
        parser_end goto eof;
        parser_get
        if (token->type != TokenType_Special || token->subtype != Special_LSB) goto end;
        parser->position++;

        expr->main_type = MainType_Expr;
        node_set_position(expr, parser->list->data[parser->position - 1]->data);
        expr->type = PrimType_List;

        parser_end goto eof;
        parser_get
        if (token->type == TokenType_Special && token->subtype == Special_RSB) {
            parser->position++;
            result = SN_Status_Success;
            goto end;
        }

        while (parser->position < parser->list->size) {
            expr_add(expr)
            check_call(ident_new_expr(parser, expr_next), goto err;)

            parser_end goto eof;
            parser_get
            if (token->type == TokenType_Special && token->subtype == Special_RSB) {
                parser->position++;
                result = SN_Status_Success;
                goto end;
            }
            if (token->type != TokenType_Special || token->subtype != Special_COMMA) break;
            parser->position++;
        }
        goto err;
    }
analyze_end
}