/*****************************************
 * \file main.c
 * \brief This code is a CPU simulator
 * \date 1/27/2023
 * \author Nima Balazadeh
 * \mainpage
 *****************************************/
/** @file */
/***************
 * Include Files
 * \brief "ctype.h" is added for toupper function and "string.h" is added for strcpy function
 ***************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

char buffer[20];
char funcname[20];
int s[32];

int stack[50][1];
int Par[8] = {0};
int target_line = 1, current_line = 1, status = 1;
char inputfilename[20];
int stackcol = 49;

void ext();

void situation(int result);

void processor();

void overflow(int result, int first, int second);

int main() {
    printf("Enter the name of the file (example.txt): ");
    scanf("%s", inputfilename);
    FILE *file;
    file = fopen(inputfilename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file");
        return 1;
    }
    while (fscanf(file, "%[^\n]\n", buffer) != EOF) {
        if (status == 1) {
            for (int i = 0; i < 20; i++) {
                if (buffer[i] == ' ') {
                    break;
                } else
                    funcname[i] = buffer[i];
            }
            if (current_line >= target_line) {
                processor();
            }
            current_line++;
        }
        for (int i = 0; i < 20; i++) {
            funcname[i] = '\0';
            buffer[i] = '\0';
        }
    }
    fclose(file);
    return 0;
}

/**
 * \fn void situation(int result)
 * \brief It changes situation flags
 * @param result
 * \return void
 */
void situation(int result) {
    int count1 = 0;
    if (result == 0) {
        Par[1] = 1;
    } else
        Par[1] = 0;
    if (result < 0) {
        Par[2] = 1;
    } else
        Par[2] = 0;
    while (result > 0) {

        if ((result & 1) == 1) {
            count1++;
        }
        result = result >> 1;
    }
    if (count1 % 2 == 1) {
        Par[0] = 1;
    } else
        Par[0] = 0;
}

/**
 * \fn void processor()
 * \brief It is the most important function which is responsible for commands
 * \return void
 */
void processor() {
    for (int i = 0; i < 20; i++) {
        funcname[i] = toupper(funcname[i]);
    }
    for (int i = 0; i < 20; i++) {
        buffer[i] = toupper(buffer[i]);
    }
    if (strcmp(funcname, "ADD") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "ADD S%d, S%d, S%d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] + s[sourse2];
            situation(s[destination]);
            overflow(s[destination], s[sourse1], s[sourse2]);
        }
    } else if (strcmp(funcname, "SUB") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "SUB S%d, S%d, S%d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] - s[sourse2];
            situation(s[destination]);
            overflow(s[destination], s[sourse1], s[sourse2] * -1);
        }
    } else if (strcmp(funcname, "AND") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "AND S%d, S%d, S%d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] & s[sourse2];
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "XOR") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "XOR S%d, S%d, S%d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] ^ s[sourse2];
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "OR") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "OR S%d, S%d, S%d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] | s[sourse2];
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "ADDI") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "ADDI S%d, S%d, %d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] + sourse2;
            situation(s[destination]);
            overflow(s[destination], s[sourse1], sourse2);
        }
    } else if (strcmp(funcname, "SUBI") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "SUBI S%d, S%d, %d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] - sourse2;
            situation(s[destination]);
            overflow(s[destination], s[sourse1], sourse2 * -1);
        }
    } else if (strcmp(funcname, "ANDI") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "ANDI S%d, S%d, %d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] & sourse2;
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "XORI") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "XORI S%d, S%d, %d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] ^ sourse2;
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "ORI") == 0) {
        int destination, sourse1, sourse2;
        sscanf(buffer, "ORI S%d, S%d, %d", &destination, &sourse1, &sourse2);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The destination is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The sourse is out of range!!\n");
        } else {
            s[destination] = s[sourse1] | sourse2;
            situation(s[destination]);
        }
    } else if (strcmp(funcname, "MOV") == 0) {
        int destination, sourse1;
        if (buffer[8] == 'S' || buffer[9] == 'S') {
            sscanf(buffer, "MOV S%d, S%d", &destination, &sourse1);
            if (destination < 0 || destination > 31) {
                fprintf(stderr, "The destination is out of range!!\n");
            } else if (sourse1 < 0 || sourse1 > 31) {
                fprintf(stderr, "The sourse is out of range!!\n");
            } else {
                s[destination] = s[sourse1];
            }
        } else {
            sscanf(buffer, "MOV S%d, %d", &destination, &sourse1);
            if (destination < 0 || destination > 31) {
                fprintf(stderr, "The destination is out of range!!\n");
            } else {
                s[destination] = s[sourse1];
            }
        }
    } else if (strcmp(funcname, "SWP") == 0) {
        int destination, sourse1, tmp;
        sscanf(buffer, "SWP S%d, S%d", &destination, &sourse1);
        if (destination < 0 || destination > 31) {
            fprintf(stderr, "The first one is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The second one is out of range!!\n");
        } else {
            tmp = destination;
            destination = sourse1;
            sourse1 = tmp;
        }
    } else if (strcmp(funcname, "DUMP_REGS") == 0) {
        for (int i = 0; i < 32; i++) {
            printf("register[%d]=%d\n", i, s[i]);
        }
        printf("current situation: ");
        for (int i = 0; i < 8; i++) {
            printf("%d", Par[i]);
        }
        putchar('\n');
    } else if (strcmp(funcname, "INPUT") == 0) {
        printf("Enter number: ");
        scanf("%d", &s[0]);
        situation(s[0]);
    } else if (strcmp(funcname, "OUTPUT") == 0) {
        printf("S0 value is: %d\n", s[0]);
    } else if (strcmp(funcname, "DUMP_REGS_F") == 0) {
        FILE *f = fopen("DUMP_REGS_F", "w+");
        for (int i = 0; i < 32; ++i) {
            fprintf(f, "register[%d]=%d\n", i, s[i]);
        }
        fprintf(f, "situation=");
        for (int i = 0; i < 8; i++) {
            fprintf(f, "%d", Par[i]);
        }
    } else if (strcmp(funcname, "JMP") == 0) {
        int test;
        char jmp[5];
        sscanf(buffer, "JMP %d", &test);
        if (test < current_line) {
            fprintf(stderr, "This will cause an endless loop,Do you want to continue?(y/n):\n");
            scanf("%s", jmp);
            if (strcmp(jmp, "n") == 0) {
                ext();
            } else {
                FILE *file;
                file = fopen(inputfilename, "r");
                current_line = 1;
                while (fscanf(file, "%[^\n]\n", buffer) != EOF) {
                    if (status == 1) {
                        for (int i = 0; i < 20; i++) {
                            if (buffer[i] == ' ') {
                                break;
                            } else
                                funcname[i] = buffer[i];
                        }
                        if (current_line >= test) {
                            processor();
                        }
                        current_line++;
                    }
                    for (int i = 0; i < 20; i++) {
                        funcname[i] = '\0';
                        buffer[i] = '\0';
                    }
                }
                fclose(file);
            }
        } else if (test == current_line) {
            fprintf(stderr, "you can't jump into current lines!\n");
            fprintf(stderr, "Do you want to continue?(y/n) ");
            char answer[1];
            scanf("%s", answer);
            if (strcmp(answer, "n") == 0) {
                ext();
            }
        } else if (test > current_line) {
            target_line = test;
        }
    } else if (strcmp(funcname, "EXIT") == 0) {
        ext();
    } else if (strcmp(funcname, "DIV") == 0) {
        int sourse1, sourse2, tmp;
        sscanf(buffer, "ADD S%d, S%d", &sourse1, &sourse2);
        if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            tmp = s[sourse1];
            s[sourse1] = s[sourse1] / s[sourse2];
            s[sourse2] = tmp % s[sourse2];
        }
    } else if (strcmp(funcname, "SKIE") == 0) {
        int sourse1, sourse2;
        sscanf(buffer, "SKIE S%d, S%d", &sourse1, &sourse2);
        if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            if (s[sourse1] == s[sourse2]) {
                target_line = current_line + 2;
            }
        }
    } else if (strcmp(funcname, "PUSH") == 0) {
        int sourse1;
        sscanf(buffer, "PUSH S%d", &sourse1);
        stack[stackcol][0] = s[sourse1];
        stackcol--;
    } else if (strcmp(funcname, "POP") == 0) {
        int sourse1;
        sscanf(buffer, "POP S%d", &sourse1);
        s[sourse1] = stack[stackcol + 1][0];
        stack[stackcol + 1][0] = 0;
    } else if (strcmp(funcname, "PEEK") == 0) {
        int sourse1;
        sscanf(buffer, "PEEK %d", &sourse1);
        printf("stack[%d]=%d\n", sourse1, stack[50 - sourse1][0]);
    } else if (strcmp(funcname, "ISEMPTY") == 0) {
        if (stack[49][0] == 0 && stack[48][0] == 0) {
            printf("The stack is empty!\n");
        } else
            printf("The stack is not empty!\n");
    } else if (strcmp(funcname, "ISFULL") == 0) {
        if (stack[0][0] == 0 && stack[1][0] == 0) {
            printf("The stack is not full!\n");
        } else
            printf("The stack is full!\n");
    } else if (strcmp(funcname, "MULL") == 0) {
        int sourse2, sourse1, result;
        sscanf(buffer, "MULL S%d, S%d", &sourse2, &sourse1);
        if (sourse2 < 0 || sourse2 > 31) {
            fprintf(stderr, "The first sourse is out of range!!\n");
        } else if (sourse1 < 0 || sourse1 > 31) {
            fprintf(stderr, "The second sourse is out of range!!\n");
        } else {
            int count0 = 0, count1 = 0;
            result = s[sourse1] * s[sourse2];
            int temp = result;
            s[sourse1] = result & 15;
            while (result > 0) {
                if ((result & 1) == 1) {
                    count1++;
                }
                result = result >> 1;
            }
            result = temp;
            int num1 = count1;
            while (count1 > 0) {
                if ((result & 1) == 0) {
                    count0++;
                } else {
                    count1--;
                }
                result = result >> 1;
            }
            int num0 = count0;
            if (num0 + num1 > 4) {
                result = temp;
                s[sourse2] = result >> (num0 + num1 - 4);
            } else
                s[sourse2] = s[sourse1];
        }
        situation(result);

    } else {
        fprintf(stderr, "%s is an invalid syntax!!\n", funcname);
    }
}

/**
 * \fn void overflow(int result, int first, int second)
 * \brief It checks whether the overflow has occurred or not
 * @param result
 * @param first
 * @param second
 * \return void
 */
void overflow(int result, int first, int second) {
    if (first > 0 && second > 0) {
        if (result < 0) {
            Par[5] = 1;
        } else
            Par[5] = 0;
    } else if (first < 0 && second < 0) {
        if (result > 0) {
            Par[5] = 1;
        } else
            Par[5] = 0;
    } else if (first * second < 0) {
        Par[5] = 0;
    }
}

/**
 * \fn void ext()
 * \brief It is for exit command
 * \return void
 */
void ext() {
    fprintf(stderr, "Are you sure you want to exit??(y/n)\n");
    char answer[1];
    scanf("%s", answer);
    if (strcmp(answer, "y") == 0) {
        status = 0;
        fprintf(stderr, "program is finished successfully!");
    }
}