#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void read_cargo_line(const char* line, size_t len, char* cols[]);
char* move_cargo(char* from,char* to);
char* movencargo(char* from, char* to, int len);

int main(void) {
    FILE *fp;
    char* cargo_line; 
    // Allocate space for reading the file line by line
    cargo_line = (char*) malloc(50 * sizeof(char)); 

    fp = fopen("cargo.txt", "r");
    fgets(cargo_line,50,fp);

    // Determine number of cargo spaces and allocate space for them
    int num_columns = strlen(cargo_line)/4;
    char *cols[num_columns];
    char *cols2[num_columns];

    for(int i=0;i<num_columns;i++) {
        cols[i] = (char*)malloc(50*sizeof(char));
        cols2[i] = (char*)malloc(50*sizeof(char));
    }

    read_cargo_line(cargo_line,strlen(cargo_line),cols);
    read_cargo_line(cargo_line,strlen(cargo_line),cols2);

    while(fgets(cargo_line,50,fp) != NULL) {

        if(strstr(cargo_line," 1 ") != NULL) {
            break;
        } else {
            read_cargo_line(cargo_line,strlen(cargo_line),cols);
            read_cargo_line(cargo_line,strlen(cargo_line),cols2);
        }
    }

    //Process instructions:
    int quantity,from,to;

    fgets(cargo_line,50,fp);

    while(fgets(cargo_line,50,fp) != NULL) {

        sscanf(cargo_line,"move %d from %d to %d", &quantity,&from,&to);
        //Part 1 moves
        for(int i=0;i<quantity;i++) {

            cols[to-1] = move_cargo(cols[from-1],cols[to-1]);
        }

        //Part 2 moves
        cols2[to-1] = movencargo(cols2[from-1],cols2[to-1],quantity);

    }
    //Print output/order
    printf("----\n");
    for(int i=0;i<num_columns;i++) {
        printf("%s\n",cols[i]);
    }

    printf("----\n");
    for(int i=0;i<num_columns;i++) {
        printf("%s\n",cols2[i]);
    }


    //clean up
    free(cargo_line);
    for(int i=0;i<num_columns;i++) {
        free(cols[i]);
        free(cols2[i]);
    }

    return 0;
}

void read_cargo_line(const char* line, size_t len, char* cols[]) {
    for(int i =1;i<len;i+=4) {

        if(line[i] == ' ') {
            continue;
        } else {
            strncat(cols[i/4],&line[i],1);
        }

    }
}

char* move_cargo(char* from,char* to) {

    char* output = (char*) malloc(50*sizeof(char));
    strncpy(output,from,1);
    strcat(output,to);

    memmove(from,from+1,strlen(from));

    return output;
}

char* movencargo(char* from, char* to, int len) {

    char* output = (char*) malloc(50*sizeof(char));
    strncpy(output,from,len);
    strcat(output,to);

    memmove(from,from+len,strlen(from));

    return output;

}