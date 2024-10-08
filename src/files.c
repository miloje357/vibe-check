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

int set_standard_root(char **dir) {
  const char *home_dirpath = NULL;
  set_homedir(&home_dirpath);
  if (!home_dirpath) {
    *dir = NULL;
    return 0;
  }

  int len = asprintf(dir, "%s/%s/%s", home_dirpath, ".cache", PACKAGE_NAME);
  if (len <= 0) {
    DEV_ERROR("Can't open %s: Not enough memory", *dir);
    *dir = NULL;
  }

  return len;
}

bool set_rootpath(const char *custom_root_path) {
  int len;
  if (custom_root_path) {
    len = asprintf(&vc_rootpath, "%s", custom_root_path);
    if (len <= 0) {
      DEV_ERROR("Can't open %s: Not enough memory", custom_root_path);
      return false;
    }
  } else {
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

void free_rootpath() {
  if (vc_rootpath) {
    free(vc_rootpath);
    vc_rootpath = NULL;
  }
}

void set_log_filepath(char **path) {
  if (!vc_rootpath) {
    DEV_ERROR("Root path is null: Run set_rootpath() first");
    *path = NULL;
    return;
  }
  int s = asprintf(path, "%s/%s", vc_rootpath, LOG_FILENAME);
  if (s <= 0) {
    DEV_ERROR("Can't set log filepath: Not enough memory");
    *path = NULL;
    return;
  }
}
