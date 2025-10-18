/*
 * File: lexer.c
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#include "lexer.h"

NemLexer *nem_lexer_create( char *buffer, char *file ) {
    NemLexer *lexer = malloc( sizeof *lexer );
    if ( lexer == NULL ) { return NULL; }

    lexer->buffer   = buffer;
    lexer->size     = strlen( buffer );
    lexer->file     = file;
    lexer->ch       = ' ';
    lexer->position = 0;
    lexer->line     = 1;
    lexer->column   = 1;

    return lexer;
}

void nem_lexer_destroy( NemLexer **lexer ) {
    if ( lexer == NULL ) { return; }
    if ( *lexer == NULL ) { return; }

    free( ( *lexer )->buffer );
    free( ( *lexer )->file );
    ( *lexer )->buffer   = NULL;
    ( *lexer )->file     = NULL;
    ( *lexer )->position = 0;
    ( *lexer )->size     = 0;
    ( *lexer )->line     = 1;
    ( *lexer )->column   = 1;
    ( *lexer )->ch       = '\0';
    free( *lexer );
    *lexer = NULL;
}

static char nem_lexer_next( NemLexer **lexer ) {
    if ( lexer == NULL ) { return '\0'; }
    if ( *lexer == NULL ) { return '\0'; }

    ( *lexer )->ch = ( *lexer )->buffer[( *lexer )->position];
    ( *lexer )->position++;
    ( *lexer )->column++;
    return ( *lexer )->ch;
}

static char nem_lexer_peek( NemLexer **lexer ) {
    if ( lexer == NULL ) { return '\0'; }
    if ( *lexer == NULL ) { return '\0'; }

    if ( ( *lexer )->position < ( *lexer )->size ) {
        return ( *lexer )->buffer[( *lexer )->position];
    }
    return '\0';
}

static char nem_lexer_double_peek( NemLexer **lexer ) {
    if ( lexer == NULL ) { return '\0'; }
    if ( *lexer == NULL ) { return '\0'; }

    if ( ( *lexer )->position + 1 < ( *lexer )->size ) {
        return ( *lexer )->buffer[( *lexer )->position + 1];
    }
    return '\0';
}

static void nem_lexer_skip_white_space( NemLexer **lexer ) {
    if ( lexer == NULL ) { return; }
    if ( *lexer == NULL ) { return; }

    while ( 1 ) {
        char ch = nem_lexer_peek( lexer );
        switch ( ch ) {
            case ' ': [[fallthrough]];
            case '\t':
                ( *lexer )->column += 8 - ( ( *lexer )->column % 8 );
                [[fallthrough]];
            case '\v': [[fallthrough]];
            case '\f': [[fallthrough]];
            case '\r': nem_lexer_next( lexer ); break;
            case '\n':
                ( *lexer )->line++;
                nem_lexer_next( lexer );
                break;
            case '/': // C++ and C style comments get skipped
                if ( nem_lexer_double_peek( lexer ) == '/' ) {
                    while ( nem_lexer_peek( lexer ) != '\n' &&
                            ( ( *lexer )->position < ( *lexer )->size ) ) {
                        nem_lexer_next( lexer );
                    }
                } else if ( nem_lexer_double_peek( lexer ) == '*' ) {
                    while ( ( *lexer )->position < ( *lexer )->size &&
                            ( nem_lexer_peek( lexer ) != '*' ||
                              nem_lexer_double_peek( lexer ) != '/' ) ) {
                        nem_lexer_next( lexer );
                    }
                    nem_lexer_next( lexer );
                    nem_lexer_next( lexer );
                } else {
                    return;
                }
                [[fallthrough]];
            default: return;
        }
    }
}

static bool nem_lexer_match( NemLexer **lexer, const char ch ) {
    if ( lexer == NULL ) { return false; }
    if ( *lexer == NULL ) { return false; }
    if ( !( ( *lexer )->position < ( *lexer )->size ) ) { return false; }
    if ( nem_lexer_peek( lexer ) != ch ) { return false; }
    nem_lexer_next( lexer );
    return true;
}

static NemToken *nem_lexer_string( NemLexer **lexer ) {
    NemToken    *token;
    const size_t string_start  = ( *lexer )->position;
    size_t       string_length = 0;

    while ( nem_lexer_peek( lexer ) != '"' &&
            ( ( *lexer )->position < ( *lexer )->size ) ) {
        if ( nem_lexer_peek( lexer ) == '\n' ) { ( *lexer )->line++; }
        nem_lexer_next( lexer );
        string_length++;
    }

    if ( !( ( *lexer )->position < ( *lexer )->size ) ) {
        token =
            nem_token_create( NTT_ERROR, "String not terminated.",
                              strlen( "String not terminated." ), string_start,
                              ( *lexer )->line, ( *lexer )->column );
    }

    // closing quote
    nem_lexer_next( lexer );
    char *buffer = malloc( sizeof *buffer * ( string_length + 1 ) );
    memcpy( buffer, ( *lexer )->buffer + string_start, string_length );
    buffer[string_length] = '\0';

    token =
        nem_token_create( NTT_STRING, buffer, strlen( buffer ), string_start,
                          ( *lexer )->line, ( *lexer )->column );

    free( buffer );
    buffer = NULL;

    return token;
}

static bool nem_lexer_is_decimal( char ch ) { return '0' <= ch && ch <= '9'; }
static bool nem_lexer_is_alphabet( char ch ) {
    return ( 'a' <= ch && ch <= 'z' ) || ( 'A' <= ch && ch <= 'Z' );
}

static bool nem_lexer_is_alpha_numeric( char ch ) {
    return nem_lexer_is_alphabet( ch ) || nem_lexer_is_decimal( ch );
}

static NemToken *nem_lexer_number( NemLexer **lexer ) {
    NemToken    *token;
    const size_t number_start  = ( *lexer )->position - 1;
    size_t       number_length = 1;

    while ( nem_lexer_is_decimal( nem_lexer_peek( lexer ) ) ) {
        nem_lexer_next( lexer );
        number_length++;
    }

    if ( nem_lexer_peek( lexer ) == '.' &&
         nem_lexer_is_decimal( nem_lexer_double_peek( lexer ) ) ) {

        // consume the "."
        nem_lexer_next( lexer );
        number_length++;

        while ( nem_lexer_is_decimal( nem_lexer_peek( lexer ) ) ) {
            nem_lexer_next( lexer );
            number_length++;
        }
    }

    char *buffer = malloc( sizeof *buffer * ( number_length + 1 ) );
    memcpy( buffer, ( *lexer )->buffer + number_start, number_length );
    buffer[number_length] = '\0';

    token =
        nem_token_create( NTT_NUMBER, buffer, strlen( buffer ), number_start,
                          ( *lexer )->line, ( *lexer )->column );

    free( buffer );
    buffer = NULL;

    return token;
}

static bool nem_lexer_is_identifier_alphabet( char ch ) {
    return nem_lexer_is_alphabet( ch ) || ch == '_' ||
           nem_lexer_is_decimal( ch );
}

static NemToken *nem_lexer_identifier_type( NemLexer   **lexer,
                                            const char  *buffer,
                                            const size_t identifier_start ) {
    NemToken *token;
    switch ( buffer[0] ) {
        case 'a': {
            if ( nem_token_check_keyword( NTT_AUTO, buffer ) ) {
                token = nem_token_create( NTT_AUTO, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'b': {
            if ( nem_token_check_keyword( NTT_BOOL, buffer ) ) {
                token = nem_token_create( NTT_BOOL, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_BREAK, buffer ) ) {
                token = nem_token_create( NTT_BREAK, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'c': {
            if ( nem_token_check_keyword( NTT_CASE, buffer ) ) {
                token = nem_token_create( NTT_CASE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_CHAR, buffer ) ) {
                token = nem_token_create( NTT_CHAR, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_CONST, buffer ) ) {
                token = nem_token_create( NTT_CONST, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_CONTINUE, buffer ) ) {
                token = nem_token_create( NTT_CONST, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'd': {
            if ( nem_token_check_keyword( NTT_DEFAULT, buffer ) ) {
                token = nem_token_create( NTT_DEFAULT, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_DO, buffer ) ) {
                token = nem_token_create( NTT_DO, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_DOUBLE, buffer ) ) {
                token = nem_token_create( NTT_DOUBLE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'e': {
            if ( nem_token_check_keyword( NTT_ELSE, buffer ) ) {
                token = nem_token_create( NTT_ELSE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_ENUM, buffer ) ) {
                token = nem_token_create( NTT_ENUM, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_EXTERN, buffer ) ) {
                token = nem_token_create( NTT_EXTERN, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'f': {
            if ( nem_token_check_keyword( NTT_FALSE, buffer ) ) {
                token = nem_token_create( NTT_FALSE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_FLOAT, buffer ) ) {
                token = nem_token_create( NTT_FLOAT, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_FOR, buffer ) ) {
                token = nem_token_create( NTT_FOR, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'i': {
            if ( nem_token_check_keyword( NTT_IF, buffer ) ) {
                token = nem_token_create( NTT_IF, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_INT, buffer ) ) {
                token = nem_token_create( NTT_INT, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'g': {
            if ( nem_token_check_keyword( NTT_GOTO, buffer ) ) {
                token = nem_token_create( NTT_GOTO, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'l': {
            if ( nem_token_check_keyword( NTT_LONG, buffer ) ) {
                token = nem_token_create( NTT_LONG, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'r': {
            if ( nem_token_check_keyword( NTT_REGISTER, buffer ) ) {
                token = nem_token_create(
                    NTT_REGISTER, buffer, strlen( buffer ), identifier_start,
                    ( *lexer )->line, ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_RETURN, buffer ) ) {
                token = nem_token_create( NTT_RETURN, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 's': {
            if ( nem_token_check_keyword( NTT_SHORT, buffer ) ) {
                token = nem_token_create( NTT_SHORT, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_SIGNED, buffer ) ) {
                token = nem_token_create( NTT_SIGNED, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_SIZEOF, buffer ) ) {
                token = nem_token_create( NTT_SIZEOF, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_STATIC, buffer ) ) {
                token = nem_token_create( NTT_STATIC, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_STRUCT, buffer ) ) {
                token = nem_token_create( NTT_STRUCT, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_SWITCH, buffer ) ) {
                token = nem_token_create( NTT_SWITCH, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 't': {
            if ( nem_token_check_keyword( NTT_TRUE, buffer ) ) {
                token = nem_token_create( NTT_TRUE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_TYPEDEF, buffer ) ) {
                token = nem_token_create( NTT_TYPEDEF, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'u': {
            if ( nem_token_check_keyword( NTT_UNION, buffer ) ) {
                token = nem_token_create( NTT_UNION, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_UNSIGNED, buffer ) ) {
                token = nem_token_create(
                    NTT_UNSIGNED, buffer, strlen( buffer ), identifier_start,
                    ( *lexer )->line, ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'v': {
            if ( nem_token_check_keyword( NTT_VOID, buffer ) ) {
                token = nem_token_create( NTT_VOID, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            if ( nem_token_check_keyword( NTT_VOLATILE, buffer ) ) {
                token = nem_token_create(
                    NTT_VOLATILE, buffer, strlen( buffer ), identifier_start,
                    ( *lexer )->line, ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        case 'w': {
            if ( nem_token_check_keyword( NTT_WHILE, buffer ) ) {
                token = nem_token_create( NTT_WHILE, buffer, strlen( buffer ),
                                          identifier_start, ( *lexer )->line,
                                          ( *lexer )->column );
                return token;
            }
            [[fallthrough]];
        }

        default:
            token = nem_token_create( NTT_IDENTIFIER, buffer, strlen( buffer ),
                                      identifier_start, ( *lexer )->line,
                                      ( *lexer )->column );
    }
    return token;
}

static NemToken *nem_lexer_identifier( NemLexer **lexer ) {
    NemToken    *token;
    const size_t identifier_start  = ( *lexer )->position - 1;
    size_t       identifier_length = 1;

    while ( nem_lexer_is_identifier_alphabet( nem_lexer_peek( lexer ) ) ) {
        nem_lexer_next( lexer );
        identifier_length++;
    }

    char *buffer = malloc( sizeof *buffer * ( identifier_length + 1 ) );
    memcpy( buffer, ( *lexer )->buffer + identifier_start, identifier_length );
    buffer[identifier_length] = '\0';

    token = nem_token_create( NTT_IDENTIFIER, buffer, strlen( buffer ),
                              identifier_start, ( *lexer )->line,
                              ( *lexer )->column );
    token = nem_lexer_identifier_type( lexer, buffer, identifier_start );

    free( buffer );
    buffer = NULL;

    return token;
}

NemToken *nem_lexer_scan( NemLexer **lexer ) {

    nem_lexer_skip_white_space( lexer );
    NemToken *token;

    // find longest match at start of input
    // convert matching substring into a token
    char ch = nem_lexer_next( lexer );
    if ( nem_lexer_is_alphabet( ch ) || ch == '_' ) {
        token = nem_lexer_identifier( lexer );
        return token;
    }
    if ( nem_lexer_is_decimal( ch ) ) {
        token = nem_lexer_number( lexer );
        return token;
    }
    switch ( ch ) {

        case '+': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token =
                    nem_token_create( NTT_PLUS_ASSIGNMENT, "+=", strlen( "+=" ),
                                      ( *lexer )->position - 2,
                                      ( *lexer )->line, ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_PLUS, "+", strlen( "+" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '-': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_MINUS_ASSIGNMENT, "-=", strlen( "-=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_MINUS, "-", strlen( "-" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '*': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_MULTIPLY_ASSIGNMENT, "*=", strlen( "*=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_MULTIPLY, "*", strlen( "*" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '/': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_DIVIDE_ASSIGNMENT, "/=", strlen( "/=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_DIVIDE, "/", strlen( "/" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '%': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_MODULO_ASSIGNMENT, "%=", strlen( "%=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_MODULO, "%", strlen( "%" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '~': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_INVERT_ASSIGNMENT, "~=", strlen( "~=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_INVERT, "~", strlen( "~" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '&': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_BITWISE_AND_ASSIGNMENT, "&=", strlen( "&=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token =
                    nem_token_create( NTT_BITWISE_AND, "&", strlen( "&" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '|': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_BITWISE_OR_ASSIGNMENT, "|=", strlen( "|=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token =
                    nem_token_create( NTT_BITWISE_OR, "|", strlen( "|" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '^': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_BITWISE_XOR_ASSIGNMENT, "^=", strlen( "^=" ),
                    ( *lexer )->position - 2, ( *lexer )->line,
                    ( *lexer )->column );
            } else {
                token =
                    nem_token_create( NTT_BITWISE_XOR, "^", strlen( "^" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '<': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token =
                    nem_token_create( NTT_LESS_EQUAL, "<=", strlen( "<=" ),
                                      ( *lexer )->position - 2,
                                      ( *lexer )->line, ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_LESS, "<", strlen( "<" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '>': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token =
                    nem_token_create( NTT_GREATER_EQUAL, ">=", strlen( ">=" ),
                                      ( *lexer )->position - 2,
                                      ( *lexer )->line, ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_GREATER, ">", strlen( ">" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '=': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token = nem_token_create(
                    NTT_EQUAL, "==", strlen( "==" ), ( *lexer )->position - 2,
                    ( *lexer )->line, ( *lexer )->column );
            } else {
                token =
                    nem_token_create( NTT_ASSIGNMENT, "=", strlen( "=" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '!': {
            if ( nem_lexer_match( lexer, '=' ) ) {
                token =
                    nem_token_create( NTT_NOT_EQUAL, "!=", strlen( "!=" ),
                                      ( *lexer )->position - 2,
                                      ( *lexer )->line, ( *lexer )->column );
            } else {
                token = nem_token_create(
                    NTT_NOT, "!", strlen( "!" ), ( *lexer )->position - 1,
                    ( *lexer )->line, ( *lexer )->column );
            }
            return token;
        }

        case '?': {
            token = nem_token_create( NTT_QUESTION, "?", strlen( "?" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '(': {
            token = nem_token_create( NTT_OPEN_PARENTHESIS, "(", strlen( "(" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case ')': {
            token = nem_token_create( NTT_CLOSE_PARENTHESIS, ")", strlen( ")" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '{': {
            token = nem_token_create( NTT_OPEN_CURLY_BRACE, "{", strlen( "{" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '}': {
            token = nem_token_create( NTT_CLOSE_CURLY_BRACE, "}", strlen( "}" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '[': {
            token = nem_token_create( NTT_OPEN_SQUARE_BRACKET, "[",
                                      strlen( "[" ), ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case ']': {
            token = nem_token_create( NTT_CLOSE_SQUARE_BRACKET, "]",
                                      strlen( "]" ), ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case ',': {
            token = nem_token_create( NTT_COMMA, ",", strlen( "," ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '.': {
            token = nem_token_create( NTT_PERIOD, ".", strlen( "." ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case ';': {
            token = nem_token_create( NTT_SEMICOLON, ";", strlen( ";" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case ':': {
            token = nem_token_create( NTT_COLON, ":", strlen( ":" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        case '"': token = nem_lexer_string( lexer ); return token;

        case '\0': {
            token = nem_token_create( NTT_EOF, "EOF", strlen( "EOF" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }

        default: { // if no match is found, return error token
            token = nem_token_create( NTT_ERROR, "error", strlen( "error" ),
                                      ( *lexer )->position - 1,
                                      ( *lexer )->line, ( *lexer )->column );
            return token;
        }
    }

    return token;
}
