#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main() {
    FILE *finp, *fout;
    finp = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    char inpline[100], temp[20][20], name[20];
    int staddr, len, curloc;

    int reladdress;

    printf("Enter the address to relocate to: ");
    scanf("%X", &reladdress);



    fscanf(finp, "%s", inpline);
    int level=0, j;
    for(int i=0; inpline[i]!='\0'; i++){
        if(inpline[i]=='^'){
            level++;
            j=0;
            continue;
        }
        temp[level][j] = inpline[i];
        temp[level][j+1] = '\0';
        j++; 
    }
    strcpy(name, temp[1]);
    sscanf(temp[2], "%x", &staddr);
    sscanf(temp[3], "%x", &len);

    fprintf(fout,"Program Name: %s\n", name);
    fprintf(fout,"Program Length: %X\n\n", len);
    fprintf(fout,"Starting Address: %X\n\n", staddr);
    fprintf(fout,"Relocated Starting Address: %X\n\n", reladdress);

    fscanf(finp, "%s", inpline);
    while(inpline[0]!='E'){
        level=0;
        for(int i=0; inpline[i]!='\0'; i++){
            if(inpline[i]=='^'){
                if(level<3){
                    level++;
                    j=0;
                }
                continue;
            }
            temp[level][j] = inpline[i];
            temp[level][j+1] = '\0';
            j++;
        }
        sscanf(temp[1], "%X", &curloc);
        for(int i=0; i<strlen(temp[3]); i+=2){
            fprintf(fout, "%06X ---> %c%c\n", reladdress-staddr+curloc+i/2, temp[3][i], temp[3][i+1]);
        }
        fscanf(finp, "%s", inpline);
    }
    fclose(finp);
    fclose(fout);

}