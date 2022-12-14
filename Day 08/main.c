#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WIDTH 150
#define MAX_HEIGHT 150 

int field[MAX_HEIGHT][MAX_WIDTH];

void populate_field(int *height, int *width);
bool isVisible(int x,int y,const int *height,const int *width);
bool checkLeft(int x, int y);
bool checkRight(int x, int y, const int *width);
bool checkUp(int x, int y);
bool checkDown(int x, int y, const int *height);
int scenicScore(int x, int y, const int *height, const int* width);

int main(void) {
    int height = 0;
    int width = 0;
    int num_visible = 0;

    populate_field(&height,&width);

    for(int i = 1;i<height-1;i++) {
        for(int j = 1;j<width-1;j++) {

            if(isVisible(j,i,&height,&width)) {
                num_visible++;
            }
        }
    }

    printf("Total visible: %d\n",num_visible+2*(height+width-2));

    //Part 2
    int score = 0;
    int tmp = 0;
    for(int i=1;i<height-1;i++) {
        for(int j=1;j<width-1;j++) {
            tmp = scenicScore(j,i,&height,&width);
            if(tmp > score) {
                score = tmp;
            }
        }
    }


    printf("Highest scenic score is: %d\n",score);

    //cleanup
    return 0;
}

void populate_field(int *height, int *width) {

    FILE *fp;

    char *line = (char*)malloc(MAX_WIDTH * sizeof(char));

    fp = fopen("trees.txt","r");

    int j;
    int i = 0;
    int pos;

    while(fgets(line,MAX_WIDTH,fp)) {
        *width=strlen(line);
        i = 0;
        while(sscanf(line,"%1d",&j) == 1) {
            field[*height][i] = j;
            line +=1;
            i++;
        }
        *height+=1;
    }

    fclose(fp);
}



bool isVisible(int x,int y,const int *height,const int *width){

    return checkLeft(x,y) || checkRight(x,y,width) || checkDown(x,y,height) || checkUp(x,y);
}
bool checkLeft(int x, int y) {
    int threshold = field[y][x];
    int dx = 0;
    for(;dx<x;dx++) {
        if(field[y][dx] >= threshold) {
            return false;
        }
    }
    return true;
}

bool checkRight(int x, int y, const int *width) {
    int threshold = field[y][x];
    int dx = x+1;
    for(;dx<*width;dx++) {
        if(field[y][dx] >= threshold) {
            return false;
        }
    }
    return true;
}

bool checkUp(int x, int y) {
    int threshold = field[y][x];
    int dy = 0;
    for(;dy<y;dy++) {
        if(field[dy][x] >= threshold) {
            return false;
        }
    }
    return true;
}

bool checkDown(int x, int y, const int *height) {
    int threshold = field[y][x];
    int dy = y+1;
    for(;dy<*height;dy++) {
        if(field[dy][x] >= threshold) {
            return false;
        }
    }
    return true;

}

int scenicScore(int x, int y, const int *height, const int* width) {

    int threshold = field[y][x];
    int dx=0;
    int dy=0;
    int scenicScore = 1;

    //Calc Up 
    for(dy=y-1;dy>=0;dy--) {
        if(field[dy][x] >= threshold || dy==0) {
            scenicScore *= (y - dy);
            break;
        }
    }
    //Calc Down
    for(dy=y+1;dy<*height;dy++) {
        if(field[dy][x] >= threshold || dy == *height-1) {
            scenicScore *= (dy-y);
            break;
        }
    }

    //Calc Left
    for(dx=x-1;dx>=0;dx--) {
        if(field[y][dx] >= threshold || dx == 0) {
            scenicScore *= (x-dx);
            break;
        }
    }

    //Calc Right
    for(dx=x+1;dx<*width;dx++) {
        if(field[y][dx] >= threshold || dx == *width-1) {
            scenicScore *= (dx - x);
            break;
        }
    }
    return scenicScore;
}