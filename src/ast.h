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

typedef struct NemAST                      NemAST;
typedef struct NemExprPrimary              NemExprPrimary;
typedef struct NemExprPostfix              NemExprPostfix;
typedef struct NemExprArgList              NemExprArgList;
typedef struct NemExprUnary                NemExprUnary;
typedef struct NemOpUnary                  NemOpUnary;
typedef struct NemExprCast                 NemExprCast;
typedef struct NemExprMult                 NemExprMult;
typedef struct NemExprAdd                  NemExprAdd;
typedef struct NemExprShift                NemExprShift;
typedef struct NemExprRel                  NemExprRel;
typedef struct NemExprEq                   NemExprEq;
typedef struct NemExprBitAnd               NemExprBitAnd;
typedef struct NemExprBitXor               NemExprBitXor;
typedef struct NemExprBitOr                NemExprBitOr;
typedef struct NemExprLogicAnd             NemExprLogicAnd;
typedef struct NemExprLogicOr              NemExprLogicOr;
typedef struct NemExprCond                 NemExprCond;
typedef struct NemExprAssign               NemExprAssign;
typedef struct NemOpAssign                 NemOpAssign;
typedef struct NemExpr                     NemExpr;
typedef struct NemExprConst                NemExprConst;
typedef struct NemDecl                     NemDecl;
typedef struct NemDeclSpec                 NemDeclSpec;
typedef struct NemDeclInitList             NemDeclInitList;
typedef struct NemDeclInit                 NemDeclInit;
typedef struct NemSpecStorageClass         NemSpecStorageClass;
typedef struct NemSpecType                 NemSpecType;
typedef struct NemSpecStructOrUnion        NemSpecStructOrUnion;
typedef struct NemStructOrUnion            NemStructOrUnion;
typedef struct NemDeclStructList           NemDeclStructList;
typedef struct NemDeclStruct               NemDeclStruct;
typedef struct NemSpecQualList             NemSpecQualList;
typedef struct NemStructDeclaratorList     NemStructDeclaratorList;
typedef struct NemStructDeclarator         NemStructDeclarator;
typedef struct NemSpecEnum                 NemSpecEnum;
typedef struct NemEnumList                 NemEnumList;
typedef struct NemEnum                     NemEnum;
typedef struct NemTypeQual                 NemTypeQual;
typedef struct NemDeclarator               NemDeclarator;
typedef struct NemDirectDeclarator         NemDirectDeclarator;
typedef struct NemPtr                      NemPtr;
typedef struct NemTypeQualList             NemTypeQualList;
typedef struct NemParamTypeList            NemParamTypeList;
typedef struct NemParamList                NemParamList;
typedef struct NemDeclParam                NemDeclParam;
typedef struct NemIdentList                NemIdentList;
typedef struct NemTypeName                 NemTypeName;
typedef struct NemAbstractDeclarator       NemAbstractDeclarator;
typedef struct NemDirectAbstractDeclarator NemDirectAbstractDeclarator;
typedef struct NemInitializer              NemInitializer;
typedef struct NemInitializerList          NemInitializerList;
typedef struct NemStmt                     NemStmt;
typedef struct NemStmtLabeled              NemStmtLabeled;
typedef struct NemStmtCompound             NemStmtCompound;
typedef struct NemDeclList                 NemDeclList;
typedef struct NemStmtList                 NemStmtList;
typedef struct NemExprStmt                 NemExprStmt;
typedef struct NemStmtSelection            NemStmtSelection;
typedef struct NemStmtIteration            NemStmtIteration;
typedef struct NemStmtJump                 NemStmtJump;
typedef struct NemTranslationUnit          NemTranslationUnit;
typedef struct NemDeclExternal             NemDeclExternal;
typedef struct NemFuncDef                  NemFuncDef;

struct NemAST;

extern NemAST *nem_ast_create( void );
extern void    nem_ast_destroy( NemAST **ast );

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
