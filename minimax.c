#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct NodArbore
{
    int v;
    char joc[3][3];
    struct NodLista *headCopii;
    struct NodArbore *parinte;
} NodArbore; // arbore multicai

typedef struct NodLista
{
    struct NodArbore *na;
    struct NodLista *next;
} NodLista; // lista de noduri de arbore

typedef struct
{
    NodLista *head;
    NodLista *tail;
} Coada;

void adaugaInCoada(Coada *c, NodArbore *n)
{
    NodLista *nodNou = malloc(sizeof(NodArbore));
    nodNou->na = n;
    nodNou->next = NULL;

    if (c->head == NULL) // coada vida
    {
        c->head = nodNou;
        c->tail = nodNou;
        return;
    }

    // coada nu este vida => il adaug la final
    c->tail->next = nodNou;
    c->tail = nodNou;
}

NodArbore *stergeDinCoada(Coada *c)
{
    if (c->head == NULL) // coada vida
        return NULL;

    // extrag nodul din fata
    NodLista *curent = c->head;
    c->head = curent->next;

    // intorc nodul de arbore
    NodArbore *n = curent->na;

    free(curent);

    return n;
}

NodArbore *creazaNodArbore(int val, char joc[3][3])
{
    NodArbore *n = malloc(sizeof(NodArbore));
    n->v = val;

    if (joc != NULL)
    {
        int i, j;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                n->joc[i][j] = joc[i][j];
    }

    n->headCopii = NULL;
    n->parinte = NULL;
    return n;
}

void adaugaCopil(NodArbore *parinte, NodArbore *copil)
{
    // pun legatura de la copil la parinte
    copil->parinte = parinte;

    // creez un nou nod in lista de copii a nodului parinte in care pun nodul copil
    NodLista *nodNou = malloc(sizeof(NodLista));
    nodNou->na = copil;
    nodNou->next = NULL;

    if (parinte->headCopii == NULL)     // primul copil al nodului parinte
    {
        parinte->headCopii = nodNou;
        return;
    }

    // nodul parinte mai are alti copii, il adaug pe acesta pe ultima pozitie
    NodLista *curent = parinte->headCopii;
    while (curent->next != NULL)
        curent = curent->next;

    curent->next = nodNou;
}

void stergeArbore(NodArbore *nod)
{
    if (nod->headCopii == NULL)         // nod frunza
    {
        free(nod);
        return;
    }

    NodLista *curent = nod->headCopii;
    NodLista *sters;
    while (curent != NULL)
    {
        // sterg subarborele fiecarui copil al nodului curent
        stergeArbore(curent->na);
        sters = curent;
        curent = curent->next;
        // sterg nodul de lista
        free(sters);
    }

    free(nod);
}

void afiseazaArbore(NodArbore *nod, int nrTab, int afisezJocul, int afisezCaInt, FILE *g)
{
    // afisez informatiile din nodul curent
    if (afisezJocul == 1)
    {
        int i, j, k;
        for (i = 0; i < 3; i++)
        {
            for (k = 0; k < nrTab; k++)
            {
                //printf("\t");
                fprintf(g, "\t");
            }

            for (j = 0; j < 2; j++)
            {
                //printf("%c ", nod->joc[i][j]);
                fprintf(g, "%c ", nod->joc[i][j]);
            }

            //printf("%c\n", nod->joc[i][2]);
            fprintf(g, "%c\n", nod->joc[i][2]);
        }
        //printf("\n");
        fprintf(g, "\n");
    }
    else
    {
        int i;
        for (i = 0; i < nrTab; i++)
        {
            //printf("\t");
            fprintf(g, "\t");
        }

        if (afisezCaInt)
        {
            //printf("%d\n", nod->v);
            fprintf(g, "%d\n", nod->v);
        }
        else
        {
            //printf("%c\n", nod->v);
            fprintf(g, "%c\n", nod->v);
        }
    }

    if (nod->headCopii == NULL) // nod frunza
    {
        return;
    }

    // afisez recursiv informatile din fiecare copil
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        afiseazaArbore(curent->na, nrTab + 1, afisezJocul, afisezCaInt, g);
        curent = curent->next;
    }
}

int jocCastigat(char joc[3][3])
{
    int i;
    // verific liniile
    for (i = 0; i < 3; i++)
        if (joc[0][i] == joc[1][i] && joc[1][i] == joc[2][i] && joc[0][i] != '-')
            return 1;

    // verific coloanele
    for (i = 0; i < 3; i++)
        if (joc[i][0] == joc[i][1] && joc[i][1] == joc[i][2] && joc[i][0] != '-')
            return 1;

    // verific diagonala principala
    if (joc[0][0] == joc[1][1] && joc[1][1] == joc[2][2] && joc[0][0] != '-')
        return 1;

    // verific diagonala secundara
    if (joc[0][2] == joc[1][1] && joc[1][1] == joc[2][0] && joc[0][2] != '-')
        return 1;

    return 0;
}

void creazaArboreDinJoc(NodArbore *nod)
{
    char urmator;
    if (nod->v == 'X')
        urmator = 'O';
    else
        urmator = 'X';

    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (nod->joc[i][j] == '-')      // este pozitie libera
            {
                // adaug la configuratia aceasta caracterul jucatorului ce este la mutare 
                // (jucatorul din nodul curent) 
                nod->joc[i][j] = nod->v;

                // creez un nod copil pe baza acestei configuratii
                NodArbore *copil = creazaNodArbore(urmator, nod->joc);
                adaugaCopil(nod, copil);

                // daca nu este configuratie finala atunci creez recursiv subarborele nodului copil
                if (jocCastigat(copil->joc) == 0)
                    creazaArboreDinJoc(copil);

                // refac configuratia
                nod->joc[i][j] = '-';
            }
        }
    }
}

void aflaValoriSiSau(NodArbore *nod, char primulJucator)
{
    if (nod->headCopii == NULL)         // nod frunza
    {
        // determin valoare de adevar pe baza configuratiei jocului
        if (nod->v != primulJucator && jocCastigat(nod->joc))
            nod->v = 'T';
        else
            nod->v = 'F';

        return;
    }

    // apelez recurziv functia pentru ca sa aflu valoare de adevar a copiilor
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        aflaValoriSiSau(curent->na, primulJucator);
        curent = curent->next;
    }

    // nod intern. Determin valoare de adebar pe baza copiilor lui
    curent = nod->headCopii;
    while (curent)
    {
        if (nod->v != primulJucator) // nod SI
        {
            if (curent->na->v == 'F')
            {
                nod->v = 'F';
                return;
            }
        }
        else // nod SAU
        {
            if (curent->na->v == 'T')
            {
                nod->v = 'T';
                return;
            }
        }

        curent = curent->next;
    }

    if (nod->v != primulJucator) // nod SAU
    {
        nod->v = 'T';
    }
    else // nod SI
    {
        nod->v = 'F';
    }
}

void cerinta1_2(char *fin, char *fout, int cerinta)
{
    FILE *f = fopen(fin, "r");
    FILE *g = fopen(fout, "w");

    char joc[3][3], *p, line[10];

    char primulJucator;
    fgets(line, 10, f);
    primulJucator = line[0];

    int i;
    for (i = 0; i < 3; i++)
    {
        fgets(line, 10, f);
        p = strtok(line, " \n");
        joc[i][0] = p[0];

        p = strtok(NULL, " \n");
        joc[i][1] = p[0];

        p = strtok(NULL, " \n");
        joc[i][2] = p[0];
    }

    // printf("J:%c\n", primulJucator);
    // int j;
    // for (i = 0; i < 3; i++)
    // {
    //     for (j = 0; j < 3; j++)
    //     {
    //         printf("%c ", joc[i][j]);
    //     }
    //     printf("\n");
    // }

    NodArbore *rad = creazaNodArbore(primulJucator, joc);
    creazaArboreDinJoc(rad);

    if (cerinta == 1)
    {
        afiseazaArbore(rad, 0, 1, 0, g);
    }
    else
    {
        aflaValoriSiSau(rad, primulJucator);
        afiseazaArbore(rad, 0, 0, 0, g);
    }

    stergeArbore(rad);

    fclose(f);
    fclose(g);
}

int Minim(NodArbore *nod);

int Maxim(NodArbore *nod)
{
    if (nod->headCopii == NULL)         // nod frunza
        return nod->v;

    // initialize valoare nodului cu cel mai prost caz pentru maxim
    nod->v = INT_MIN;

    int aux;

    // actualizez valoarea ca fiind maximul dintre veloriile copiilor lui
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        // copii sunt de tip minim
        aux = Minim(curent->na);
        if (aux > nod->v)
            nod->v = aux;

        curent = curent->next;
    }

    return nod->v;
}

int Minim(NodArbore *nod)
{
    if (nod->headCopii == NULL)             // nod frunza
        return nod->v;

    // initialize valoare nodului cu cel mai prost caz pentru minim
    nod->v = INT_MAX;

    int aux;

    // actualizez valoarea ca fiind miminul dintre veloriile copiilor lui
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        // copii sunt de tip maxim
        aux = Maxim(curent->na);
        if (aux < nod->v)
            nod->v = aux;

        curent = curent->next;
    }

    return nod->v;
}

int MinimAB(NodArbore *nod, int a, int b);

int MaximAB(NodArbore *nod, int a, int b)
{
    if (nod->headCopii == NULL)             // nod frunza
        return nod->v;

    // initialize valoare nodului cu cel mai prost caz pentru maxim
    nod->v = INT_MIN;

    int aux;

    // actualizez valoarea ca fiind maximul dintre veloriile copiilor lui
    NodLista *prev;
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        // copii lui sunt de tip minim
        aux = MinimAB(curent->na, a, b);
        if (aux > nod->v)
            nod->v = aux;

        // daca valoare curenta a nodului este mai mare decat beta => beta-taiere
        if (nod->v >= b)
            break;

        // actualizez alfa
        if (nod->v > a)
            a = nod->v;

        prev = curent;
        curent = curent->next;
    }

    if (curent)         // a avut loc beta-taiere
    {
        // sterg subarborii copiilor neevaluati
        prev = curent;
        curent = curent->next;
        prev->next = NULL;

        while (curent)
        {
            stergeArbore(curent->na);
            prev = curent;
            curent = curent->next;
            free(prev);
        }
    }

    return nod->v;
}

int MinimAB(NodArbore *nod, int a, int b)
{
    if (nod->headCopii == NULL)         // nod frunza
        return nod->v;

    // initialize valoare nodului cu cel mai prost caz pentru maxim
    nod->v = INT_MAX;

    int aux;

    // actualizez valoarea ca fiind minimul dintre veloriile copiilor lui
    NodLista *prev;
    NodLista *curent = nod->headCopii;
    while (curent)
    {
        // copii lui sunt de tip maxim
        aux = MaximAB(curent->na, a, b);
        if (aux < nod->v)
            nod->v = aux;

        // daca valoare curenta a nodului este mai mica decat alfa => alfa-taiere
        if (nod->v <= a)
            break;

        // actualizez beta
        if (nod->v < b)
            b = nod->v;

        prev = curent;
        curent = curent->next;
    }

    if (curent)         // a avut loc alfa-taiere
    {
        // sterg subarborii copiilor neevaluati
        prev = curent;
        curent = curent->next;
        prev->next = NULL;

        while (curent)
        {
            stergeArbore(curent->na);
            prev = curent;
            curent = curent->next;
            free(prev);
        }
    }

    return nod->v;
}

void cerinta3_b(char *fin, char *fout, char cerinta)
{
    FILE *f = fopen(fin, "r");
    FILE *g = fopen(fout, "w");

    int nrLinii;
    char cuv[100];

    fscanf(f, "%s", cuv);

    nrLinii = atoi(cuv);

    NodArbore *rad = creazaNodArbore(0, NULL);

    Coada c;
    c.head = NULL;
    c.tail = NULL;

    NodArbore *curent = rad;

    int i, nr, j;
    while (curent != NULL)
    {
        fscanf(f, "%s", cuv);
        nr = atoi(cuv + 1);

        if (cuv[0] == '(')
        {
            // nodul curent are nr copii
            for (j = 0; j < nr; j++)
            {
                NodArbore *copil = creazaNodArbore(0, NULL);
                adaugaCopil(curent, copil);

                adaugaInCoada(&c, copil);
            }
        }
        else
        {
            // nodul curent este de tip frunza
            curent->v = nr;
        }

        curent = stergeDinCoada(&c);
    }

    if (cerinta == '3')
        Maxim(rad);
    else
        MaximAB(rad, INT_MIN, INT_MAX);

    afiseazaArbore(rad, 0, 0, 1, g);

    stergeArbore(rad);

    fclose(f);
    fclose(g);
}

int main(int argc, char **argv)
{
    if (strcmp(argv[1], "-c1") == 0)
    {
        cerinta1_2(argv[2], argv[3], 1);
    }
    else if (strcmp(argv[1], "-c2") == 0)
    {
        cerinta1_2(argv[2], argv[3], 2);
    }
    else if (strcmp(argv[1], "-c3") == 0)
    {
        cerinta3_b(argv[2], argv[3], '3');
    }
    else if (strcmp(argv[1], "-b") == 0)
    {
        cerinta3_b(argv[2], argv[3], 'b');
    }

    return 0;
}