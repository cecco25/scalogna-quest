
void impostaGioco();
void gioca();

enum ClasseGiocatore
{
    BARBARO,
    NANO,
    ELFO,
    MAGO
};

enum TipoZona
{
    CORRIDIO,
    SCALA,
    SALA_BANCHETTO,
    MAGAZZINO,
    POSTO_GUARDIA,
    PRIGIONE,
    CUCINA,
    ARMERIA,
    TEMPIO
};

enum TipoTesoro
{
    NESSUN_TESORO,
    VELENO,
    GUARIGIONE,
    DOPPIA_GUARIGIONE
};

enum TipoPorta
{
    NESSUNA_PORTA,
    PORTA_NORMALE,
    PORTA_DA_SCASSINARE
};

typedef struct ZonaSegrete
{
    struct ZonaSegrete *zonaSuccessiva;
    struct ZonaSegrete *zonaPrecedente;
    enum TipoZona tipoZona;
    enum TipoTesoro tipoTesoro;
    enum TipoPorta tipoPorta;

} ZonaSegrete;

typedef struct Giocatore
{
    char nomeGiocatore[40];
    enum ClasseGiocatore classeGiocatore;
    struct ZonaSegrete *posizione;
    unsigned char pVita;
    unsigned char dadiAttacco;
    unsigned char dadiDifesa;
    unsigned char mente;
    unsigned char valoreSpeciale;

} Giocatore;