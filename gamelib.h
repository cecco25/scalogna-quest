
void impostaGioco();
void gioca();
void terminaGioco();

typedef enum ClasseGiocatore
{
    BARBARO,
    NANO,
    ELFO,
    MAGO
} ClasseGiocatore;

typedef enum TipoZona
{
    CORRIDIO,
    SCALA,
    SALA_BANCHETTO,
    MAGAZZINO,
    GIARDINO,
    POSTO_GUARDIA,
    PRIGIONE,
    CUCINA,
    ARMERIA,
    TEMPIO
} TipoZona;

typedef enum TipoTesoro
{
    NESSUN_TESORO,
    VELENO,
    GUARIGIONE,
    DOPPIA_GUARIGIONE
} TipoTesoro;

typedef enum TipoPorta
{
    NESSUNA_PORTA,
    PORTA_NORMALE,
    PORTA_DA_SCASSINARE
} TipoPorta;

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
    unsigned char potereSpeciale;
} Giocatore;

typedef struct AbitanteDelleSegrete
{
    char nome[15];
    unsigned char pVita;
    unsigned char dadiAttacco;
    unsigned char dadiDifesa;
} AbitanteDelleSegrete;