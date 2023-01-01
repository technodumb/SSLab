#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main(){
    FILE *fint, *fout, *fopc, *fsym, *flen, *flis;
    fint = fopen("intermediate.txt", "r");
    fopc = fopen("optab.txt", "r");
    fsym = fopen("symtab.txt", "r");
    flen = fopen("length.txt", "r");
    fout = fopen("output.txt", "w");
    flis = fopen("list.txt", "w");


    char label[20], opcode[20], operand[20];
    char symlab[20], symadd[20];
    char mneu[20], mneucode[20];
    char textrec[100];
    int textlen=0;


    int staddr, addr, len;
    fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
    fprintf(flis, "%X\t%s\t%s\t%s\n", addr, label, opcode, operand);

    if(!strcmp(opcode, "START")){
        sscanf(operand, "%x", &addr);
        staddr = addr;
        fscanf(flen, "%x", &len);
        fprintf(fout, "H^%s^%06X^%06X\n", label, staddr,len);
        fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
        fprintf(flis, "%X\t%s\t%s\t%s", addr, label, opcode, operand);
    }
    else {
        staddr = 0;
    }
    fprintf(fout, "T^%06X", staddr);
    while(strcmp(opcode, "END")){
        if(textlen>30){
            // printf("ssd");
            fprintf(fout, "^%02X", textlen);
            fprintf(fout, "%s\n", textrec);
            strcpy(textrec, "");
            fprintf(fout, "T^%06X", addr);
            textlen=0;
        }
            // printf("ssd");
        if(!strcmp(opcode, "BYTE")){
                strcat(textrec, "^");
                char temp[20];
            if(operand[0]=='X'){
                for(int i=2; i<strlen(operand)-1; i++){
                    sprintf(temp, "%s%c", temp, operand[i]);
                }
                
            }
            else if(operand[0]=='C'){
                for(int i=2; i<strlen(operand)-1; i++){
                    sprintf(temp, "%s%02X", temp, (int)operand[i]);
                }
            }
            strcat(textrec, temp);
            fprintf(flis, "\t%s\n", temp);
            textlen += strlen(operand)-3;
        }
        else if(!strcmp(opcode, "WORD")){
            sprintf(textrec, "%s^%06X", textrec, atoi(operand));
            fprintf(flis, "\t%06X\n", atoi(operand));
            textlen += 3;
        }
        else if(!strcmp(opcode, "RESB") || !strcmp(opcode, "RESW")){
            fprintf(flis, "\n");
        }
        
        else{
            fseek(fopc, 0, SEEK_SET);
            fscanf(fopc, "%s %s", mneu, mneucode);
            while(!feof(fopc)){
                printf("%s %s -- ", mneu, opcode);
                if(!strcmp(mneu, opcode)){
                    strcat(textrec, "^");
                    strcat(textrec, mneucode);
                    fprintf(flis, "\t%s", mneucode);
                    break;
                }
                fscanf(fopc, "%s %s", mneu, mneucode);
            }
            fseek(fsym, 0, SEEK_SET);
            fscanf(fsym, "%s %s", symlab, symadd);
        
            while(!feof(fsym)){
                if(!strcmp(symlab, operand)){
                    strcat(textrec, symadd);
                    fprintf(flis, "\t%s\n", symadd);
                    break;
                }
                fscanf(fsym, "%s %s", symlab, symadd);
            }
            textlen += 3;
        }
        fscanf(fint, "%X %s %s %s", &addr, label, opcode, operand);
        fprintf(flis, "%X\t%s\t%s\t%s", addr, label, opcode, operand);

    }
    if(textlen<30){
        fprintf(fout, "^%02X", textlen);
            fprintf(fout, "%s\n", textrec);
    }
    fprintf(fout, "E^%06X", staddr);
    fclose(fint);
    fclose(fopc);
    fclose(fsym);
    fclose(flen);
    fclose(fout);
}