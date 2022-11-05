#include<stdio.h>
#include<string.h>

void main(){
    int start, locctr;
    char label[10], mneu[10], oper[10], name[10];
    char opmneu[10], opaddr[10];
    char symlabel[10], symaddr[10], sym1label[10], sym1addr[10];
    char lines[10][10];
    FILE *finp, *fout, *foptab, *fsymb, *fsymb1, *flist;
    finp = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    foptab = fopen("optab.txt", "r");
    fsymb = fopen("symtab.txt", "w+");
    fsymb1 = fopen("symtab1.txt", "w+");
    flist = fopen("listinglines.txt", "w+");
    fscanf(finp, "%s%s%s", label, mneu, oper);
    if(strcmp(mneu, "START")==0){
        start = strtol(oper, NULL, 16);
        locctr = start;
        strcpy(name, label);
        fscanf(finp, "%s%s%s", label, mneu, oper);
    }
    else
        locctr = start = 0;
    while(strcmp(mneu, "END")!=0){
        if(strcmp(label, "-")!=0){
            int noLabel=1;
            fseek(fsymb, 0,SEEK_SET);
            while(feof(fsymb)!=0){
                fscanf(fsymb, "%s%s", symlabel, symaddr);
                if(strcmp(symlabel, label)==0){
                    noLabel--;
                    break;
                }
            }
            if(noLabel){
                fseek(fsymb, 0, SEEK_END);
                fprintf(fsymb, "%s%x\n", label, locctr);
                fseek(fsymb1, 0, SEEK_SET);
                while(feof(fsymb1)!=0) {
                    fscanf(fsymb1, "%s%s", sym1label, sym1addr);
                    if(strcmp(sym1label, label)==0){
                        
                    }
                }
            }
        }
        // fseek(foptab, SEEK_SET, 0);
        // fscanf(foptab, "%s%s", opmneu, opaddr);
        // while(!feof(foptab)){
        //     // if(strcmp(mneu, opmneu)==0){
        //     //     fprintf(fout, "%s\t%s\t%s\n", mneu, oper);
        //     //     break;
        //     // }
        //     // fscanf(foptab, "%s%s", opmneu, opaddr);
        // }

    }
}