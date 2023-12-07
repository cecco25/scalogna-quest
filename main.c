#include "gamelib.h"
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int c = 0; // Variabile per pulizia buffer
    unsigned short scelta = 0;

    system("clear"); // Pulizia schermo
    printf("\n");
    printf("\033[1;33m░██████╗░█████╗░░█████╗░██╗░░░░░░█████╗░░██████╗░███╗░░██╗░█████╗░░░░░░░░██████╗░██╗░░░██╗███████╗░██████╗████████╗\n"
           "██╔════╝██╔══██╗██╔══██╗██║░░░░░██╔══██╗██╔════╝░████╗░██║██╔══██╗░░░░░░██╔═══██╗██║░░░██║██╔════╝██╔════╝╚══██╔══╝\n"
           "╚█████╗░██║░░╚═╝███████║██║░░░░░██║░░██║██║░░██╗░██╔██╗██║███████║█████╗██║██╗██║██║░░░██║█████╗░░╚█████╗░░░░██║░░░\n"
           "░╚═══██╗██║░░██╗██╔══██║██║░░░░░██║░░██║██║░░╚██╗██║╚████║██╔══██║╚════╝╚██████╔╝██║░░░██║██╔══╝░░░╚═══██╗░░░██║░░░\n"
           "██████╔╝╚█████╔╝██║░░██║███████╗╚█████╔╝╚██████╔╝██║░╚███║██║░░██║░░░░░░░╚═██╔═╝░╚██████╔╝███████╗██████╔╝░░░██║░░░\n"
           "╚═════╝░░╚════╝░╚═╝░░╚═╝╚══════╝░╚════╝░░╚═════╝░╚═╝░░╚══╝╚═╝░░╚═╝░░░░░░░░░╚═╝░░░░╚═════╝░╚══════╝╚═════╝░░░░╚═╝░░░\033[1;0m\n");
    printf("\nBenvenuto in \033[1;33mScalogna-Quest\033[0m! Selezionare \"Imposta gioco\" per impostare il gioco!");
    do
    {
        printf("\n");
        printf("> \033[1;36m1\033[0m: Imposta Gioco.\n");
        printf("> \033[1;35m2\033[0m: Gioca.\n");
        printf("> \033[1;91m3\033[0m: Termina Gioco.\n\n");
        printf("\033[92mScelta:\033[0m ");

        scanf("%hu", &scelta);
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        switch (scelta)
        {
        case 1:
            impostaGioco();
            break;

        case 2:

            break;

        case 3:

            break;

        default:
            printf("\033[31mAttenzione!\033[0m Opzione non valida, per favore inserisci \033[31mun numero da 1 a 3\033[0m.\n");
            break;
        }

    } while (scelta != 3);

    return 0;
}
