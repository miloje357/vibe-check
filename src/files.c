#include "logger.h"
#include <config.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *vc_rootpath;

void set_homedir(const char **dir) {
  struct passwd *pw = getpwuid(getuid());
  if (!pw) {
    LOG_ERROR("Couldn't find the home directory: %s", strerror(errno));
    dir = NULL;
    return;
  }
  *dir = pw->pw_dir;
}

bool has_cache(const char *dir) {
  char *cache_dirpath;
  asprintf(&cache_dirpath, "%s/%s", dir, ".cache");

  DIR *cache_dir = opendir(cache_dirpath);
  if (!cache_dir && errno != ENOENT) {
    LOG_ERROR("Couldn't open %s: %s", cache_dirpath, strerror(errno));
    free(cache_dirpath);
    return false;
  }

  // cache_dir doesn't exist
  if (!cache_dirpath && errno == ENOENT) {
    free(cache_dirpath);
    return false;
  }

  closedir(cache_dir);
  free(cache_dirpath);
  return true;
}

void set_standard_root(char **dir) {
  const char *home_dirpath = NULL;
  set_homedir(&home_dirpath);
  if (!home_dirpath) {
    *dir = NULL;
    return;
  }

  int  s;
  if (has_cache(home_dirpath)) {
    s = asprintf(dir, "%s/%s/%s", home_dirpath, ".cache", PACKAGE_NAME);
  } else {
    s = asprintf(dir, "%s/%s", home_dirpath, "." PACKAGE_NAME);
  }
  if (s <= 0) {
    LOG_ERROR("Can't open %s: Not enough memory", *dir);
  }
}

bool set_rootpath(const char *custom_root_path) {
  int s = asprintf(&vc_rootpath, "%s", custom_root_path);
  if (s <= 0) {
    LOG_ERROR("Can't open %s: Not enough memory", custom_root_path);
  }

  if (!vc_rootpath) {
    set_standard_root(&vc_rootpath);
    if (!vc_rootpath) {
      return false;
    }
  }

  DIR *vc_root = opendir(vc_rootpath);
  if (!vc_root) {
    LOG_ERROR("Can't open %s: %s", custom_root_path, strerror(errno));
    return false;
  }
  closedir(vc_root);

  return true;
}

void free_rootpath() { free(vc_rootpath); }
