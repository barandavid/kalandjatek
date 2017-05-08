#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX 1000
#define DEBUG 2				//arra való, hogyha ez 1-esre állítjuk tudunk vele kiíratni kívánt elemeket


typedef struct list {			//lista, ahova beolvasom majd a dolgokat
	int sorszam;
	char helyszin[MAX];
	char leiras[MAX];
	char op1_leiras[MAX];
	char op2_leiras[MAX];
	char szuks_targy[MAX];
	char plusz_minusz_targy[MAX];
	int hova_op1;
	int hova_op2;
	char mi_tort_op1[MAX];
	char mi_tort_op2[MAX];
	struct lista *kov;
}lista;

typedef struct targy {				//tárgyak listája
	char nev[20];
	int drb;
	struct targyak *kov;
}targyak;


int ekezet(char c) {
	//egy karakter ellenorzese, hogy ekezetes karakter-e
	//kis es nagybetus ekezetek
	char s[18] = { 'á', 'Á', 'é', 'É', 'í', 'Í', 'ó', 'Ó', 'ü', 'Ü', 'ű', 'Ű', 'ö', 'Ö', 'ő', 'Ő', 'ú', 'Ú' };
	int i, van = 0;

	for (i = 0; i < 18 && van == 0; i++) {
		if (c == s[i]) van = 1;
	}
	if (van) return 1;
	return 0;
}
int magyarbetu(char c) {
	//egy karakter ellenorzese hogy a magyar ABC-ben benn van-e
	//kis es nagybetuk egyarant, ekezetekkel
	if ((c >= 'a'&&c <= 'z') || (c >= 'A' && c <= 'Z') || ekezet(c)) return 1;
	return 0;
}




void szetszed(char s[], struct list *akt);
int ellenoriz(char s[]);
void kiir(lista * elso, int szam);
int opvalasztas();
int jatek(lista *elso);
int sorszamell(struct lista *elso);
int targyell(targyak *elso, char  string[]);
int targyhozzaad(targyak  *elso, char s[]);
void debug_targylista(targyak *elso);

int getline(char s[], int n) {
	int c, i;
	for (i = 0; i<n && (c = getchar()) != EOF&&c != '\n'; i++) s[i] = c;
	s[i] = '\0';
	while (c != EOF&&c != '\n') c = getchar();
	return i;
}

int main() {
	FILE *fp;				//filepointer
	char st[MAX];			//string, ahova lementem a dolgokat a csv-ből
	lista *elso = NULL, *akt = NULL, *temp = NULL;


	system("chcp 1250");			//ékezetes betűket is ki tudjunk írni
	system("cls");					//letörli a felületet

	fp = fopen("jatek.csv", "r");			//olvasásra nyitjuk meg a CSV-t

	if (fp == NULL) {
		printf("Hiba a fajl megnyitasakor! \n");
		return 0;
	}


	while (!feof(fp)) {
		fgets(st, MAX, fp);				//soronként beolvas
		if (ellenoriz(st) == 0) {
			printf("Rossz volt a csv formátuma \n");
			return 0;
		}
	}

	rewind(fp);						//fájlmutatót az elejére állítja

	while (!feof(fp)) {
		fgets(st, MAX, fp);			//beolvasom soronként 

		akt = calloc(1, sizeof(lista));			//memóriafoglalás
		if (akt == NULL) {
			printf("Nem siekrult a memoriafoglalas!\n");
			return 0;
		}
		if (elso == NULL) elso = akt;
		else temp->kov = akt;
		akt->kov = NULL;
		szetszed(st, akt);
		temp = akt;
	}

	//ide kell majd a sorszamell
	if (!sorszamell(elso)) {
		printf("Hibas sorszam!\n");
		return 0;
	}
	if (DEBUG == 1) {
		akt = elso;							//lista bejárás
		while (akt != NULL) {
			printf("sorszam: %d\n", akt->sorszam);
			printf("helyszin: %s\n", akt->helyszin);
			printf("leiras: %s\n", akt->leiras);
			printf("op1 leiras: %s\n", akt->op1_leiras);
			printf("op2 leiras: %s\n", akt->op2_leiras);
			printf("szukseges targy 1: %s\n", akt->szuks_targy);
			printf("plusz/minusz targy op1: %s\n", akt->plusz_minusz_targy);
			printf("hova op1: %d\n", akt->hova_op1);
			printf("hova op2: %d\n", akt->hova_op2);
			printf("mi tortenik op1: %s\n", akt->mi_tort_op1);
			printf("mi tortenik op2: %s\n", akt->mi_tort_op2);
			printf("----------------------------uj sor---------------------\n");
			akt = akt->kov;
		}
	}
	//kiir(elso, 4);
	if (!jatek(elso)) return 0;
}

void szetszed(char s[], struct list *akt) {		//szétszedi a stringbe fájlból beolvasott dolgokat láncolt listába
	char string[MAX];
	int i = 0, j, k = 0;
	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {	//ciklus, ami ;-ig vagy sorvégig megy 
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';/*									//lezárja a stringet ha eléri a ;-t vagy sorvéget
						 printf("%s\n", string);*/

		switch (k) {							//van egy változó és egyes esetekben ,,case,,-ekben mit kell csinálni
		case 0:
			akt->sorszam = atoi(string);
		case 1:
			strcpy(akt->helyszin, string);
		case 2:
			strcpy(akt->leiras, string);
		case 3:
			strcpy(akt->op1_leiras, string);
		case 4:
			strcpy(akt->op2_leiras, string);
		case 5:
			strcpy(akt->szuks_targy, string);
		case 6:
			strcpy(akt->plusz_minusz_targy, string);
		case 7:
			akt->hova_op1 = atoi(string);
		case 8:
			akt->hova_op2 = atoi(string);
		case 9:
			strcpy(akt->mi_tort_op1, string);
		case 10:
			strcpy(akt->mi_tort_op2, string);
		}
		k++;
		i++;			//átléptetjük a ;-n, azért kell ide az i++
	} while (s[i] != '\0' && s[i] != '\n');
	/*printf("----------------------------uj sor---------------------\n");*/
}

int ellenoriz(char s[]) { //leellenőrzi, hogy az adott helyen megfelelő karakter/szám van-e
	char string[MAX];
	int i = 0, j, k = 0, c;

	/*printf("%s\n", s);*/

	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {	//ciklus, ami ;-ig vagy sorvégig megy 
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';								//lezárja a stringet ha eléri a ;-t vagy sorvéget
		if (DEBUG == 1) printf("%s\n", string);

		if (k == 0 || k == 7 || k == 8) {  //sorszám mező, tovább ugrások mezők helyességének ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c] <'0' || string[c]>'9') && string[c] != '-') return 0;
			}
		}

		else if (k == 5) { //szükséges tárgyak nevének ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
		}
		else if (k == 6) { // +/- tárgyak mezők ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
				//FOYLTATNI )targyhozzaadas: szam - nev - +-
				if ((string[c]< '0' || string[c]> '9') && string[c] != '-' && string[c] != '+' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
		}
		// A többi fajta mezőt nem szükséges ellenőrizni
		k++;
		i++;
	} while (s[i] != '\0' && s[i] != '\n');

	return 1;
}

int sorszamell(struct lista *elso) {		//átrakja a sorszámokat egy ,,sorszamok,, listába, ahol majd megnézi, hogy az op1 és op2 benne vannak-e
	lista *akt1 = NULL, *akt2 = NULL;

	akt1 = elso;	//op1 es op2
	while (akt1 != NULL) {
		if (akt1->hova_op1 != 0 && akt1->hova_op2 != 0) {
			akt2 = elso;
			while (akt2 != NULL && (akt2->sorszam != akt1->hova_op1)) akt2 = akt2->kov; //amíg nincs vege a listanak ES nem talaltunk ilyen sorszamot
			if (akt2 == NULL) {
				if (DEBUG == 1) printf("op1 %d\n", akt1->sorszam); //csak azért kell, ha hibát keresünk
				return 0; //az op1_hova nem volt talalhato a palya sorszamok kozott
			}

			akt2 = elso;
			while (akt2 != NULL && (akt2->sorszam != akt1->hova_op2)) akt2 = akt2->kov; //ameg nincs vege a listanak ES nem talaltunk ilyen sorszamot
			if (akt2 == NULL) {
				if (DEBUG == 1) printf("op2 %d\n", akt1->sorszam); //csak azért kell, ha hibát keresünk
				return 0; //az op2_hova nem volt talalhato a palya sorszamok kozott
			}
		}
		akt1 = akt1->kov;
	}
	return 1;
}


void kiir(lista *elso, int szam) {				//függvény, amely kiírja a megadott lista elemet
	lista *akt;

	akt = elso;
	while ((akt != NULL) && (akt->sorszam != szam)) {
		akt = akt->kov;
	}
	if (akt->hova_op1 == 0) {
		printf("Vége a játéknak!");
	}
	else {
		system("cls");
		printf("\t\t\t%s\n\n", akt->helyszin);								//helyszín kiírása
		printf("%s\n\n", akt->leiras);										//leírás kiírása
		printf("1. %s\n2. %s\n", akt->op1_leiras, akt->op2_leiras);			//választási lehetőségek kiírása
	}
}

int opvalasztas() {						//opció választás, ahol az '1'  és a '2'  az elfogadott
	char c;
	do {
		c = _getche();	//bekérjük a konzolablakból a számok, amelyiket választjuk
		putchar('\n');
		if (c == '1') return 1;
		else if (c == '2') return 2;
		else if (c == 'x') return 0;			//x-re kilép a program
	} while (1);
}

int targyell(targyak *elso, char  string[]) {		//leellenőrzi, hogy benne van-e a tárgy, ha nincs akkor hozzáadja
	int i, j = 0;
	char szam[50];
	char targy[1000];
	int plusz = 0;		//ha plusz==1, akkor novelunk, ha 0 akkor kivonunk
	targyak *akt;
	
	for (i = 0; string[i] != '\0'; i++) {						//szam stringbe teszi a darabszámot
		if (string[i] >= '0' && string[i] <= '9') {
			szam[j] = string[i];
			j++;
		}
		
	}
	szam[j] = '\0';
	i = 0;
	j = 0;
	for (i = 0; string[i] != '\0'; i++) {						//targy stringbe teszi a targyat
		if (string[i] < '0' || string[i] > '9') {
			targy[j] = string[i];
			j++;
		}
		
	}
	targy[j] = '\0';

	akt = elso;
	while (akt != NULL) {
		if (!strcmp(akt->nev, targy) && akt->drb>=atoi(szam)) {						//ha már van ilyen tárgy, akkor a darabszámot megnöveli egyel
			return 1;	//van iylen targy (es van annyi db)
		}
		akt = akt->kov;
	}
	return 0;	//nincs ilyen targy, vagy nincs annyi db
}

int targyhozzaad(targyak  *elso, char s[]) {
	targyak *akt, *temp;
	akt = elso;
	char szam[50];
	char targy[1000];
	int plusz;
	int i, j;
	int volt;

	if (s[0] == '\0') return 1;

	i = 0;
	do{
		plusz = j = 0;
		//pl.: 3arany-2kard+
		// string (targy) darabokra bontasa
		for (; s[i] >= '0' && s[i] <= '9' && s[i] != '\0'; i++) {
			szam[j] = s[i];
			j++;
		}

		szam[j] = '\0';

		//i = 0;
		j = 0;
		for (; s[i] != '\0' && s[i] != '+' &&s[i] != '-'; i++) {						//targy stringbe teszi a targyat
			targy[j] = s[i];
			j++;
		}
		targy[j] = '\0';

		//hozzaadjunk vagy kivonjunk?

		if (s[i] == '+') plusz++;


		akt = elso;
		volt = 0;
		while (akt != NULL && !volt) {
			if (!strcmp(akt->nev, targy)) {						//ha már van ilyen tárgy, akkor a darabszámot megnöveli egyel
				if (plusz) {
					akt->drb += atoi(szam);
					volt++;
				}
				else {				//kivonas lesz
					if ((akt->drb - atoi(szam)) < 0) return 0;
					else {
						akt->drb -= atoi(szam);
						volt++;
					}
				}
			}
			if (!volt) akt = akt->kov;
		}

		if (akt == NULL) {		//akkor lesz NULL, ha nem talaltunk ilyen targyat a listaban, VAGY meg nincs egy targyunk sem
			akt = elso;
			if (akt == NULL) {	//NINCS MEG TARGYUNK
				akt = calloc(1, sizeof(targyak));
				elso = akt;
				akt->kov = NULL;
				strcpy(akt->nev, targy);
				akt->drb = atoi(szam);
			}
			else {

				while (akt->kov != NULL) akt = akt->kov;

				temp = calloc(1, sizeof(targyak));
				//ellenorzes, hogy sikerult-e lefoglalni
				strcpy(temp->nev, targy);
				temp->drb = atoi(szam);

				akt->kov = temp;
				temp->kov = NULL;
			}
		}

		i++;	//a kovetkezo resz kezdese (vagy a \0)

		if (DEBUG == 2) {
			printf("TARGY HOZZAADVA: %s %s db\n", targy, szam);
		}

	}while (s[i] != '\0');

	return 1;
	
}

int jatek(lista *elso) {				//játék függvény
	targyak *mut = NULL;
	lista *akt;
	int aktsorszam = 1;
	int op;

	mut = calloc(1, sizeof(targyak));
	mut->kov = NULL;
	do {
		system("cls");				//felület letörlése
		akt = elso;
		while (akt->sorszam != aktsorszam) akt = akt->kov; //ide lehet kell meg a NULL ellenorzes

		kiir(elso, aktsorszam);
		op = opvalasztas();		//1   vagy  2 a visszateres
		if (op == 1) {
			if (akt->szuks_targy[0] == '\0')		// ha nincs a tovabblepesnek feltetele
			{
				if (DEBUG == 2) {
					printf("nincs a tovabblepesnek feltetele\n");
					system("pause");
				}
				if(targyhozzaad(mut,akt->plusz_minusz_targy)) aktsorszam = akt->hova_op1;		//ha 1-est választjuk oda ugrik ahova az egyes opció után kell
				else {
					//gaz volt a targy hozzaadasanal
					if (DEBUG == 2) {
						printf("gaz volt a targy hozzaadasanal\n");
						system("pause");
					}
					aktsorszam = akt->hova_op1;
				}
			}
			else {		//ha van a tovabblepesnek feltetele
				if (targyell(mut, akt->szuks_targy)) {				//ha megvan a szukseges targy es a kello darabszam
					
					if (!targyhozzaad(mut, akt->plusz_minusz_targy)) {
						if (DEBUG == 2) {
							printf("para volt a targyhozzaadasnal\n");
							system("pause");
						}

					}
					if (DEBUG == 2) {
						printf("van a tovabblepesnek feltetele, es teljesult\n");
						debug_targylista(mut);
						system("pause");
					}
					aktsorszam = akt->hova_op1;		//ha 1-est választjuk oda ugrik ahova az egyes opció után kell
					
				}
				else {			// ha nincs meg a kello targy vagy nincs annyi db belole
					if (DEBUG == 2) {
						printf("van a tovabblepesnek feltetele, de nem teljesult\n");
						debug_targylista(mut);
						system("pause");
					}
					aktsorszam = akt->hova_op2;
					if (DEBUG == 2) {
						printf("Nincs ilyen targy, vagy nincs ennyi db.");
						system("pause");
					}
				}
			}
		}
		else if (op == 2) aktsorszam = akt->hova_op2;						//ha 2-esz választjuk ida ugrik ahova a kettes opció után kell
		else return -1; //KILEPES





	} while (aktsorszam != 0);				//azért 0, mert akkor lép ki a játékból


}

void debug_targylista(targyak *elso) {
	targyak *akt;

	akt = elso;
	printf("TARGYLISTA: ");
	while (akt != NULL) {
		printf("%s %d db, ", akt->nev, akt->drb);
		akt = akt->kov;
	}
}

/*
kell még:
tárgyellenőrzés
tárgy hozzáadás
aktuális pozíció lementése egy txt-be, amit később onnan lehet tovább folytatni
op1 és op2 szövegének kiíárása, attól függ mit választott, ahhoz írja ki a szöveget
kilépés: 1. vissza mész az elejére 2.kilépés.. és kilép az egész programból
CSV rendes átírása, hogy értelmes legyen


volt- ha egy adott pályán már voltál már nem adhat tárgyat   ,majd kiírja, hogy itt már jártál, nem kaphatsz megint ilyen tárgyat
leellenőrizni a tárgy formátumát
memóriafoglalás sikerült-e
*/