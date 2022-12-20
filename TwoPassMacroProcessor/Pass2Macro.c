#include <stdio.h>
#include <string.h>
void main(){
    char label[20], opcode[20], operand[20];
    char dlabel[20], dopcode[20], doperand[20];
    FILE *fint, *fout, *mnt, *mdt, *argt;
    fint = fopen("inter.txt", "r");
    mnt = fopen("mName.txt", "r");
    mdt = fopen("mDef.txt", "r");
    argt = fopen("mArg.txt", "r");
    fout = fopen("output.txt", "w");
    fscanf(fint, "%s %s %s", label, opcode, operand);
    char mname[20], operands[20][10], temp[20], dtemp[20];
    int mloc;
    while(strcmp(opcode, "END")){
        fseek(mnt, 0, SEEK_SET);
        fscanf(mnt, "%s %d", mname, &mloc);
        if(!strcmp(opcode, mname)){
            fseek(mdt, mloc, SEEK_SET);
            fscanf(mdt, "%s %s %s", dlabel, dopcode, doperand);
            for(int i=0, j=0, k=0; operand[i]!='\0'; i++){
                if(operand[i]==',' || operand[i]=='\0'){
                    j=0;
                    if(doperand[k]!='\0'){
                        while(doperand[k]!=',' || doperand[k]!='\0'){
                            dtemp[j]=doperand[k];
                            dtemp[j+1] = '\0';
                        }
                        operands[atoi(dtemp)] = temp;
                    }
                }
                temp[j] = operand[i];
                temp[j+1] = '\0';
            }
            for(int i = 0, j=0; doperand[i]!='\0'; i++){
                if(doperand[i]==','){
                    j=0;
                    
                }
                temp[j]=doperand[i];
                temp[j+1] = '\0';
            }
        }
        else{
            fprintf(fout, "%s %s %s\n", label, opcode, operand);
        }
    }
}