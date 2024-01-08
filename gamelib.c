#include "gamelib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

Giocatore giocatori[4];

// Funzioni per imposta gioco
static void creaGiocatore(Giocatore *pGiocatore, int i);
static void sceltaClasse(Giocatore *pGiocatore);
static void assegnaValoriClasse(Giocatore *pGiocatore); // Assegna i valori alla classe del giocatore
static void sacrificaPunti(Giocatore *pGiocatore);
static void cancellaGiocatore(Giocatore *pGiocatore);
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
static int avanza(Giocatore *pGioatore);
static void indietreggia(Giocatore *pGiocatore);
static void apriPorta(Giocatore *pGiocatore);
static void prendiTesoro(Giocatore *pGiocatore);
static void duelloAbitante(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante);
static int scappa(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante);
static int combatti(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante);
static int potereSpeciale(Giocatore *pGiocatore);
static void stampaZona(ZonaSegrete *zona);
static AbitanteDelleSegrete *creaAbitanteSegrete(ZonaSegrete *zona);
static void stampaAbitante(AbitanteDelleSegrete *pAbitante);
static void sconfittaAbitante(AbitanteDelleSegrete *pAbitante);
static void resetFineGioco();

// Conversione enum a testo
static char *getTipoZona(TipoZona tipo);
static char *getTipoTesoro(TipoTesoro tipo);
static char *getTipoPorta(TipoPorta tipo);
static char *getClasseGiocatore(ClasseGiocatore classe);
static char *getDadoCombattimento(DadoCombattimento dado);

// Pulizia Buffer
int c = 0;
static void puliziaBuffer();

unsigned short isImpostato = 0; // Controlla se il gioco è già stato impostato
unsigned short nGiocatori = 0;
unsigned short nTurno = 0;
unsigned short fineGioco = 0;

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
            cancellaGiocatore(&giocatori[i]);
        }
        cancellaMappa();
    }

    do
    {
        printf("\033[1;37mInserisci il numero di giocatori (max 4):\033[1;0m ");
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
        sleep(2);
    }
    // Menu Mappa
    system("clear");
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
            system("clear");
            inserisciZona();
            stampaMappa();
            break;
        case 3:
            cancellaZona();
            stampaMappa();
            break;
        case 4:
            system("clear");
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
        sleep(2);
        system("clear");
        while (fineGioco != 1)
        {
            nTurno++;
            mescolaGiocatori();
            for (int i = 0; i < nGiocatori; i++)
            {
                turno(&giocatori[i]);
                if (nGiocatori == 0)
                {
                    printf("\n\033[1;31mLA PARTITA SI CONCLUDE CON %d TURNI, TUTTI I GIOCATORI SONO STATI SCONFITTI...\033[0m\n", nTurno);
                    fineGioco = 1;
                    break;
                }
            }
        }
        resetFineGioco();
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
        cancellaGiocatore(&giocatori[i]);
    }
    cancellaMappa();

    printf("\nGrazie per aver giocato a \033[1;33mScalogna-Quest\033[0m, alla prossima!\n");
}

static void creaGiocatore(Giocatore *pGiocatore, int i)
{
    do
    {
        system("clear");
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
        pGiocatore->mente = ((rand() % 2) == 0) ? 2 : 3;
        break;
    case 2: // ELFO
        pGiocatore->dadiAttacco = 2;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 6;
        pGiocatore->potereSpeciale = 1;
        pGiocatore->mente = ((rand() % 2) == 0) ? 3 : 4;
        break;
    case 3: // MAGO
        pGiocatore->dadiAttacco = 1;
        pGiocatore->dadiDifesa = 2;
        pGiocatore->pVita = 4;
        pGiocatore->potereSpeciale = 3;
        pGiocatore->mente = ((rand() % 2) == 0) ? 4 : 5;
        break;
    default:
        break;
    }
    system("clear");
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
        printf("\n\033[1;37mNotifica\033[1;0m: Hai rifiutato di sacrificare i punti.\n");
        break;
    }
}

static void cancellaGiocatore(Giocatore *pGiocatore)
{
    // Trova l'indice del giocatore nell'array
    int indice = -1;
    for (int i = 0; i < nGiocatori; ++i)
    {
        if (&giocatori[i] == pGiocatore)
        {
            indice = i;
            break;
        }
    }

    if (indice != -1)
    {
        // Sposta gli altri giocatori nell'array per coprire lo spazio lasciato dal giocatore eliminato
        for (int i = indice; i < nGiocatori - 1; ++i)
        {
            giocatori[i] = giocatori[i + 1];
        }

        nGiocatori--;

        // Setta a zero la struct del giocatore rimosso
        memset(&giocatori[nGiocatori], 0, sizeof(Giocatore));
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
        pNuovaZona->tipoZona = rand() % 10;
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
    if (dimensioneLista() >= 2)
    {
        isImpostato = 1;
        system("clear");
        printf("\033[1;37mFine Creazione Mappa.\033[1;0m\n");
    }
    else
    {
        printf("\033[1;31mAttenzione! Controlla di aver generato almeno 15 zone della mappa!\033[1;0m\n");
    }
}

static char *getTipoZona(TipoZona tipo)
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
    case GIARDINO:
        return "Giardino";
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

static char *getTipoTesoro(TipoTesoro tipo)
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

static char *getTipoPorta(TipoPorta tipo)
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

static char *getClasseGiocatore(ClasseGiocatore classe)
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
    short sceltaTurno = -1;
    short azioni = 0;
    short avanzato = 0;
    printf("*-----------------------------------------*\n");
    printf("\n\t\033[1;33m---- Turno %d ----\033[1;0m\n", nTurno);
    printf("\n\tE' il turno di \033[1;37m%s\033[1;0m\n", pGiocatore->nomeGiocatore);
    printf("*----------------------------------------*\n\n");

    do
    {
        if (fineGioco == 1)
            break;
        if (pGiocatore->pVita <= 0)
        {
            printf("\n\033[1;37m%s\033[1;31m è stato sconfitto...\n\n\033[0m", pGiocatore->nomeGiocatore);
            cancellaGiocatore(pGiocatore);
            return;
        }
        printf("\n\033[1;37mScegli cosa fare\033[1;0m:\n");
        printf("> \033[1;36m1\033[0m: Avanza.\n");
        printf("> \033[1;35m2\033[0m: Indietreggia.\n");
        printf("> \033[1;34m3\033[0m: Stampa Giocatore.\n");
        printf("> \033[1;33m4\033[0m: Stampa Zona.\n");
        printf("> \033[1;36m5\033[0m: Apri Porta.\n");
        printf("> \033[1;35m6\033[0m: Prendi Tesoro.\n");
        printf("> \033[1;31m0\033[0m: Passa Turno.\n");
        printf("\033[92mScelta:\033[0m ");
        scanf("%hd", &sceltaTurno);
        puliziaBuffer();

        system("clear");
        printf("*-----------------------------------------*\n");
        printf("\n\t\033[1;33m---- Turno %d ----\033[1;0m\n", nTurno);
        printf("\n\tE' il turno di \033[1;37m%s\033[1;0m\n", pGiocatore->nomeGiocatore);
        printf("*----------------------------------------*\n\n");

        switch (sceltaTurno)
        {
        case 0:
            printf("\n\033[1;37mHai passato il turno\033[0m\n");
            break;
        case 1:
            if (avanzato == 0)
            {
                if (azioni <= 3)
                {

                    avanzato = avanza(pGiocatore);
                    azioni++;
                }
                else
                {
                    printf("\n\033[1;31mAttenzione!\033[1;37m Hai raggiunto il numero massimo di azioni per questo turno.\n");
                }
            }
            else
            {
                printf("\n\033[1;31mAttenzione!\033[1;37m Non puoi avanzare di nuovo.\n");
            }
            break;
        case 2:
            if (azioni <= 3)
            {
                indietreggia(pGiocatore);
                azioni++;
            }
            else
            {
                printf("\n\033[1;31mAttenzione!\033[1;37m Hai raggiunto il numero massimo di azioni per questo turno.\n");
            }
            break;
        case 3:
            stampaGiocatore(pGiocatore);
            break;
        case 4:
            stampaZona(pGiocatore->posizione);
            break;
        case 5:
            if (azioni <= 3)
            {

                apriPorta(pGiocatore);
                azioni++;
            }
            else
            {
                printf("\n\033[1;31mAttenzione!\033[1;37m Hai raggiunto il numero massimo di azioni per questo turno.\n");
            }
            break;
        case 6:
            if (azioni <= 3)
            {

                prendiTesoro(pGiocatore);
                azioni++;
            }
            else
            {
                printf("\n\033[1;31mAttenzione!\033[1;37m Hai raggiunto il numero massimo di azioni per questo turno.\n");
            }
            break;
        default:
            printf("\033[1;31mAttenzione!\033[1;0m Cosa stai cercando di fare?!\n");
            break;
        }
    } while (sceltaTurno != 0);
}

// Ritorna 1 se il giocatore è andato avanti, altrimenti 0
static int avanza(Giocatore *pGiocatore)
{
    TipoPorta porta = pGiocatore->posizione->tipoPorta;

    if (porta == PORTA_DA_SCASSINARE || porta == PORTA_NORMALE)
    {
        printf("\033[1;31mAhia!\033[1;37m Hai sbattuto contro una porta...\033[1;0m\n");
        return 0;
    }

    pGiocatore->posizione = pGiocatore->posizione->zonaSuccessiva;
    printf("\n\033[1;37mSei entrato in una nuova zona.\033[1;0m\n");
    // Generazione abitante
    int r = rand() % 3;
    int spawnAbitante = (r == 0) ? 1 : 2; // Generazione con 33% di probabilità
    if (pGiocatore->posizione->zonaSuccessiva == NULL || spawnAbitante == 1)
    {
        AbitanteDelleSegrete *abitante = creaAbitanteSegrete(pGiocatore->posizione);
        spawnAbitante = 1;
        printf("...\n");
        sleep(2);
        printf("\n... Oh ...\n");
        sleep(2);
        printf("E' apparso davanti a te \033[1;31m%s\033[1;37m! Ora dovrai combatterlo!\033[1;0m\n", abitante->nome);
        stampaAbitante(abitante);
        printf("\n\033[1;37mInvia qualcosa quando ti senti pronto...\033[1;0m\n");
        getchar();
        puliziaBuffer();
        duelloAbitante(pGiocatore, abitante);
    }
    if (pGiocatore->posizione->zonaSuccessiva == NULL && pGiocatore->pVita > 0)
    {
        printf("\n\033[1;33mLA PARTITA SI CONCLUDE CON %d TURNI, IL VINCITORE E' \033[1;37m%s\033[0m\n", nTurno, pGiocatore->nomeGiocatore);
        fineGioco = 1;
    }
    return 1;
}

static void indietreggia(Giocatore *pGiocatore)
{
    if (pGiocatore->posizione->zonaPrecedente == NULL)
    {
        printf("\033[1;31mAttenzione!\033[1;0m Non puoi indietreggiare, non ci sono zone.\n");
        return;
    }

    pGiocatore->posizione = pGiocatore->posizione->zonaPrecedente;
    printf("\n\033[1;37mSei tornato indietro.\033[1;0m\n");

    pGiocatore->posizione->tipoTesoro = rand() % 4;

    int r = rand() % 3;
    int spawnAbitante = (r == 0) ? 1 : 2; // Generazione con 33% di probabilità
    if (pGiocatore->posizione->zonaSuccessiva == NULL || spawnAbitante == 1)
    {
        AbitanteDelleSegrete *abitante = creaAbitanteSegrete(pGiocatore->posizione);
        spawnAbitante = 1;
        printf("...\n");
        sleep(2);
        printf("\n... Oh ... \n");
        sleep(2);
        printf("\033[1;37mE' apparso davanti a te \033[1;31m%s\033[1;37m! Ora dovrai combatterlo!\033[1;0m\n", abitante->nome);
        stampaAbitante(abitante);
        printf("\n\033[1;37mInvia qualcosa quando ti senti pronto...\033[1;0m\n");
        getchar();
        puliziaBuffer();
        duelloAbitante(pGiocatore, abitante);
    }
}

static void apriPorta(Giocatore *pGiocatore)
{
    TipoPorta porta = pGiocatore->posizione->tipoPorta;
    if (porta == NESSUNA_PORTA)
    {
        printf("\n\033[1;37mNon c'era nessuna porta... (hai bevuto troppe pozioni?)\033[1;0m\n");
    }
    else if (porta == PORTA_NORMALE)
    {
        pGiocatore->posizione->tipoPorta = NESSUNA_PORTA;
        printf("\n\033[1;37mHai aperto la porta.\033[1;0m\n");
    }
    else if (porta == PORTA_DA_SCASSINARE)
    {
        printf("\n\033[1;37mLa porta va scassinata, vediamo se ci riuscirai...\033[1;0m\n");
        int dado = rand() % 6 + 1;
        sleep(2);
        printf("\033[92mDado:\033[0m %d\n", dado);
        sleep(1);
        if (dado <= pGiocatore->mente)
        {
            pGiocatore->posizione->tipoPorta = NESSUNA_PORTA;
            printf("\n\033[92mSei riuscito a scassinare la porta!\033[0m\n");
        }
        else
        {
            printf("\n\033[0;31mMi dispiace, non sei riuscito a scassinare la porta!\033[0;0m\n");

            int r = rand() % 100;

            if (r < 10)
            {
                // 10% di probabilità
                pGiocatore->posizione = pFirst;
                printf("\n\033[1;37mTorni alla prima zona dellle segrete: %s\n", getTipoZona(pFirst->tipoZona));
            }
            else if (r < 60)
            {
                // 50% di probabilità (da 10 a 59)
                pGiocatore->pVita -= 1;
                printf("\n\033[1;31m-1\033[1;37m Punto Vita\n");
            }
            else
            {
                // 40% di probabilità (da 60 a 99)
                AbitanteDelleSegrete *abitante = creaAbitanteSegrete(pGiocatore->posizione);
                printf("...\n");
                sleep(2);
                printf("\n... Oh ... \n");
                sleep(2);
                printf("\033[1;37mE' apparso davanti a te \033[1;31m%s\033[1;37m! Ora dovrai combatterlo!\033[1;0m\n", abitante->nome);
                stampaAbitante(abitante);
                printf("\n\033[1;37mInvia qualcosa quando ti senti pronto...\033[1;0m\n");
                getchar();
                puliziaBuffer();
                duelloAbitante(pGiocatore, abitante);
            }
        }
    }
}

static void prendiTesoro(Giocatore *pGiocatore)
{
    if (pGiocatore->posizione->tipoTesoro == VELENO)
    {
        pGiocatore->pVita -= 2;
        printf("\n\033[1;37mNel tesoro c'era del \033[1;35mVeleno\033[1;37m!\n\033[1;31m-2 punti vita.\033[0m\n");
    }
    else if (pGiocatore->posizione->tipoTesoro == GUARIGIONE)
    {
        pGiocatore->pVita += 1;
        printf("\n\033[1;37mNel tesoro c'era una \033[1;32mGuarigione\033[1;37m!\n\033[1;32m+1 punti vita.\033[0m\n");
    }
    else if (pGiocatore->posizione->tipoTesoro == DOPPIA_GUARIGIONE)
    {
        pGiocatore->pVita += 2;
        printf("\n\033[1;37mNel tesoro c'era una \033[1;32mDoppia Guarigione\033[1;37m!\n\033[1;32m+2 punti vita.\033[0m\n");
    }
    else
    {
        printf("\n\033[1;37mNon c'era nessun tesoro.\033[0m\n");
    }
    pGiocatore->posizione->tipoTesoro = NESSUN_TESORO;
}

static void stampaZona(ZonaSegrete *zona)
{
    unsigned short tesoro = (zona->tipoTesoro == NESSUN_TESORO) ? 0 : 1;
    unsigned short porta = (zona->tipoPorta == NESSUNA_PORTA) ? 0 : 1;
    printf("\033[1;37mTipo Zona\033[1;0m: %s - \033[1;37mTesoro\033[1;0m: %s - \033[1;37mPorta\033[1;0m: %s\n", getTipoZona(zona->tipoZona), (tesoro == 0) ? "No" : "Sì", (porta == 0) ? "No" : "Sì");
}

static void duelloAbitante(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante)
{
    unsigned short sceltaDuello = 0;
    int win = 0;
    system("clear");
    printf("*-----------------------------------------*\n");
    printf("\n\t\033[1;31m%s \033[1;33mVS \033[1;37m%s\033[1;0m\n", pAbitante->nome, pGiocatore->nomeGiocatore);
    printf("*-----------------------------------------*\n\n");
    do
    {
        if (pGiocatore->pVita <= 0)
        {
            return;
        }
        printf("\n\033[1;37mScegli cosa fare\033[1;0m:\n");
        printf("> \033[1;36m1\033[0m: Combatti.\n");
        printf("> \033[1;35m2\033[0m: Scappa.\n");
        printf("> \033[1;34m3\033[0m: Gioca Potere Speciale.\n");
        printf("> \033[1;33m4\033[0m: Stampa Giocatore.\n");
        printf("> \033[1;32m5\033[0m: Stampa Avversario.\n");
        printf("\033[92mScelta:\033[0m ");
        scanf("%hd", &sceltaDuello);
        puliziaBuffer();

        system("clear");
        printf("*-----------------------------------------*\n");
        printf("\n\t\033[1;31m%s \033[1;33mVS \033[1;37m%s\033[1;0m\n", pAbitante->nome, pGiocatore->nomeGiocatore);
        printf("*-----------------------------------------*\n\n");

        switch (sceltaDuello)
        {
        case 1:
            win = combatti(pGiocatore, pAbitante);
            break;
        case 2:
            win = scappa(pGiocatore, pAbitante);
            break;
        case 3:
            win = potereSpeciale(pGiocatore);
            break;
        case 4:
            stampaGiocatore(pGiocatore);
            break;
        case 5:
            stampaAbitante(pAbitante);
            break;
        default:
            printf("\033[1;31mAttenzione!\033[1;0m Cosa stai cercando di fare?!\n");
            break;
        }

    } while ((sceltaDuello != 0 && sceltaDuello >= 6) || win == 0);
}

// Ritorna 1 se il giocatore ha sconfitto l'abitante, altrimenti 0
static int combatti(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante)
{
    DadoCombattimento dadiAttaccoGiocatore[pGiocatore->dadiAttacco];
    DadoCombattimento dadiDifesaGiocatore[pGiocatore->dadiDifesa];

    DadoCombattimento dadiAttaccoAbitante[pAbitante->dadiAttacco];
    DadoCombattimento dadiDifesaAbitante[pAbitante->dadiDifesa];

    system("clear");
    do
    {
        printf("*-----------------------------------------*\n");
        printf("\n\t\033[1;31m%s \033[1;33mVS \033[1;37m%s\033[1;0m\n", pAbitante->nome, pGiocatore->nomeGiocatore);
        printf("*-----------------------------------------*\n\n");
        sleep(2);
        int dadoAvversario = rand() % 6 + 1;
        printf("\n\033[0;31mDado Avversario\033[0m: %d\n", dadoAvversario);
        sleep(2);
        int dadoGiocatore = rand() % 6 + 1;
        printf("\033[92mDado Giocatore:\033[0m %d\n", dadoGiocatore);
        sleep(1);

        if (dadoGiocatore >= dadoAvversario)
        {
            // TURNO GIOCATORE
            printf("\nInizia \033[1;37m%s\033[0m\n", pGiocatore->nomeGiocatore);
            printf("\n\033[1;31m---ATTACCO GIOCATORE---\033[0m\n");

            short dannoGiocatore = 0;

            // Tiro dei dadi attacco del giocatore
            for (int i = 0; i < pGiocatore->dadiAttacco; i++)
            {
                int atk = rand() % 6;
                dadiAttaccoGiocatore[i] = atk;
                printf("%s\n", getDadoCombattimento(dadiAttaccoGiocatore[i]));

                dannoGiocatore += (dadiAttaccoGiocatore[i] == TESCHIO1 || dadiAttaccoGiocatore[i] == TESCHIO2 || dadiAttaccoGiocatore[i] == TESCHIO3) ? 1 : 0;
            }

            printf("\n\033[1;36m---DIFESA AVVERSARIO---\033[0m\n");

            short difesaAvversario = 0;

            // Tiro dedi dadi difesa dell'avversario
            for (int j = 0; j < pAbitante->dadiDifesa; j++)
            {
                int def = rand() % 6;
                dadiDifesaAbitante[j] = def;
                printf("%s\n", getDadoCombattimento(dadiDifesaAbitante[j]));

                difesaAvversario += (dadiDifesaAbitante[j] == SCUDO_NERO) ? 1 : 0;
            }

            if ((dannoGiocatore - difesaAvversario) > 0)
            {

                if (pAbitante->pVita < (dannoGiocatore - difesaAvversario))
                    pAbitante->pVita = 0;
                else
                {

                    pAbitante->pVita -= dannoGiocatore - difesaAvversario;
                }

                printf("\n\033[1;31m-%d \033[1;37mPunti Vita all'avversario\033[0m\n", (dannoGiocatore - difesaAvversario));
            }
            else
            {
                printf("\n\033[1;36mL'avversario si è difeso!\033[0m\n");
            }

            // TURNO AVVERSARIO
            printf("\n\033[1;31m---ATTACCO AVVERSARIO---\033[0m\n");

            short dannoAvversario = 0;

            // Tiro dei dadi attacco dell'avversario
            for (int i = 0; i < pAbitante->dadiAttacco; i++)
            {
                int atk = rand() % 6;
                dadiAttaccoAbitante[i] = atk;
                printf("%s\n", getDadoCombattimento(dadiAttaccoAbitante[i]));

                dannoAvversario += (dadiAttaccoAbitante[i] == TESCHIO1 || dadiAttaccoAbitante[i] == TESCHIO2 || dadiAttaccoAbitante[i] == TESCHIO3) ? 1 : 0;
            }

            printf("\n\033[1;36m---DIFESA GIOCATORE---\033[0m\n");

            short difesaGiocatore = 0;

            // Tiro dedi dadi difesa del giocatore
            for (int j = 0; j < pGiocatore->dadiDifesa; j++)
            {
                int def = rand() % 6;
                dadiDifesaGiocatore[j] = def;
                printf("%s\n", getDadoCombattimento(dadiDifesaGiocatore[j]));

                difesaGiocatore += (dadiDifesaGiocatore[j] == SCUDO_BIANCO1 || dadiDifesaGiocatore[j] == SCUDO_BIANCO2) ? 1 : 0;
            }

            if ((dannoAvversario - difesaGiocatore) > 0)
            {
                if (pGiocatore->pVita < (dannoAvversario - difesaGiocatore))
                    pGiocatore->pVita = 0;
                else
                    pGiocatore->pVita -= dannoAvversario - difesaGiocatore;
                printf("\n\033[1;31m-%d \033[1;37mPunti Vita al giocatore\033[0m\n", (dannoAvversario - difesaGiocatore));
            }
            else
            {
                printf("\n\033[1;36mTi sei difeso!\033[0m\n");
            }
        }
        else
        {
            printf("\nInizia \033[1;37m%s\033[0m\n", pAbitante->nome);
            printf("\n\033[1;31m---ATTACCO AVVERSARIO---\033[0m\n");

            short dannoAvversario = 0;

            // Tiro dei dadi attacco dell'abitante
            for (int i = 0; i < pAbitante->dadiAttacco; i++)
            {
                int atk = rand() % 6;
                dadiAttaccoAbitante[i] = atk;
                printf("%s\n", getDadoCombattimento(dadiAttaccoAbitante[i]));

                dannoAvversario += (dadiAttaccoAbitante[i] == TESCHIO1 || dadiAttaccoAbitante[i] == TESCHIO2 || dadiAttaccoAbitante[i] == TESCHIO3) ? 1 : 0;
            }

            printf("\n\033[1;36m---DIFESA GIOCATORE---\033[0m\n");

            short difesaGiocatore = 0;

            // Tiro dedi dadi difesa del giocatore
            for (int j = 0; j < pGiocatore->dadiDifesa; j++)
            {
                int def = rand() % 6;
                dadiDifesaGiocatore[j] = def;
                printf("%s\n", getDadoCombattimento(dadiDifesaGiocatore[j]));

                difesaGiocatore += (dadiDifesaGiocatore[j] == SCUDO_BIANCO1 || dadiDifesaGiocatore[j] == SCUDO_BIANCO2) ? 1 : 0;
            }

            if ((dannoAvversario - difesaGiocatore) > 0)
            {
                if (pGiocatore->pVita < (dannoAvversario - difesaGiocatore))
                    pGiocatore->pVita = 0;
                else
                    pGiocatore->pVita -= dannoAvversario - difesaGiocatore;
                printf("\n\033[1;31m-%d \033[1;37mPunti Vita al giocatore\033[0m\n", (dannoAvversario - difesaGiocatore));
            }
            else
            {
                printf("\n\033[1;36mSei riuscito a difenderti!\033[0m\n");
            }

            printf("\n\033[1;31m---ATTACCO GIOCATORE---\033[0m\n");

            short dannoGiocatore = 0;

            // Tiro dei dadi attacco del giocatore
            for (int i = 0; i < pGiocatore->dadiAttacco; i++)
            {
                int atk = rand() % 6;
                dadiAttaccoGiocatore[i] = atk;
                printf("%s\n", getDadoCombattimento(dadiAttaccoGiocatore[i]));

                dannoGiocatore += (dadiAttaccoGiocatore[i] == TESCHIO1 || dadiAttaccoGiocatore[i] == TESCHIO2 || dadiAttaccoGiocatore[i] == TESCHIO3) ? 1 : 0;
            }

            printf("\n\033[1;36m---DIFESA AVVERSARIO---\033[0m\n");

            short difesaAvversario = 0;

            // Tiro dedi dadi difesa dell'avversario
            for (int j = 0; j < pAbitante->dadiDifesa; j++)
            {
                int def = rand() % 6;
                dadiDifesaAbitante[j] = def;
                printf("%s\n", getDadoCombattimento(dadiDifesaAbitante[j]));

                difesaAvversario += (dadiDifesaAbitante[j] == SCUDO_NERO) ? 1 : 0;
            }

            if ((dannoGiocatore - difesaAvversario) > 0)
            {
                if (pAbitante->pVita < (dannoGiocatore - difesaAvversario))
                    pAbitante->pVita = 0;
                else
                    pAbitante->pVita -= dannoGiocatore - difesaAvversario;
                printf("\n\033[1;31m-%d \033[1;37mPunti Vita all'avversario\033[0m\n", (dannoGiocatore - difesaAvversario));
            }
            else
            {
                printf("\n\033[1;36mL'avversario si è difeso!\033[0m\n");
            }
        }

        printf("\nPV Giocatore: %d\nPV Abitante: %d\n", pGiocatore->pVita, pAbitante->pVita);

        printf("\n\033[1;37mInvia qualcosa per continuare il combattimento\033[0m\n");
        getchar();
        puliziaBuffer();
        system("clear");
    } while (pGiocatore->pVita > 0 && pAbitante->pVita > 0);

    if (pAbitante->pVita <= 0)
    {
        printf("\n\033[1;32mComplimenti! Hai sconfitto \033[1;31m%s\033[0m.\n", pAbitante->nome);
        sconfittaAbitante(pAbitante);
    }

    if (pGiocatore->pVita <= 0)
    {
        return 0;
    }
    return 1;
}

static char *getDadoCombattimento(DadoCombattimento dado)
{
    switch (dado)
    {
    case TESCHIO1:
        return "Teschio";

    case TESCHIO2:
        return "Teschio";

    case TESCHIO3:
        return "Teschio";

    case SCUDO_BIANCO1:
        return "Scudo Bianco";

    case SCUDO_BIANCO2:
        return "Scudo Bianco";

    case SCUDO_NERO:
        return "Scudo Nero";

    default:
        return "0";
    }
}

// Ritorna 1 se il player è scappato, altrimenti 0
static int scappa(Giocatore *pGiocatore, AbitanteDelleSegrete *pAbitante)
{
    int dado = rand() % 7;
    sleep(2);
    printf("\n\033[92mDado:\033[0m %d\n", dado);
    sleep(1);

    if (dado <= pGiocatore->mente)
    {
        if (pGiocatore->posizione->zonaPrecedente == NULL)
        {
            printf("\033[1;32mSei riuscito a scappare!\033[0;0m\n");
            return 1;
        }
        else
        {
            pGiocatore->posizione = pGiocatore->posizione->zonaPrecedente;
            printf("\033[1;32mSei riuscito a scappare!\033[0;37m Torni alla zona precedente: \033[1;37m%s\033[0m\n", getTipoZona(pGiocatore->posizione->tipoZona));
            return 1;
        }
    }
    else
    {
        int attaccoTot = (pAbitante->dadiAttacco) - (int)(floor(pGiocatore->dadiDifesa / 2));
        pGiocatore->pVita -= (attaccoTot > pGiocatore->pVita) ? pGiocatore->pVita : attaccoTot;
        printf("\n\033[0;37m\033[1;31m%s \033[1;37mti ha attaccato!\n\033[1;31m -%d Punti Vita\033[0m\n", pAbitante->nome, attaccoTot);
        return 0;
    }
}

// Ritorna 1 se l'abitante è stato sconfitto, altrimenti 0
static int potereSpeciale(Giocatore *pGiocatore)
{
    if (pGiocatore->potereSpeciale <= 0)
    {
        printf("\033[1;31mAttenzione!\033[1;37m Non hai abbastanza potere speciale.\033[1;0m\n");
        return 0;
    }

    pGiocatore->potereSpeciale -= 1;
    printf("\033[0;37mStavolta l'hai scampata...\033[1;32m Hai sconfitto l'abitante delle segrete!\n\033[1;34mPotere Speciale: \033[1;0m%d\n", pGiocatore->potereSpeciale);
    return 1;
}

static AbitanteDelleSegrete *creaAbitanteSegrete(ZonaSegrete *zona)
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
    // printf("\033[1;33mZona\033[1;0m: %s\n", getTipoZona(pAbitante->posizione->tipoZona));
}

static void sconfittaAbitante(AbitanteDelleSegrete *pAbitante)
{
    free(pAbitante);
}

static void resetFineGioco()
{
    isImpostato = 0;
    cancellaMappa();
    for (int i = 0; i < 4; i++)
    {
        memset(&giocatori[i], 0, sizeof(Giocatore));
    }
    nGiocatori = 0;
    nTurno = 0;
    fineGioco = 0;
}

static void puliziaBuffer()
{
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}