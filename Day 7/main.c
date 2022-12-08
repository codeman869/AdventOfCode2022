#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 10
#define MAX_DIRS 10
#define MAX_NAME_SIZE 50
#define MAX_LINE_SIZE 50
#define SYSTEM_FILE_SIZE 70000000
#define UPDATE_SIZE 30000000

typedef enum {
    LIST_FILES,
    CHANGE_DIR,
    CMD_PROCESSING,
} Command;

typedef enum {
    FILE_TYPE,
    DIRECTORY_TYPE,
} Artifact_Type;

typedef struct {
    int size;
    char name[MAX_NAME_SIZE];
}File;

typedef struct Directory {
   char name[MAX_NAME_SIZE];
   struct Directory *parent; 
   int directory_size;
   File *files[MAX_FILES];
   int num_files;
   int num_subdirs;
   struct Directory *subdirs[MAX_DIRS];
} Directory;

typedef struct {
    Directory *cur_dir;
} Cursor;

Directory * create_directory(Directory *parent,const char *name);
int add_subdir(Directory *parent, Directory *subdir);
File * create_file(int size, const char* name);
int add_file_to_dir(Directory *dir,File *file);
int create_file_in_dir(Directory *dir, const char* name,int size);
void remove_subdirs(Directory *parent);
Command parse_command(const char* command);
char* parse_args(char* command);
Cursor* change_directory(Cursor* cur_dir,char* directory);
Artifact_Type parse_file(const char* info, char* name, int *size);
void trim(char* string);
void update_parent_dir_size(Directory *cursor, int size);
void find_dirs_by_size_lt(Directory *cursor,int size,Directory *matches[],int*num_matches);
void find_dirs_by_size_gte(Directory *cursor,int size,Directory *matches[],int*num_matches);

int main(void) {

    FILE *fp;

    char *cmd_line = (char*)malloc(MAX_LINE_SIZE*sizeof(char));  

    fp = fopen("commands.txt","r");
    Directory *root = create_directory(NULL,"root");
    Cursor *current_dir = (Cursor*)malloc(sizeof(Cursor));
    current_dir->cur_dir = root;
    Command current_command;

    //Build directory structure
    while(fgets(cmd_line,MAX_LINE_SIZE,fp)) {

        Command next_cmd = parse_command(cmd_line);
        char* args; 
        char* name = (char*)malloc(MAX_LINE_SIZE*sizeof(char));
        int size = 0;
        if(next_cmd == CHANGE_DIR) {
            // Process change dir
            args = parse_args(cmd_line);
            current_command = CHANGE_DIR;
            current_dir = change_directory(current_dir,args);
        } else if(next_cmd == LIST_FILES) {
            // Process List Files
            current_command = LIST_FILES;
            continue;
        }

        if(current_command == LIST_FILES) {
            Artifact_Type artifact = parse_file(cmd_line,name,&size);
            switch (artifact)
            {
            case DIRECTORY_TYPE:
                create_directory(current_dir->cur_dir,name);
                break;
            case FILE_TYPE: 
                //create_file_in_dir(current_dir->cur_dir,name,size);
                create_file_in_dir(current_dir->cur_dir,name,size);
                break;
            default:
                break;
            }
        }

    }

    printf("Finding matches...\n");
    Directory *matches[50]; 
    int num_matches = 0;
    int total_size = 0;

    find_dirs_by_size_lt(root,100000,matches,&num_matches);

    for(int i = 0; i<num_matches;i++) {
        total_size += matches[i]->directory_size;
        printf("Found directory: %s with size %d\n", matches[i]->name, matches[i]->directory_size);
    }

    printf("Total size is: %d\n",total_size);

    // Part 2
    int target_size = SYSTEM_FILE_SIZE - root->directory_size;
    target_size = UPDATE_SIZE - target_size;
    num_matches = 0;
    int min_size = SYSTEM_FILE_SIZE;

    find_dirs_by_size_gte(root,target_size,matches,&num_matches);

    for(int i = 0;i<num_matches;i++) {
        printf("Found directory %s with size %d\n", matches[i]->name,matches[i]->directory_size);

        if(matches[i]->directory_size < min_size) {
            min_size = matches[i]->directory_size;
        }
    }

    printf("Smallest dir has size: %d\n", min_size);

    //Clean up
    remove_subdirs(root);
    free(root);
    free(current_dir);
    fclose(fp);
    return 0;
}

Directory * create_directory(Directory *parent,const char *name) {

    Directory *new_dir = malloc(sizeof(Directory));
    new_dir->parent = parent;
    new_dir->num_files = 0;
    new_dir->num_subdirs = 0;
    new_dir->directory_size = 0;
    strcpy(new_dir->name,name);

    if(parent != NULL) {
        add_subdir(parent,new_dir);
    }

    return new_dir;
}

int add_subdir(Directory *parent, Directory *subdir) {

    if(parent->num_subdirs >= MAX_DIRS) {
        return 1;
    }
    parent->subdirs[parent->num_subdirs] = subdir;
    parent->num_subdirs++;
    return 0;
}

File * create_file(int size, const char* name) {

    File *new_file = (File*)malloc(sizeof(File));
    new_file->size = size;
    strcpy(new_file->name,name);
    return new_file;
}

int add_file_to_dir(Directory *dir,File *file) {
    int num_files = dir->num_files;

    if(num_files >= MAX_FILES) {
        return 1;
    }

    dir->files[dir->num_files] = file;
    dir->num_files++;
    dir->directory_size += file->size;
    update_parent_dir_size(dir->parent,file->size);

    return 0;
}

void remove_subdirs(Directory *parent) {
    for(int i=0;i<parent->num_subdirs;i++) {
        remove_subdirs(parent->subdirs[i]);
        free(parent->subdirs[i]);
    }
}

Command parse_command(const char* command) {
    if(strncmp(command,"$ cd",4) == 0) {
        return CHANGE_DIR;
    }

    if(strncmp(command,"$ ls",4) == 0) {
        return LIST_FILES;
    }

    return CMD_PROCESSING;
}

char* parse_args(char* command) {
    char* token = strtok(command," ");

    while(strcmp(token,"cd")) {
        token = strtok(NULL," ");
    }

    token = strtok(NULL, " ");
    trim(token);

    return token;
}

Cursor* change_directory(Cursor* cur_dir,char* directory) {
    Cursor *new_dir = (Cursor *)malloc(sizeof(Cursor));
    Directory *current_directory;
    current_directory = cur_dir->cur_dir;

    if(strcmp(directory,"..") == 0) {
        // Go up 1 director
        new_dir->cur_dir = cur_dir->cur_dir->parent;
    } else if(strcmp(directory,"/") == 0) {
        // Go to root directory
        while(cur_dir->cur_dir->parent != NULL) {
            cur_dir->cur_dir = cur_dir->cur_dir->parent;
        }
        new_dir->cur_dir =cur_dir->cur_dir;
    } else if(strcmp(directory,".") == 0) {
        // Go to same directory 
        return cur_dir;
    } else {
        // Go to sub directory
        for(int i = 0;i<current_directory->num_subdirs;i++) {
            if(strcmp(current_directory->subdirs[i]->name,directory) == 0) {
                new_dir->cur_dir = current_directory->subdirs[i];
            }
        }
    }
    return new_dir;
}

Artifact_Type parse_file(const char* info, char* name, int* size) {

    char *token = strtok(info," ");

    if(token[0] == 'd') {
        token = strtok(NULL, " ");
        trim(token);
        strcpy(name,token);
        return DIRECTORY_TYPE;
    } else {
        *size = atoi(token);
        token = strtok(NULL," ");
        trim(token);
        strcpy(name,token);
        return FILE_TYPE;
    }

}

int create_file_in_dir(Directory *dir, const char* name,int size) {
    File *new_file = create_file(size,name);
    add_file_to_dir(dir,new_file);

    return 0;
}
void trim(char* string) {
    string[strlen(string)-1] = '\0';
}

void update_parent_dir_size(Directory *cursor, int size){
    if(cursor == NULL) {
        return;
    }
    cursor->directory_size += size;
    update_parent_dir_size(cursor->parent,size);
}

void find_dirs_by_size_lt(Directory *cursor,int size,Directory *matches[],int*num_matches) {

    if(cursor->num_subdirs != 0) {
        for(int i=0;i<cursor->num_subdirs;i++) {
            find_dirs_by_size_lt(cursor->subdirs[i],size,matches,num_matches);
        }
    } 
    if(cursor->directory_size < size) {
        matches[*num_matches] = cursor;
        *num_matches += 1;
    }
}

void find_dirs_by_size_gte(Directory *cursor,int size,Directory *matches[],int*num_matches) {

    if(cursor->num_subdirs != 0) {
        for(int i=0;i<cursor->num_subdirs;i++) {
            find_dirs_by_size_gte(cursor->subdirs[i],size,matches,num_matches);
        }
    } 
    if(cursor->directory_size >= size) {
        matches[*num_matches] = cursor;
        *num_matches += 1;
    }

}