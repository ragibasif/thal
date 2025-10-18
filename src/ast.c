/*
 * File: ast.c
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#include "ast.h"

NemAST *nem_ast_create( void ) {
    NemAST *ast = malloc( sizeof *ast );
    if ( ast == NULL ) { return NULL; }

    ast->data = malloc( sizeof *ast->data );
    if ( ast->data == NULL ) {
        free( ast );
        ast = NULL;
        return NULL;
    }

    return ast;
}

void nem_ast_destroy( NemAST **ast ) {
    if ( ast == NULL ) { return; }
    if ( *ast == NULL ) { return; }

    free( ( *ast )->data );
    ( *ast )->data = NULL;

    free( *ast );
    *ast = NULL;
}

NemASTExpr *nem_ast_expr_create( void ) {
    NemASTExpr *expr = malloc( sizeof *expr );
    if ( expr == NULL ) { return NULL; }
    return expr;
}

void nem_ast_expr_destroy( NemASTExpr **expr ) {
    if ( expr == NULL ) { return; }
    if ( *expr == NULL ) { return; }
    free( *expr );
    *expr = NULL;
}

NemASTStmt *nem_ast_stmt_create( void ) {
    NemASTStmt *stmt = malloc( sizeof *stmt );
    if ( stmt == NULL ) { return NULL; }
    return stmt;
}

void nem_ast_stmt_destroy( NemASTStmt **stmt ) {
    if ( stmt == NULL ) { return; }
    if ( *stmt == NULL ) { return; }
    free( *stmt );
    *stmt = NULL;
}

NemASTDecl *nem_ast_decl_create( void ) {
    NemASTDecl *decl = malloc( sizeof *decl );
    if ( decl == NULL ) { return NULL; }
    return decl;
}

void nem_ast_decl_destroy( NemASTDecl **decl ) {
    if ( decl == NULL ) { return; }
    if ( *decl == NULL ) { return; }
    free( *decl );
    *decl = NULL;
}
