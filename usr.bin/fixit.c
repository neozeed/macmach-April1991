/* fixit.c -- a directory utility, see usage() for operating instructions */

/* I wrote this to do simple functions that needed to be done.  If you have
 * any comments, gripes or bugs, please let me know.
 * Zon Williamson, zon+@andrew.cmu.edu
 */

#define VERSION "fixit version 1.1"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <time.h>
#include <assert.h>

#define BUFFSIZ 0x800 /* buffer size -- one page */
#define PATHSIZ 1000  /* maximum path size */

/* stderr is redirected to stdout to collect all output to any log file */
/* there is still a need to output messages to the user tty, /dev/tty */
FILE *tty;

/* this macro writes a message to the user tty */
#define ttymsg(msg) fputs(msg, tty); fflush(tty)

char this_path[PATHSIZ], *this_file; /* global name buffer */
char that_path[PATHSIZ], *that_file; /* global name buffer */

int error = 0; /* global error flag */

time_t touch_time; /* global touch time */

/* point stderr to stdout so that log file collects all messages */
stderr_to_stdout()
{
  if (dup2(1, 2) == -1) {
    printf("fixit: can not make change stderr to stdout\n");
    exit(-1);
  }
}

/* open access to user tty */
open_tty()
{
  if (!(tty = fopen("/dev/tty", "w"))) tty = fopen("/dev/null", "w");
  if (!tty) {
    printf("fixit: can not write /dev/tty or /dev/null\n");
    _exit(-2);
  }
}

/* local version of exit() which writes message to tty before exit if error */
/* note that exit(-1) does not write message but does exit non-zero */
exit(int code)
{
  if (code > 0) fprintf(tty, "fixit: error %d encountered\n", code);
  _cleanup();
  _exit(code);
}

/* apply function to all files in directory */
/* does not recurse into directory if function returns non-zero */
apply(char *path, int (*function)(char *path, struct stat *s))
{
  DIR *dirp;
  struct direct *dp;
  struct stat s;
  int end, fix;
  struct dir { char *d_name; struct dir *next; } *dir, *d;
  extern char *alloca();

  if (lstat(path, &s)) return;
  if ((*function)(path, &s)) return;
  if ((s.st_mode & S_IFMT) != S_IFDIR) return;
  fix = end = strlen(path);
  if (!(dirp = opendir(path))) {
    printf("can't opendir(%s)\n", path); exit(1);
  }
  if ((fix > 0) && (path[fix - 1] != '/')) path[fix++] = '/';
  for (dir = 0, dp = readdir(dirp); dp; dp = readdir(dirp)) {
    if ((dp->d_name[0] == '.') &&
        (!dp->d_name[1] || ((dp->d_name[1] == '.') && !dp->d_name[2])))
      continue;
    if (!dir) dir = d = (struct dir *)alloca(sizeof(struct dir));
    else { d->next = (struct dir *)alloca(sizeof(struct dir)); d = d->next; }
    d->next = 0; d->d_name = alloca(strlen(dp->d_name) + 1);
    strcpy(d->d_name, dp->d_name);
  }
  closedir(dirp);
  for (d = dir; d; d = d->next) {
    path[fix] = 0; apply(strcat(path, d->d_name), function);
  }
  path[end] = 0;
}

/* fast file compare, return byte position of difference or -1 if same */
long diff_file(char *fileA, char *fileB)
{
  register long *A, *B, *C;
  int a, b, n, nA, nB, nC, i;
  long buffA[BUFFSIZ], buffB[BUFFSIZ], position;
  char *x, *y;

  if ((a = open(fileA, O_RDONLY)) == -1) return 0;
  if ((b = open(fileB, O_RDONLY)) == -1) { close(a); return 0; }
  position = 0;
  while (1) {
    if ((nA = read(a, (char *)buffA, sizeof(buffA))) < 0) nA = 0;
    if ((nB = read(b, (char *)buffB, sizeof(buffB))) < 0) nB = 0;
    if (nA != nB) {
      if (nA < nB) ((char *)buffB)[nA] = ((char *)buffA)[nA] ^ 1;
      else ((char *)buffA)[nB] = ((char *)buffB)[nB] ^ 1;
    }
    else if (nA == 0) { close(a); close(b); return -1; }
    nC = (nA + nA % sizeof(*buffA)) / sizeof(*buffA);
    for (n = nA; n < nC * sizeof(*buffA); n++)
      ((char *)buffA)[n] = ((char *)buffB)[n] = 0;
    for (A = buffA, B = buffB, C = &buffA[nC]; A != C; )
      if (*(A++) != *(B++)) {
        close(a); close(b);
        x = (char *)(--A); y = (char *)(--B);
        position += x - (char *)buffA;
        for (i = 0; i < sizeof(*buffA); i++)
          if (*(x++) == *(y++)) position++; else break;
        return position;
      }
    position += nA;
  }
}

/* this function links files in this directory to that directory */
/* uses global this_file, this_path  and that_file, that_path */
/* this_path should already exist, that_path is the one created */
/* this is an argument to apply() */
int link_this_to_that(char *path, struct stat *s)
{
  struct stat sbuf;
  char link[PATHSIZ];
  int i;

  *that_file = 0;
  strcpy(that_file, this_file);
  if (!lstat(that_path, &sbuf)) printf("EXISTS: %s\n", that_path);
  else switch (s->st_mode & S_IFMT) {
    case S_IFDIR:
      printf("directory: %s\n", that_path);
      if (mkdir(that_path, 0700)) {
        printf("can't mkdir(%s, 0700)\n", that_path); exit(2);
      }
      break;
    case S_IFREG:
      printf("file: %s\n", that_path);
      if (symlink(this_path, that_path)) {
        printf("can't symlink(%s, %s)\n", this_path, that_path); exit(3);
      }
      break;
    case S_IFLNK:
      printf("link: %s\n", that_path);
      if ((i = readlink(this_path, link, sizeof(link))) == -1) {
        printf("can't readlink(%s, *, *)\n", this_path); exit(4);
      }
      link[i] = 0;
      if (*link == '/') {
        if (symlink(link, that_path)) {
          printf("can't symlink(%s, %s)\n", link, that_path); exit(5);
        }
      }
      else {
        if (symlink(this_path, that_path)) {
          printf("can't symlink(%s, %s)\n", this_path, that_path); exit(6);
        }
      }
      break;
    default:
      printf("IGNORE: %s\n", that_path);
      break;
  }
  fflush(stdout);
  return 0;
}

/* this function compares a file in this directory with one in that */
/* outputs note of changed or extra files in this directory */
/* uses global this_file, this_path  and that_file, that_path */
/* this is an argument to apply() */
#define SHOW_LINES 2  /* number of lines of changes to show */
#define LOOK_BACK  80 /* maximum chars to look back for beginning of line */
int count, extra, changed;   /* counts of extra and changed files */
int diff_this_with_that(char *path, struct stat *s)
{
  struct stat sbuf;
  char link1[PATHSIZ], link2[PATHSIZ];
  int i, c;
  long position, p;
  FILE *f;

  count++;
  position = -1;
  *that_file = 0;
  strcpy(that_file, this_file);
  /* try to open file in that directory, note extra file in this directory */
  if (lstat(that_path, &sbuf)) {
    printf("extra: %s\n", this_file);
    fflush(stdout);
    extra++;
    return 1;
  }
  /* compare files, return if no difference */
  if ((sbuf.st_mode & S_IFMT) == (s->st_mode & S_IFMT))
      switch(sbuf.st_mode & S_IFMT) {
    case S_IFDIR:
    case S_IFSOCK:
      return 0;
    case S_IFCHR:
    case S_IFBLK:
      if (sbuf.st_dev == s->st_dev) return 0;
      break;
    case S_IFREG:
      if ((position = diff_file(this_path, that_path)) == -1) return 0;
      break;
    case S_IFLNK:
      if ((i = readlink(this_path, link1, sizeof(link1))) == -1) {
        printf("can't readlink(%s, *, *)\n", this_path); exit(7);
      }
      link1[i] = 0;
      if ((i = readlink(that_path, link2, sizeof(link2))) == -1) {
        printf("can't readlink(%s, *, *)\n", that_path); exit(8);
      }
      link2[i] = 0;
      if (!strcmp(link1, link2)) return 0;
      break;
  }
  /* note file in this directory is changed */
  printf("changed: %s\n", this_file);
  changed++;
  /* if a change position noted, display SHOW_LINES changes */
  if (position >= 0) {
    if (!(f = fopen(this_path, "r"))) {
      printf("can't fopen(%s, *)\n", this_path); exit(9);
    }
    for (i = 0, p = position; (i < LOOK_BACK) && (p >= 0); i++, p--) {
      if (fseek(f, p, 0)) {
        printf("can't fseek(%s, %d, 0)\n", this_path, position); exit(10);
      }
      if ((c = getc(f)) == '\n') break;
    }
    if ((c == '\n') || (p < 1)) for (i = 0; i < SHOW_LINES; i++) {
      char s[PATHSIZ];
      if (fgets(s, sizeof(s), f)) {
        fputs("  ", stdout);
        fputs(s, stdout);
      }
    }
    else printf("  <difference at position %d>\n", position);
    fclose(f);
  }
  fflush(stdout);
  return 0;
}

/* this function compares a file in that directory with one in this */
/* outputs note of files missing from this directory */
/* uses global this_file, this_path  and that_file, that_path */
/* this is an argument to apply() */
int missing; /* count of missing files */
int diff_that_with_this(char *path, struct stat *s)
{
  struct stat sbuf;

  *this_file = 0;
  /* assume apply(that_path, diff_that_with_this) */
  /* this_file points to the end of this_path directory */
  /* that_file points to the end of that_path directory */
  /* the following puts the file name from that_path into this_path */
  strcpy(this_file, that_file);
  if (lstat(this_path, &sbuf)) {
    printf("missing: %s\n", this_file);
    fflush(stdout);
    missing++;
    return 1;
  }
  return 0;
}

/* this function secures a file using global uid, gid and gid_access flag */
/* this is an argument to apply() */
int uid, gid, gid_access;
int secure_file(char *path, struct stat *s)
{
  register int i;
  int mode = 0;
  int printed = 0;

  if ((s->st_mode & S_IFMT) == S_IFLNK) return 0;
  if ((s->st_uid != uid) || (s->st_gid != gid)) {
    printed++;
    printf("%s: set uid %d, gid %d", path, uid, gid);
    if (chown(path, uid, gid)) { perror(path); error++; }
  }
  if (gid_access) {
    if ((s->st_mode & 07007) || ((s->st_mode & 00660) != 00660))
      mode = (s->st_mode & 07777 & ~07007) | 00660;
  }
  else {
    if ((s->st_mode & 07077) || ((s->st_mode & 00600) != 00600))
      mode = (s->st_mode & 07777 & ~07077) | 00600;
  }
  if (mode) {
    if (!printed) printf("%s: ", path);
    else printf(", ");
    printf("set mode 0%o\n", mode);
    if (chmod(path, mode)) { perror(path); error++; }
  }
  else if (printed) printf("\n");
  return 0;
}

/* this function lists file information */
int do_sum; /* if do_sum non-zero, checksum is calculated, see SUM(1) */
char *list_file(char *path, struct stat *s)
{
  int i, c;
  char link[PATHSIZ], dir[80];
  unsigned sum;
  long nbytes;
  FILE *f;
  static char buffer[BUFFSIZ];

  switch(s->st_mode & S_IFMT) {
    case S_IFDIR:
      sprintf(dir, "d");
      break;
    case S_IFSOCK:
      sprintf(dir, "s");
      break;
    case S_IFCHR:
      sprintf(dir, "c %d %d", major(s->st_rdev), minor(s->st_rdev));
      break;
    case S_IFBLK:
      sprintf(dir, "b %d %d", major(s->st_rdev), minor(s->st_rdev));
      break;
    case S_IFREG:
      if (!do_sum) sprintf(dir, "r %d", s->st_size);
      else {
        if (!(f = fopen(path, "r"))) {
          printf("can't fopen %s", path); exit(11);
        }
        sum = 0;
        nbytes = 0;
        while ((c = getc(f)) != EOF) {
          nbytes++;
          if (sum & 01) sum = (sum >> 1) + 0x8000; else sum >>= 1;
          sum += c; sum &= 0xFFFF;
        }
        if (ferror(f)) {
          printf("can't read %s\n", path); exit(12);
        }
        sprintf(dir, "r %05u %ld", sum, (nbytes+BUFSIZ-1)/BUFSIZ);
        fclose(f);
      }
      break;
    case S_IFLNK:
      if ((i = readlink(path, link, sizeof(link))) == -1) {
        printf("can't readlink(%s, *, *)\n", path); exit(13);
      }
      link[i] = 0;
      sprintf(dir, "l %s", link);
      break;
    default:
      printf("bad mode 0x%X for %s\n", s->st_mode & S_IFMT, path); exit(14);
  }
  sprintf(buffer, "%s 0%o %d %d %d %s",
    path, s->st_mode & 07777, s->st_uid, s->st_gid, s->st_mtime, dir);
  return buffer;
}

/* this function writes the result of list_file() to stdout */
/* this is an argument to apply() */
int write_list_file(char *path, struct stat *s)
{
  puts(list_file(path,s));
  return 0;
}

#define CHECK_FILES 0 /* check files, stdin is list, stdout is status info */
#define FIX_FILES   1 /* try to fix files to agree with stdin list */
#define FAST_FIX    2 /* like FIX_FILES, no checksum or type-specific checks */
#define UPDATE_LIST 3 /* copy stdin to stdout, updating all file information */
/* fix files using (possibly null) specified root according to fixit code */
/* this is a stdin to stdout filter */
int fix_files(char *root, int fixit, char **files)
{
  struct stat s;
  char *buffer1, buffer2[BUFFSIZ], path[PATHSIZ], *original_path, *slash;
  int mode1, mode2, uid1, uid2, gid1, gid2, nargs, dev_major, dev_minor, line;
  time_t mtime1, mtime2, timep[2];
  char type1, type2;
  char arg1a[BUFFSIZ], arg1b[BUFFSIZ], arg2a[BUFFSIZ], arg2b[BUFFSIZ];
  int i;

  /* paths are stored in a single buffer */
  /* the root is a prefix to the original path from the list file input */
  strcpy(path, root); original_path = &path[strlen(path)];
  /* keep track of list file line numbers for error reporting */
  line = 0;
  /* read the next line from the list file */
  while (gets(buffer2)) {
    line++;
    /* ignore empty lines and comment lines */
    if (!*buffer2 || *buffer2 == '#') {
      if (fixit == UPDATE_LIST) puts(buffer2);
      continue;
    }
    /* parse arguments from list file line */
    nargs = sscanf(buffer2, "%s %o %d %d %d %c %s %s",
      original_path, &mode2, &uid2, &gid2, &mtime2, &type2, arg2a, arg2b);
    if ((nargs < 6) || (nargs > 8)) {
      printf("line %d bad format: %s\n", line, buffer2); error++;
      continue;
    }
    if (*original_path != '/') {
      printf("line %d relative path: %s\n", line, buffer2); error++;
      continue;
    }
    switch (type2) {
      case 'd':
        if (nargs == 6) break;
        printf("line %d bad directory: %s\n", line, buffer2); error++;
        continue;
      case 's':
        if (nargs == 6) break;
        printf("line %d bad socket: %s\n", line, buffer2); error++;
        continue;
      case 'c':
      case 'b':
        if (nargs == 8 &&
            sscanf(arg2a, "%d", &dev_major) == 1 &&
            sscanf(arg2b, "%d", &dev_minor) == 1)
          break;
        printf("line %d bad device: %s\n", line, buffer2); error++;
        continue;
      case 'r':
        if (nargs <= 8) break;
        printf("line %d bad file: %s\n", line, buffer2); error++;
        continue;
      case 'l':
        if (nargs == 7) break;
        printf("line %d bad link: %s\n", line, buffer2); error++;
        continue;
      default:
        printf("line %d bad type: %s\n", line, buffer2); error++;
        continue;
    }
    /* check list of specified files */
    if (files) {
      for (i = 0; files[i]; i++) if (!strcmp(files[i], original_path)) break;
      if (!files[i]) {
        if (fixit == UPDATE_LIST) puts(buffer2);
        continue;
      }
    }
    /* look at the file, don't follow symbolic links */
    if (lstat(path, &s)) {
      /* file does not exist */
      /* when updating list, simply omit file in output list */
      if (fixit == UPDATE_LIST) {
        fprintf(tty, "fixit: ignored missing file %s\n", path);
        fflush(tty);
        continue;
      }
      /* otherwise, try to create file */
      else switch (type2) {
        case 'd':
          if (!fixit) printf("%s: missing directory\n", path);
          else if (mkdir(path, mode2)) perror(path);
          else if (chown(path, uid2, gid2)) perror(path);
          else {
            printf("%s: fixed, directory created\n", path);
            error--;
          }
          break;
        case 's':
          /* ignore socket files */
          continue;
        case 'c':
        case 'b':
          if (!fixit) printf("%s: missing device\n", path);
          else if (mknod(path,
              mode2 | (type2 == 'c' ? S_IFCHR : S_IFBLK),
              makedev(dev_major, dev_minor)))
            perror(path);
          else if (chown(path, uid2, gid2)) perror(path);
          else {
            printf("%s: fixed, %c device created\n", path, type2);
            error--;
          }
          break;
        case 'r':
          /* can't fix a missing file */
          printf("%s: missing file\n", path);
          break;
        case 'l':
          if (!fixit) printf("%s: missing link\n", path);
          else if (symlink(arg2a, path)) perror(path);
          else {
            printf("%s: fixed, link created\n", path);
            error--;
          }
          break;
      }
      /* note error, continue to next line from list file input */
      error++;
      continue;
    }
    /* file does exist */
    /* determine if checksum should be calculated */
    do_sum = ((type2 == 'r') && (nargs > 7) && (fixit != FAST_FIX));
    /* gather actual file information */
    buffer1 = list_file(path, &s);
    *arg1a = *arg1b = 0;
    sscanf(buffer1, "%*s %o %d %d %d %c %s %s",
       &mode1, &uid1, &gid1, &mtime1, &type1, arg1a, arg1b);
    /* if updating list, output actual information */
    if (fixit == UPDATE_LIST) {
      printf("%s 0%o %d %d %d %c",
        original_path, mode1, uid1, gid1, mtime2 ? mtime1 : 0, type1);
      if ((nargs > 7) && *arg1b) printf(" %s %s\n", arg1a, arg1b);
      else if (nargs > 6) printf(" %s\n", arg1a);
      else printf("\n");
      continue;
    }
    /* otherwise, first insure that file type is correct */
    if (type1 != type2) {
      printf("%s: type %c should be %c\n", path, type1, type2);
      error++;
      continue;
    }
    /* ignore socket files */
    if (type2 == 's') continue;
    /* check mode */
    if ((mode1 != mode2) && (type2 != 'l')) {
      error++;
      if (!fixit) printf("%s: mode 0%o should be 0%o\n", path, mode1, mode2);
      else if (chmod(path, mode2)) perror(path);
      else {
        printf("%s: fixed, set mode 0%o\n", path, mode2);
        error--;
      }
    }
    /* check uid and gid */
    if ((uid1 != uid2 || gid1 != gid2) && (type2 != 'l')) {
      error++;
      if (!fixit) {
        if (uid1 != uid2) printf("%s: uid %d should be %d\n", path, uid1, uid2);
        if (gid1 != gid2) printf("%s: gid %d should be %d\n", path, gid1, gid2);
      }
      else if (chown(path, uid2, gid2)) perror(path);
      else {
      	if (uid1 != uid2) printf("%s: fixed, set uid %d\n", path, uid2);
      	if (gid1 != gid2) printf("%s: fixed, set gid %d\n", path, gid2);
        error--;
      }
    }
    /* if not fast fix, do type specific checks */
    if (fixit != FAST_FIX) switch (type2) {
      case 'd':
        break;
      case 'c':
      case 'b':
        if (strcmp(arg1a, arg2a) || strcmp(arg1b, arg2b)) {
          error++;
          if (!fixit) printf("%s: device %c %s %s should be %c %s %s\n",
            path, type1, arg1a, arg1b, type2, arg2a, arg2b);
          else {
            if (unlink(path)) perror(path);
            else {
              if (mknod(path,
                  mode2 | (type2 == 'c' ? S_IFCHR : S_IFBLK),
                  makedev(dev_major, dev_minor)))
                perror(path);
              if (chown(path, uid2, gid2)) perror(path);
              else {
                printf("%s: fixed, set device %c %s %s\n",
                  path, type2, arg2a, arg2b);
                error--;
              }
            }
          }
        }
        break;
      case 'r':
        if (nargs == 6) continue;
        if (do_sum) {
          if (strcmp(arg1a, arg2a) || strcmp(arg1b, arg2b)) {
            printf("%s: sum %s %s should be %s %s\n",
              path, arg1a, arg1b, arg2a, arg2b);
            error++;
            continue;
          }
        }
        else {
          if (strcmp(arg1a, arg2a)) {
            printf("%s: size %s should be %s\n", path, arg1a, arg2a); error++;
            continue;
          }
        }
        break;
      case 'l':
        if (strcmp(arg1a, arg2a)) {
          if (fixit == UPDATE_LIST) break;
          error++;
          if (!fixit) printf("%s: link %s should be %s\n",
            path, arg1a, arg2a);
          else if (unlink(path)) perror(path);
          else if (symlink(arg2a, path)) perror(path);
          else {
            printf("%s: fixed, set link to %s\n", path, arg2a);
            error--;
          }
        }
        break;
    }
    /* check modification times */
    if (mtime2 && (mtime1 != mtime2) && (type2 != 'l')) {
      error++;
      if (!fixit) printf("%s: mtime %d should be %d\n", path, mtime1, mtime2);
      else if (lstat(path, &s)) perror(path);
      else {
        timep[0] = s.st_atime; timep[1] = mtime2;
        if (utime(path, timep)) perror(path);
        else {
          printf("%s: fixed, set mtime %d\n", path, mtime2);
          error--;
        }
      }
    }
  }
}

/* this stdin/stdout filter extracts file names from a fixit list */
/* the (possibly null) root will be prefixed to each file name output */
extract_names(char *root)
{
  char buffer[BUFFSIZ], path[PATHSIZ];

  while (gets(buffer)) {
    if (!*buffer || *buffer == '#') continue;
    sscanf(buffer, "%s %*o %*d %*d %*d %*c %*s %*s", path);
    printf("%s%s\n", root, path);
  }
}

/* this stdin/stdout filter adds a root prefix to each file in a list */
/* all comments and blank lines are preserved */
add_root_prefix(char *root)
{
  char buffer[BUFFSIZ];

  while (gets(buffer)) {
    if (!*buffer || *buffer == '#') printf("%s\n", buffer);
    else printf("%s%s\n", root, buffer);
  }
}

/* this stdin/stdout filter removes a root prefix from each file in a list */
/* all comments and blank lines are preserved */
remove_root_prefix(root)
char *root;
{
  char buffer[BUFFSIZ], path[BUFFSIZ];
  int i = strlen(root);

  while (gets(buffer)) {
    if (!*buffer || *buffer == '#') printf("%s\n", buffer);
    else {
      strncpy(path, buffer, i);
      path[i] = 0;
      if (strcmp(path, root)) {
        printf("root missmatch: %s\n", buffer);
        error++;
      }
      else puts(&buffer[i]);
    }
  }
}

/* compare this directory with that */
/* output notes changed, extra or missing files */
diff_directories(char *this, char *that)
{
  printf("comparing: %s\nagainst: %s\n", this, that);
  fflush(stdout);
  strcpy(this_path, this); strcpy(that_path, that);
  this_file = &this_path[strlen(this_path)];
  that_file = &that_path[strlen(that_path)];
  extra = changed = 0;
  *this_file = 0;
  apply(this_path, diff_this_with_that);
  missing = 0;
  *that_file = 0;
  apply(that_path, diff_that_with_this);
  printf("summary: %d files, %d changed, %d extra, %d missing\n",
    count, changed, extra, missing);
}

/* touch a file -- set mtime */
int touch_file(char *path, struct stat *s)
{
  time_t timep[2];

  if ((s->st_mode & S_IFMT) == S_IFREG) {
    timep[0] = timep[1] = touch_time;
    if (utime(path, timep)) { perror(path); error++; }
    count++;
  }
  return 0;
}

/* touch all files in a directory */
touch_files(char *path)
{
  time(&touch_time);
  count = error = 0;
  apply(strcpy(this_path, path), touch_file);
  printf("summary: %d files, %d errors\n", count, error);
}

/* make a directory secure for specified user and (possibly null) group */
/* output is log of changes made to files */
secure_directory(char *usr, char *grp, char *dir)
{
  struct passwd *p;
  struct group *g;

  if (!(p = getpwnam(usr))) {
    printf("%s: unknown user\n", usr); exit(15);
  }
  uid = p->pw_uid; gid = p->pw_gid;
  if (*grp) {
    if (!(g = getgrnam(grp))) {
      printf("%s: unknown group\n", grp); exit(16);
    }
    gid = g->gr_gid;
    gid_access = 1;
  }
  else gid_access = 0;
  printf("making %s secure for user %s, uid %d, gid %d\n", dir, usr, uid, gid);
  apply(strcpy(this_path, dir), secure_file);
}

/* link this directory to that using symbolic links for regular files */
/* output is log of links and directories created */
link_directories(this, that)
char *this, *that;
{
  printf("linking: %s\nto create: %s\n", this, that);
  fflush(stdout);
  strcpy(this_path, this); strcpy(that_path, that);
  this_file= &this_path[strlen(this_path)];
  that_file= &that_path[strlen(that_path)];
  *this_file = 0;
  apply(this_path, link_this_to_that);
}

/* generate file list, do checksum instead of size if flag non-zero */
generate_list(char *path, int sum_flag)
{
  do_sum = sum_flag;
  apply(strcpy(this_path, path), write_list_file);
}

/* output usage instructions */
usage(int error)
{
  if (error) ttymsg("fixit: usage error\n");
  printf("%s usage:\n", VERSION);
  puts("  fixit [ -list ] [ -sum ] /root >list");
  puts("  fixit [ -check [ /root [ files ] ] ] <list >log");
  puts("  fixit -fix [ /root [ files ] ] <list >log");
  puts("  fixit -fastfix [ /root [ files ] ]<list >log");
  puts("  fixit -update [ /root [ files ] ] <list >list");
  puts("  fixit -name [ root ] <list >names");
  puts("  fixit -root /root <list >list");
  puts("  fixit -unroot /root <list >list");
  puts("  fixit -user usr [ -group grp ] /root >log");
  puts("  fixit -link /from /to >log");
  puts("  fixit -diff /this /that >diffs");
  puts("  fixit -touch /root >errors");
  puts("list format: <path> <octal mode> <uid> <gid> <mtime> <type specific>");
  puts("  d                 -- directory");
  puts("  s                 -- socket");
  puts("  c <major> <minor> -- char device");
  puts("  b <major> <minor> -- block device");
  puts("  r <size>          -- regular file without -sum option");
  puts("  r <sum>           -- regular file with -sum (see SUM(1))");
  puts("  l <link>          -- symbolic link");
  puts("if mtime==0, mtime not checked, if r <null>, size/sum not checked");
  exit(error ? 17 : 0);
}

/* parse command line arguments, call appropriate functions */
#define arg1 argv[1]
#define arg2 argv[2]
#define arg3 argv[3]
#define arg4 argv[4]
#define arg5 argv[5]
main(argc, argv)
int argc;
char **argv;
{

  /* initialize */
  stderr_to_stdout();
  open_tty();

  /* -check, the default */
  if (argc <= 1 || !strcmp(arg1, "-check")) {
    if (isatty(fileno(stdin))) usage(1);
    ttymsg("fixit: checking files\n");
    fix_files(argc > 2 ? arg2 : "", CHECK_FILES, argc > 3 ? &arg3 : 0);
    exit(error ? -18 : 0);
  }

  /* -fix or -f */
  if (!strcmp(arg1, "-fix") || !strcmp(arg1, "-f")) {
    if (isatty(fileno(stdin))) usage(1);
    ttymsg("fixit: fixing files\n");
    fix_files(argc > 2 ? arg2 : "", FIX_FILES, argc > 3 ? &arg3 : 0);
    exit(error ? -19 : 0);
  }

  /* -fastfix */
  if (!strcmp(arg1, "-fastfix")) {
    if (isatty(fileno(stdin))) usage(1);
    ttymsg("fixit: fixing files (modes/uid/gid/mtimes only)\n");
    fix_files(argc > 2 ? arg2 : "", FAST_FIX, argc > 3 ? &arg3 : 0);
    exit(error ? -20 : 0);
  }

  /* -update */
  if (!strcmp(arg1, "-update")) {
    if (isatty(fileno(stdin))) usage(1);
    ttymsg("fixit: updating file list\n");
    fix_files(argc > 2 ? arg2 : "", UPDATE_LIST, argc > 3 ? &arg3 : 0);
    if (error) printf("*** %d ERRORS ENCOUNTERED ***\n", error);
    exit(error ? -21 : 0);
  }

  /* -name */
  if (!strcmp(arg1, "-name")) {
    if (argc > 3 || isatty(fileno(stdin))) usage(1);
    ttymsg("fixit: extracting names\n");
    extract_names(argc == 3 ? arg2 : "");
    exit(0);
  }

  /* -root */
  if (!strcmp(arg1, "-root")) {
    if (argc != 3 || isatty(fileno(stdin))) usage(1);
    if (*arg2 != '/') usage(1);
    ttymsg("fixit: adding root prefix to file list\n");
    add_root_prefix(arg2);
    exit(0);
  }

  /* -unroot */
  if (!strcmp(arg1, "-unroot")) {
    if (argc != 3 || isatty(fileno(stdin))) usage(1);
    if (*arg2 != '/') usage(1);
    ttymsg("fixit: removing root prefix from file list\n");
    remove_root_prefix(arg2);
    if (error) printf("*** %d ERRORS ENCOUNTERED ***\n", error);
    exit(error ? 22 : 0);
  }

  /* -user */
  if (!strcmp(arg1, "-user")) {
    if (argc == 6) {
      if (strcmp(arg3, "-group")) usage(1);
      if (arg2[0] == '/') usage(1);
      if (arg4[0] == '/') usage(1);
      if (arg5[0] != '/') usage(1);
    }
    else if (argc == 4) {
      if (arg2[0] == '/') usage(1);
      if (arg3[0] != '/') usage(1);
    }
    else usage(1);
    ttymsg("fixit: securing directory for user\n");
    secure_directory(arg2, argc == 6 ? arg4 : "", argc == 6 ? arg5 : arg3);
    exit(error ? 23 : 0);
  }

  /* -link or -ln */
  if (!strcmp(arg1, "-link") || !strcmp(arg1, "-ln")) {
    if (argc != 4) usage(1);
    if (arg2[0] != '/' || arg3[0] != '/') usage(1);
    ttymsg("fixit: duplicating directory with symbolic links\n");
    link_directories(arg2, arg3);
    exit(0);
  }

  /* -diff */
  if (!strcmp(arg1, "-diff")) {
    if (argc != 4) usage(1);
    if (arg2[0] != '/' || arg3[0] != '/') usage(1);
    ttymsg("fixit: comparing directories\n");
    diff_directories(arg2, arg3);
    exit((extra || changed || missing) ? -1 : 0);
  }

  /* -touch */
  if (!strcmp(arg1, "-touch")) {
    if (argc != 3) usage(1);
    if (arg2[0] != '/') usage(1);
    ttymsg("fixit: touching files\n");
    touch_files(arg2);
    exit(0);
  }

  /* -help or -h */
  if (!strcmp(arg1, "-help") || !strcmp(arg1, "-h")) usage(0);

  /* -list */
  if (!strcmp(arg1, "-list")) { argv++; argc--; }
  if (argc < 2) usage(1);
  if (argc > 3) usage(1);
  if (argc == 3 && strcmp(arg1, "-sum")) usage(1);
  if (argv[argc - 1][0] != '/') usage(1);
  ttymsg("fixit: generating file list\n");
  generate_list(argv[argc - 1], argc == 3);
  exit(error ? 24 : 0);

} /* main() */
