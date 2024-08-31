#include "files.h"
#include "logger.h"
#include <config.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char *vc_rootpath = NULL;

void standardize_dirpath(char *dirpath, int len) {
  if (dirpath[len - 1] == '/') {
    dirpath[len - 1] = '\0';
  }
}

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
  if (!cache_dir && errno == ENOENT) {
    free(cache_dirpath);
    return false;
  }

  closedir(cache_dir);
  free(cache_dirpath);
  return true;
}

int set_standard_root(char **dir) {
  const char *home_dirpath = NULL;
  set_homedir(&home_dirpath);
  if (!home_dirpath) {
    *dir = NULL;
    return 0;
  }

  int len;
  if (has_cache(home_dirpath)) {
    len = asprintf(dir, "%s/%s/%s", home_dirpath, ".cache", PACKAGE_NAME);
  } else {
    len = asprintf(dir, "%s/%s", home_dirpath, "." PACKAGE_NAME);
  }
  if (len <= 0) {
    LOG_ERROR("Can't open %s: Not enough memory", *dir);
  }

  return len;
}

bool set_rootpath(const char *custom_root_path) {
  int len = asprintf(&vc_rootpath, "%s", custom_root_path);
  if (len <= 0) {
    LOG_ERROR("Can't open %s: Not enough memory", custom_root_path);
  }

  if (!custom_root_path) {
    len = set_standard_root(&vc_rootpath);
    if (!vc_rootpath) {
      return false;
    }
  }

  DIR *vc_root = opendir(vc_rootpath);
  if (!vc_root && errno != ENOENT) {
    LOG_ERROR("Can't open %s: %s", vc_rootpath, strerror(errno));
    return false;
  }
  if (!vc_root && errno == ENOENT) {
    int s = mkdir(vc_rootpath, 0755);
    if (s == -1) {
      LOG_ERROR("Couldn't create directory %s: %s", vc_rootpath,
                strerror(errno));
      return false;
    }
  }
  closedir(vc_root);

  standardize_dirpath(vc_rootpath, len);
  return true;
}

void free_rootpath() { free(vc_rootpath); }

void set_token_filepath(char **path) {
  if (!vc_rootpath) {
    DEV_ERROR("Root path is null: Run set_rootpath() first");
    *path = NULL;
    return;
  }
  asprintf(path, "%s/%s", vc_rootpath, TOKEN_FILENAME);
}
