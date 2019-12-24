#include "AnalyseurSemantique.c"

int main() {
    char dir[100];
    printf("Quel est le ficher vous vouler compiler?\n");
    scanf("%s",dir);
    int result=AnalyzeLexical(dir);
    if (result==0) result=AnalyzeSyntactic();
    if (result==0) result=AnalyzeSemantic(); //l'execution est fait apres l'analyze semantique reussi
    return 0;
}
