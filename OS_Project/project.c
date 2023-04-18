#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

int checkInput(char input[]) {

    if (input[0] != '-') {
        return 0;
    } else {
        for (int i = 1; input[i] != '\0'; i++) {
            if (input[i] != 'n' && input[i] != 'd' && input[i] != 'h' && input[i] != 'm' && input[i] != 'a' && input[i] != 'l') {
                return 0;
            }
        }
    }
    return 1;
}

void display_access_rights(mode_t mode) {
    printf("User:\n");
    printf("Read - %s\n", (mode & S_IRUSR) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWUSR) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXUSR) ? "yes" : "no");
    printf("Group:\n");
    printf("Read - %s\n", (mode & S_IRGRP) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWGRP) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXGRP) ? "yes" : "no");
    printf("Others:\n");
    printf("Read - %s\n", (mode & S_IROTH) ? "yes" : "no");
    printf("Write - %s\n", (mode & S_IWOTH) ? "yes" : "no");
    printf("Exec - %s\n", (mode & S_IXOTH) ? "yes" : "no");
}

void displayRegularFile_info(const char* path, char input[]) {
    struct stat st;
    if (lstat(path, &st) < 0) {
        printf("Error: unable to retrieve information about file %s\n", path);
        return;
    }
     for (int i = 0; input[i]!='\0'; i++)
    {
        switch (input[i])
            {
            case 'n':
                printf("Name:%s\n", path);
                break;
            case 'd':
                printf("Size: %lld bytes\n", (long long)st.st_size);
                break;
            case 'h':
                printf("Hard link count: %ld\n", st.st_nlink);
                break;
            case 'm':
                printf("Time of last modification: %s", ctime(&st.st_mtime));
                break;
            case 'a':
                display_access_rights(st.st_mode);
                break;
            case 'l':
                char linkName[64];
                printf("Enter name of the link: ");
                scanf("%s", linkName);
            
                if (symlink(path, linkName) < 0) {
                    printf("Error: Cannot create symbolic link '%s'\n", linkName);
                }
                break;
            default:
                break;
            }
    
    }

}
    
int main(int argc, char *argv[]) {

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            struct stat stats;
            if (lstat(argv[i], &stats) < 0) {
                printf("Error: unable to retrieve information about file %s\n", argv[i]);
                continue;
            }
            if (!S_ISREG(stats.st_mode)) {
                printf("Argument %d is not a regular file\n", i);
                continue;
            }
            printf("Argument %d is a regular file\n", i);
            printf("Select option(s) (n/d/h/m/a/l)(Include the hyphen before!!!): ");
            char input[7];
            scanf("%6s", input);
            while (!checkInput(input)) {
                printf("Error: invalid option(s). Valid options are n/d/h/m/a/l\n");
                printf("Select option(s) (n/d/h/m/a/l)(Include the hyphen before!!!): ");
                scanf("%6s", input);
            }
            displayRegularFile_info(argv[i], input);
        }
    } else {
        printf("Error: not enough arguments\n");
    }
    return 0;
}
