#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_OFFSET 21
#define OP_OFFSET 23
#define START_OFFSET 18
#define TRUE_OFFSET 29
#define FALSE_OFFSET 30
#define NUM_MONKEYS 8 
#define MAX_ITEMS 100

typedef enum {
    OP_ADD,
    OP_MULT,
    OP_DIV,
    OP_SQUARE
} OperationType;

typedef struct {
    OperationType op;
    int n;
} Operation;


typedef struct {
    size_t id;
    uint64_t items[MAX_ITEMS];
    size_t len_items;
    Operation op;
    Operation test;
    size_t t;
    size_t f;
    uint64_t insp_count;
} Monkey;

int parse_input(Monkey *monkeys);
void run(int rounds, Monkey monkeys[], int num_monkeys, int relief_factor);
void inspect_items(int num_monkeys,Monkey monkeys[], int relief_factor,int shared_modulus);
uint64_t perform_op(uint64_t num,Operation op);
uint64_t monkey_business(Monkey monkeys[], int len_monkeys);

int main(void) {

    Monkey monkeys_part1[NUM_MONKEYS];
    Monkey monkeys_part2[NUM_MONKEYS];

    int len_monkeys = parse_input(monkeys_part1);
    //Copy to part 2
    memcpy(&monkeys_part2,&monkeys_part1,NUM_MONKEYS*sizeof(Monkey));
    //Part 1
    run(20,monkeys_part1,len_monkeys,3);
    uint64_t m_bus = monkey_business(monkeys_part1,len_monkeys);
    printf("Monkey Business Part 1: %llu\n",m_bus);

    //Part 2
    run(10000,monkeys_part2,len_monkeys,1);
    m_bus = monkey_business(monkeys_part2, len_monkeys);
    printf("Monkey Business Part 2: %llu\n",m_bus);

    return 0;
}

int parse_input(Monkey monkeys[]) {
    const int line_size = 100;
    FILE *fp;
    fp = fopen("monkeys.txt", "r");
    char *line = (char*)malloc(line_size*sizeof(char));
    int i = 0;
    int id;
    uint64_t item;
    char op;
    int op_num;
    int div;
    int true_pass;
    int false_pass;

    while(fgets(line,line_size,fp) != 0) {

        if(strstr(line,"Monkey") != NULL) {
            // Get the monkey id 
            sscanf(line,"Monkey %d", &id);
            fgets(line,line_size,fp);
            // Starting items
            int offset;
            line += START_OFFSET;
            int j=0;
            uint64_t items[MAX_ITEMS];

             while(sscanf(line,"%lld %n",&item, &offset) != EOF) {
                items[j] = item;
                line += offset+2;
                j++;
             }

            // Operation
            fgets(line,line_size,fp);
            line += OP_OFFSET;
            int num_filled = sscanf(line,"%c %d",&op,&op_num);
            if(num_filled != 2 && op == '*') {
                op = 's';
            }
            fgets(line,line_size,fp);
            // Test
            line += TEST_OFFSET;
            sscanf(line,"%d",&div);
            fgets(line,line_size,fp);
            line += TRUE_OFFSET;
            sscanf(line,"%d",&true_pass);
            fgets(line,line_size,fp);
            line += FALSE_OFFSET;
            sscanf(line,"%d",&false_pass);
            

            // Create Monkey
            monkeys[i].id = id;
            memcpy(monkeys[i].items,items,j*sizeof(uint64_t));
            monkeys[i].len_items = j;
            Operation operation;
            operation.n = op_num;

            switch (op)
            {
            case '*':                
                operation.op = OP_MULT;
                break;
            case '+':
                operation.op = OP_ADD;
                break;
            case '/':
                operation.op = OP_DIV;
                break;
            case 's':
                operation.op = OP_SQUARE;
                break;
            default:
                break;
            }

            monkeys[i].op = operation;
            operation.op = OP_DIV; 
            operation.n = div;
            monkeys[i].test = operation;
            monkeys[i].t = true_pass;
            monkeys[i].f = false_pass;
            monkeys[i].insp_count = 0;

            i++;
        }
    }

    fclose(fp);
    return i;
}

void run(int rounds, Monkey monkeys[], int num_monkeys, int relief_factor) {
    int shared_modulus = 1;

    for(int j=0;j<num_monkeys;j++) {
        shared_modulus *= monkeys[j].test.n;
    }

    for(int i=0;i<rounds;i++) {
        inspect_items(num_monkeys,monkeys,relief_factor,shared_modulus);
    }
}

void inspect_items(int num_monkeys,Monkey monkeys[], int relief_factor,int shared_modulus) {
    int num_items;
    for(int i=0;i<num_monkeys;i++) {
        num_items = monkeys[i].len_items;
        for(int j =0;j<num_items;j++) {
            // Inspect Item
            int location;
            uint64_t item = perform_op(monkeys[i].items[j], monkeys[i].op);
            item /= relief_factor;
            monkeys[i].insp_count++;

            // Test & throw item
            size_t receiver;
            int new_item = item % shared_modulus;
            if(new_item % monkeys[i].test.n == 0) { 
                receiver = monkeys[i].t;
            } else {
                receiver = monkeys[i].f;
            }

            if(num_items >= MAX_ITEMS) {
                // Do something
            }
            location = monkeys[receiver].len_items;
            monkeys[receiver].items[location] = new_item;
            monkeys[receiver].len_items++;

        }
        memset(monkeys[i].items,0,num_items*sizeof(uint64_t));
        monkeys[i].len_items = 0;
    }

}

uint64_t perform_op(uint64_t num,Operation op) {
    switch (op.op)
    {
    case OP_MULT:
        num *= op.n;
        break;
    case OP_DIV:
        num /= op.n;
        break;
    case OP_ADD:
        num += op.n;
        break;
    case OP_SQUARE:
        num *= num;
        break;
    default:
        break;
    }

    return num;
}

uint64_t monkey_business(Monkey monkeys[],int len_monkeys) {
    uint64_t max1 = 0;uint64_t max2 = 0;
    for(int i=0;i<len_monkeys;i++) {
        if(monkeys[i].insp_count > max1) {
            max1 = monkeys[i].insp_count;
        }
    }
    for(int i=0;i<len_monkeys;i++) {
        if(monkeys[i].insp_count>max2 && monkeys[i].insp_count <max1) {
            max2 = monkeys[i].insp_count;
        }
    }
    printf("Max1: %lld, max2: %lld\n",max1,max2);
    return max1*max2;
}