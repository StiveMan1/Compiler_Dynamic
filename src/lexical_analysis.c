#include <stdio.h>
#include "lexical_analysis.h"

void la_parse(struct token_st *token, struct la_parser *parser){
    la_special(token, parser);
    if (token->type != TokenType_None) return;
    la_string(token, parser);
    if (token->type != TokenType_None) return;
    la_integer(token, parser);
    if (token->type != TokenType_None) return;
    la_keyword(token, parser);
    if (token->type != TokenType_None) return;
    la_identifier(token, parser);
}

void tokenize(struct la_parser *parser) {
    parser->position = parser->current_line = 0;
    struct token_st *token = token_new();
    int comment_type = 0;
    while (parser->position < parser->str_size) {
        if (SpaceChar(parser->data[parser->position])) {
            if (parser->data[parser->position] == '\n') {
                if (comment_type == 1) comment_type = 0;
                parser->current_line++;
            }
            parser->position++;
            continue;
        }
        if (comment_type != 0) {
            if (comment_type == 2 && parser->data[parser->position] == '*' && parser->data[parser->position + 1] == '/') {
                parser->position++;
                comment_type = 0;
            }
            parser->position++;
            continue;
        }
        la_parse(token, parser);
        print_token(token, 0);
        if (token->type == TokenType_None) goto bad_end;
        if (token->type == TokenType_Special) {
            switch (token->subtype) {
                case Special_COM_LINE:
                    comment_type = 1;
                    break;
                case Special_COM_STR:
                    comment_type = 2;
                    break;
                case Special_LSB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSB;
                    if (parser->scope_pos > MaxBracketNesting) goto bad_end;
                    break;
                case Special_LSQB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSQB;
                    if (parser->scope_pos > MaxBracketNesting) goto bad_end;
                    break;
                case Special_LCB:
                    parser->scope_buf[parser->scope_pos++] = Special_LCB;
                    if (parser->scope_pos > MaxBracketNesting) goto bad_end;
                    break;
                case Special_RSB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSB) goto bad_end;
                    break;
                case Special_RSQB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSQB) goto bad_end;
                    break;
                case Special_RCB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LCB) goto bad_end;
                    break;
                default:
                    break;
            }
        }
        if (comment_type == 0) {
            array_add_new(parser->list, TOKEN_TYPE);
            token_set(parser->list->data[parser->list->size - 1]->data, token);
        }
        token_clear(token);
    }

    printf("parser->scope_pos : %d\n", parser->scope_pos);
    if (parser->scope_pos != 0) goto bad_end;
    token_free(token);
    return;

    bad_end:
    token_free(token);
    array_clear(parser->list);
}
