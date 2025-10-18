/*
 * File: token.h
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#ifndef TOKEN_H
#define TOKEN_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

// token

typedef enum {
    // Special tokens
    NTT_ERROR,
    NTT_EOF,
    NTT_COMMENT,

    // Literals
    NTT_IDENTIFIER, // main
    NTT_NUMBER,     // 12345
    NTT_DIGIT,      // [0-9]
    NTT_INTEGER,    // 12345
    NTT_DOUBLE,
    NTT_FLOAT,     // 123.45
    NTT_CHARACTER, // 'a'
    NTT_STRING,    // "abc"
    NTT_CONSTANT,

    // Operators
    NTT_PLUS,     // +
    NTT_MINUS,    // -
    NTT_MULTIPLY, // *
    NTT_DIVIDE,   // /
    NTT_MODULO,   // %
    NTT_INVERT,   // ~

    NTT_BITWISE_AND,         // &
    NTT_BITWISE_OR,          // |
    NTT_BITWISE_XOR,         // ^
    NTT_BITWISE_SHIFT_LEFT,  // <<
    NTT_BITWISE_SHIFT_RIGHT, // >>

    NTT_PLUS_ASSIGNMENT,     // +=
    NTT_MINUS_ASSIGNMENT,    // -=
    NTT_MULTIPLY_ASSIGNMENT, // *=
    NTT_DIVIDE_ASSIGNMENT,   // /=
    NTT_MODULO_ASSIGNMENT,   // %=
    NTT_INVERT_ASSIGNMENT,   // ~=

    NTT_BITWISE_AND_ASSIGNMENT,         // &=
    NTT_BITWISE_OR_ASSIGNMENT,          // |=
    NTT_BITWISE_XOR_ASSIGNMENT,         // ^=
    NTT_BITWISE_SHIFT_LEFT_ASSIGNMENT,  // <<=
    NTT_BITWISE_SHIFT_RIGHT_ASSIGNMENT, // >>=

    NTT_LOGICAL_AND, // &&
    NTT_LOGICAL_OR,  // ||
    NTT_ARROW,       // ->
    NTT_INCREMENT,   // ++
    NTT_DECREMENT,   // --

    NTT_EQUAL,      // ==
    NTT_LESS,       // <
    NTT_GREATER,    // >
    NTT_ASSIGNMENT, // =
    NTT_NOT,        // !
    NTT_QUESTION,   // ?

    NTT_NOT_EQUAL,     // !=
    NTT_LESS_EQUAL,    // <=
    NTT_GREATER_EQUAL, // >=
    NTT_ELLIPSIS,      // ...
    NTT_DEFINE,        // :=

    NTT_POINTER,
    NTT_TYPE_NAME,
    NTT_TYPEDEF,

    // Delimiters
    NTT_OPEN_PARENTHESIS,     // (
    NTT_OPEN_SQUARE_BRACKET,  // [
    NTT_OPEN_CURLY_BRACE,     // {
    NTT_CLOSE_PARENTHESIS,    // )
    NTT_CLOSE_SQUARE_BRACKET, // ]
    NTT_CLOSE_CURLY_BRACE,    // }

    NTT_COMMA,        // ,
    NTT_PERIOD,       // .
    NTT_SEMICOLON,    // ;
    NTT_COLON,        // :
    NTT_SINGLE_QUOTE, // '
    NTT_DOUBLE_QUOTE, // "

    // Keywords
    NTT_AUTO,
    NTT_BREAK,
    NTT_CASE,
    NTT_CONST,
    NTT_CONTINUE,
    NTT_PRINT,
    NTT_TRUE,
    NTT_FALSE,

    NTT_DEFAULT,
    NTT_DO,
    NTT_ELSE,
    NTT_FOR,
    NTT_ENUM,
    NTT_EXTERN,
    NTT_INT,
    NTT_CHAR,
    NTT_LONG,
    NTT_REGISTER,
    NTT_SHORT,
    NTT_SIGNED,
    NTT_SIZEOF,
    NTT_STATIC,
    NTT_STRUCT,
    NTT_UNDEF,
    NTT_UNION,
    NTT_UNSIGNED,
    NTT_VOLATILE,
    NTT_WHILE,

    NTT_FUNCTION,
    NTT_GOTO,
    NTT_IF,
    NTT_IMPORT,

    NTT_RETURN,

    NTT_SWITCH,
    NTT_VARIABLE, // [A-Za-z_]+ [A-Za-z0-9_]*
                  // built-in data types
    NTT_VOID,
    NTT_BOOL,
    NTT_INT8,   // char
    NTT_UINT8,  // unsigned char
    NTT_INT16,  // short
    NTT_UINT16, // unsigned short
    NTT_INT32,  // int
    NTT_UINT32, // unsigned int
    NTT_INT64,  // long long
    NTT_UINT64, // unsigned long long
    NTT_FLT32,  // float
    NTT_FLT64,  // double (64 bits)

    // Internal
    NTT_ID,
    NTT_COUNT
} NemTokenType;

typedef struct {
    NemTokenType type;
    char        *lexeme;
    size_t       position; // start position (index) in the source buffer
    size_t       size;     // size (length) of the token
    unsigned int line;     // line number (always >= 1)
    unsigned int column;   // column number of start of token (always >= 1)
} NemToken;

extern NemToken *nem_token_create( const NemTokenType type, const char *lexeme,
                                   const size_t size, const size_t position,
                                   const unsigned int line,
                                   const unsigned int column );

extern void nem_token_type_print( const NemTokenType type );
extern void nem_token_destroy( NemToken **token );
extern bool nem_token_check_keyword( const NemTokenType type,
                                     const char        *buffer );

#ifdef __cplusplus
}
#endif
#endif // TOKEN_H
