#include <stdio.h>

enum hand {Rock=1,Paper=2,Scissors=3};
enum outcome {Win=6,Loss=0,Draw=3};

enum outcome evalHand(const enum hand a,const enum hand b);
enum hand convertHand(const char *play);
enum outcome convertOutcome(const char *play);
enum hand findHand(const enum hand a, const enum outcome b);

int main(void) {
  FILE *fp;

  fp = fopen("scoresheet.txt", "r");
  char a, b;
  int score = 0;
  int altScore = 0;


  while(fscanf(fp, "%c %c\n",&a,&b) != EOF) {

  enum hand opponent = convertHand(&a);
  enum hand player = convertHand(&b);
  enum outcome gameStatus = convertOutcome(&b);

  score += evalHand(opponent,player) +player;
  altScore += gameStatus + findHand(opponent,gameStatus);

  
}
  printf("Final score is: %d\n",score);
  printf("Alternate score is: %d\n",altScore);
  fclose(fp);
}

enum hand convertHand(const char *play) {
  if(*play == 'A' || *play == 'X') {
    return Rock;
  } else if(*play == 'B' || *play == 'Y') {
    return Paper;
  } else {
    return Scissors;
  }
}

enum outcome evalHand(const enum hand a,const enum hand b) {
    if(a-b == 0) {
      return Draw;
    } else if((b == Rock && a == Paper) || (b == Paper && a == Scissors) || (b == Scissors && a == Rock)) {
      return Loss;
    } else {
      return Win;
    }
}

enum outcome convertOutcome(const char *play) {
  if(*play == 'X') {
    return Loss;
  } else if (*play == 'Y') {
    return Draw;
  } else {
    return Win;
  }
}

enum hand findHand(const enum hand a, const enum outcome b) {
  if(b == Draw) {
    return a;
  }

  if(a==Rock) {
    return b == Win ? Paper : Scissors;
  } else if(a==Paper) {
    return b == Win ? Scissors : Rock;
  } else {
    return b == Win ? Rock : Paper;
  }
  
  
}