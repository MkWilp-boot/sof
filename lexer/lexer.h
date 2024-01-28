#ifndef SOF_LEXER_H
#define SOF_LEXER_H

#include "../pkg/error_codes.h"

/*
 * Function: lexer_reade
 * ----------------------------
 * Readers a source sof file
 *
 * file_name: full path of a sof source file.
 *
 * returns: lexer_file struct.
 */
struct lexer_file lexer_read(char* file_name);

/*
 * Function: lexer_build_identifiers
 * ----------------------------
 * creates an array of identifiers based on a lexer_file
 *
 * file: sof lexer containing information about the source file
 *
 * returns: lexer_file_identifiers struct.
 * 
 * IMPORTANT: this function frees "file.content"
 */
struct lexer_file_identifiers lexer_build_identifiers(struct lexer_file file);

/*
 * Function: lexer_free_identifiers
 * ----------------------------
 * frees all identifiers within a lexer_file_identifiers range
 *
 * identifiers: lexer identifiers to be freed.
 *
 * returns: void.
 */
void lexer_free_identifiers(struct lexer_file_identifiers identifiers);
#endif // SOF_LEXER_H