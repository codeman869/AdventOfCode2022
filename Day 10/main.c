#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CMDS 150
#define SCREEN_X 40
#define SCREEN_Y 6

typedef enum {
    ADDX,
    NOOP,
} Operation;

typedef struct {
    Operation operation;
    int ticks;
    int argument;
} Command;

typedef struct {
    int amount; 
    int reported;
} Cycle;

void read_commands(Command *commands, int *len);
int process_cmds(Command *cmd, int len);
int process_cmd(Command cmd, int current_tick);
int calc_signal_str(int signal_str,int next_tick);
void draw_pixels(int tick);
void tick(int ticks,int current_tick);
void print_screen(void);

int reg_x;
Cycle CYCLES[] = {{20,0},{60,0},{100,0},{140,0},{180,0},{220,0}};
char screen[SCREEN_Y][SCREEN_X];

int main(void) {

    Command commands[MAX_CMDS];

    reg_x = 1;
    int length = 0;
    int signal_str = 0;

    read_commands(commands,&length);
    signal_str = process_cmds(commands,length);

    print_screen();

    printf("Calculated %d signal strength\n",signal_str);

    return 0;
}

void read_commands(Command *commands, int *len) {
    FILE *fp;
    char *line = (char*)malloc(10*sizeof(char));
    char *cmd = (char*)malloc(4*sizeof(char));
    int argument = 0;
    *len = 0;
    fp = fopen("program.txt", "r");

    while(fgets(line,10,fp) != NULL) {
        if(line[0] == 'n') {
            commands[*len].operation = NOOP;
            commands[*len].ticks = 1;
        } else {
            commands[*len].operation = ADDX;
            commands[*len].ticks = 2;
            sscanf(line,"%s %d",cmd,&argument);
            commands[*len].argument = argument;
        }

        (*len)++;
    }

    fclose(fp);
    free(line);
    free(cmd);
}

int process_cmds(Command *cmd, int len) {
    int ticks = 0;
    int next_tick = 0;
    int signal_str = 0;

    for(int i=0;i<len;i++) {
        next_tick = ticks + cmd[i].ticks;
        signal_str = calc_signal_str(signal_str,next_tick);
        // Draw pixels
        tick(cmd[i].ticks,ticks);

        ticks = process_cmd(cmd[i],ticks);
    }
    
    return signal_str; 
}

int process_cmd(Command cmd, int current_tick) {

    switch (cmd.operation)
    {
    case NOOP:
        //return current_tick+cmd.ticks;
        break;
    case ADDX:
        reg_x += cmd.argument;
        break;
    default:
        break;
    }

    return current_tick+cmd.ticks;
}

int calc_signal_str(int signal_str,int next_tick) {

    for(int i=0;i<6;i++) {
        if(!CYCLES[i].reported && next_tick >= CYCLES[i].amount ) {
            CYCLES[i].reported = 1;
            return signal_str + CYCLES[i].amount * reg_x;
        } else if(CYCLES[i].amount > next_tick) {
            break;
        }
    }

    return signal_str;
}

void draw_pixels(int tick) {
    int row = tick / SCREEN_X;
    int column = tick % SCREEN_X;

    if(abs(reg_x - column) <= 1) {
        //Draw lit pixel
        screen[row][column] = '#';
    } else {
        // Draw dim pixel
        screen[row][column] = '.';
    }

}

void tick(int ticks,int current_tick) {
    int i=0;
    do
    {
        draw_pixels(current_tick+i);
        ticks--;
        i++;
    } while (ticks>0);
}

void print_screen(void) {
    for(int i=0;i<SCREEN_Y;i++) {
        for(int j=0;j<SCREEN_X;j++) {
            printf("%c",screen[i][j]);
        }
        printf("\n");
    }

}