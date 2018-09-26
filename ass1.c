/* Solution to comp10002 Assignment 1, 2018 semester 2.

   Authorship Declaration:

   I certify that the program contained in this submission is completely my
   own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been
   developed by another student, or by me in collaboration with other
   students, or by non-students as a result of request, solicitation, or
   payment, may not be submitted for assessment in this subject.  I further
   understand that submitting for assessment work developed by or in
   collaboration with other students or non-students constitutes Academic
   Misconduct, and may be penalized by mark deductions, or by other
   penalties determined via the University of Melbourne Academic Honesty
   Policy, as described at https://academicintegrity.unimelb.edu.au.

   I further certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor
   will I do so until after the marks are released. I understand that
   providing my work to other students, regardless of my intention or any
   undertakings made to me by that other student, is also Academic
   Misconduct.

   Signed by: Adrian Chi Hou Tang, 987978
   Dated:     2018/09/15

*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXFRAG 1000    //max number of fragments in a text file
#define MAXCHAR 20      //max number of characters in each fragment    

#define DEBUG 0
#if DEBUG
#define DUMP_DBL(x) printf("line %d: %s = %.5f\n", __LINE__, #x, x)
#else
#define DUMP_DBL(x)
#endif

int stage0(char [][MAXCHAR]);
void stage1(char [][MAXCHAR], char [], int);
void stage2(char [][MAXCHAR], char [], int);
void stage3(char [][MAXCHAR], char [], int);
int mygetchar();
void print_stage(int);
void print_sstring(int, int, int, char []);
void compile_sstr(char [][MAXCHAR], char [], int, int);
int checkpresent(char [], char []);
int checktailoverlap(char [], char []);
int checkheadoverlap(char [], char []);
int in_proc(int, int [], int);
void copy_str(char [], char [], int);

typedef char str_t[MAXCHAR*MAXFRAG+1];

int main(int argc, char const *argv[])
{
    str_t superstr1, superstr2, superstr3;
    char frags[MAXFRAG][MAXCHAR];
    int fragnum = stage0(frags);
    stage1(frags,superstr1, fragnum);
    stage2(frags,superstr2, fragnum);
    stage3(frags,superstr3, fragnum);

    return 0;
}


//read content from a txt, return the number of fragment and characters
int stage0(char frags[][MAXCHAR]){
    int ch, fragidx = 0, charidx = 0, charcount;

    print_stage(0);     //print info for stage no. 0

    while ((ch = mygetchar()) != EOF){
        if ((ch == '\n')){
            fragidx += 1;
            charidx = 0;
        } else {
            frags[fragidx][charidx] = ch;
            charidx += 1;
            charcount += 1;
        }
    }

    printf("%d fragments read, %d characters in total\n", fragidx, charcount);

    if (DEBUG){
        int u = 0;
        for (u=0; u<fragidx; u++){
            printf("frag[%d] = %s\n", u, frags[u]);
        }
    }

    return fragidx;
}

//taken the fragment as input, create the superstring in stage 1
void stage1(char frags[][MAXCHAR], char sstr[], int fnum){
    int loop, pos;
    int slen;

    print_stage(1);     //print info for stage no. 1

    for (loop=0; loop<fnum; loop++){
        if (loop == 0){
            strcpy(sstr,frags[0]);
            sstr[0] = toupper(sstr[0]);
        }
        else if ((pos = checkpresent(sstr, frags[loop]))){
            sstr[pos] = toupper(sstr[pos]);
        }
        else {
            pos = checktailoverlap(sstr, frags[loop]);
            int flen = strlen(frags[loop]);
            strncat(sstr, frags[loop]+pos, flen-pos);
            int slen = strlen(sstr);
            sstr[slen-flen] = toupper(sstr[slen-flen]);
        }

        slen = strlen(sstr);
        print_sstring(loop, loop, slen, sstr);
    }

    printf("---\n");
    if(slen>54){
        printf("%2d: frg=%2d, slen=%3d  %.*s .. %s\n",
             loop-1, -1, slen, 25, sstr, sstr+slen-25);
    }else{
        printf("%2d: frg=%2d, slen=%3d  %s\n", loop-1, -1, slen, sstr);
    }
}

//taken the fragment as input, create the superstring in stage 2
void stage2(char frags[][MAXCHAR], char sstr[], int fnum){
    int head_overlap_check = 0;     //does it need to do checking on head.
    print_stage(2);     //print info for stage no. 2
    compile_sstr(frags, sstr, fnum, head_overlap_check);
}

//taken the fragment as input, create the superstring in stage 3
void stage3(char frags[][MAXCHAR], char sstr[], int fnum){
    int head_overlap_check = 1;     //does it need to do checking on head.
    print_stage(3);     //print info for stage no. 3
    compile_sstr(frags, sstr, fnum, head_overlap_check);
}

//obtained in Unimelb Comp10002 Assignment 1 2018s2,
//this is an alternative version of getchar(),
//which works won text files on both PC and Mac.
int mygetchar(){
    int c;  
    while ((c=getchar())=='\r'){
        }
    return c;
}

//print the curret stage info
void print_stage(int stage){
    printf("\nStage %d Output\n", stage);
    printf("--------------");
    printf("\n");
}

//print the superstring when each fragment is appended
void print_sstring(int loop, int frg, int slen, char str[]){
    if (loop<=10 || loop%5==0){
        if (slen>54){
            printf("%2d: frg=%2d, slen=%3d  %.*s .. %s\n",
             loop, frg, slen, 25, str, str+slen-25);
        }
        else {
            printf("%2d: frg=%2d, slen=%3d  %s\n", loop, frg, slen, str);
        }
    }
}


//return the postion when whole fragment exist in supserstring, else 0.
int checkpresent(char str[], char f[]){
    int i, u, t, pos;
    int slen = strlen(str);
    int flen = strlen(f);

    for (i=0; i<=(slen-flen+1); i++){
        u = 0;
        t = i;
        pos = i;
        while (f[u] == tolower(str[t])){
            if (u == flen-1){
                return pos;
            }
            u += 1;
            t += 1;
        }
    }

    return 0;

}

//return number of overlap characters in the tail, if no overlapping return 0.
int checktailoverlap(char str[], char f[]){
    int i, s, dulpcount;
    int slen = strlen(str);

    for (i=0; str[i]!='\0'; i++){
        s = i;
        dulpcount = 0;
        while (f[dulpcount] == tolower(str[s])){
            dulpcount += 1;
            if (s==slen-1){
                return dulpcount;
            }
            s += 1;
        }
    }

    return 0;
}

//return number of overlap characters in the head, if no overlapping return 0.
int checkheadoverlap(char str[], char f[]){
    int i, s, dulpcount, fragidx;
    int slen = strlen(str);
    int flen = strlen(f);

    for (i=slen-1; i>=0; i--){
        s = i;
        fragidx = flen - 1;
        dulpcount = 0;
        while (f[fragidx] == tolower(str[s])){
            dulpcount += 1;
            if (s==00){
                return dulpcount;
            }
            fragidx -= 1;
            s -= 1;
        }
    }

    return 0;
}

//produce the superstring required in stage 2 and 3.
//First append the first fragment as the start of the string,
//then if any full length fragment exist in the string, marked it as procceed.
//If not, append the most overlapping fragment in the head or tail and
//mark that fragment as procceed.
//Any overlapping position is indicated by a capital letter.
void compile_sstr(char frags[][MAXCHAR], char sstr[], int fnum, int ckhead){
    int f, pos, loop = 0, tlen = 0, hlen = 0;
    int proc_count = 0, proc_frag[fnum];
    int is_present, frag_id, zero, head_overlap, maxoverlap;
    int flen, slen;
    memset(proc_frag, 0, sizeof proc_frag);

    for (loop=0; loop<fnum; loop++){
        if (loop == 0){
            strcpy(sstr, frags[0]);
            proc_frag[proc_count] = 0;
            proc_count += 1;
            frag_id = 0;
            sstr[0] = toupper(sstr[0]);
        }
        else {
            is_present = 0;
            zero = 0;
            head_overlap = 0;
            maxoverlap = 0;
            for (f=1; f<fnum; f++){
                //check is the current frags[f] marked as procced 
                if (!in_proc(f, proc_frag, fnum)){
                    if ((pos = checkpresent(sstr, frags[f]))){
                        proc_frag[proc_count] = f;
                        proc_count += 1;
                        sstr[pos] = toupper(sstr[pos]);
                        is_present = 1;
                        frag_id = f;
                        break;
                    }
                    else{
                        //is it required to do a checking in the head of sstr?
                        if (ckhead){
                            hlen = checkheadoverlap(sstr, frags[f]);
                            if (DEBUG) {
                                printf("\ncheckheadoverlap: %s...hlen is %d \n",
                                frags[f],hlen);
                            }
                        }
                        tlen = checktailoverlap(sstr, frags[f]);
                        if (DEBUG) {
                            printf("\nchecktailoverlap: %s...tlen is %d \n",
                            frags[f],tlen);
                        }

                        if (tlen == 0 && hlen == 0 && !zero){
                            zero = 1;
                            frag_id = f;
                        }
                        else if (hlen > tlen && hlen > maxoverlap){
                            head_overlap = 1;
                            maxoverlap = hlen;
                            frag_id = f;
                            zero = 1;
                        }
                        else if (tlen >= hlen && tlen > maxoverlap){
                            head_overlap = 0;
                            maxoverlap = tlen;
                            frag_id = f;
                            zero = 1;
                        }
                    }
                }
            }

            if (is_present == 0){

                if (DEBUG){
                    printf("\nfrag id: %d, and maxoverlap is: %d\n",
                    frag_id, maxoverlap);
                }

                proc_frag[proc_count] = frag_id;
                proc_count += 1;
                flen = strlen(frags[frag_id]);

                //more overlapping occur in tail, thus should not concern head.
                if (head_overlap == 0){
                    strncat(sstr, frags[frag_id]+maxoverlap, flen-maxoverlap);
                    slen = strlen(sstr);
                    sstr[slen-flen] = toupper(sstr[slen-flen]);
                }
                //head_overlap == 1, so append frag to the head of superstring.
                else {
                    copy_str(sstr, frags[frag_id], maxoverlap);
                    sstr[0] = toupper(sstr[0]);
                }
            }

        }

        slen = strlen(sstr);
        print_sstring(loop, frag_id, slen, sstr);
    }

    printf("---\n");
    
    if(slen>54){
        printf("%2d: frg=%2d, slen=%3d  %.*s .. %s\n",
             loop-1, -1, slen, 25, sstr, sstr+slen-25);
    }else{
        printf("%2d: frg=%2d, slen=%3d  %s\n", loop-1, -1, slen, sstr);
    }

}


//check the current fragment no. is in the procceed fragment array or not
//return 1 if yes, otherwise 0.
int in_proc(int fnum, int proc_arry[], int size){
    int p, exist = 0;

    for (p=0; p<size; p++){
        if (fnum == proc_arry[p]){
            exist = 1;
            return exist;
        }

    }

    return exist;
}

//copy the non overlap characters of fragment into an temp array,
//apend the superstring to the tail of temp
//then copy the result superstring back to str.
void copy_str(char str[], char f[], int size){
    char tmp[MAXCHAR*MAXFRAG];
    printf("tmp is %s\n",tmp);
    int flen = strlen(f);
    f[flen-size] = '\0';
    strcpy(tmp,f);
    strcat(tmp, str);
    strcpy(str,tmp);
}


/*
algorithms are fun.
*/