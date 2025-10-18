/*
 * File: nem_ast_test.c
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 *
 */

#include "../../src/ast.h"
#include "../../src/dbg.h"
#include "utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void nem_ast_helper( void ) {
    NemAST *ast = nem_ast_create();
    dbg( ast );
    dbg( sizeof *ast );
    dbg( sizeof ast->type );
    dbg( sizeof *ast->data );

    ast->type      = NANT_ROOT;
    ast->data->ast = nem_ast_create();

    dbg( ast->data->ast );
    dbg( sizeof *ast->data->ast );
    dbg( sizeof ast->data->ast->type );
    dbg( sizeof *ast->data->ast->data );

    nem_ast_destroy( &ast->data->ast );
    nem_ast_destroy( &ast );

    // NemASTExpr *expr = nem_ast_expr_create();

    // NemASTStmt *stmt = nem_ast_stmt_create();
    // NemASTDecl *decl = nem_ast_decl_create();

    // dbg( expr );
    // dbg( sizeof *expr );
    // dbg( sizeof *expr->data );

    // dbg( stmt );
    // dbg( sizeof *stmt );
    // dbg( sizeof *stmt->data );
    //
    // dbg( decl );
    // dbg( sizeof *decl );
    // dbg( sizeof *decl->data );

    // nem_ast_expr_destroy( &expr );
    // nem_ast_stmt_destroy( &stmt );
    // nem_ast_decl_destroy( &decl );
}

int main( int argc, char **argv ) {
    nem_ast_helper();

    TEST_PASSED();
}
