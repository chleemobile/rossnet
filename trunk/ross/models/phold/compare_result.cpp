#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

    FILE * bs_p_File;
    FILE * p_File;
    int c, d;
    int n = 0;
    
    bs_p_File=fopen("bs_frfc_phold_result.txt","r");
    p_File=fopen("phold_result.txt","r");
    
    if (bs_p_File==NULL || p_File == NULL) perror ("Error opening file");
    else
    {
        do {
            c = fgetc (bs_p_File);
            d = fgetc (p_File);
            if (c != d) n++;
        } while (c != EOF);
        
        fclose (bs_p_File);
        fclose (p_File);
        printf ("miss match %d\n",n);
    }
    return 0;
    
}