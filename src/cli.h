/*
 * File: cli.h
 * Author: Ragib Asif
 * Email: ragibasif@tuta.io
 * GitHub: https://github.com/ragibasif
 * LinkedIn: https://www.linkedin.com/in/ragibasif/
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Ragib Asif
 * Version 1.0.0
 */

#ifndef CLI_H
#define CLI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct {
    char **input_files;  // Required: files to be compiled
    char  *output_file;  // Optional: path to final binary file
    char **include_dirs; // searched for after the #include "...", before <...>
    bool   preprocess;   // print preprocessed source and stop evaluation
    bool   lex;          // print tokenized source and stop evaluation
    bool   parse;        // print the generated AST and stop evaluation
    bool   compile;      // generate .s file and stop evaluation
    bool   assemble;     // generate .o file and stop evaluation
    bool   link;         // true by default, generate binary executable
    bool   color;        // true by default, show colors
    bool   trace;        // false by default, debug tracing
    bool   warnings;     // suppress warnings
    char **shared_libs;  // shared libraries to be passed to linker (-l<name>)
    char **lib_dirs;     // directories to be searched during linking (-L<dir>)
    char **defines;      // definitions passed as cli args
    char **undefines;    // undefined macros
} NemCLI;

extern NemCLI *nem_cli_create( int argc, char **argv );
extern void    nem_cli_destroy( NemCLI **cli );

#ifdef __cplusplus
}
#endif
#endif // CLI_H
