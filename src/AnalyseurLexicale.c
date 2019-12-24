//
// Created by wf980 on 2019-11-26.
//
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
//justifier un lettre d'entree est un nombre ou pas
int checkNum(int a){
    static char num[11]="0123456789";
    int i;
    char c=(char)a;
    int flag=0;
    for (i=0;i<10;i++){
        if (num[i]==c){
            flag=1;
            break;
        }
    }
    return(flag);
}
//justifier un lettre d'entree est un charactere ou pas
int checkAlphabet(int a){
    char alphabet[53]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
     int i;
    char c=(char)a;
    int flag=0;
    for (i=0;i<52;i++){
        if (alphabet[i]==c){
            flag=1;
            break;
        }
    }
    return(flag);
}

//justifier un lettre d'entree est un signe ou pas
int checkSign(int a){
    static char formatSign[12]={' ','`','[',']',',','(',')','"','=','{','}'};
    int i;
    char c=(char)a;
    int flag=0;

    for (i=0;i<11;i++){
        if (formatSign[i]==c){
            flag=1;
            break;
        }
    }
    return(flag);
}

//justifier trois lettres d'entree est un signe ou pas
int checkSpe(int a[]){
    char special[]={"一二三四五六七八九零十→"};
    long int spe[50]; //les UTF-8 nombres pour ces caractere specials
    long int l=0;
    for (int i=0;i<strlen(special);i++){
        spe[l]=(int)special[i];
        l++;
    }
    int flag=0;
    for (int i=0;i<l-2;i++) if ((a[0]==spe[i])&&(a[1]==spe[i+1])&&(a[2]==spe[i+2])){
        flag=1;
        break;
    }
    return(flag);
}

int AnalyzeLexical(char dir[]){
    FILE *fp;
    fp=fopen(dir,"r");
    if (fp==NULL){
        fclose(fp);
        printf("File introuve!");
        return(1);
    }
    int s[1000]={};
    long int t=0,tt=0;
    char ch=' ',preChar=' ';
    int flagComment=0; //flag to judge whether a comment has been started
    while (ch!=EOF) {
        while (ch!=EOF && flagComment){
            preChar=ch;         //pour aider savoir quel etat on est maintenant
            ch=fgetc(fp);
            if (preChar=='*' && ch=='/'){
                flagComment=0;
                break;
            }
        }
        while (1) { //dans ce boucle, on supprime les notes et les tabs
            preChar=ch;
            ch = fgetc(fp);
            if (ch!='\n' && ch!=EOF && ch!='\t') {
                s[t] = ch;
                t++;
            }
            if (ch==EOF) break;
            if (preChar=='/' && ch=='*'){
                t=t-1;
                flagComment=1;
                break;
            }else
            if (preChar=='/' && ch=='/'){
                t=t-1;
                char tmpCh=ch;
                while (tmpCh!='\n') tmpCh=fgetc(fp);
            }
        }
    }
    fclose(fp);
    //for (int i=0;i<t;i++) printf("%c",s[i]);

    fp=fopen("tmp.txt","w"); //ce ficher est utilise dans l'analyseur syntaxique
    //printf("%d\n",t);
    s[t]='\0';
    char word_1[]="Automate";
    char word_2[]="etats";
    char word_3[]="initial";
    char word_4[]="final";
    char word_5[]="transitions"; //seulement ces 5 mots sont autorises
    for (int i=0;i<t;i++){
        //if (s[i]==' ') fprintf(fp,"%c",s[i]); else
        if (s[i]!='\n' && s[i]!='\t' && s[i]!='/') { //juste les lettres et les blancs sont stocke dans tmp.txt
            if (s[i]>=0){ //c'est a dire ce n'est pas un caractere special
                int flagN=checkNum(s[i]);
                int flagA=checkAlphabet(s[i]);
                int flagS=checkSign(s[i]);
                if(flagN==0&&flagA==0&&flagS==0){
                    printf("%s %c\n","Erreur Lexicale: Caractere inconnu: ",s[i]);
                    fclose(fp);
                    return(1);
                } else if (s[i]!=' ') fprintf(fp,"%c",s[i]);
                if ((i>0 && flagA==1 && s[i-1]!=(int)'"' && s[i-1]!=(int)'`' && checkAlphabet(s[i-1])==0)||(i==0 && flagA==1)) { //pour examiner les mots
                    if (s[i] == 'A') {
                        int j=i+1;
                        while (checkAlphabet(s[j])==1){
                            if (j>i+7 || s[j]!=word_1[j-i]){
                                printf("%s\n","Erreur Lexicale: mot inconnu: ");
                                int k=i;
                                while (checkAlphabet(s[k])==1) {
                                    printf("%c",s[k]);
                                    k++;
                                }
                                printf("\n");
                                fclose(fp);
                                return(1);
                            }
                            j++;
                        }
                    } else
                    if (s[i] == 'e') {
                        int j=i+1;
                        while (checkAlphabet(s[j])==1){
                            if (j>i+4 || s[j]!=word_2[j-i]){
                                printf("%s\n","Erreur Lexicale: mot inconnu: ");
                                int k=i;
                                while (checkAlphabet(s[k])==1) {
                                    printf("%c",s[k]);
                                    k++;
                                }
                                printf("\n");
                                fclose(fp);
                                return(1);
                            }
                            j++;
                        }
                    } else
                    if (s[i] == 'i') {
                        int j=i+1;
                        while (checkAlphabet(s[j])==1){
                            if (j>i+6 || s[j]!=word_3[j-i]){
                                printf("%s\n","Erreur Lexicale: mot inconnu: ");
                                int k=i;
                                while (checkAlphabet(s[k])==1) {
                                    printf("%c",s[k]);
                                    k++;
                                }
                                printf("\n");
                                fclose(fp);
                                return(1);
                            }
                            j++;
                        }
                    } else
                    if (s[i] == 'f') {
                        int j=i+1;
                        while (checkAlphabet(s[j])==1){
                            if (j>i+4 || s[j]!=word_4[j-i]){
                                printf("%s\n","Erreur Lexicale: mot inconnu: ");
                                int k=i;
                                printf("%c ",s[i-1]);
                                while (checkAlphabet(s[k])==1) {
                                    printf("%c",s[k]);
                                    k++;
                                }
                                printf("\n");
                                fclose(fp);
                                return(1);
                            }
                            j++;
                        }
                    } else
                    if (s[i] == 't') {
                        int j=i+1;
                        while (checkAlphabet(s[j])==1){
                            if (j>i+10 || s[j]!=word_5[j-i]){
                                printf("%s\n","Erreur Lexicale: mot inconnu: ");
                                int k=i;
                                while (checkAlphabet(s[k])==1) {
                                    printf("%c",s[k]);
                                    k++;
                                }
                                printf("\n");
                                fclose(fp);
                                return(1);
                            }
                            j++;
                        }
                    } else{
                        printf("%s\n","Erreur Lexicale: mot inconnu: ");
                        int k=i;
                        while (checkAlphabet(s[k])==1) {
                            printf("%c",s[k]);
                            k++;
                        }
                        printf("\n");
                        fclose(fp);
                        return(1);
                    }
                }
            } else //c'est peut-etre un caractere special
            if (s[i]<0) {
                int sp[3]={s[i],s[i+1],s[i+2]}; //UTF-8 caractere avec 3 chiffres
                char ss[3]={s[i],s[i+1],s[i+2]};
                if (checkSpe(sp)==0){
                    printf("%s %s\n","Erreur Lexicale: Charactere inconnu: ",ss);
                    fclose(fp);
                    return(1);
                } else {
                    //printf("%s",ss);
                    fprintf(fp,"\n%d %d %d\n",s[i],s[i+1],s[i+2]); //on ajoute les \n pour faciliter l'entrer dans le processus suivant
                    i=i+2;
                }
            }
        }
    }
    printf("L'analyse lexicale reussi!\n");
    fclose(fp);
    return(0);
}
