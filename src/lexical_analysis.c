#include "lexical_analysis.h"

#define set_error_lexical(message) error_fill_in(parser->error_obj, LEXICAL_ANALYSIS_ERROR, message, parser->position - 1, parser->current_line, parser->line_pos);

// Running several subparsers for different token types 
void la_parse(struct token_st *token, struct la_parser *parser) {
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

// tokenize function 
void tokenize(struct la_parser *parser) {
    parser->position = parser->current_line = 0;
    struct token_st *token = token_new();
    int comment_type = 0;
    // comment_type:
    //   0 - without
    //   1 - one line
    //   2 - multiline
    while (parser->position < parser->str_size) {
        // Skip space symbols
        if (SpaceChar(parser->data[parser->position])) {
            // Inc lines and finish one line comment if needed
            if (parser->data[parser->position] == '\n') {
                if (comment_type == 1) comment_type = 0;
                parser->current_line++;
                parser->line_pos = parser->position + 1;
            }
            parser->position++;
            continue; // Skip
        }
        // Skipping comments
        if (comment_type != 0) {
            // If multiline comment finished
            if (comment_type == 2 && parser->data[parser->position] == '*' &&
                parser->data[parser->position + 1] == '/') {
                parser->position++;
                comment_type = 0;
            }
            parser->position++;
            continue; // Skip
        }
        // If neither space nor comment, then parse
        la_parse(token, parser);
        if (token->type == TokenType_None) {
            if (error_is_null(parser->error_obj)) {
                set_error_lexical("Unrecognized token")
            }
            goto bad_end;
        }
        if (token->type == TokenType_Special) {
            switch (token->subtype) {
                // Comment detected
                case Special_COM_LINE:
                    comment_type = 1;
                    break;
                case Special_COM_STR:
                    comment_type = 2;
                    break;
                    // Left scopes
                case Special_LSB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        set_error_lexical("Scope length more then max scopes nesting")
                        goto bad_end;
                    }
                    break;
                case Special_LSQB:
                    parser->scope_buf[parser->scope_pos++] = Special_LSQB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        set_error_lexical("Unrecognized token")
                        goto bad_end;
                    }
                    break;
                case Special_LCB:
                    parser->scope_buf[parser->scope_pos++] = Special_LCB;
                    if (parser->scope_pos > MaxBracketNesting) {
                        set_error_lexical("Scope length more then max scopes nesting")
                        goto bad_end;
                    }
                    break;
                    // Right scopes
                case Special_RSB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            set_error_lexical("Scope closed incorrectly. Must be ']' instead of ')'")
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            set_error_lexical("Scope closed incorrectly. Must be '}' instead of ')'")
                        }
                        goto bad_end;
                    }
                    break;
                case Special_RSQB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LSQB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            set_error_lexical("Scope closed incorrectly. Must be ')' instead of ']'")
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LCB) {
                            set_error_lexical("Scope closed incorrectly. Must be '}' instead of ']'")
                        }
                        goto bad_end;
                    }
                    break;
                case Special_RCB:
                    if (parser->scope_pos - 1 < 0 || parser->scope_buf[--parser->scope_pos] != Special_LCB) {
                        if (parser->scope_buf[parser->scope_pos] == Special_LSQB) {
                            set_error_lexical("Scope closed incorrectly. Must be ']' instead of '}'")
                        } else if (parser->scope_buf[parser->scope_pos] == Special_LSB) {
                            set_error_lexical("Scope closed incorrectly. Must be ')' instead of '}'")
                        }
                        goto bad_end;
                    }
                    break;
                default:
                    break;
            }
        }
        // If the token is not comment
        if (comment_type == 0) {
            array_add_new(parser->list, TOKEN_TYPE); // Add new Object type token to the parser
            token_set(parser->list->data[parser->list->size - 1]->data, token);
        }
        token_clear(token);
    }
    if (parser->scope_pos != 0) {
        set_error_lexical("Scope is not closed...")
        goto bad_end;
    }
    token_free(token);
    return;

    bad_end:
    token_free(token);
    array_clear(parser->list);
}
