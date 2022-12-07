#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>


#define MAXGiocatori 5
#define MAXCaselle 10

typedef struct {
    int segno;
    int posizione;
}Giocatore;

typedef enum{
    Scala,Scivolo,Buco,Vuota
}TipoCas;

typedef struct{
    char displayValue;
    TipoCas tipoCasella;
    int posizioneInTab;
    Giocatore gInCasella[MAXGiocatori];
    int nGiocatoriInCas;
}Casella;

typedef struct {
    Giocatore g;
    struct turno* next;
}turno;


//!Variabili
int nGiocatori;
Giocatore giocatori[MAXGiocatori];
Casella Tabellone[MAXCaselle];
turno* testa=NULL;
turno* coda=NULL;
turno* currentTurno;
int continuaGioco=1;




//!Funzioni

void InstanziaGiocatoriETurni(){
    for (int i = 0; i < nGiocatori; ++i) {
        Giocatore new;
        new.posizione=0;
        new.segno=i;
        giocatori[i]=new;
        if(testa==NULL){

            testa=(turno*)malloc(sizeof(turno));
            testa->g=new;
            testa->next=NULL;
        }else{



            if(testa->next==NULL){
                coda=(turno*)malloc(sizeof(turno));
                coda->g=new;
                coda->next=NULL;
                testa->next=coda;
            }else{
                turno* nextTurno=(turno*) malloc(sizeof(turno));

                nextTurno->g=new;
                nextTurno->next=NULL;
                coda->next=nextTurno;
                coda=nextTurno;
            }
        }
    }

    currentTurno=(turno*) malloc(sizeof(turno));
    currentTurno=testa;
}

void RequeueTurni(){
    currentTurno=testa;
}

void InstanziaTabellone(){
    srand(time(NULL));
    for (int i = 0; i < MAXCaselle; ++i) {
        Casella new;
        new.posizioneInTab=i;

        int r=rand()%5+1;
        switch (r) {
            case 1:{
                new.displayValue='S';//scala
                new.tipoCasella=Scala;
                break;
            }
            case 2:{
                new.displayValue='C';//scivolo
                new.tipoCasella=Scivolo;
                break;
            }
            case 3:{
                new.displayValue='B';//BUco
                new.tipoCasella=Buco;

                break;
            }
            case 4:{
                new.displayValue=' ';//vuota
                new.tipoCasella=Vuota;

                break;
            }
            case 5:{
                new.displayValue=' ';//vuota
                new.tipoCasella=Vuota;

                break;
            }

        }

        new.nGiocatoriInCas=0;


        if(i==0){
            int j;
            for (j = 0; j < nGiocatori; ++j) {
                new.gInCasella[j]=giocatori[j];
            }
            new.nGiocatoriInCas=j;
        }
        Tabellone[i]=new;
    }
}

void DefaultSettings(){
    InstanziaGiocatoriETurni();

    InstanziaTabellone();
}

void StampaTab(){

    for (int i = 0; i < MAXCaselle; ++i) {
        if(Tabellone[i].nGiocatoriInCas==0){
            printf("| %c ",Tabellone[i].displayValue);
        }else{
            printf("|");
        }
            for (int j = 0; j < Tabellone[i].nGiocatoriInCas; ++j) {
                printf(" %d ",Tabellone[i].gInCasella[j].segno);
            }

        printf("|\n");

    }

/*for (int i = 0; i < MAXCaselle; ++i) {
        printf(" | ");
        if(Tabellone[i].nGiocatoriInCas==0){
            printf("%c",Tabellone[i].displayValue);
        }
            for (int j = 0; j < Tabellone[i].nGiocatoriInCas; ++j) {
                printf("%d ",Tabellone[i].gInCasella[j].segno);
            }

        printf(" | ");
    }
 *
 *
    for (int i = 0; i < MAXCaselle; ++i) {
        printf("%d",Tabellone[i].nGiocatoriInCas);
    }
*/
 }

void RemovePlayerFromCasella(int ind,Giocatore g){

    for (int i = 0; i < Tabellone[ind].nGiocatoriInCas; ++i) {
        if(g.segno==Tabellone[ind].gInCasella[i].segno){
           for (int j = 1; j < Tabellone[ind].nGiocatoriInCas; ++j) {
                Tabellone[ind].gInCasella[j-1]=Tabellone[ind].gInCasella[j];
            }

             Tabellone[ind].nGiocatoriInCas-=1;
             i=Tabellone[ind].nGiocatoriInCas;
            }
    }

}

void AddPlayerToCasella(int ind,Giocatore g){
    Tabellone[ind].gInCasella[Tabellone[ind].nGiocatoriInCas]=g;
    Tabellone[ind].nGiocatoriInCas++;
}



void Gioca(){
    printf("\n\nturno del giocatore: %d",currentTurno->g.segno);
    srand(time(NULL));
    int r=rand()%6+1;
    printf("\n\nrisultato lancio dei dati: %d\n\n",r);

    RemovePlayerFromCasella(giocatori[currentTurno->g.segno].posizione,giocatori[currentTurno->g.segno]);
    giocatori[currentTurno->g.segno].posizione+=r;
    AddPlayerToCasella(giocatori[currentTurno->g.segno].posizione,giocatori[currentTurno->g.segno]);

    if(giocatori[currentTurno->g.segno].posizione>=MAXCaselle){
        printf("\nil giocatore %d ha vinto!!\n\n\n",giocatori[currentTurno->g.segno].segno);
        AddPlayerToCasella(MAXCaselle-1,giocatori[currentTurno->g.segno]);
        continuaGioco=0;
    }else{
        if(currentTurno->next!=NULL)
            currentTurno=currentTurno->next;
        else{
            RequeueTurni();
        }
    }


}

int main() {

    do{
        printf("nGiocatori:\n");
        scanf("%d",&nGiocatori);
    }while(nGiocatori<=0||nGiocatori>MAXGiocatori);



    DefaultSettings();

    StampaTab();
    while(continuaGioco){
        getch();
        system("cls");
        Gioca();
        StampaTab();

    }

    fflush(stdin);
    getch();
    return 0;
}
