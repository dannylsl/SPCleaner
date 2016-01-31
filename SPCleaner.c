#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "lib/SPCLog.h"
#include "lib/SPCList.h"
#include "lib/SPCStack.h"
#include "lib/INIRead.h"

#define FT_DIR   0
#define FT_FILE  1

void print_file_type(const char *path) {
    struct stat buf;
    char file_type[30];

    memset(file_type, 0x00, 30);
    if(lstat(path, &buf) < 0) {
        SPC_MSG(LOGERR, strerror(errno));
        return;
    }

    if(S_ISREG(buf.st_mode)) {
        strcpy(file_type,"Regular file");
    } else if(S_ISDIR(buf.st_mode)) {
        strcpy(file_type,"Directory");
    } else if(S_ISCHR(buf.st_mode)) {
        strcpy(file_type,"Character device");
    } else if(S_ISBLK(buf.st_mode)) {
        strcpy(file_type,"Block device");
    } else if(S_ISFIFO(buf.st_mode)) {
        strcpy(file_type,"FIFO");
    } else if(S_ISLNK(buf.st_mode)) {
        strcpy(file_type,"Symbolic link");
    } else if(S_ISSOCK(buf.st_mode)) {
        strcpy(file_type,"SOCKET");
    }
    SPC_MSG(LOGINF, file_type);
    return;
}


int file_or_dir(const char *path) {
    struct stat buf;
    if(lstat(path, &buf) < 0) {
        SPC_MSG(LOGERR, path);
        SPC_MSG(LOGERR, strerror(errno));
        return -1;
    }
    if(S_ISDIR(buf.st_mode)) {
        return FT_DIR; // DIRECTORY
    }else {
        return FT_FILE; // FILE
    }
}

int file_process(const char *path, struct List *module_list) {

    SPC_MSG(LOGINF, path);
    return 0;
}


int traverse(const char *path, const char* parent_dir,
        struct List *module_list, struct List *file_suffix_list) {

    struct dirent *entry;
    DIR *dirp;
    char current_path[PATH_MAX_LEN];
    struct Stack *stk;
    struct Element *elem;

    memset(current_path, 0x00, PATH_MAX_LEN);
    stk = SPCStack_init();

    if( parent_dir != NULL) {
        strcpy(current_path, parent_dir);
        SPC_MSG(LOGINF,current_path);
    }

    if( NULL == (dirp = opendir(path))) {
        SPC_MSG(LOGERR, path);
        SPC_MSG(LOGERR, strerror(errno));
        exit(EXIT_FAILURE);
    }

    elem = SPCStack_new_elem(path, dirp);
    SPCStack_push(stk, elem);

    if(chdir(path) < 0) {
        SPC_MSG(LOGERR, "DIR CHANGE ERROR");
        exit(EXIT_FAILURE);
    }

    while(stk->size != 0) {
        entry = readdir(dirp);
        if(entry == NULL) {
            SPCStack_pop(stk, elem);
            chdir("..");
            closedir(dirp);
            if(stk->tail == NULL) {
                break;
            }
            elem = stk->tail;
            dirp = elem->dirp;
            continue;
        }
        if( 0 == strcmp(entry->d_name,".")
            || (0 == strcmp(entry->d_name,".."))) {
            continue;
        }

        if(FT_DIR == file_or_dir(entry->d_name)) {
            if( NULL == (dirp = opendir(entry->d_name))) {
                SPC_MSG(LOGERR, path);
                SPC_MSG(LOGERR, strerror(errno));
                exit(EXIT_FAILURE);
            }

            elem = SPCStack_new_elem(entry->d_name,dirp);
            SPCStack_push(stk, elem);
            chdir(entry->d_name);
        }else if(FT_FILE == file_or_dir(entry->d_name)) {
            memset(current_path, 0x00, PATH_MAX_LEN);
            SPCStack_get_path(stk, current_path);
            strcat(current_path,"/");
            strcat(current_path,entry->d_name);
            file_process(current_path, module_list);
        }
    }

    return 0;
}

void usage() {
    printf("SPCleaner usage.");
    return;
}

int main(int argc, char *argv[]) {
    struct List *module_list = NULL;
    struct List *file_suffix_list = NULL;
    char path[PATH_MAX_LEN];

    SPC_INIT();

    if(argc != 2) {
        SPC_MSG(LOGERR, "argument count error");
        usage();
        SPC_FREE();
        exit(EXIT_FAILURE);
    }
    memset(path, 0x00, PATH_MAX_LEN);
    strcpy(path, argv[1]);
    SPC_MSG(LOGDBG, argv[1]);

    if(FT_DIR == file_or_dir(path)) {
        traverse(path, NULL, module_list, file_suffix_list);
    } else if(FT_FILE == file_or_dir(path)){
        file_process(path, module_list);
    }

    SPC_FREE();
    return 0;
}
