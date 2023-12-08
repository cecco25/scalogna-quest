#include "gamelib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

Giocatore giocatori[4];

// Funzioni per imposta gioco
static void creaGiocatore(Giocatore *pGiocatore, int i);
static void sceltaClasse(Giocatore *pGiocatore);
static void assegnaValoriClasse(Giocatore *pGiocatore); // Assegna i valori alla classe del giocatore
static void sacrificaPunti(Giocatore *pGiocatore);
static void cancellaGiocatori(Giocatore *pGiocatori);
static void stampaGiocatori(unsigned short nGiocatori);

// Mappa
static void generaMappa();
static void inserisciZona();
static void inserimentoInTesta();
static void inserimentoInCoda();
static void inserimentoInPosizione(unsigned short posizione);
static void stampaMappa();

// Pulizia Buffer
int c = 0;
static void puliziaBuffer();

unsigned short isImpostato = 0; // Variabile di controllo per notificare che il gioco è già stato impostato
ZonaSegrete *pFirst = NULL;
ZonaSegrete *pLast = NULL;

void impostaGioco()
{
    time_t tempo;
    srand((unsigned)time(&tempo));

    unsigned short nGiocatori = 0;

    if (isImpostato == 1)
    {
        printf("\033[1;33m\nAvviso!\033[1;0m Il gioco aveva già delle impostazioni salvate, ora sono state eliminate.\n");
        isImpostato = 0;
        nGiocatori = 0;
        int i = 0;
        for (i = 0; i < 4; i++)
        {
            cancellaGiocatori(&giocatori[i]);
        }
    }

    do
    {
        printf("\033[1;37mInserisci il numero di giocatori (max 4)\033[1;0m\n");
        scanf("%hu", &nGiocatori);
        puliziaBuffer();
        if (nGiocatori == 0 || nGiocatori > 4)
        {
            printf("\033[1;37mIl numero di giocatori va \033[1;31mda 1 a 4\033[0m.\n");
        }
    } while (nGiocatori == 0 || nGiocatori > 4);

    int i = 0;
    for (i = 0; i < nGiocatori; i++)
    {
        creaGiocatore(&giocatori[i], i);
    }

    // Menu Mappa
    unsigned short sceltaMappa = 0;
    do
    {
        printf("\033[1;37mMenu della mappa, \033[1;0m Scegli cosa fare:\n");
        printf("> \033[1;36m1\033[0m: Genera Mappa.\n");
        printf("> \033[1;35m2\033[0m: Inserisci Zona.\n");
        printf("> \033[1;34m3\033[0m: Cancella Zona.\n");
        printf("> \033[1;33m4\033[0m: Stampa Mappa.\n");
        printf("> \033[1;31m4\033[0m: Chiudi.\n");
        printf("\033[92mScelta:\033[0m ");
        scanf("%hd", &sceltaMappa);
        puliziaBuffer();
        switch (sceltaMappa)
        {
        case 1:
            generaMappa();
            printf("La mappa è stata generata con 15 zone\n");
            stampaMappa();
            break;
        case 2:
            inserisciZona();
            stampaMappa();
            break;
        default:
            printf("\033[1;31mAttenzione!\033[1;0m Inserisci un numero tra 1 e 5.\n");
            break;
        }

    } while (sceltaMappa != 5);

    isImpostato = 1;
    return;
}

static void creaGiocatore(Giocatore *pGiocatore, int i)
{
    do
    {
        printf("\n\033[1;37mInserisci il nickname del giocatore n. %d\033[1;0m: ", i + 1); // Input del nome del giocatore
        fgets(pGiocatore->nomeGiocatore, 10, stdin);
        if (strcmp(pGiocatore->nomeGiocatore, "") == 0)
            printf("\n\033[31mAttenzione!\033[0m Inserisci un nome valido.");
    } while (strcmp(pGiocatore->nomeGiocatore, "") == 0);
    sceltaClasse(pGiocatore);
}

static void sceltaClasse(Giocatore *pGiocatore)
{
    unsigned short sceltaClasse = 0;
    printf("\nScegli la \033[1;37mclasse\033[1;0m del tuo giocatore (il valore mente viene deciso casualmente tra quei due valori):");
    printf("\n> \033[1;33m1) BARBARO\033[1;0m: \033[1;31m3 dadi attacco\033[1;0m - \033[1;34m2 dadi difesa\033[1;0m - \033[1;32m8 punti vita\033[1;0m - \033[1;35m1 o 2 mente\033[1;0m");
    printf("\n> \033[1;36m2) NANO\033[1;0m: \033[1;31m2 dadi attacco\033[1;0m - \033[1;34m2 dadi difesa\033[1;0m - \033[1;32m7 punti vita\033[1;0m - \033[1;35m2 o 3 mente\033[1;0m");
    printf("\n> \033[1;32m3) ELFO\033[1;0m: \033[1;31m2 dadi attacco\033[1;0m - \033[1;34m2 dadi difesa\033[1;0m - \033[1;32m6 punti vita\033[1;0m - \033[1;35m3 o 4 mente\033[1;0m");
    printf("\n> \033[1;35m4) MAGO\033[1;0m: \033[1;31m1 dado attacco\033[1;0m - \033[1;34m2 dadi difesa\033[1;0m - \033[1;32m4 punti vita\033[1;0m -  \033[1;35m4 o 5 mente\033[1;0m");
    do
    {
        printf("\nScegli Classe: ");
        scanf("%hu", &sceltaClasse);
        puliziaBuffer();
        if (sceltaClasse < 1 || sceltaClasse > 4)
            printf("\n\033[31mAttenzione!\033[0m Classe non riconosciuta, inserisci un numero tra 1 e 4.");
    } while (sceltaClasse < 1 || sceltaClasse > 4);
    pGiocatore->classeGiocatore = --sceltaClasse;
    assegnaValoriClasse(pGiocatore);
}

static void assegnaValoriClasse(Giocatore *pGiocatore)
{
    switch (pGiocatore->classeGiocatore)
    {
    case 0: // BARBARO
        pGiocatore->dadiAttacco = 3;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 8;
        pGiocatore->valoreSpeciale = 0;
        pGiocatore->mente = rand() % 2 + 1;
        break;
    case 1: // NANO
        pGiocatore->dadiAttacco = 2;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 7;
        pGiocatore->valoreSpeciale = 1;
        pGiocatore->mente = rand() % 3 + 2;
        break;
    case 2: // ELFO
        pGiocatore->dadiAttacco = 2;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 6;
        pGiocatore->valoreSpeciale = 1;
        pGiocatore->mente = rand() % 4 + 3;
        break;
    case 3: // MAGO
        pGiocatore->dadiAttacco = 1;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 4;
        pGiocatore->valoreSpeciale = 3;
        pGiocatore->mente = rand() % 5 + 4;
        break;
    default:
        break;
    }
    sacrificaPunti(pGiocatore);
}

static void sacrificaPunti(Giocatore *pGiocatore)
{
    unsigned short scelta = 0;
    printf("\n\033[1;37mScegli se sacrificare dei punti oppure no\033[1;0m");
    printf("\n> 1)\033[1;31m -1 Mente\033[1;0m \033[1;32m+1 Punti Vita\033[1;0m");
    printf("\n> 2)\033[1;32m +1 Mente\033[1;0m \033[1;31m-1 Punti Vita\033[1;0m");
    printf("\n\033[37mInserisci qualsiasi altro numero per rifiutare...\033[1;0m\n");
    printf("Scelta: ");
    scanf("%hu", &scelta);
    puliziaBuffer();
    switch (scelta)
    {
    case 1:
        pGiocatore->mente -= 1;
        pGiocatore->pVita += 1;
        printf("\nAggiornamento classe giocatore:\033[1;35m Punti Mente: %d\033[1;0m - \033[1;32mPunti Vita: %d\033[1;0m.\n", pGiocatore->mente, pGiocatore->pVita);
        break;
    case 2:
        pGiocatore->mente += 1;
        pGiocatore->pVita -= 1;
        printf("\nAggiornamento classe giocatore:\033[1;35m Punti Mente: %d\033[1;0m - \033[1;32mPunti Vita: %d\033[1;0m.", pGiocatore->mente, pGiocatore->pVita);
        break;
    default:
        printf("\n\033[1;37mNotifica\033[1;0m:Hai rifiutato di sacrificare i punti.");
        break;
    }
}

static void stampaGiocatori(unsigned short nGiocatori)
{
    int i = 0;
    for (i = 0; i < nGiocatori; i++)
    {
        Giocatore *pGiocatore = &giocatori[i];
        printf("\nGiocatore Numero %d\n", (i + 1));
        printf("\tNickname: %s\n", pGiocatore->nomeGiocatore);
        printf("\tClasse: %d\n", pGiocatore->classeGiocatore);
        printf("\tAtk: %d\n", pGiocatore->dadiAttacco);
        printf("\tDef: %d\n", pGiocatore->dadiDifesa);
        printf("\tPV: %d\n", pGiocatore->pVita);
        printf("\tValore Speciale: %d\n", pGiocatore->valoreSpeciale);
        printf("\tMente: %d\n", pGiocatore->mente);
    }
}

static void cancellaGiocatori(Giocatore *pGiocatore)
{
    int i = 0;
    for (i = 0; i < 4; i++)
    {
        pGiocatore->dadiAttacco = 0;
        pGiocatore->dadiDifesa = 0;
        pGiocatore->pVita = 0;
        pGiocatore->mente = 0;
        pGiocatore->valoreSpeciale = 0;
        pGiocatore->classeGiocatore = -1;
    }
}

static void generaMappa()
{
    for (int i = 0; i < 15; i++)
    {
        ZonaSegrete *nuovaZona = (ZonaSegrete *)malloc(sizeof(ZonaSegrete));
        nuovaZona->zonaPrecedente = NULL;
        nuovaZona->zonaSuccessiva = NULL;
        nuovaZona->tipoPorta = rand() % 3;
        nuovaZona->tipoTesoro = rand() % 4;
        nuovaZona->tipoZona = rand() % 10;

        // Primo Elemento della lista
        if (i == 0)
        {
            pFirst = nuovaZona;
            pLast = nuovaZona;
        }
        else
        {
            pLast->zonaSuccessiva = nuovaZona;
            nuovaZona->zonaPrecedente = pLast;
            pLast = nuovaZona;
        }
    }
    /*printf("\nPuntatore alla prima zona: %p\n", (void *)pFirst);
    printf("\nPuntatore all'ultima zona: %p\n", (void *)pLast);*/
    // stampaMappa();
}

static void inserisciZona()
{
    unsigned short posizioneZona = 0;
    printf("Inserisci la posizione in cui inserire la zona\n");
    printf("\033[92mScelta:\033[0m ");
    scanf("%hd", &posizioneZona);
    puliziaBuffer();

    // Inserisco in Testa
    if (posizioneZona <= 0 || posizioneZona == 1)
    {
        inserimentoInTesta();
        printf("\nHai inserito in testa\n");
    }
    // Inserisco in Coda
    else if (posizioneZona >= 15)
    {
        inserimentoInCoda();
        printf("\nHai inserito in coda\n");
    }
    // Inserisco in Posizione
    else
    {
        inserimentoInPosizione(posizioneZona);
        printf("\nHai inserito in posizione %hu\n", posizioneZona);
    }
}

static void inserimentoInTesta()
{
    ZonaSegrete *pNuovaZona = (ZonaSegrete *)malloc(sizeof(ZonaSegrete));
    pNuovaZona->zonaPrecedente = NULL;
    pNuovaZona->zonaSuccessiva = NULL;
    pNuovaZona->tipoPorta = rand() % 3;
    pNuovaZona->tipoTesoro = rand() % 4;
    pNuovaZona->tipoZona = rand() % 10;
    if (pFirst == NULL)
        pFirst = pNuovaZona;
    else
    {
        pNuovaZona->zonaSuccessiva = pFirst;
        pFirst->zonaPrecedente = pNuovaZona;
        pFirst = pNuovaZona;
    }
}

static void inserimentoInCoda()
{
    ZonaSegrete *pNuovaZona = (ZonaSegrete *)malloc(sizeof(ZonaSegrete));
    pNuovaZona->zonaPrecedente = NULL;
    pNuovaZona->zonaSuccessiva = NULL;
    pNuovaZona->tipoPorta = rand() % 3;
    pNuovaZona->tipoTesoro = rand() % 4;
    pNuovaZona->tipoZona = rand() % 10;
    if (pFirst == NULL)
    {
        pFirst = pNuovaZona;
        pLast = pNuovaZona;
    }
    else
    {
        pLast->zonaSuccessiva = pNuovaZona;
        pNuovaZona->zonaPrecedente = pLast;
        pLast = pNuovaZona;
    }
}

static void inserimentoInPosizione(unsigned short posizione)
{
    if (pFirst == NULL)
    {
        inserimentoInTesta();
    }
    else
    {
        ZonaSegrete *pNuovaZona = (ZonaSegrete *)malloc(sizeof(ZonaSegrete));
        pNuovaZona->tipoPorta = rand() % 3;
        pNuovaZona->tipoTesoro = rand() % 4;
        pNuovaZona->tipoZona = rand() % 10;
        pNuovaZona->zonaSuccessiva = NULL;
        pNuovaZona->zonaPrecedente = NULL;

        unsigned short posizioneCorrente = 0;
        while (pFirst->zonaSuccessiva != NULL && posizioneCorrente < posizione - 1)
        {
            pFirst = pFirst->zonaSuccessiva;
            posizioneCorrente++;
        }
        pNuovaZona->zonaSuccessiva = pFirst->zonaSuccessiva;
        pFirst->zonaSuccessiva = pNuovaZona;
    }
}

static void stampaMappa()
{
    // Stampa lista
    if (pFirst == NULL)
    {
        printf("Non ci sono zone da stampare\n");
    }
    else
    {
        ZonaSegrete *pScan = pFirst;
        int a = 0;
        do
        {
            printf("%d) Zona: %d - Indirizzo: %p - Prec: %p - Succ: %p\n", a + 1, pScan->tipoZona, pScan, pScan->zonaPrecedente, pScan->zonaSuccessiva);
            pScan = pScan->zonaSuccessiva;
            a++;
        } while (pScan != NULL);
    }
}

static void puliziaBuffer()
{
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}