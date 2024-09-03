#ifndef FILES_H_
#define FILES_H_

#include "config.h"
#include <stdbool.h>

#define LOG_FILENAME PACKAGE_NAME ".log"

/**
 * Sets the root for all of the files that the vibe_check needs
 * @param custom_root_path If not set to NULL, uses it as the root, otherwise
 * uses the standard (~/.cache/vibe_check)
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
 * Sets the path for the log file
 * @param path Path of the log file to be set, NULL if an error ocurred
 * @see tests/check_files.c
 * @since 0.1.0
 * Note: run set_rootpath() first
 */
void set_log_filepath(char **path);

#endif
