#include <stdio.h>
#include <stddef.h>

int min_idx(const int *arr,size_t len);

int main(void) {

  FILE *fp;
  fp = fopen("calories.txt", "r");

  int most_calories[] = {0,0,0};
  int total_calories = 0;
  int current_calories = 0;
  int returnVal = 0;
  char buf[255];

  while(fgets(buf,255,fp) != NULL) {
   
    if(buf[0] == '\n') {
      int idx = min_idx(most_calories,sizeof(most_calories) / sizeof(*most_calories));
      most_calories[idx] = total_calories > most_calories[idx] ? total_calories : most_calories[idx];
      total_calories = 0;
    } else {
      sscanf(buf,"%d",&current_calories);
      total_calories += current_calories;  
    }
    
  }

  int idx = min_idx(most_calories,sizeof(most_calories) / sizeof(*most_calories));
      most_calories[idx] = total_calories > most_calories[idx] ? total_calories : most_calories[idx];
  printf("%d\n",most_calories[0]+most_calories[1]+most_calories[2]);
  
  fclose(fp);
  return 0;
}

int min_idx(const int *arr,size_t len) {
  int min = arr[0];
  int k = 0;
  for(int i=1;i<len;i++) {
    if(arr[i]<min) {
      min = arr[i];
      k=i;
    }
  }

  return k;
}