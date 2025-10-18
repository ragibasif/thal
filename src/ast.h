/*
 * File: ast.h
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#ifndef AST_H
#define AST_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef enum {
    NANT_ROOT,
    NANT_EXPR,
    NANT_STMT,
    NANT_DECL,
    // internal
    NANT_COUNT,

} NemASTNodeType;

typedef struct NemAST     NemAST;
typedef struct NemASTExpr NemASTExpr;
typedef struct NemASTStmt NemASTStmt;
typedef struct NemASTDecl NemASTDecl;

struct NemAST {
    NemASTNodeType type;
    union {
        NemAST     *ast;
        NemASTExpr *expr;
        NemASTStmt *stmt;
        NemASTDecl *decl;
    } *data;
};

struct NemASTExpr {
    char *data;
};

struct NemASTStmt {
    char *data;
};

struct NemASTDecl {
    char *data;
};

extern NemAST *nem_ast_create( void );
extern void    nem_ast_destroy( NemAST **ast );

extern NemASTExpr *nem_ast_expr_create( void );
extern void        nem_ast_expr_destroy( NemASTExpr **expr );

extern NemASTStmt *nem_ast_stmt_create( void );
extern void        nem_ast_stmt_destroy( NemASTStmt **stmt );

extern NemASTDecl *nem_ast_decl_create( void );
extern void        nem_ast_decl_destroy( NemASTDecl **decl );

#ifdef __cplusplus
}
#endif
#endif // AST_H

////////////////////////////////////////////////////////////////////////////////
// Notes
////////////////////////////////////////////////////////////////////////////////

/*

   - statement
   - expression
   - declaration
   - the grammar of the language determines the AST nodes and node types

*/
