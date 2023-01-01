#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main() {
    FILE *finp, *fopc, *fsym, *fsnf, *fint, *fout;
    finp = fopen("input.txt", "r");
    fopc = fopen("optab.txt", "r");
    fsym = fopen("symtab.txt", "w+");
    fsnf = fopen("symnotfound.txt", "w+");
    fint = fopen("interm.txt", "w+");
    fout = fopen("output.txt", "w+");

    int locctr, staddr, addr;
    int ntexts=0;

    char label[20], opcode[20], operand[20];
    char optmne[20], optcod[20];
    char symlab[20], symadd[20];
    char snflab[20], snfadd[20];
    char name[20];

    fscanf(finp, "%s %s %s", label, opcode, operand);
    if(!strcmp(opcode, "START")){
        sscanf(operand, "%x", &addr);
        locctr = staddr = addr;
        strcpy(name, label);
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    else {
        locctr = staddr = 0;
    }
    while(strcmp(opcode, "END")){
        char curopr[20]="0000";
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
            fprintf(fsym, "%s %X\n", label, locctr);
        }
        fseek(fopc, 0, SEEK_SET);
        fscanf(fopc, "%s %s", optmne, optcod);
        char curopc[20]="";
        while(!feof(fopc)){
            if(!strcmp(opcode, optmne)){
                strcpy(curopc, optcod);
                if(!strcmp(curopr, "0000")){
                    fprintf(fsnf,"%s %X\n", operand, locctr+1);
                }
                fprintf(fint, "%06X\t%s\t%s\t%s\t%s%s\n", locctr, label, opcode, operand, curopc, curopr);
                locctr+=3;
                break;
            }
            fscanf(fopc, "%s %s", optmne, optcod);
        }
        if(!strcmp(opcode, "RESB") ){
            fprintf(fint, "%06X\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, "-");
            locctr += atoi(operand);
        }
        else if(!strcmp(opcode, "RESW") ){
            fprintf(fint, "%06X\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, "-");
            locctr += atoi(operand)*3;
        }
        else if(!strcmp(opcode, "BYTE") ){
            int byteindex=0;
            for(int i=2; i<strlen(operand)-1; i++){
                if(operand[0]=='X'){
                    curopr[byteindex] = operand[i];
                    byteindex++;
                }
                if(operand[0]=='C'){
                    byteindex+= sprintf(&curopr[byteindex],"%X", (int)operand[i]);
                }
                curopr[byteindex] = '\0';
            }
            fprintf(fint, "%06X\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, curopr);
            locctr += strlen(operand)-3;
        }
        else if(!strcmp(opcode, "WORD") ){
            int wordval;
            sscanf(operand, "%x", &wordval);
            fprintf(fint, "%06X\t%s\t%s\t%s\t%06X\n", locctr, label, opcode, operand, wordval);
            locctr += strlen(operand)-3;
        }
        fscanf(finp, "%s %s %s", label, opcode, operand);
    }
    fprintf(fout, "H^%s^%06X^%06X\n", name, staddr, locctr-staddr);
    fseek(fint, 0, SEEK_SET);

    char codeAddr[20], code[20];
    char textrec[70]="";
    int textreclen=0;

    while(!feof(fint)){
        fscanf(fint, "%s %s %s %s %s", codeAddr, label, opcode, operand, code);
        if(!strcmp(opcode, "RESB") || !strcmp(opcode, "RESW")){
            continue;
        }
            if(strlen(textrec)>70){
                fprintf(fout, "T^%s^%02X%s\n", codeAddr, textreclen, textrec);
                textreclen=0;
                strcpy(textrec, "");
            }
            strcat(textrec, "^");
            strcat(textrec, code);
            textreclen+=strlen(code)/2;
    }
    if(strlen(textrec)<70){
        fprintf(fout, "T^%s^%02X%s\n", codeAddr, textreclen, textrec);
        textreclen=0;
        strcpy(textrec, "");
    }
    fseek(fsnf, 0, SEEK_SET);
    fscanf(fsnf, "%s %s", snflab, snfadd);
    while(!feof(fsnf)){
        fseek(fsym, 0, SEEK_SET);
        fscanf(fsym, "%s %s", symlab, symadd);
        while(!feof(fsym)){
            printf("%s %s", symlab, snflab);
            if(!strcmp(symlab, snflab)){
                fprintf(fout, "T^%s^02^%s\n", snfadd, symadd);
                break;
            }
            fscanf(fsym, "%s %s", symlab, symadd);
        }
        fscanf(fsnf, "%s %s", snflab, snfadd);
    }
    fprintf(fout, "E^%06X", staddr);
    printf("done");
    fclose(finp);
    fclose(fopc);
    fclose(fsym);
    fclose(fsnf);
    fclose(fint);
    fclose(fint);
}
