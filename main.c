#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>


#define MAXGiocatori 5
#define MAXCaselle 20

typedef struct {
    int segno;
    int posizione;
    int Fermo;
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

typedef struct{
    char domanda[50];
    char risposta[50];
    struct Domanda* next;
}Domanda;

//!Variabili
int nGiocatori;
Giocatore giocatori[MAXGiocatori];
Casella Tabellone[MAXCaselle];
turno* testa=NULL;
turno* coda=NULL;
turno* currentTurno;
Domanda* testaDomande=NULL;
Domanda* currentDomande=NULL;
Domanda* codaDomande=NULL;
int continuaGioco=1;//! il gioco continua fino a che questa var è = a 1





//!Funzioni

// metodo incaricato di instanziare le variabili "giocatori", "testa","coda" e "currentTurno"
void InstanziaGiocatoriETurni(){
    for (int i = 0; i < nGiocatori; ++i) {
        Giocatore new;
        new.posizione=0;
        new.segno=i;
        new.Fermo=0;
        giocatori[i]=new;
        //creo la coda per la gestione dei turni
        if(testa==NULL){
            //creo il primo elemento della coda
            testa=(turno*)malloc(sizeof(turno));
            testa->g=new;
            testa->next=NULL;
        }else{
            if(testa->next==NULL){
                //secondo elemento della coda
                coda=(turno*)malloc(sizeof(turno));
                coda->g=new;
                coda->next=NULL;
                testa->next=coda;
            }else{
                // dal terzo all'n elemento della coda
                turno* nextTurno=(turno*) malloc(sizeof(turno));

                nextTurno->g=new;
                nextTurno->next=NULL;
                coda->next=nextTurno;
                coda=nextTurno;
            }
        }
    }

    currentTurno=(turno*) malloc(sizeof(turno));
    currentTurno=testa;//faccio partire la coda dal primo elemento
}

//metodo che fa riiniziare la coda dei turni dei giocatori
void RequeueTurni(){
    currentTurno=testa;
}

//metodo incaricato di instaziare il tabellone quindi tutti le caselle
void InstanziaTabellone(){
    srand(time(NULL));
    for (int i = 0; i < MAXCaselle; ++i) {
        Casella new;
        new.posizioneInTab=i;//posizioneInTab indica la posizione della casella nel tabellone

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
        //ogni casella al momento della creazione non ha nessun giocatore al suo interno tranne la prima
        new.nGiocatoriInCas=0;

        //metto tutti i giocatori presenti nell'array giocatori all'interno della prima casella
        if(i==0){
            int j;
            for (j = 0; j < nGiocatori; ++j) {
                new.gInCasella[j]=giocatori[j];
            }
            new.nGiocatoriInCas=j;
        }
        //aggiungo la casella appena creata al tabellone
        Tabellone[i]=new;
    }
}

//metodo incaricato di gestire il riempimento delle domande e delle risposte con i valori presi da file
void setArray(char s[50],int mode){
    int i;
    for ( i = 0; s[i]!='\n'; ++i) {
        if(mode==0)//set risposta
            testaDomande->risposta[i]=s[i];
        else if(mode==1)
            testaDomande->domanda[i]=s[i];
        else if(mode ==2)
            codaDomande->risposta[i]=s[i];
        else
            codaDomande->domanda[i]=s[i];
    }
    //metto il carattere di fine riga solo alle risposte perchè ho bisogno che sia presente per un controllo che farò in futuro
    if(mode==0)
        testaDomande->risposta[i]='\0';
    else if(mode==2)
        codaDomande->risposta[i]='\0';
}

//metodo incaricato di instanziare le domande , le risposte e la coda delle stesse domande prendendo i valori dai file
void InstanziaDomande(){
    //apro i file
    FILE *D = fopen("fileDomande.txt", "r");
    FILE *R = fopen("fileRisposte.txt", "r");

    char dom[50];
    char risp[50];
    if(D==NULL||R==NULL){
        return;
    }

    //fino a che il file delle domande non è finito leggo
    while (fgets(dom, sizeof(dom), D) != NULL) {
        fgets(risp, sizeof(risp),R);//leggo la risposta

        //creo la coda
        //prima posizione della coda
        if(testaDomande==NULL){
            testaDomande=(Domanda*) malloc(sizeof(Domanda));
            if(testaDomande==NULL)
                return;
            Domanda new;
            setArray(risp,0);
            setArray(dom,1);
            testaDomande->next=NULL;
        }else if(testaDomande->next==NULL){

            //seconda posizione
            codaDomande=(Domanda*) malloc(sizeof(Domanda));
            if(codaDomande==NULL)
                return;
            setArray(risp,2);
            setArray(dom,3);
            testaDomande->next=codaDomande;
            codaDomande->next=NULL;
        }else{
            //dalla 3 alla N posizione
            Domanda* new=(Domanda*) malloc(sizeof(Domanda));
            if(new==NULL)
                return;
            for (int i = 0; risp[i]!='\n'; ++i) {
                new->risposta[i]=risp[i];
            }
            for (int i = 0; dom[i]!='\n'; ++i) {
                new->domanda[i]=dom[i];
            }
            new->next=NULL;
            codaDomande->next=new;
            codaDomande=new;
        }

    }
    //faccio partire la coda dalla prima domanda inserita
    currentDomande=testaDomande;
    //chiudo i file
    fclose(D);
    fclose(R);
}

//hub che viene chiamato solo una volta all'inizio del programma per fare il set up dell'ambiente
void DefaultSettings(){
    InstanziaDomande();

    InstanziaGiocatoriETurni();

    InstanziaTabellone();
}

//metodo che stampa la grafica del gioco
void StampaTab(){

    for (int i = 0; i < MAXCaselle; ++i) {

            printf("| %c | ",Tabellone[i].displayValue);//stampa il carattere che indentifica il ruolo della casella

            for (int j = 0; j < Tabellone[i].nGiocatoriInCas; ++j) {//stampa i giocatori
                printf(" %d ",Tabellone[i].gInCasella[j].segno);
            }

        printf("\n");

    }

 }

 //metodo incaricato di togliere il giocatore che si è appena spostato in una nuova casella dalla casella vecchia
void RemovePlayerFromCasella(int ind,Giocatore g){

    for (int i = 0; i < Tabellone[ind].nGiocatoriInCas; ++i) {  //per ogni giocatore presente nella casella in questione
        if(g.segno==Tabellone[ind].gInCasella[i].segno){        //cerco il mio giocatore all'interno dell'array della casella
           for (int j = 1; j < Tabellone[ind].nGiocatoriInCas; ++j) {//appena lo trovo faccio lo schift del resto dei giocatori, eliminando il giocatore cercato
                Tabellone[ind].gInCasella[j-1]=Tabellone[ind].gInCasella[j];
            }

             Tabellone[ind].nGiocatoriInCas-=1;//-1 del numero dei giocatori della casella
             i=Tabellone[ind].nGiocatoriInCas;//esco dal for
            }
    }

}

//metodo per aggiungere il giocatore che si è appena spostato nella nuova casella
void AddPlayerToCasella(int ind,Giocatore g){
    //aggiungo il player nell'array interno della casella e aumento di 1 il numero di giocatori in essa contenuti
    Tabellone[ind].gInCasella[Tabellone[ind].nGiocatoriInCas]=g;
    Tabellone[ind].nGiocatoriInCas++;
}

//metodo che definisce il comportamento delle caselle di tipo SCIVOLO
void StepScivolo(){
    printf("\nil Giocatore %d arriva ad uno scivolo!\n\n",giocatori[currentTurno->g.segno].segno);
    srand(time(NULL));
    //calcolo un valore da 1 a posizione dove si trova il giocatore
    int getOverHere=rand()%(MAXCaselle-(MAXCaselle-giocatori[currentTurno->g.segno].posizione))+1;

    //aggiorno i miei dati
    giocatori[currentTurno->g.segno].posizione-=getOverHere;
    printf("Torna indietro di %d caselle!\n\n",getOverHere);
    currentTurno->g.posizione=giocatori[currentTurno->g.segno].posizione;
}

//calcola lunghezza di una stringa passata
int Lenght(char s[50]){
    int toReturn;
    for (toReturn = 0;s[toReturn]!='\0'; ++toReturn);
    return toReturn;
}

//metodo che definisce il comportamento delle caselle di tipo BUCO
void StepBuco(){
    //set della variabile che mi dice se il giocatore deve stare fermo oppure no
    giocatori[currentTurno->g.segno].Fermo=1;
    currentTurno->g.Fermo=1;
    printf("\nil Giocatore %d cade in un buco!\n\n",giocatori[currentTurno->g.segno].segno);
}

//metodo che definisce il comportamento delle caselle di tipo SCIVOLO
void StepScala(){
    srand(time(NULL));
    int r=rand()%10+1;
    //faccio rendere conto all'utente che è arrivato su una scala
    printf("\nil giocatore %d ha raggiunto una scala!\n rispondi alla domanda per avere un bonus...",currentTurno->g.segno);
    //stampo la domanda corrente
    printf("\n%s",currentDomande->domanda);

    //prendo la risposta dell'utente in input
    fflush(stdin);
    char parola[50];
    scanf("%s",parola);


    //confronto la lunghezza della risposta data e la risposta del file
    //se sono diverse non ha senso controllare ulteriormente quindi la risposta è sbagliata
    if(Lenght(currentDomande->risposta)== Lenght(parola)){
        //se le lunghezze combaciano controllo carattere per carattere

        int check=1;//check=1 (le stringhe sono uguali) // check=0 (le stringhe sono diverse)

        for (int i = 0;parola[i]!='\0'; ++i) {
            if(currentDomande->risposta[i]!=parola[i])
                check=0;
        }




        if(check==0){
            printf("\n risposta sbagliata\n\n");
            return;
        }
    }else{
        printf("\n risposta sbagliata\n\n");
        return;
    }

    //se il compilatore arriva qui la risposta è corretta

    printf("\n risposta Giusta!\n il giocatore si sposta di : %d\n\n",r);

    //do al giocatore il bonus di avanzamento
    currentTurno->g.posizione+=r;
    giocatori[currentTurno->g.segno].posizione+=r;

    //SOLO se si indovina la coda delle domande va avanti

    if(currentDomande->next!=NULL)
        currentDomande=currentDomande->next;
    else
        currentDomande=testaDomande;


    return;

}


//metodo gestore di tutto il gioco
void Gioca(){
    //stampo di chi è il turno
    printf("\n\nturno del giocatore: %d",currentTurno->g.segno);
    srand(time(NULL));
    int r=rand()%6+1;
    printf("\n\nrisultato lancio dei dati: %d\n\n",r);

    //Rimuovo il giocatore dalla vecchia casella
    RemovePlayerFromCasella(giocatori[currentTurno->g.segno].posizione,giocatori[currentTurno->g.segno]);

    //muovo il giocatore
    giocatori[currentTurno->g.segno].posizione+=r;
    currentTurno->g.posizione=giocatori[currentTurno->g.segno].posizione;

    //in base al tipo di casella avrò un comportamento diverso
    if(Tabellone[giocatori[currentTurno->g.segno].posizione].tipoCasella==Buco){
        StepBuco();
    }else if(Tabellone[giocatori[currentTurno->g.segno].posizione].tipoCasella==Scivolo){
        StepScivolo();
    }else if(Tabellone[giocatori[currentTurno->g.segno].posizione].tipoCasella==Scala){
        StepScala();
    }

    //controllo se il giocatore ha vinto
    if(giocatori[currentTurno->g.segno].posizione>=MAXCaselle){
        //il giocatore ha vinto quindi faccio vedere il giocatore nell'ultima casella e stampo la stringa di vittoria

        printf("\nil giocatore %d ha vinto!!\n\n\n",giocatori[currentTurno->g.segno].segno);
        AddPlayerToCasella(MAXCaselle-1,giocatori[currentTurno->g.segno]);
        continuaGioco=0;
        getch();
        getch();
    }else{
        //il giocatore non ha vinto
        //
        //aggiungo il giocatore nella casella nuova
        AddPlayerToCasella(giocatori[currentTurno->g.segno].posizione,giocatori[currentTurno->g.segno]);

        //faccio avanzare la coda dei turni
        if(currentTurno->next!=NULL)
            currentTurno=currentTurno->next;
        else{
            RequeueTurni();
        }

        //! check se il giocatore deve stare Fermo
        while(currentTurno->g.Fermo!=0){
            //se deve stare fermo salto il suo turno facendo avanzare la coda
            currentTurno->g.Fermo=0;
            if(currentTurno->next!=NULL)
                currentTurno=currentTurno->next;
            else{
                RequeueTurni();
            }
        }
    }


}

int main() {
    //prendo in input il numero dei giocatori
    do{
        printf("nGiocatori:\n");
        scanf("%d",&nGiocatori);
    }while(nGiocatori<=0||nGiocatori>MAXGiocatori);


    //inizializzo le variabili globali
    DefaultSettings();

    //stampo il tabellone
    StampaTab();

    //fino a che il gioco non finisce
    //quindi fino a che "continuaGioco" non è uguale a 0
    //continuo ad eseguire il metodo Gioca e StampaTab
    while(continuaGioco){
        getch();

        //pulisco la console
        system("cls");

        //gioca
        Gioca();
        StampaTab();
    }
    
    return 0;
}
