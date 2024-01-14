
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LISTA {
    char sana[30];
    int luokka;
    struct LISTA *pSeuraava;
} LISTA;

char* kysyNimet(char *cNimi);
LISTA *lueTiedosto(char *cNimi);
int *analysoi(LISTA *pAlku, int aTaulukko[]);
void kirjoitaTiedosto(int aTaulukko[], char *cNimi);
int valikko();
void vapauta(LISTA *pAlku);

int main(void) {
    int iValinta, iTarkastus = 0;
    char cNimi[30];
    int aTaulukko[30] = {0};
    LISTA *pAlku = NULL;

    do {
        iValinta = valikko();

        if (iValinta == 1) {
            kysyNimet(cNimi);
            vapauta(pAlku);
            pAlku = lueTiedosto(cNimi);
        } else if (iValinta == 2) {
            if (pAlku == NULL) {
                printf("Ei analysoitavaa, lue tiedosto ennen analyysiä.\n\n");
                continue;
            }
            analysoi(pAlku,aTaulukko);
            iTarkastus++;
        } else if (iValinta == 3) {
            if (iTarkastus == 0) {
                printf("Ei kirjoitettavia tietoja, analysoi tiedot ennen tallennusta.\n\n");
                continue;
            }
            kysyNimet(cNimi);
            kirjoitaTiedosto(aTaulukko, cNimi);
        } else if (iValinta == 0) {
            printf("Lopetetaan.\n\n");
            printf("Kiitos ohjelman käytöstä.\n");
        } else {
            printf("Tuntematon valinta, yritä uudestaan.\n\n");
        }
    } while (iValinta != 0);
    vapauta(pAlku);
    return(0);
}

char* kysyNimet(char *cNimi) {
    printf("Anna tiedoston nimi: ");
    scanf("%s", cNimi);
    return(cNimi);
}

LISTA *lueTiedosto(char *cNimi) {
    char rivi[250];
    char *i1, *i2;
    LISTA *pUusi, *pLoppu = NULL, *pAlku = NULL;
    FILE *tiedosto;

    if ((tiedosto = fopen(cNimi, "r")) == NULL) {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }

    if(fgets(rivi, 250, tiedosto) == NULL) {
        perror("Tiedoston lukeminen epäonnistui");
        exit(0);
    }
    while (fgets(rivi, 250, tiedosto) != NULL) {
        if ((pUusi = (LISTA*)malloc(sizeof(LISTA))) == NULL) {
            perror("Muistin varaus epäonnistui.");
            exit(0);
        }

        if ((i1 = strtok(rivi, ";")) == NULL) {
            perror("Merkkijonon pilkkominen epäonnistui");
            exit(0);
        }
        if ((i2 = strtok(NULL, ";")) == NULL) {
            perror("Merkkijonon pilkkominen epäonnistui");
            exit(0);
        }

        strcpy(pUusi->sana, i1);
        pUusi->luokka = atoi(i2);
        pUusi->pSeuraava = NULL;

        if (pAlku == NULL) {
            pAlku = pUusi;
            pLoppu = pUusi;
        } else {
            pLoppu->pSeuraava = pUusi;
            pLoppu = pUusi;
        }
    }
    fclose(tiedosto);
    printf("Tiedosto '%s' luettu.\n\n",cNimi);
    return(pAlku);
}

int *analysoi(LISTA *pAlku, int aTaulukko[]) {
    LISTA *ptr = pAlku;
    char *cEka, *cVika;
    int i = 1, k = 0;

    for (int k = 0; k < 30; k++) {
        aTaulukko[k] = 0;
    }

    if (pAlku != NULL) {
        cEka = pAlku->sana;
    }

    while (ptr != NULL) {
        i = ptr->luokka;
        aTaulukko[i]++;
        k++;
        cVika = ptr->sana;
        ptr = ptr->pSeuraava;
    }

    printf("Analysoitu %d data-alkiota, sanasta '%s' sanaan '%s'.\n\n",k,cEka,cVika);
    return(aTaulukko);
}

void kirjoitaTiedosto(int aTaulukko[], char *cNimi) {
    char rivi[50];
    int i, j;
    FILE *tiedosto;

    if ((tiedosto = fopen(cNimi, "w")) == NULL) {
        perror("Tiedoston avaaminen epäonnistui, lopetetaan");
        exit(0);
    }

    fprintf(tiedosto,"Sanaluokka;Lkm\n");
    for (i = 1; i<11; i++) {
        j = aTaulukko[i];
        fprintf(tiedosto, "Luokka %d;%d\n",i,j);
    }
    printf("Tiedosto '%s' kirjoitettu.\n\n",cNimi);
    fclose(tiedosto);
}

int valikko() {
    int iValinta;
    printf("Valitse haluamasi toiminto:\n");
    printf("1) Lue tiedosto\n");
    printf("2) Analysoi tiedot\n");
    printf("3) Kirjoita tulokset\n");
    printf("0) Lopeta\n");
    printf("Anna valintasi: ");
    scanf("%d", &iValinta);
    return(iValinta);
}

void vapauta(LISTA *pAlku) {
    LISTA *ptr = pAlku;
    while (ptr != NULL) {
        pAlku = ptr->pSeuraava;
        free(ptr);
        ptr = pAlku;
    }
}
