//
// Created by wf980 on 2019-12-4.
//
#include "AnalyseurLexicale.c"

int AnalyzeSyntactic(){
    FILE* fp1;
    fp1=fopen("tmp.txt","r");
    int step=0;
    char ch=' ';
    char preChar=' ';
    int bb[5]={0}; //tous les facteurs construit
    int states[51][5]={0}; //pour les noms des etats
    int numPile;
    int numState=0;
    int stateInitial;
    int stateFinal[50]={0}; //les etats finaux
    int numFinal=0;
    int now=0;
    int flagF=0;
    int a[101]={0}; //ou la transition commence
    int b[101]={0}; //ou la transition arrive
    char charTrans[101]={}; //le lettre utilise pour realiser cette transition
    char charStack1[101]={}; //le lettre de manipulation lie a pile 1
    char charStack2[101]={};
    int stack1[101]={0};  //entrer dans le pile: 1 sortir du pile: 2
    int stack2[101]={0};
    int numTrans=0; //nobre total de transitions
    int inTrans=0; //Est-ce qu'on est en train de definir une transition
    int side=0;
    int manipulation=0; //on est dans quel etape, 0 pour commence l'analyze
    int numBracket=0; //le nombre de crochets qui n'est pas pair
    preChar=ch;
    ch=fgetc(fp1);
    int flagQuot=0; //est-ce que on est apres un seul guillemet
    while (ch!=EOF){
        //printf("%c %c %d\n",preChar,ch,manipulation);
        if (preChar=='}'){
            fclose(fp1);
            printf("Syntax Error: Unexpected %c \n",ch);
            return(1);
        }
        if (step==0){ //c'est le situation qu'on doit donner le nombre de piles
            if (preChar==' ') {
                if (ch != 'A') {
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Automate should be defined at first");
                    return (1);
                } else
                    for (int i = 1; i <= 7; i++) {
                        preChar = ch;
                        ch = fgetc(fp1);
                    }
            }
            if (preChar=='e') {
                if (ch != '(') {
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected '(' for the number of Piles of Automate");
                    return (1);
                }
            }
            if (preChar=='('){
                char num[5] = {};
                int l = 0;
                while (ch != ')') {
                    //printf("%c\n",ch);
                    if (checkNum((int) ch) == 0) {
                        fclose(fp1);
                        printf("%s\n", "Syntax Error: Expected integer for the number of piles of automate");
                        return (1);
                    }
                    if (l == 5) {
                        fclose(fp1);
                        printf("%s\n", "Syntax Error: the number of piles of automate is too long");
                        return (1);
                    }
                    num[l] = ch;
                    l++;
                    preChar=ch;
                    ch = fgetc(fp1);
                }
                numPile = atoi(num);
                printf("The number of piles of the Automate is %d\n",numPile);
            }
            if (preChar==')'){
                if (ch != '=') {
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected '=' after 'etats' ");
                    return (1);
                }
                bb[0]=1;
            }
            if (preChar=='='){
                if (ch != '{') {
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected '{' to begin the description of the automate");
                    return (1);
                }
                else step=1;
            }
        } else if (step==1){ //c'est a dire on veut les noms des etats
            if (preChar=='{'){
                if (ch!='e'){
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected 'etats' of the automate ");
                    return (1);
                }
                for(int i=1;i<=4;i++) {preChar=ch; ch=fgetc(fp1);}
            }
            if (preChar=='s'){
                if (ch != '=') {
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected '=' ");
                    return (1);
                }
            }
            if (preChar=='='){
                if (ch != '['){
                    fclose(fp1);
                    printf("%s\n", "Syntax Error: Expected '[' to define 'etats' ");
                    return (1);
                }
            }
            if (preChar=='['){
                if (ch!=34 && ch!='`'){
                    fclose(fp1);
                    printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                    return(1);
                } else {
                    flagQuot=(int)ch; //alors on peut utiliser deux types de guillements
                    numState++;
                }
            }
            if (preChar==34||preChar=='`'){ //pour avoir les nons des etats
                if (flagQuot!=0){ //on a fini ce pair de guillements
                    preChar='*'; //c'est encore apres un guillement
                    if (ch=='\n'){
                        for (int i=0;i<3;i++) fscanf(fp1,"%d",&states[numState][i]);
                        //printf("%d\n",states[numState][1]);
                        fgetc(fp1);
                        //printf("%c\n",c);
                    } else if (checkAlphabet(ch)==1 || checkNum(ch)==1){ //pour detecter les caracteres impropres
                        int i=0;
                        while (ch!=flagQuot) {
                            states[numState][i] = (int) ch;
                            i++;
                            ch=fgetc(fp1);
                        }
                        flagQuot=0;
                        preChar=ch;
                    } else {
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                } else
                if (flagQuot==0){ //pas apres un guillement
                    if (ch==','){

                    } else if (ch==']'){
                        step=2;
                    } else{
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                }
            } else
            if (preChar=='*'){ //apres un guillement, et ca doit etre fini
                //printf("%d\n",flagQuot);
                //printf("%d\n",ch);
                if (ch!=flagQuot){
                    fclose(fp1);
                    printf("Syntax Error: Unexpected ' %c\n", ch);
                    return (1);
                }
                preChar=ch;
                flagQuot=0;
            }
            if (preChar==','){
                if (ch!=34 && ch!='`'){
                    fclose(fp1);
                    printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                    return(1);
                } else {
                    flagQuot=(int)ch;
                    numState++;
                }
            }
        } else if (step==2){ //on doit definir l'etat initial et les etats finaux
            if (now==0){ //l'ordre de ces trois information peut etre changer, now==0 signifie que on est pas en train de definir quelques choses
                if (ch=='i'){
                    now=1;
                    for (int i=1;i<=6;i++) {preChar=ch; ch=fgetc(fp1);}
                } else if (ch=='f'){
                    now=2;
                    for (int i=1;i<=4;i++) {preChar=ch; ch=fgetc(fp1);}
                } else if (ch=='t'){
                    now=3;
                    for (int i=1;i<=10;i++) {preChar=ch; ch=fgetc(fp1);}
                }
            } else
            if (now==1 ){ // on est en train de definir l'etat initial
                if (preChar=='l'){
                    if (ch!='='){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }

                }
                if (preChar=='='){
                    if (checkNum(ch)==0){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                    stateInitial=(int)ch-48;
                    //ll++;
                }
                if (checkNum(preChar)==1){
                    if (checkNum(ch)==1){
                        stateInitial=stateInitial*10+(int)ch-48;
                    }
                    if (checkAlphabet(ch)==1){
                        now=0;
                        flagF=1;
                    }
                }
            } else
            if (now==2){ // on est en train de definir les etats finaux
                if (preChar=='l'){
                    if (ch!='='){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                }
                if (preChar=='='){
                    if (ch!='['){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                }
                if (preChar=='['){
                    if (checkNum(ch)==0){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                    stateFinal[numFinal]=(int)ch-48;
                }
                if (checkNum(preChar)==1){
                    if (checkNum(ch)==1){
                        stateFinal[numFinal]+=(int)ch-48;
                    } else if (ch==','){
                        numFinal++;
                    } else if (ch==']'){
                        now=0;
                    } else{
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                }
                if (preChar==','){
                    if (checkNum(ch)==1){
                        stateFinal[numFinal]+=(int)ch-48;
                    } else{
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                }
            } else
            if (now==3){ // on est en train de definir les transitions
                if (preChar=='s'){
                    if (ch!='='){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                } else
                if (preChar=='='){
                    if (ch!='['){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                } else
                if (preChar=='[' || (preChar==',' && inTrans==0)){
                    if (ch!='('){
                        fclose(fp1);
                        printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                        return(1);
                    }
                    numBracket++;
                    manipulation=0;
                    numTrans++;
                    inTrans=1;
                } else
                if (preChar=='('){
                    if (manipulation==0){ //manipulation==0 signifie qu'on est en train de decider ou la transition commence
                        if (checkNum(ch)==0){
                            fclose(fp1);
                            printf("%s%c\n","Syntax Error: Unexpected Character: ",ch);
                            return(1);
                        }
                        side=0;
                        a[numTrans]=(int)ch-48;
                    }
                    if (manipulation==2){
                        if (ch=='\n'){ //manipulation==2 signifie qu'on est en train de decider les manipulation des piles2
                            int tmp;
                            for (int i=0;i<3;i++) fscanf(fp1,"%d",&tmp);
                            fgetc(fp1);
                            stack1[numTrans]=1;
                        } else
                        if (ch=='`'){
                            charStack1[numTrans]=fgetc(fp1);
                            if (checkNum(charStack1[numTrans])==0 && checkAlphabet(charStack1[numTrans])==0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                            fgetc(fp1);
                            stack1[numTrans]=2;
                        } else if (ch==')'){
                            numBracket--;
                            if (numBracket<0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                        }
                    }
                    if (manipulation==3){ //manipulation==3 signifie qu'on est en train de decider les manipulation des piles2
                        if (ch=='\n'){
                            int tmp;
                            for (int i=0;i<3;i++) fscanf(fp1,"%d",&tmp);
                            fgetc(fp1);
                            stack2[numTrans]=1;
                        }
                        if (ch=='`'){
                            charStack2[numTrans]=fgetc(fp1);
                            if (checkNum(charStack2[numTrans])==0 && checkAlphabet(charStack2[numTrans])==0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                            fgetc(fp1);
                            stack2[numTrans]=2;
                        }else if (ch==')'){
                            numBracket--;
                            if (numBracket<0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                        }
                    }
                } else
                if (checkNum(preChar)==1){
                    if (ch=='\n'){
                       int tmp;
                       for (int i=0;i<3;i++) fscanf(fp1,"%d",&tmp);

                       fgetc(fp1);
                       side=1;
                    }
                    if (ch==','){
                        manipulation=1;
                    }
                    if (checkNum(ch)==1){
                        if (side==0){
                            a[numTrans]=a[numTrans]*10+(int)ch-48;
                        }
                        if (side==1){
                            b[numTrans]=b[numTrans]*10+(int)ch-48;
                        }
                    }
                } else
                if (preChar=='\n'){
                    if (ch==','){

                    }
                    if (ch==')'){
                        numBracket--;
                        if (numBracket<0){
                            fclose(fp1);
                            printf("Syntax Error: Unexpected ')' ");
                            return(1);
                        }
                    }
                    if (checkNum(ch)==1){
                        b[numTrans]=(int)ch-48;
                    }
                } else
                if (preChar==',' && inTrans==1){
                    if (manipulation==1){
                        if (ch=='`') {
                            charTrans[numTrans] = fgetc(fp1);
                            if (checkNum(charTrans[numTrans])==0 && checkAlphabet(charTrans[numTrans])==0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                            fgetc(fp1);
                        } else if (ch=='('){
                            manipulation++;
                            numBracket++;
                            //printf("num:%d\n",numBracket);
                        }
                    } else if (manipulation==2){
                        if(ch=='`'){
                            charStack1[numTrans]=fgetc(fp1);
                            if (checkNum(charStack1[numTrans])==0 && checkAlphabet(charStack1[numTrans])==0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                            fgetc(fp1);
                        }
                        else if (ch=='('){
                            manipulation++;
                            numBracket++;
                            //printf("num:%d\n",numBracket);
                        } else
                        if (ch=='\n'){
                            int tmp;
                            for (int i=0;i<3;i++) fscanf(fp1,"%d",&tmp);
                            fgetc(fp1);
                        }
                    } else if (manipulation==3){
                        if(ch=='`'){
                            charStack2[numTrans]=fgetc(fp1);
                            if (checkNum(charStack2[numTrans])==0 && checkAlphabet(charStack2[numTrans])==0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                            fgetc(fp1);
                        }
                        if (ch=='\n'){
                            int tmp;
                            for (int i=0;i<3;i++) fscanf(fp1,"%d",&tmp);
                            fgetc(fp1);
                        }
                    }
                } else
                if (preChar=='`'){
                    if (manipulation==1){
                        if (ch==','){
                            //manipulation=2;
                        } else
                        if (ch==')'){
                            numBracket--;
                            if (manipulation==1) {
                                if (numBracket < 0) {
                                    fclose(fp1);
                                    printf("Syntax Error: Unexpected ')' ");
                                    return (1);
                                }
                                if (numBracket != 0) {
                                    fclose(fp1);
                                    printf("Syntax Error: Expected ')' ");
                                    return (1);
                                }
                                inTrans = 0;
                            }
                        }
                    }else if (manipulation==2){
                        if (ch==','){

                        }else if (ch==')'){
                            numBracket--;
                            if (numBracket<0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                        } else {
                            fclose(fp1);
                            printf("Syntax Error: Unexpected %c \n",ch);
                            return(1);
                        }
                    } else if (manipulation==3){
                        if (ch==','){

                        }else if (ch==')'){
                            numBracket--;
                            if (numBracket<0){
                                fclose(fp1);
                                printf("Syntax Error: Unexpected ')' ");
                                return(1);
                            }
                        } else {
                            fclose(fp1);
                            printf("Syntax Error: Unexpected %c \n",ch);
                            return(1);
                        }
                    }
                } else
                if (preChar==')'){
                    if (ch==')'){
                        numBracket--;
                        if (numBracket<0){
                            fclose(fp1);
                            printf("Syntax Error: Unexpected ')' ");
                            return(1);
                        }
                        if (numBracket>0){
                            fclose(fp1);
                            printf("Syntax Error: Expected ')' ");
                            return(1);
                        }
                        inTrans=0;
                    } else if (ch==','){

                    } else if (ch==']'){
                        now=0;
                    }else{
                        fclose(fp1);
                        printf("Syntax Error: Unexpected %c \n",ch);
                        return(1);
                    }
                }

            }
        }
        if (flagF==0 && preChar!='*')preChar=ch; //pour evider le boucle infini
        if (flagF==0) ch=fgetc(fp1);
        flagF=0;
        while (ch==' ') ch=fgetc(fp1);
    }
    fclose(fp1);
    printf("L'analyse Syntaxique reussi!\n");
    printf("Voir info.txt pour les informations de l'automate\n");
    FILE* fp;
    fp=fopen("info.txt","w"); //pour presenter l'arbre syntaxique
    fprintf(fp,"number of piles:\n");
    fprintf(fp,"%d\n",numPile);
    fprintf(fp,"etats:");
    for (int i=1;i<=numState;i++) {
        fprintf(fp,"\n");
        int j=0;
        while (states[i][j]!=0) {fprintf(fp,"%d ",states[i][j]); j++;}
    }
    fprintf(fp,"\ninitial:\n");
    fprintf(fp,"%d\n", stateInitial);
    fprintf(fp,"final:\n");
    for (int i=0;i<=numFinal;i++) fprintf(fp,"%d ",stateFinal[i]);
    fprintf(fp,"\ntransitions:\n");
    fprintf(fp,"Nombre de transitions: %d\n",numTrans);
    for (int i=1;i<=numTrans;i++){
        fprintf(fp,"de:%d  a:%d  utilisant:%c  char de pile1:%c  entrer ou sortir:%d  char de pile2:%c  entrer ou sortir:%d\n",a[i],b[i],charTrans[i],charStack1[i],stack1[i],charStack2[i],stack2[i]);
    }
    fprintf(fp,"1 signifie entrer, 2 signifie sortir\n");
    fclose(fp);
    FILE* fp2;
    fp2=fopen("tmp1.txt","w"); //pour faciliter l'analyze semantique
    fprintf(fp2,"number of piles:\n");
    fprintf(fp2,"%d\n",numPile);
    fprintf(fp2,"etats:\n");
    fprintf(fp2,"%d",numState);
    for (int i=1;i<=numState;i++) {
        fprintf(fp2,"\n");
        int j=0;
        while (states[i][j]!=0) {
            if (j==0) fprintf(fp2,"%d",states[i][j]);
            else fprintf(fp2," %d",states[i][j]);
            j++;
        }
    }
    fprintf(fp2,"\ninitial:\n");
    fprintf(fp2,"%d\n", stateInitial);
    fprintf(fp2,"final:\n");
    fprintf(fp2,"%d\n",numFinal+1);
    for (int i=0;i<=numFinal;i++) fprintf(fp2,"%d ",stateFinal[i]);
    fprintf(fp2,"\ntransitions:\n");
    fprintf(fp2,"%d\n",numTrans);
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%d ",a[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%d ",b[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%c",charTrans[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%c",charStack1[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%d ",stack1[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%c",charStack2[i]); fprintf(fp2,"\n");
    for (int i=1;i<=numTrans;i++) fprintf(fp2,"%d ",stack2[i]); fprintf(fp2,"\n");
    fclose(fp2);
    return 0;
}