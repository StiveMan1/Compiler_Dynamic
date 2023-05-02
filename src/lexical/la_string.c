#include "lexical_analysis.h"

#define set_error_lexical(message) error_fill_in(parser->error_obj, LEXICAL_ANALYSIS_ERROR, message, parser->position, parser->current_line, parser->line_pos);

void la_string(struct token_st *token, struct la_parser *parser) {
    // Checking if string started
    if (parser->data[parser->position] != '"' && parser->data[parser->position] != '\'') return;

    // Getting the string until the end token found
    size_t pos = parser->position + 1;
    if (parser->data[parser->position] == '"') {
        for (; pos < parser->str_size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '"') break;
        }
        if (parser->str_size == pos) {
            set_error_lexical("String started but did not end use for this \" ")
            return;
        }
    } else {
        for (; pos < parser->str_size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '\'') break;
        }
        if (parser->str_size == pos) {
            set_error_lexical("String started but did not end use for this ' ")
            return;
        }
    }

    token->type = TokenType_String;
    // Allocating space for token
    token_resize(token, pos - parser->position - 1);
    memcpy(token->data, &parser->data[parser->position + 1], token->size);
    token_set_pos(token, parser);
    parser->position = pos + 1;
}