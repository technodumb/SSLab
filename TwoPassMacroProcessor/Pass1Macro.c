#include <stdio.h>
#include <string.h>
int main() {
    char line[256], label[20], opcode[20], operand[20];
    char mName[20], mOper[20], arg[20];
    FILE *finp, *mnt, *mdt, *argt, *fint;
    finp = fopen("input.txt", "r");
    mnt = fopen("mName.txt", "w");
    mdt = fopen("mDef.txt", "w");
    argt = fopen("mArg.txt", "w+");
    fint = fopen("inter.txt", "w");
    
    int MDTC=1, MNTC=1, ARGTC=0;
    // fgets(line, sizeof(line), finp);
    fscanf(finp, "%s %s %s", label, opcode, operand);
    while(strcmp(opcode,"END")){
        if(!strcmp(opcode,"MACRO")){
            strcpy(mName, label);
            strcpy(mOper, operand);
            MDTC=ftell(mdt);
            fprintf(mnt, "%s %d\n", mName, MDTC);
            fprintf(mdt, "%s %s ", label, opcode);
            MNTC++;
            strcpy(arg, "");
            // printf("%s", arg);
            for(int i=0, j=0; mOper[i]!='\0'; i++, j++){
                if(mOper[i]==',' || mOper[i]=='\n'){
                    fprintf(argt, "%d %s\n", ARGTC, arg);
                    fprintf(mdt, "%d,", ARGTC);
                    ARGTC++;
                    printf("%d %s\n", ARGTC, arg);
                    strcpy(arg, "");
                    j=-1;
                    continue;
                }
                arg[j] = mOper[i];
                arg[j+1] = '\0';
            }
            if(strcmp(arg, "")){
                fprintf(argt, "%d %s\n", ARGTC, arg);
                fprintf(mdt, "%d\n", ARGTC);
                ARGTC++;
                printf("%d %s\n", ARGTC, arg);
            }
            // MDTC++;
            fscanf(finp, "%s %s %s", label, opcode, operand);
            fseek(argt, 0, SEEK_SET);
            while(strcmp(opcode,"MEND")){
                if(operand[0]=='&'){
                    char argnum[20],argval[20];
                    fscanf(argt, "%s %s", argnum, argval);
                    while(!feof(argt)){
                        if(!strcmp(argval, operand)){
                            strcpy(operand, argnum);
                            break;
                        }
                        fscanf(argt, "%s %s", argnum, argval);
                    }
                }
                fprintf(mdt, "%s %s %s\n", label, opcode, operand);
                // MDTC++;
                fscanf(finp, "%s %s %s", label, opcode, operand);   
            }
            fseek(argt, 0, SEEK_END);

            fprintf(mdt, "%s %s %s\n", label, opcode, operand);
        }
        else {
            fprintf(fint, "%s %s %s\n", label, opcode, operand);
        }
        fscanf(finp, "%s %s %s", label, opcode, operand);   
    }
    fprintf(fint, "%s %s %s\n", label, opcode, operand);

    fclose(finp);
    fclose(mnt);
    fclose(mdt);
    fclose(argt);
    fclose(fint);
}