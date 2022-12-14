#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char find_dup_item(const char *items);
char find_badge(const char **items, size_t len);

int main(void) {

    FILE *fp;
    char buf[255];
    char *search_pos;
    int priority = 0;
    int priority2 = 0;

    char *groups[3];
    for(int i = 0;i<3;i++) {
        groups[i] = (char*) malloc(sizeof(char)*255);
    }

    fp = fopen("rucksacks.txt","r");
    int idx = 0;

    while(fscanf(fp, "%s",buf) != EOF) {
        idx++;
        char test = find_dup_item(buf);
        priority += islower(test) ? test - '`' : test - '&';

        strcpy(groups[idx-1],buf);

        if(idx%3 == 0) {
            idx = 0;
            char badge = find_badge(groups,3);
            priority2 += islower(badge) ? badge - '`' : badge - '&';
        }
    }

    printf("Part 1: %d\n",priority);
    printf("Part 2: %d\n",priority2);
    fclose(fp);

    for(int i = 0;i<3;i++) {
        free(groups[i]);
    }
    
    return 0;
}

char find_dup_item(const char *items) {
    char * search_pos;

    int midpoint = strlen(items) / 2;

    for(int i=0;i<midpoint;i++) {
        search_pos = strchr(&items[midpoint],items[i]);

        if(search_pos != NULL) {
            break;
        }

    }
    return *search_pos;
}

char find_badge(const char **items, size_t len) {

    char * search_pos;
    char * search_pos2;

    for(int i =0; i<strlen(items[0]);i++) {
        search_pos = strchr(items[1],items[0][i]);

        if(search_pos != NULL) {
            for(int j=0;j<strlen(items[2]);j++) {
                search_pos2 = strchr(items[2],items[0][i]);

                if(search_pos2 != NULL) {
                    return items[0][i];
                }
            }
        }
    }
}