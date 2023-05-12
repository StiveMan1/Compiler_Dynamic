# Functions 

- `la_identifier(struct token_st *, struct la_parser *)` - function go through the source code and trying to get identifier from it
  and store the data into the token  

The functions in the bottom doing have the same functionality

- `la_integer(struct token_st *, struct la_parser *)` - working with integers 

- `la_keyword(struct token_st *, struct la_parser *)` - working with keywords

- `la_string(struct token_st *, struct la_parser *)` - working with strings

- `la_special(struct token_st *, struct la_parser *)` - working with special char such as `+ - /...`