#include <stdio.h>
#include <string.h>
void main(){
    char label[20], opcode[20], operand[20];
    FILE *fint, *fout, *mnt, *mdt, *argt;
    fint = fopen("inter.txt", 'r');
    mnt = fopen("mName.txt", 'r');
    mdt = fopen("mDef.txt", 'r');
    argt = fopen("mArg.txt", 'r');
    fout = fopen("output.txt", 'w');
    fscanf(fint, "%s %s %s", label, opcode, operand);
    char mname[20], operands[20][10];
    int mloc;
    while(strcmp(opcode, "END")){
        fseek(mnt, 0, SEEK_SET);
        fscanf(mnt, "%s %d", mname, &mloc);
        if(!strcmp(opcode, mname)){
            
        }
    }
}