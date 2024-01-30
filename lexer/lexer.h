#ifndef SOF_LEXER_H
#define SOF_LEXER_H

#include "../pkg/error_codes.h"

/**
 * @brief Readers a source SOF file.
 *
 * @param file_name full path of a sof source file.
 * @return lexer_file struct
 */
struct lexer_file lexer_read(char *file_name);

/**
 * @brief creates an array of identifiers based on a lexer_file.
 *
 * @attention This function frees the parameter "file.content"
 * 
 * @param file SOF lexer containing information about the source file.
 * @return lexer_file_identifiers struct
 */
struct lexer_file_identifiers lexer_build_identifiers(struct lexer_file file);

/**
 * @brief frees all identifiers within a lexer_file_identifiers range.
 *
 * @attention This function frees the parameter "file.content"
 * 
 * @param identifiers lexer identifiers to be freed.
 */
void lexer_free_identifiers(struct lexer_file_identifiers *identifiers);

#endif // SOF_LEXER_H