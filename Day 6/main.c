#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_marker(char *stream,size_t size);

int main(void) {
    FILE *fp;

    fp = fopen("datastream.txt","r");

    char *stream = (char*)malloc(5000*sizeof(char));

    fscanf(fp,"%s",stream);

    int position = find_marker(stream,14);

    printf("Marker found at position: %d\n", position);

    //cleanup
    fclose(fp);
    free(stream);
    return 0;
}


int find_marker(char *stream,size_t len) {
        
    char *test = (char*)malloc(len*sizeof(char));
    char *found;
    int i = 0;
    int j = 0;
    int stream_length = strlen(stream);

    for(int i =0;i<stream_length;i++) {
        memmove(test,stream,len);
        for(int j=0;j<len;j++) {
            char rtest = test[0];
            memmove(test,test+1,strlen(test));
            test[len-j] = '\0';
            found = strchr(test,rtest);
            if(found != NULL) {
                break;
            }
        }
        if(found == NULL) {
            free(test);
            return i+len;
        }
        memmove(stream,stream+1,strlen(stream));
   }
   free(test);
   return 0;
}