#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main(){
    FILE *fint, *fout, *fopc, *fsym, *flen;
    fint = fopen("intermediate.txt", "r");
    fopc = fopen("optab.txt", "r");
    fsym = fopen("symtab.txt", "r");
    flen = fopen("length.txt", "r");
    fout = fopen("output.txt", "w");


    char label[20], opcode[20], operand[20];
    char symlab[20], symadd[20];
    char mneu[20], mneucode[20];
    char textrec[100];
    int textlen=0;


    int staddr, addr, len;
    fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
    if(!strcmp(opcode, "START")){
        sscanf(operand, "%x", &addr);
        staddr = addr;
        fscanf(flen, "%x", &len);
        fprintf(fout, "H^%s^%06X^%06X\n", label, staddr,len);
        fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
    }
    else {
        staddr = 0;
    }
    fprintf(fout, "T^%06X", staddr);
    while(strcmp(opcode, "END")){
        if(textlen>60){
            // printf("ssd");
            fprintf(fout, "^%02X", textlen);
            fprintf(fout, "%s\n", textrec);
            strcpy(textrec, "");
            textlen=0;
            fprintf(fout, "T^%06X", addr);
        }
            // printf("ssd");
        if(!strcmp(opcode, "BYTE")){
            if(operand[0]=='X'){
                strcat(textrec, "^");
                strcat(textrec, operand+2);
            }
            else if(operand[0]=='C'){
                strcat(textrec, "^");
                for(int i=2; i<strlen(operand)-1; i++){
                    sprintf(textrec, "%s%02X", textrec, (int)operand[i]);
                }
            }
                textlen += strlen(operand)-3;
        }
        else if(!strcmp(opcode, "WORD")){
            sprintf(textrec, "%s^%06X", textrec, atoi(operand));
            textlen += 3;
        }
        else if(strcmp(opcode, "RESB") && strcmp(opcode, "RESW")){
            printf("hdfs");
            fseek(fopc, 0, SEEK_SET);
            fscanf(fopc, "%s %s", mneu, mneucode);
            while(!feof(fopc)){
                printf("%s %s -- ", mneu, opcode);
                if(!strcmp(mneu, opcode)){
                    strcat(textrec, "^");
                    strcat(textrec, mneucode);
                    break;
                }
                fscanf(fopc, "%s %s", mneu, mneucode);
            }
            fseek(fsym, 0, SEEK_SET);
            fscanf(fsym, "%s %s", symlab, symadd);
        
            while(!feof(fsym)){
                if(!strcmp(symlab, operand)){
                    strcat(textrec, symadd);
                    break;
                }
                fscanf(fsym, "%s %s", symlab, symadd);
            }
            textlen += 3;
        }
        fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
    }
    if(strlen(textrec)<70){
        fprintf(fout, "%s\n", textrec);
    }
    fprintf(fout, "E^%06X", staddr);
    fclose(fint);
    fclose(fopc);
    fclose(fsym);
    fclose(flen);
    fclose(fout);
}