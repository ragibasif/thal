#!/bin/sh

clang -DNEM_DEBUG nem_token_test.c ../../src/token.c -o nem_token_test && ./nem_token_test && rm -f nem_token_test
clang -DNEM_DEBUG nem_lexer_test.c ../../src/token.c ../../src/lexer.c -o nem_lexer_test && ./nem_lexer_test && rm -f nem_lexer_test
clang -DNEM_DEBUG nem_ast_test.c ../../src/ast.c -o nem_ast_test && ./nem_ast_test && rm -f nem_ast_test
