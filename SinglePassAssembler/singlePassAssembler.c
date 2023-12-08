#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main(){
    FILE *finp, *fopt, *fsym, *fsnf, *flist, *fout;
    finp = fopen("input.txt", "r");
    fopt = fopen("optab.txt", "r");
    fsym = fopen("symtab.txt", "w+");
    fsnf = fopen("symnotfound.txt", "w+");
    flist = fopen("list.txt", "w+");
    fout = fopen("output.txt", "w+");

    char label[20], opcode[20], operand[20];
    char opmne[20], opopc[20];
    char symlab[20], symadd[20];
    char snflab[20], snfadd[20];

    int locctr, staddr, addr;
    char name[20];
    fscanf(finp, "%s %s %s", label, opcode, operand);
    if(!strcmp(opcode, "START")){
        sscanf(operand, "%x", &addr);
        locctr=staddr = addr;
        strcpy(name, label);
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    else{
        locctr=staddr = 0;
    }
    fprintf(fout, "H^%s^%06X^%06X\n", name, staddr, 0);
    fprintf(fout, "T^%06X^", locctr);

    char textrec[70]="";
    int textreclen=0;

    while(strcmp(opcode, "END")){
        char curopc[20]="", curopr[20]="0000", curobj[20]="";
        fseek(fsym, 0, SEEK_SET);
        fscanf(fsym, "%s %s", symlab, symadd);
        while(!feof(fsym)){
            if(!strcmp(symlab, operand)){
                strcpy(curopr, symadd);
                break;
            }
            fscanf(fsym, "%s %s", symlab, symadd);
        }
        if(strcmp(label, "-")){
            fseek(fsym, 0, SEEK_END);
            fprintf(fsym, "%s %04X\n", label, locctr);
        }
        fseek(fopt, 0, SEEK_SET);
        fscanf(fopt, "%s %s", opmne, opopc);
        while(!feof(fopt)){
            if(!strcmp(opcode, opmne)){
                strcpy(curopc, opopc);
                if(!strcmp(curopr, "0000")){
                    fprintf(fsnf, "%s %X\n", operand, locctr+1);
                }
                strcpy(curobj, curopc);
                strcat(curobj, curopr);
                fprintf(flist, "%06X %s %s %s %s\n", locctr, label, opcode, operand, curobj);
                locctr+=3;
                break;
            }
            fscanf(fopt, "%s %s", opmne, opopc);
        }
        if(!strcmp(opcode, "RESB")){
            fprintf(flist, "%06X %s %s %s -\n", locctr, label, opcode, operand);
            locctr+=atoi(operand);
        }
        else if(!strcmp(opcode, "RESW")){
            fprintf(flist, "%06X %s %s %s -\n", locctr, label, opcode, operand);
            locctr+=atoi(operand)*3;
        } 
        else if(!strcmp(opcode, "WORD")){
            int wordval;
            sscanf(operand, "%x", &wordval);
            sprintf(curobj, "%06X", wordval);
            fprintf(flist, "%06X %s %s %s %s\n", locctr, label, opcode, operand, curobj);
            locctr+=3;
        }
        else if(!strcmp(opcode, "BYTE")){
            if(operand[0]=='X'){
                for(int i=2; i<strlen(operand)-1; i++){
                        sprintf(curobj, "%s%c", curobj, operand[i]);
                }
            }
            if(operand[0]=='C'){
                for(int i=2; i<strlen(operand)-1; i++){
                    sprintf(curobj, "%s%X", curobj, (int)operand[i]);
                }
            }
            fprintf(flist, "%06X %s %s %s %s\n", locctr, label, opcode, operand, curobj);
            locctr+=strlen(curobj);
        }
        if(strlen(textrec)+strlen(curobj)>60){
            fprintf(fout, "%X%s\n", strlen(textrec)/2, textrec);
            strcpy(textrec, "");
            fprintf(fout, "T^%06X^", locctr);
        }
        if(strlen(curobj)>0)
            sprintf(textrec, "%s^%s", textrec, curobj);
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    if(strlen(textrec)>0){
        fprintf(fout, "%02X%s\n", strlen(textrec)/2, textrec);
    }
    fseek(fsnf, 0, SEEK_SET);
    fscanf(fsnf, "%s %s", snflab, snfadd);
    while(!feof(fsnf)){
        fseek(fsym, 0, SEEK_SET);
        fscanf(fsym, "%s %s", symlab, symadd);
        while(!feof(fsym)){
            if(!strcmp(snflab, symlab)){
                fprintf(fout, "T^00%s^04^%s\n", snfadd, symadd);
                break;
            }
            fscanf(fsym, "%s %s", symlab, symadd);
        }
        fscanf(fsnf, "%s %s", snflab, snfadd);
    }
    fprintf(fout, "E^%06X", staddr);
    fprintf(flist, "%06X %s %s %s -\n", locctr, label, opcode, operand);
    fseek(fout, 0, SEEK_SET);
    fprintf(fout, "H^%s^%06X^%06X\n", name, staddr, locctr-staddr);
    fclose(finp);
    fclose(fsym);
    fclose(fsnf);
    fclose(fopt);
    fclose(flist);
    fclose(fout);
}