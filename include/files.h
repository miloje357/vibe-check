#ifndef FILES_H_
#define FILES_H_

#include <stdbool.h>

#define TOKEN_FILENAME "token.json"

/**
 * Sets the root for all of the files that the vibe_check needs
 * @param custom_root_path If not set to NULL, uses it as the root, otherwise
 * uses the standard (~/.cache/vibe_check or ~/.vibe_check)
 * @return true if the root was successfully set, otherwise false
 * @see tests/check_files.c
 * @since 0.1.0
 * Note: use free_rootpath() to prevent memory leaks
 */
bool set_rootpath(const char *custom_root_path);

/**
 * Frees the rootpath string
 * @see tests/check_files.c
 * @since 0.1.0
 */
void free_rootpath();

/**
 * Sets the path for the token file
 * @param path Path of the token file to be set
 * @see tests/check_files.c
 * @since 0.1.0
 */
void set_token_filepath(char **path);

#endif
