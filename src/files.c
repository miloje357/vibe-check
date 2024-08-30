#include "logger.h"
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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
  return true;
}

void set_standard_root(char **dir) {
  const char *homedirname = NULL;
  set_homedir(&homedirname);
  if (!homedirname) {
    *dir = NULL;
    return;
  }

  /* if has_cache:
   *  homedir + .cache + vibe-check
   * else
   *  homedir + .vibe-check
   */
}

bool set_root(const char *custom_root_path) {
  DIR *vc_root;
  if (!custom_root_path) {

    char *standard_root = NULL;
    set_standard_root(&standard_root);
    if (!standard_root) {
      return false;
    }

    vc_root = opendir(standard_root);
  } else {
    vc_root = opendir(custom_root_path);
  }

  if (!vc_root) {
    LOG_ERROR("Can't open %s: %s", custom_root_path, strerror(errno));
    return false;
  }

  closedir(vc_root);
  return true;
}
