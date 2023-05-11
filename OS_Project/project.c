#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <string.h>

int checkInput(char input[], int option)
{

    if (input[0] != '-')
    {
        return 0;
    }
    else
    {
        for (int i = 1; input[i] != '\0'; i++)
        {
            if ((input[i] != 'n' && input[i] != 'd' && input[i] != 'h' && input[i] != 'm' && input[i] != 'a' && input[i] != 'l') && option == 1)
            {
                return 0;
            }

            if ((input[i] != 'n' && input[i] != 'l' && input[i] != 'd' && input[i] != 't' && input[i] != 'a') && option == 2)
            {
                return 0;
            }

            if ((input[i] != 'n' && input[i] != 'd' && input[i] != 'h' && input[i] != 'm' && input[i] != 'a' && input[i] != 'l' && input[i] != 'c') && option == 3)
            {
                return 0;
            }
        }
    }
    return 1;
}

void display_access_rights(mode_t mode)
{
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

void displayRegularFile_info(const char *path, char input[])
{
    struct stat st;
    if (lstat(path, &st) < 0)
    {
        printf("Error: unable to retrieve information about file %s\n", path);
        return;
    }
    for (int i = 0; input[i] != '\0'; i++)
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

            if (symlink(path, linkName) < 0)
            {
                printf("Error: Cannot create symbolic link '%s'\n", linkName);
            }
            break;

        default:
            break;
        }
    }
}

void displayDirectory_info(const char *path, char input[])
{

    struct stat st;

    if (lstat(path, &st) == -1)
    {
        printf("Error: unable to retrieve information about file %s\n", path);
        return;
    }

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    if (dir == NULL)
    {
        perror("\nError opening directory: ");
        exit(1);
    }

    for (int i = 0; input[i] != '\0'; i++)
    {
        switch (input[i])
        {
        case 'n':

            while ((entry = readdir(dir)) != NULL)
            {
                printf("\nThe directory name is: %s", entry->d_name);
            }
            printf("\n");
            closedir(dir);
            break;
        case 'd':
            printf("Size: %lld bytes\n", (long long)st.st_size);
            break;
        case 'c':
            dir = opendir(path);

            if (dir == NULL)
            {
                perror("\nError opening directory: ");
                exit(1);
            }

            int noOfCFiles = 0;

            while ((entry = readdir(dir)) != NULL)
            {

                char names[1024];

                snprintf(names, sizeof(names), "%s/%s", path, entry->d_name);

                if (lstat(names, &st) == -1)
                {
                    perror("\nError getting file st: ");
                    exit(1);
                }

                if (S_ISREG(st.st_mode) && strstr(entry->d_name, ".c"))
                {

                    noOfCFiles++;
                }
            }

            printf("\nTotal number of .c files in the directory is: %d\n", noOfCFiles);

            closedir(dir);
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

            if (symlink(path, linkName) < 0)
            {
                printf("Error: Cannot create symbolic link '%s'\n", linkName);
            }
            break;
        default:
            break;
        }
    }
}

void displayLink_info(const char *path, char input[])
{
    struct stat info;
    if (lstat(path, &info) == -1)
    {
        printf("Error: unable to retrieve information about file %s\n", path);
        return;
    }

    for (int i = 0; input[i] != '\0'; i++)
    {
        switch (input[i])
        {
        case 'l':
            if (unlink(path))
                printf("Error removing the link\n");
            else
                printf("Link %s removed successfuly\n", path);
            break;
        case 'n':
            printf("Name:%s\n", path);
            break;
        case 'd':
            printf("Size: %ld bytes\n", info.st_size);
            break;
        case 'a':
            display_access_rights(info.st_mode);
            break;
        case 't':
            char target_path[256];
            ssize_t len = readlink(path, target_path, sizeof(target_path) - 1);

            struct stat targetstat;

            if (len != -1)
            {
                target_path[len] = '\0';
                if (lstat(target_path, &targetstat) == 0)
                {
                    if (S_ISREG(targetstat.st_mode))
                    {
                        printf("\nThe size of the target file is: %lld bytes\n", (long long)targetstat.st_size);
                    }
                    else
                    {
                        printf("\nTarget is not a regular file.\n");
                    }
                }
                else
                {
                    perror("lstat");
                    return;
                }
            }
            else
            {
                perror("readlink");
                return;
            }
            break;
        default:
            break;
        }
    }
}
int main(int argc, char *argv[])
{

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            struct stat stats;
            if (lstat(argv[i], &stats) < 0)
            {
                printf("Error: unable to retrieve information about file %s\n", argv[i]);
                continue;
            }
            if (S_ISREG(stats.st_mode))
            {

                printf("Argument %d is a regular file\n", i);
                printf("Select option(s) (n/d/h/m/a/l)(Include the hyphen before!!!): ");
                char input[7];
                scanf("%6s", input);
                while (!checkInput(input, 1))
                {
                    printf("Error: invalid option(s). Valid options are n/d/h/m/a/l\n");
                    printf("Select option(s) (n/d/h/m/a/l)(Include the hyphen before!!!): ");
                    scanf("%6s", input);
                }
                displayRegularFile_info(argv[i], input);
            }

            if (S_ISDIR(stats.st_mode))
            {

                printf("Argument %d is a Directory\n", i);
                printf("Select option(s) (n/d/h/m/a/l/c)(Include the hyphen before!!!): ");
                char input[7];
                scanf("%6s", input);
                while (!checkInput(input, 3))
                {
                    printf("Error: invalid option(s). Valid options are n/d/h/m/a/l/c\n");
                    printf("Select option(s) (n/d/h/m/a/l/c)(Include the hyphen before!!!): ");
                    scanf("%6s", input);
                }
                displayDirectory_info(argv[i], input);
            }
            if (S_ISLNK(stats.st_mode))
            {

                printf("Argument %d is a link\n", i);
                printf("Select option(s) (n/l/d/t/a)(Include the hyphen before!!!): ");
                char input[7];
                scanf("%6s", input);
                while (!checkInput(input, 2))
                {
                    printf("Error: invalid option(s). Valid options are n/l/d/t/a\n");
                    printf("Select option(s) (n/l/d/t/a)(Include the hyphen before!!!): ");
                    scanf("%6s", input);
                }
                displayLink_info(argv[i], input);
            }
        }
    }
    else
    {
        printf("Error: not enough arguments\n");
    }
    return 0;
}
