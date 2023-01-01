#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main() {
    FILE *finp, *fopc, *fsym, *fint;
    finp = fopen("input.txt", "r");
    // fout = fopen("output.txt", "w");
    fopc = fopen("optab.txt", "r");
    fsym = fopen("symtab.txt", "w");
    fint = fopen("intermediate.txt", "w");

    char label[20], opcode[20], operand[20];
    char mneu[20], mneucode[20];
    char symlab[20], symadd[20];
    int staddr, locctr, ioper;
    char pName[20];
    fscanf(finp, "%s %s %x", label, opcode, &staddr);
    if(!strcmp(opcode, "START")){
        locctr = staddr;
        strcpy(pName, label);
        fprintf(fint, "%x %s %s %x\n", staddr, label, opcode, staddr);
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    else {
        locctr=staddr=0;
    }

    while(strcmp(opcode, "END")){
        fprintf(fint, "%04x\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        if(strcmp(label, "-")){
            fprintf(fsym, "%s %06x", label, locctr);
        }
        if(!strcmp(opcode, "RESB") || !strcmp(opcode,"RESW")){
            // tbc
            ioper = atoi(operand);
            if(!strcmp(opcode, "RESW")){
                locctr += ioper*3;
            }
            else
                locctr += ioper;
        }
        else if(!strcmp(opcode, "WORD")|| !strcmp(opcode, "BYTE")){
            if(strcmp(opcode, "BYTE")){
                locctr+=strlen(operand)-2;
            }
            else{
                locctr+=3;
            }
        }
        else {
            fseek(fopc, 0, SEEK_SET);
            fscanf(fopc, "%s %s", mneu, mneucode);
            while(!feof(fopc)){
                printf("hey");
                if(!strcmp(mneu, opcode)){
                    locctr+=3;
                    break;
                }
                fscanf(fopc, "%s %s", mneu, mneucode);
            }
        }
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    fclose(finp);
    fclose(fint);
    fclose(fsym);
    fclose(fopc);
}