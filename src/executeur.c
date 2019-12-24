//
// Created by wf980 on 2019-12-18.
//
//#include "AnalyseurSemantique.c"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
int execute(int states[51][5],int pos){
    int modeDebug=0; //1 pour le mode debug
    char charStates[51][5]={};
    printf("hahaha\n");
    for (int i=0; i<51; i++)
	for (int j=0; j<5; j++) if (states[i][j]!=0) charStates[i][j]=states[i][j];
    printf("Est-ce que vous voulez entrer le mode debug?\n1 pour oui, 0 pour non\n");
    scanf("%d",&modeDebug);
    getchar();
    FILE* fp;
    fp=fopen("vm.txt","r");
    int vm[500];
    //int l=0;
    for (int i=0; i<pos; i++) {
        fscanf(fp,"%d",&vm[i]);
        //printf("%d ",vm[i]);
    }
    fclose(fp);
    int stateInitial=vm[1];
    int numPile=vm[0];
    int numFinal=vm[2];
    int start[50]={0};
    int numTrans[50]={0};
    int stateFinal[50];
    int stack1[100]={0}; //le pile commence par 1
    int top1=0; //le top du pile1
    int stack2[100]={0};
    int top2=0; //le top du pile2


    //printf("%d",modeDebug);
    //getchar();
    for (int i=3;i<3+numFinal;i++) stateFinal[i-3]=vm[i];
    int j=3+numFinal;
    int now=0;
    int gap=0;
    if (numPile==0) gap=2; //pour faciliter les calculs suivants
    if (numPile==1) gap=4;
    if (numPile==2) gap=6;
    while (j<pos){ //pour savoir ou les etats commencent
        start[now]=j;
        numTrans[now]=vm[j];
        j=j+gap*vm[j]+1;
        now++;
    }
    int stateNow=stateInitial; //l'etat ou on est dans maintenant
    int startPos=start[stateNow]; //la position de cet etat dans la vm
    printf("Donner le mot d'entree:\n");
    char ch;
    ch=getchar();
    if (modeDebug==1){
        printf("  -> Etat : %s    ",charStates[stateNow]);
        if (numPile>=1) {
            printf("Pile 1 : Vide    ");
        }
        if (numPile==2) {
            printf("Pile 2 : Vide    ");
        }
        printf("\n");
    }
    while (ch!='\n'){
        if (modeDebug==1) printf("%c -> ",ch);
        //printf("%d %d %d\n",ch,stateNow,startPos);
        int exist=0; //est-ce que il y a une transition propre. 0 pour pas de ce type de transition
        for (int i=0; i<numTrans[stateNow]; i++){
            if (vm[startPos+gap*i+1]==(int)ch){
                int tmpPos=startPos+gap*i+1; //la position de transition qu'on veut
                exist=1;
                startPos=vm[tmpPos+1];
                for (int j=0; j<now; j++){
                    if (start[j]==startPos) stateNow=j; //trouver l'etat qu'on arrive
                }
                if (numPile>=1){
                    top1=top1+vm[tmpPos+3];
                    if (top1<0){
                        printf("Erreur: Stack Overflow\nLe mot est refuse!\n");
                        return(1);
                    }
                    if (vm[tmpPos+3]==1) stack1[top1]=vm[tmpPos+2];
                }
                if (numPile==2){
                    top2=top2+vm[tmpPos+5];
                    if (top2<0){
                        printf("Erreur: Stack Overflow\nLe mot est refuse!\n");
                        return(1);
                    }
                    if (vm[tmpPos+5]==1) stack2[top2]=vm[tmpPos+4];
                }
                break;
            }
        }
        if (exist==0){
            printf("Error: faux caracter \nLe mot est refuse!\n");
            return(1);
        }
        if (modeDebug==1){
            printf("Etat : %s    ",charStates[stateNow]);
            if (numPile>=1) {
                printf("Pile 1 : ");
                if (top1==0) printf("Vide    ");
                else{
                    for (int ii=1;ii<=top1;ii++) printf("%c",(char)stack1[ii]);
                    printf("    ");
                }
            }
            if (numPile==2) {
                printf("Pile 2 : ");
                if (top2==0) printf("Vide    ");
                else{
                    for (int ii=1;ii<=top2;ii++) printf("%c",(char)stack2[ii]);
                    printf("    ");
                }
            }
            printf("\n");
        }
        ch=getchar();
    }
    int b=0; //pour justifier qu'on arrive a l'etat final
    for (int i=0; i<numFinal; i++){
        if (stateNow==stateFinal[i]){
            b=1;
            break;
        }
    }
    if (b==0){
        printf("Erreur: N'arrive pas aux etats finaux!\nLe mot est refuse!\n");
        return(1);
    }
    if (top1!=0 || top2!=0){ //pour justifier les piles sont vides
        printf("Erreur: Le pile n'est pas vide a la fin!\nLe mot est refuse!\n");
        return(1);
    }
    printf("Le mot est accepte!\n");
    return(0);
}

