#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX 1000
#define DEBUG 0				//arra való, hogyha ez 1-esre állítjuk tudunk vele kiíratni kívánt elemeket


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

typedef struct sorszamok {
	int szamok;
	struct sorszamok *kov;
}sorszamok;


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

	fp = fopen("jatek.csv", "r");

	if (fp == NULL) {
		printf("Hiba a fajl megnyitasakor! \n");
		return 0;
	}

	/*ellenőrzés ide kell majd */
	while (!feof(fp)) {
		fgets(st, MAX, fp);
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
	if (DEBUG) {
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
	int i=0,j,k=0;
	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {	//ciklus, ami ;-ig vagy sorvégig megy 
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';/*									//lezárja a stringet ha eléri a ;-t vagy sorvéget
		printf("%s\n", string);*/
		
		switch (k){							//van egy változó és egyes esetekben ,,case,,-ekben mit kell csinálni
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
		if (DEBUG) printf("%s\n", string);

		if (k == 0 || k==7 || k==8) {  //sorszám mező, tovább ugrások mezők helyességének ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c] <'0' || string[c]>'9' ) && string[c] != '-') return 0;
			}
		}
		
		else if (k == 5) { //szükséges tárgyak nevének ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
		}
		else if (k == 6) { // +/- tárgyak mezők ellenőrzése
			for (c = 0; string[c] != '\0'; c++) {
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
			while (akt2 != NULL && (akt2->sorszam != akt1->hova_op1)) akt2 = akt2->kov; //ameg nincs vege a listanak ES nem talaltunk ilyen sorszamot
			if (akt2 == NULL) {
				printf("op1 %d\n", akt1->sorszam);
				return 0; //az op1_hova nem volt talalhato a palya sorszamok kozott
			}

			akt2 = elso;
			while (akt2 != NULL && (akt2->sorszam != akt1->hova_op2)) akt2 = akt2->kov; //ameg nincs vege a listanak ES nem talaltunk ilyen sorszamot
			if (akt2 == NULL) {
				printf("op2 %d\n", akt1->sorszam);
				return 0; //az op2_hova nem volt talalhato a palya sorszamok kozott
			}
		}
		akt1 = akt1->kov;
	}
	return 1;
}


void kiir(lista *elso,int szam) {				//függvény, amely kiírja a megadott lista elemet
	lista *akt;

	akt = elso;
	while ((akt != NULL) && (akt->sorszam != szam)) {
		akt = akt->kov;
	}
	system("cls");
	printf("\t\t\t%s\n\n", akt->helyszin);								//helyszín kiírása
	printf("%s\n\n", akt->leiras);										//leírás kiírása
	printf("1. %s\n2. %s\n", akt->op1_leiras, akt->op2_leiras);			//választási lehetőségek kiírása

}

int opvalasztas(){						//opció választás, ahol az '1'  és a '2'  az elfogadott
	char c;
	do {
		c=_getche();					//bekérjük a konzolablakból a számok, amelyiket választjuk
		if (c == '1') return 1;
		else if (c == '2') return 2;
		else if (c == 'x') return 0;			//x-re kilép a program
	} while (1);
}

int targyell(targyak *elso,char  string[]) {		//leellenőrzi, hogy benne van-e a tárgy, ha nincs akkor hozzáadja
	targyak *akt;
	akt = elso;
	while (akt != NULL) {

		akt = akt->kov;

	}

}

int targyhozzaad() {


}

int jatek(lista *elso) {				//játék függvény
	targyak *mut = NULL;
	lista *akt;
	int aktsorszam=1;
	int op;

	do {
		system("cls");				//felület letörlése
		akt = elso;
		while (akt->sorszam != aktsorszam) akt = akt->kov; //ide lehet kell meg a NULL ellenorzes

		kiir(elso, aktsorszam);
		op = opvalasztas();
		if (op == 1) aktsorszam = akt->hova_op1;		//ha 1-est választjuk oda ugrik ahova az egyes opció után kell
		else if(op == 2) aktsorszam = akt->hova_op2;						//ha 2-esz választjuk ida ugrik ahova a kettes opció után kell
		else return -1; //KILEPES





	} while (aktsorszam != 0);				//azért 0, mert akkor lép ki a játékból


}