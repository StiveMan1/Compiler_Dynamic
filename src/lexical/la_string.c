#include "lexical_analysis.h"

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
            string_set_str(parser->error_msg, "String started but did not end use for this \" ", 47);
            return;
        }
    } else {
        for (; pos < parser->str_size; pos++) {
            if ((pos == 0 || parser->data[pos - 1] != '\\') && parser->data[pos] == '\'') break;
        }
        if (parser->str_size == pos) {
            string_set_str(parser->error_msg, "String started but did not end use for this ' ", 47);
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