#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int expanding = 0;

void main()
{
    FILE *finp, *fout, *farg, *fmdt, *fmnt;
    char label[20], opcode[20], operand[20];
    char dlabel[20], dopcode[20], doperand[20];
    char mname[20], mdtline[20], arg[20];
    char arguments[20][20], argument[20];
    char expargs[20][20], exparg[20];
    int curLine=0, mloc=0, narg=0;
    int i, j, di, dj;
    finp = fopen("input.txt", "r");
    fout = fopen("output.txt", "w+");
    farg = fopen("arg.txt", "w+");
    fmdt = fopen("mdt.txt", "w+");
    fmnt = fopen("mnt.txt", "w+");
    
    fscanf(finp, "%s %s %s", label, opcode, operand);

    while(strcmp(opcode, "END")) {
        if(!strcmp(opcode, "MACRO")){
            int level=1;
            fseek(fmnt, 0, SEEK_END);
            fprintf(fmnt, "%s %d\n", label, ftell(fmdt));
            fprintf(fmdt, "%s %s ", label, opcode);
            i=0;
            while(operand[i]!='\0'){
                j=0;
                while(operand[i]!='\0' && operand[i]!=',') {
                    argument[j]=operand[i];
                    argument[j+1]='\0';
                    j++; i++;
                }
                strcpy(arguments[narg], argument);
                fprintf(fmdt, "?%d", narg);
                if(operand[i]==','){ 
                    fprintf(fmdt, ",");
                    i++;
                }
                narg++;
            }
            fprintf(fmdt, "\n");
            while(level>0) {
                fscanf(finp, "%s %s %s", label, opcode, operand);
                if(operand[0]=='&'){
                    for(int i=0; i<narg; i++){
                        if(!strcmp(operand, arguments[i])){
                            sprintf(operand, "?%d", i);
                            break;
                        }
                    }
                }
                fprintf(fmdt, "%s %s %s\n", label, opcode, operand);
                if(!strcmp(opcode, "MACRO")){
                    level++;
                }
                else if(!strcmp(opcode, "MEND")){
                    level--;
                }
            }
        }
        else {
            fseek(fmnt, 0, SEEK_SET);
            fscanf(fmnt, "%s %d", mname, &mloc);
            int found=0;
            while(!feof(fmnt)){
                if(!strcmp(opcode, mname)){
                    found++;
                    expanding = 1;
                    i=j=di=dj=0;
                    char dindex[20];
                    fseek(fmdt, mloc, SEEK_SET);
                    fscanf(fmdt, "%s %s %s", dlabel, dopcode, doperand);
                    while(operand[i]!='\0'){
                        j=0;
                        while(operand[i]!='\0' && operand[i]!=',') {
                            exparg[j]=operand[i];
                            exparg[j+1]='\0';
                            j++; i++;
                        }
                        if(operand[i]==',')
                            i++;
                        dj=0;
                        while(doperand[di]!='\0' && doperand[di]!=','){
                            dindex[dj]=doperand[di];
                            dindex[dj+1]='\0';
                            dj++; di++;
                        }
                        if(doperand[di]==',')
                            di++;
                        // printf("%d %s", atoi(dindex), exparg);
                        strncpy(dindex, dindex+1, strlen(dindex));
                        printf("%s %s\n", dindex, exparg);
                        strcpy(expargs[atoi(dindex)], exparg);
                    }
                    fscanf(fmdt, "%s %s %s", dlabel, dopcode, doperand);
                    while(strcmp(dopcode, "MEND")){
                        if(doperand[0]=='?'){
                            strncpy(doperand, doperand+1, strlen(doperand));
                            strcpy(doperand, expargs[atoi(doperand)]);
                        }
                        fprintf(fout, "%s %s %s\n", dlabel, dopcode, doperand);
                        fscanf(fmdt, "%s %s %s", dlabel, dopcode, doperand);
                    }
                    // break;
                }
                fscanf(fmnt, "%s %d", mname, &mloc);
            }
            if(!found){
                fprintf(fout, "%s %s %s\n", label, opcode, operand);
            }
        }
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    fprintf(fout, "%s %s %s\n", label, opcode, operand);
    fclose(finp);
    fclose(fout);
    fclose(farg);
    fclose(fmdt);
    fclose(fmnt);
}

