#ifndef SOF_LEXER_H
#define SOF_LEXER_H

#include "../pkg/error_codes.h"
#include "../pkg/collections/vector/vector.h"

#include "structs.h"

/**
 * @brief Reads a source SOF file.
 *
 * @param file_name full path of a SOF source file.
 * @return lexer_file struct
 */
struct lexer_file lexer_read(const char *const file_name);

/**
 * @brief creates an array of identifiers based on a lexer_file.
 *
 * @attention This function frees the parameter "file.content"
 * 
 * @param file SOF lexer containing information about the source file.
 * @return array of identifiers (char*)
 */
vector_t lexer_build_identifiers(struct lexer_file file);

/**
 * @brief frees all identifiers within a lexer_file_identifiers range.
 *
 * @attention This function frees the parameter "file.content"
 * 
 * @param identifiers lexer identifiers to be freed.
 */
void lexer_free_identifiers(vector_t *identifiers);

#endif // SOF_LEXER_H