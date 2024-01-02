#include "gamelib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

Giocatore giocatori[4];

// Funzioni per imposta gioco
static void creaGiocatore(Giocatore *pGiocatore, int i);
static void sceltaClasse(Giocatore *pGiocatore);
static void assegnaValoriClasse(Giocatore *pGiocatore); // Assegna i valori alla classe del giocatore
static void sacrificaPunti(Giocatore *pGiocatore);
static void cancellaGiocatori(Giocatore *pGiocatori);
static void stampaGiocatore(Giocatore *pGiocatore);

// Mappa
static void generaMappa();
static int dimensioneLista();
static void inserisciZona();
static void inserimentoInTesta();
static void inserimentoInCoda();
static void inserimentoInPosizione(unsigned short posizione);
static void cancellaZona();
static void cancellaInTesta();
static void cancellaInCoda();
static void cancellaInPosizione(unsigned short posizione);
static void cancellaMappa();
static void stampaMappa();
static void chiudiMappa();

// Gioca
static void posizionaGiocatore(Giocatore *pGiocatore);
static void mescolaGiocatori();
static void turno(Giocatore *pGiocatore);
static void avanza(Giocatore *pGioatore);
static AbitanteDelleSegrete *creaAbitanteSegrete();
static void stampaAbitante(AbitanteDelleSegrete *pAbitante);

// Conversione enum a testo
static char *getTipoZona(enum TipoZona tipo);
static char *getTipoTesoro(enum TipoTesoro tipo);
static char *getTipoPorta(enum TipoPorta tipo);
static char *getClasseGiocatore(enum ClasseGiocatore classe);

// Pulizia Buffer
int c = 0;
static void puliziaBuffer();

unsigned short isImpostato = 0; // Controlla se il gioco è già stato impostato
unsigned short nGiocatori = 0;
unsigned short nTurno = 0;

ZonaSegrete *pFirst = NULL;
ZonaSegrete *pLast = NULL;

void impostaGioco()
{
    time_t tempo;
    srand((unsigned)time(&tempo));

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
        cancellaMappa();
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
        printf("\n\033[1;37mMenu della mappa, \033[1;0m Scegli cosa fare:\n");
        printf("> \033[1;36m1\033[0m: Genera Mappa.\n");
        printf("> \033[1;35m2\033[0m: Inserisci Zona.\n");
        printf("> \033[1;34m3\033[0m: Cancella Zona.\n");
        printf("> \033[1;33m4\033[0m: Stampa Mappa.\n");
        printf("> \033[1;31m5\033[0m: Chiudi.\n");
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
        case 3:
            cancellaZona();
            stampaMappa();
            break;
        case 4:
            stampaMappa();
            break;
        case 5:
            chiudiMappa();
            break;
        default:
            printf("\033[1;31mAttenzione!\033[1;0m Inserisci un numero tra 1 e 5.\n");
            break;
        }
    } while (sceltaMappa != 5 || isImpostato == 0);
    for (int i = 0; i < nGiocatori; i++)
    {
        posizionaGiocatore(&giocatori[i]);
    }

    return;
}

void gioca()
{
    if (isImpostato == 1)
    {
        printf("\n\033[1;33mGioco impostato correttamente! Preparati a giocare...\033[0m\n");
        nTurno = 1;
        unsigned short fineGioco = 0;
        while (fineGioco != 1)
        {
            mescolaGiocatori();
            for (int i = 0; i < nGiocatori; i++)
            {
                turno(&giocatori[i]);
            }
        }
    }
    else
    {
        printf("\033[1;31mAttenzione! Il gioco non è stato impostato correttamente!\033[1;0m\n");
        return;
    }
}

void terminaGioco()
{
    for (int i = 0; i < 4; i++)
    {
        cancellaGiocatori(&giocatori[i]);
    }
    cancellaMappa();

    printf("\nGrazie per aver giocato a \033[1;33mScalogna-Quest\033[0m, alla prossima!\n");
}

static void creaGiocatore(Giocatore *pGiocatore, int i)
{
    do
    {
        printf("\n\033[1;37mInserisci il nickname del giocatore n. %d\033[1;0m: ", i + 1); // Input del nome del giocatore
        fgets(pGiocatore->nomeGiocatore, 40, stdin);
        // puliziaBuffer();
        if (strcmp(pGiocatore->nomeGiocatore, "") == 0)
            printf("\n\033[31mAttenzione!\033[0m Inserisci un nome valido.");
    } while (strcmp(pGiocatore->nomeGiocatore, "") == 0);
    sceltaClasse(pGiocatore);
}

static void sceltaClasse(Giocatore *pGiocatore)
{
    unsigned short sceltaClasse = 0;
    printf("\nScegli la \033[1;37mclasse\033[1;0m del tuo giocatore (il valore mente viene deciso casualmente tra quei due valori):");
    printf("\n> \033[1;33m1) BARBARO\033[1;0m: \033[1;31m3 dadi attacco\033[1;0m - \033[1;36m2 dadi difesa\033[1;0m - \033[1;32m8 punti vita\033[1;0m - \033[1;35m1 o 2 mente\033[1;0m - \033[1;34m0 Potere Speciale\033[1;0m");
    printf("\n> \033[1;36m2) NANO\033[1;0m: \033[1;31m2 dadi attacco\033[1;0m - \033[1;36m2 dadi difesa\033[1;0m - \033[1;32m7 punti vita\033[1;0m - \033[1;35m2 o 3 mente\033[1;0m - \033[1;34m1 Potere Speciale\033[1;0m");
    printf("\n> \033[1;32m3) ELFO\033[1;0m: \033[1;31m2 dadi attacco\033[1;0m - \033[1;36m2 dadi difesa\033[1;0m - \033[1;32m6 punti vita\033[1;0m - \033[1;35m3 o 4 mente\033[1;0m - \033[1;34m1 Potere Speciale\033[1;0m");
    printf("\n> \033[1;35m4) MAGO\033[1;0m: \033[1;31m1 dado attacco\033[1;0m - \033[1;36m2 dadi difesa\033[1;0m - \033[1;32m4 punti vita\033[1;0m -  \033[1;35m4 o 5 mente\033[1;0m - \033[1;34m3 Potere Speciale\033[1;0m");
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
        pGiocatore->potereSpeciale = 0;
        pGiocatore->mente = rand() % 2 + 1;
        break;
    case 1: // NANO
        pGiocatore->dadiAttacco = 2;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 7;
        pGiocatore->potereSpeciale = 1;
        pGiocatore->mente = rand() % 3 + 2;
        break;
    case 2: // ELFO
        pGiocatore->dadiAttacco = 2;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 6;
        pGiocatore->potereSpeciale = 1;
        pGiocatore->mente = rand() % 4 + 3;
        break;
    case 3: // MAGO
        pGiocatore->dadiAttacco = 1;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 4;
        pGiocatore->potereSpeciale = 3;
        pGiocatore->mente = rand() % 5 + 4;
        break;
    default:
        break;
    }
    stampaGiocatore(pGiocatore);
    sacrificaPunti(pGiocatore);
}

static void sacrificaPunti(Giocatore *pGiocatore)
{
    unsigned short scelta = 0;
    printf("\n\033[1;37mScegli se sacrificare dei punti oppure no\033[1;0m");
    printf("\n> 1)\033[1;35m -1 Mente\033[1;0m \033[1;32m+1 Punti Vita\033[1;0m");
    printf("\n> 2)\033[1;35m +1 Mente\033[1;0m \033[1;32m-1 Punti Vita\033[1;0m");
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
        printf("\nAggiornamento classe giocatore:\033[1;35m Punti Mente: %d\033[1;0m - \033[1;32mPunti Vita: %d\033[1;0m.\n", pGiocatore->mente, pGiocatore->pVita);
        break;
    default:
        printf("\n\033[1;37mNotifica\033[1;0m:Hai rifiutato di sacrificare i punti.\n");
        break;
    }
}

static void stampaGiocatore(Giocatore *pGiocatore)
{
    printf("\n\033[1;37mNickname\033[1;0m: %s", pGiocatore->nomeGiocatore);
    printf("\033[1;33mClasse\033[1;0m: %s\n", getClasseGiocatore(pGiocatore->classeGiocatore));
    printf("\033[1;31mAtk\033[1;0m: %d\n", pGiocatore->dadiAttacco);
    printf("\033[1;36mDef\033[1;0m: %d\n", pGiocatore->dadiDifesa);
    printf("\033[1;32mPunti Vita\033[1;0m: %d\n", pGiocatore->pVita);
    printf("\033[1;34mPotere Speciale\033[1;0m: %d\n", pGiocatore->potereSpeciale);
    printf("\033[1;35mMente\033[1;0m: %d\n", pGiocatore->mente);
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
        pGiocatore->potereSpeciale = 0;
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
        nuovaZona->tipoZona = rand() % 9;

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
}

static int dimensioneLista()
{
    int size = 0;
    ZonaSegrete *pScan = pFirst;
    while (pScan != NULL)
    {
        pScan = pScan->zonaSuccessiva;
        size++;
    }
    return size;
}

static void inserisciZona()
{
    unsigned short posizioneZona = 0;
    printf("Inserisci la posizione in cui inserire la zona\n");
    printf("\033[92mScelta:\033[0m ");
    scanf("%hd", &posizioneZona);
    puliziaBuffer();

    // Inserisco in Testa
    if (posizioneZona <= 1)
    {
        inserimentoInTesta();
        printf("\nHai inserito in testa\n");
    }
    // Inserisco in Coda
    else if (posizioneZona >= dimensioneLista())
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
    pNuovaZona->tipoZona = rand() % 9;
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
    pNuovaZona->tipoZona = rand() % 9;
    ZonaSegrete *temp = pFirst;
    if (temp == NULL)
    {
        pFirst = pNuovaZona;
        return;
    }
    else
    {
        while (temp->zonaSuccessiva != NULL)
        {
            temp = temp->zonaSuccessiva;
        }
        temp->zonaSuccessiva = pNuovaZona;
        pNuovaZona->zonaPrecedente = temp;
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
        pNuovaZona->tipoZona = rand() % 9;
        pNuovaZona->zonaSuccessiva = NULL;
        pNuovaZona->zonaPrecedente = NULL;

        ZonaSegrete *pTemp = pFirst;
        unsigned short posizioneCorrente = 0;
        while (pTemp->zonaSuccessiva != NULL && posizioneCorrente < posizione - 1)
        {
            pTemp = pTemp->zonaSuccessiva;
            posizioneCorrente++;
        }
        pNuovaZona->zonaSuccessiva = pTemp->zonaSuccessiva;
        pNuovaZona->zonaPrecedente = pTemp;
        if (pTemp->zonaSuccessiva != NULL)
        {
            pTemp->zonaSuccessiva->zonaPrecedente = pNuovaZona;
        }

        pTemp->zonaSuccessiva = pNuovaZona;
    }
}

static void cancellaZona()
{
    unsigned short posizioneZona = 0;
    printf("Inserisci la posizione della zona da eliminare\n");
    printf("\033[92mScelta:\033[0m ");
    scanf("%hd", &posizioneZona);
    puliziaBuffer();

    if (posizioneZona <= 1)
    {
        cancellaInTesta();
    }
    else if (posizioneZona >= dimensioneLista())
    {
        cancellaInCoda();
    }
    else
    {
        cancellaInPosizione(posizioneZona);
    }
}

static void cancellaInTesta()
{
    ZonaSegrete *temp = pFirst;
    pFirst = temp->zonaSuccessiva;
    if (pFirst != NULL)
    {
        pFirst->zonaPrecedente = NULL;
    }
    free(temp);
    return;
}

static void cancellaInCoda()
{
    if (pFirst == NULL)
    {
        printf("\033[1;31mAttenzione!\033[1;0m La mappa è vuota.\n");
        return;
    }

    ZonaSegrete *temp = pFirst;
    while (temp->zonaSuccessiva != NULL)
    {
        temp = temp->zonaSuccessiva;
    }

    if (temp->zonaPrecedente != NULL)
    {
        temp->zonaPrecedente->zonaSuccessiva = NULL;
    }
    else
    {
        pFirst = NULL;
    }
    free(temp);
}

static void cancellaInPosizione(unsigned short posizione)
{
    if (pFirst == NULL)
    {
        printf("\033[1;31mAttenzione!\033[1;0m La mappa è vuota.\n");
        return;
    }

    ZonaSegrete *temp = pFirst;

    int i;
    for (i = 0; temp != NULL && i < posizione - 1; i++)
    {
        temp = temp->zonaSuccessiva;
    }

    ZonaSegrete *zonaDaCancellare = temp->zonaSuccessiva;
    temp->zonaSuccessiva = zonaDaCancellare->zonaSuccessiva;
    if (zonaDaCancellare->zonaSuccessiva != NULL)
    {
        zonaDaCancellare->zonaSuccessiva->zonaPrecedente = temp;
    }
    free(zonaDaCancellare);
}

static void cancellaMappa()
{
    ZonaSegrete *pScan = pFirst;
    ZonaSegrete *succ;
    while (pScan != NULL)
    {
        succ = pScan->zonaSuccessiva;
        free(pScan);
        pScan = succ;
    }
    pFirst = NULL;
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
            // printf("%d) Zona: %d - Indirizzo: %p - Prec: %p - Succ: %p\n", a, pScan->tipoZona, pScan, pScan->zonaPrecedente, pScan->zonaSuccessiva);
            printf("Zona %d) \033[1;37mTipo Zona\033[1;0m: %s - \033[1;37mTipo Tesoro\033[1;0m: %s - \033[1;37mTipo Porta\033[1;0m: %s\n", a, getTipoZona(pScan->tipoZona), getTipoTesoro(pScan->tipoTesoro), getTipoPorta(pScan->tipoPorta));
            pScan = pScan->zonaSuccessiva;
            a++;
        } while (pScan != NULL);
    }
}

static void chiudiMappa()
{
    if (dimensioneLista() >= 15)
    {
        isImpostato = 1;
        printf("\033[1;37mFine Creazione Mappa.\033[1;0m\n");
    }
    else
    {
        printf("\033[1;31mAttenzione! Controlla di aver generato almeno 15 zone della mappa!\033[1;0m\n");
    }
}

static char *getTipoZona(enum TipoZona tipo)
{
    switch (tipo)
    {
    case CORRIDIO:
        return "Corridoio";
    case SCALA:
        return "Scala";
    case SALA_BANCHETTO:
        return "Sala Banchetto";
    case MAGAZZINO:
        return "Magazzino";
    case POSTO_GUARDIA:
        return "Posto Guardia";
    case PRIGIONE:
        return "Prigione";
    case CUCINA:
        return "Cucina";
    case ARMERIA:
        return "Armeria";
    case TEMPIO:
        return "Tempio";
    default:
        return "0";
    }
}

static char *getTipoTesoro(enum TipoTesoro tipo)
{
    switch (tipo)
    {
    case NESSUN_TESORO:
        return "Nessun Tesoro";
    case VELENO:
        return "Veleno";
    case GUARIGIONE:
        return "Guarigione";
    case DOPPIA_GUARIGIONE:
        return "Doppia Guarigione";
    default:
        return "0";
    }
}

static char *getTipoPorta(enum TipoPorta tipo)
{
    switch (tipo)
    {
    case NESSUNA_PORTA:
        return "Nessuna Porta";
    case PORTA_NORMALE:
        return "Porta Normale";
    case PORTA_DA_SCASSINARE:
        return "Porta da Scassinare";
    default:
        return "0";
    }
}

static char *getClasseGiocatore(enum ClasseGiocatore classe)
{
    switch (classe)
    {
    case BARBARO:
        return "Barbaro";
    case ELFO:
        return "Elfo";
    case NANO:
        return "Nano";
    case MAGO:
        return "Mago";
    default:
        return "0";
    }
}

static void posizionaGiocatore(Giocatore *pGiocatore)
{
    pGiocatore->posizione = pFirst;
}

static void mescolaGiocatori()
{
    for (int i = nGiocatori - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Giocatore temp = giocatori[i];
        giocatori[i] = giocatori[j];
        giocatori[j] = temp;
    }
}

static void turno(Giocatore *pGiocatore)
{
    printf("\nE' il turno di \033[1;37m%s\033[1;0m\n", pGiocatore->nomeGiocatore);
    unsigned short sceltaTurno = 0;
    do
    {
        printf("\n\033[1;37mScegli cosa fare\033[1;0m:\n");
        printf("> \033[1;36m1\033[0m: Avanza.\n");
        printf("> \033[1;35m2\033[0m: Indietreggia.\n");
        printf("> \033[1;34m3\033[0m: Stampa Giocatore.\n");
        printf("> \033[1;33m4\033[0m: Stampa Zona.\n");
        printf("> \033[1;31m5\033[0m: Chiudi.\n");
        printf("\033[92mScelta:\033[0m ");
        scanf("%hd", &sceltaTurno);
        puliziaBuffer();
        switch (sceltaTurno)
        {
        case 1:
            avanza(pGiocatore);
            break;
        case 2:

            break;
        case 3:
            stampaGiocatore(pGiocatore);
            break;
        case 4:

            break;
        case 5:

            break;
        default:
            printf("\033[1;31mAttenzione!\033[1;0m Inserisci un numero tra 1 e 5.\n");
            break;
        }
    } while (sceltaTurno != 5);
}

static void avanza(Giocatore *pGiocatore)
{
    unsigned short spawnAbitante = 0;
    if (pGiocatore->posizione->zonaSuccessiva == NULL)
    {
        AbitanteDelleSegrete *abitante = creaAbitanteSegrete();
        stampaAbitante(abitante);
        spawnAbitante = 1;
    }

    enum TipoPorta porta = pGiocatore->posizione->zonaSuccessiva->tipoPorta;

    if (porta == PORTA_DA_SCASSINARE || porta == PORTA_NORMALE)
    {
        printf("\n\033[1;37mPer avanzare devi prima aprire la porta!\n");
        return;
    }
}

static AbitanteDelleSegrete *creaAbitanteSegrete()
{
    char nomeAbitante[15][15] = {"Azuril",
                                 "Dovarius",
                                 "Drakorin",
                                 "Elariana",
                                 "Grimbold",
                                 "Lysandra",
                                 "Morvain",
                                 "Mystralor",
                                 "Seraphina",
                                 "Sylvarith",
                                 "Thaladon",
                                 "Valdoril",
                                 "Vexaria",
                                 "Xylenor",
                                 "Zythoril"};

    AbitanteDelleSegrete *abitante = (AbitanteDelleSegrete *)malloc(sizeof(AbitanteDelleSegrete));

    // Generazione casuale nome
    strcpy(abitante->nome, nomeAbitante[rand() % 15]);

    // Generazione dadi e pv in base al turno
    int max = 10;
    int range = nTurno * 2 + 1;
    range = (range > max) ? max : (nTurno * 2 + 1);
    abitante->dadiAttacco = rand() % range + 1;
    abitante->dadiDifesa = rand() % range + 1;
    abitante->pVita = rand() % range + 1;

    return abitante;
}

static void stampaAbitante(AbitanteDelleSegrete *pAbitante)
{
    printf("\n\033[1;37mNome\033[1;0m: %s\n", pAbitante->nome);
    printf("\033[1;31mAtk\033[1;0m: %d\n", pAbitante->dadiAttacco);
    printf("\033[1;36mDef\033[1;0m: %d\n", pAbitante->dadiDifesa);
    printf("\033[1;32mPunti Vita\033[1;0m: %d\n", pAbitante->pVita);
}

static void puliziaBuffer()
{
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}