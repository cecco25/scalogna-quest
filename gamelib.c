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
static void generaMappa();

int c = 0;                      // Variabile per pulizia buffer
unsigned short isImpostato = 0; // Variabile di controllo per notificare che il gioco è già stato impostato
ZonaSegrete *pFirst;
ZonaSegrete *pLast;

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
        while ((c = getchar()) != '\n' && c != EOF)
            ;
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
    generaMappa();
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
        while ((c = getchar()) != '\n' && c != EOF)
            ;
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
    // printf("\tClasse: %d\nAtk: %d\nDef: %d\nPV: %d\nMente: %d", pGiocatore->classeGiocatore, pGiocatore->dadiAttacco, pGiocatore->dadiDifesa, pGiocatore->pVita, pGiocatore->mente);
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
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    switch (scelta)
    {
    case 1:
        pGiocatore->mente -= 1;
        pGiocatore->pVita += 1;
        printf("\nAggiornamento classe giocatore:\033[1;35m Punti Mente: %d\033[1;0m - \033[1;32mPunti Vita: %d\033[1;0m.", pGiocatore->mente, pGiocatore->pVita);
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
    }
}

static void generaMappa()
{
}