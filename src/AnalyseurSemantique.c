//
// Created by wf980 on 2019-12-12.
//
#include "AnalyseurSyntaxique.c"
#include "executeur.c"

int AnalyzeSemantic(){
    FILE* fp1;
    fp1=fopen("tmp1.txt","r");
    char tmp[100]={};
    int stateInitial=-1;
    int states[51][5]={0};  //les noms des etats
    int numPile;
    int numDemandedPile=0; //combien de piles est utilise pour decrire les transitions
    int numState=0;
    int stateFinal[50]={0};
    int numFinal=0;
    int a[101]={0}; //ou une transition commence
    int b[101]={0}; //ou une transition arrive
    char charTrans[101]={};
    char charStack1[101]={};
    char charStack2[101]={};
    int stack1[101]={0};  //enter un pile:1  sortir d'un pile: 2
    int stack2[101]={0};
    int numTrans=0;
    fgets(tmp,200,fp1);
    fscanf(fp1,"%d",&numPile);
    fgetc(fp1);
    fgets(tmp,200,fp1);
    fscanf(fp1,"%d",&numState);
    for (int i=1; i<=numState;i++){ //lire les noms
        fscanf(fp1,"%d",&states[i-1][0]);
        int j=1;
        int ch=fgetc(fp1);
        while (ch!='\n'){
            fscanf(fp1,"%d",&states[i-1][j]);
            j++;
            ch=fgetc(fp1);
        }
    }
    fgets(tmp,200,fp1);
    fscanf(fp1,"%d",&stateInitial);
    if (stateInitial<0 || stateInitial>=numState){
        fclose(fp1);
        printf("Semantic error: wrong number of state");
        return(1);
    }
    fgetc(fp1);
    fgets(tmp,200,fp1);
    fscanf(fp1,"%d",&numFinal);
    for (int i=0;i<numFinal;i++){
        fscanf(fp1,"%d",&stateFinal[i]);
        if (stateFinal[i]<0 || stateFinal[i]>=numState){
            fclose(fp1);
            printf("2Semantic error: wrong number of state");
            return(1);
        }
    }
    fgetc(fp1);
    fgetc(fp1); //pour lire les blancs et les \n
    fgets(tmp,200,fp1);
    fscanf(fp1,"%d",&numTrans);
    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%d",&a[i]);
        if (a[i]<0||a[i]>=numState){
            fclose(fp1);
            printf("Semantic error: wrong number of state");
            return(1);
        }
    }
    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%d",&b[i]);
        if (b[i]<0||b[i]>=numState){
            fclose(fp1);
            printf("Semantic error: wrong number of state");
            return(1);
        }
    }
    fgetc(fp1); fgetc(fp1);
    for (int i=0;i<numTrans;i++) fscanf(fp1,"%c",&charTrans[i]);
    fgetc(fp1);
    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%c",&charStack1[i]);
    }
    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%d",&stack1[i]);
        if (stack1[i]!=0) numDemandedPile=1;
    }
    fgetc(fp1); fgetc(fp1);

    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%c",&charStack2[i]); //lire les lettres pour realiser une transition
    }
    for (int i=0;i<numTrans;i++) {
        fscanf(fp1,"%d",&stack2[i]);
        if (stack2[i]!=0) numDemandedPile=2;
    }
    if (numDemandedPile!=numPile){
        printf("%d %d\n",numDemandedPile,numPile);
        fclose(fp1);
        printf("Semantic error: wrong number of piles");
        return(1);
    }
    fclose(fp1);
    if(stateInitial==-1 || numFinal==0 ||numTrans==0) {
        printf("Semantic error: expected initial state or final state or transitions\n");
        return(1);
    }
    printf("L'analyse semantique reussi!\n");
    int vm[500];        //on commence le compile ici pour diminuer les fichers temporels
    int start[numState]; //ou la description d'un etat commence
    int pos; //la position de vm ou on est
    vm[0]=numPile;
    vm[1]=stateInitial;
    vm[2]=numFinal;
    for (int i=3;i<3+numFinal;i++) vm[i]=stateFinal[i-3];
    pos=3+numFinal;
    for (int i=0; i<numState; i++){     //construire la machine virtuelle
        start[i]=pos;
        vm[pos]=0;
        int tmpPos=pos+1;
        //printf("%d %d\n",i,pos);
        for (int j=0; j<numTrans;j++){
            //printf("%d\n",a[j]);
            if (a[j]==i){
                vm[pos]++; //nombre de transitions pour cet etat
                vm[tmpPos]=(int)charTrans[j];
                vm[tmpPos+1]=b[j]-100; //pour etre different de -1, si c'est petit que -50, on sait que ca doit etre changer selon start[i]
                tmpPos=tmpPos+2;
                if (numPile>=1){
                    if (stack1[j]!=0){
                        vm[tmpPos]=(int)charStack1[j];
                        if (stack1[j]==1) vm[tmpPos+1]=1; else vm[tmpPos+1]=-1;
                    } else {
                        vm[tmpPos]=0;
                        vm[tmpPos+1]=0;
                    }
                    tmpPos=tmpPos+2;
                }
                if (numPile==2){
                    if (stack2[j]!=0){
                        vm[tmpPos]=(int)charStack2[j];
                        if (stack2[j]==1) vm[tmpPos+1]=1; else vm[tmpPos+1]=-1;
                    } else {
                        vm[tmpPos]=0;
                        vm[tmpPos+1]=0;
                    }
                    tmpPos=tmpPos+2;
                }
            }
        }
        pos=tmpPos; //pour decrire l'etat suivant
    }
    for (int i=0; i<pos; i++) if (vm[i]<=-50){
        vm[i]=start[vm[i]+100]; //donner les positions des etats
    }
    FILE* fp;
    fp=fopen("vm.txt","w");
    for (int i=0; i<pos-1; i++) fprintf(fp,"%d ",vm[i]);
    fprintf(fp,"%d",vm[pos-1]);
    fclose(fp);
    int result=execute(states,pos); //on execute le vm ici
    return(result);
}
