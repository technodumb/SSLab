#include<stdio.h>
#include<string.h>

void main() {
    FILE *finp, *fout;
    finp = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    char line[100], name[20], addr[20], len[20];
    char lineadd[20];
    int linecount=0;
    int i, j;
    fscanf(finp, "%s", line);
    i=0; j=0;
    int level=0;
    while(line[i]!='\0'){
        if(line[i]=='^'){
            level++;
            j=0;
            i++;
            continue;
        }
        if(level==1) {
            name[j]=line[i];
            name[++j]='\0';
        }
        else if(level==2){
            addr[j]=line[i];
            addr[++j]='\0';
        }
        else if(level==3){
            len[j]=line[i];
            len[++j]='\0';
        }
        i++;
    }

    fprintf(fout, "Program Name : %s\n", name);
    fprintf(fout, "\nStarting Address : %s\n", addr);
    fprintf(fout, "\nLength : %s\n", len);
    sscanf(addr, "%X", &linecount);
    fscanf(finp, "%s", line);
    while(line[0]!='E'){
        i=0; j=0;
        level=0;
        while(line[i]!='\0'){
            if(line[i]=='^'){
                // printf("ssdf\n");
                level++;
                j=0;
                i++;
                continue;
            }
            else if(level>2){
                fprintf(fout, "%X\t%c%c\n", linecount, line[i], line[i+1]);
                linecount++;
                i+=2;
            }
            else i++;
        }
        fscanf(finp, "%s", line);
    }
    fclose(finp);
    fclose(fout);
}