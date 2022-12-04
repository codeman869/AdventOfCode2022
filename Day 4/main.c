#include <stdio.h>
#include <stdlib.h>

int main(void) {

    FILE *fp;

    int start1,end1,start2,end2, num_overlaps,num_overlap2;
    num_overlaps = 0;
    num_overlap2 = 0;

    fp = fopen("assignments.txt", "r");

    while(fscanf(fp,"%d-%d,%d-%d",&start1,&end1,&start2,&end2) != EOF) {
        if((start1 <= start2 && end1 >= end2) || (start2<=start1 && end2>= end1)) {
            num_overlaps++;
        }

        if((start1<=start2 && end1>=start2) || (start2<=start1 && end2>=start1)) {
            num_overlap2++;
        }
    }

    printf("Num of full overlaps: %d\n", num_overlaps);
    printf("Num of partial overlaps: %d\n", num_overlap2);
    return 0;
}