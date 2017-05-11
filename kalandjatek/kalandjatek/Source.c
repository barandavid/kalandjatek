#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX 1000
#define DEBUG 0				//0: kikapcsolva, 1: listak, 2: targyak, 3: fajlok, 4: mentes es egyebek


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
	char hova_op1_voltmar[MAX];
	struct lista *kov;
	int volt;	//voltunk-e mar az adott palyan (ha igen, nem adunk targyat)
}lista;

typedef struct targy {				//tárgyak listája
	char nev[20];
	int drb;
	struct targyak *kov;
}targyak;

void szetszed(char s[], struct list *akt);
int ellenoriz(char s[]);
void kiir(lista * elso, int szam);
int opvalasztas();
int jatek(lista *elso, targyak *mut, int aktsorsz, char fajlnev[]);
int sorszamell(struct lista *elso);
int targyell(targyak *elso, char  string[]);
int targyhozzaad(targyak  *elso, char s[], int voltmar);
void debug_targylista(targyak *elso);
FILE* fajlmegnyit(char argv[], int* mentes);
int ekezet(char c);
int magyarbetu(char c);
int mentesnevell(char s[]);
void memfelszabadit(lista *pelso, targyak *telso);

int getline(char s[], int n) {
	int c, i;
	for (i = 0; i<n && (c = getchar()) != EOF&&c != '\n'; i++) s[i] = c;
	s[i] = '\0';
	while (c != EOF&&c != '\n') c = getchar();
	return i;
}

int main(int argc, char* argv[]) {
	targyak *mut = NULL;		//targyak lista elso eleme
	FILE *fp, *fp2;				//filepointer
	char st[MAX];			//string, ahova lementem a dolgokat a csv-ből
	lista *elso = NULL, *akt = NULL, *temp = NULL;
	int mentes = 0;
	int i;
	int szam, betu;
	char string[MAX];
	char s1[MAX];
	char s2[MAX];
	int c, k, j, l, m;
	int aktsorszam;
	int targydb;
	char fajlnev[MAX];


	system("chcp 1250");			//ékezetes betűket is ki tudjunk írni
	system("cls");					//letörli a felületet

									//kotelezo 1 parameter (CSV vagy SAVE fajl)
	if (argc != 2) {
		printf("Hianyzik a parancssori parameter!\n");
		return 0;
	}

	fp = fajlmegnyit(argv[1], &mentes);
	if (fp == NULL) {
		printf("Hiba a fajl megnyitasakor! \n");
		return 0;
	}

	//itt mar tudjuk, hogy sima jatekfajl vagy mentes az, amit kaptunk (*mentes valtozo)

	if (mentes == 0)	//sima jatekfajl betoltese
	{
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
				akt->volt = 0;	//kezdbeten nem voltunk meg ezen a palyan
			szetszed(st, akt);
			temp = akt;
		}
		fclose(fp);
		if (!sorszamell(elso)) {
			printf("Hibas sorszam!\n");
			return 0;
		}
		if (DEBUG == 1) {
			akt = elso;							//lista bejárás
			while (akt != NULL) {
				printf("DEBUG: sorszam: %d\n", akt->sorszam);
				printf("DEBUG: helyszin: %s\n", akt->helyszin);
				printf("DEBUG: leiras: %s\n", akt->leiras);
				printf("DEBUG: op1 leiras: %s\n", akt->op1_leiras);
				printf("DEBUG: op2 leiras: %s\n", akt->op2_leiras);
				printf("DEBUG: szukseges targy 1: %s\n", akt->szuks_targy);
				printf("DEBUG: plusz/minusz targy op1: %s\n", akt->plusz_minusz_targy);
				printf("DEBUG: hova op1: %d\n", akt->hova_op1);
				printf("DEBUG: hova op2: %d\n", akt->hova_op2);
				printf("DEBUG: mi tortenik op1: %s\n", akt->mi_tort_op1);
				printf("DEBUG: mi tortenik op2: %s\n", akt->mi_tort_op2);
				printf("DEBUG: mi tortenik op2: %s\n", akt->hova_op1_voltmar);
				printf("----------------------------uj sor---------------------\n");
				akt = akt->kov;
			}
		}
		if (!jatek(elso, mut, elso->sorszam, argv[1])) {
			memfelszabadit(elso, mut);
			return 0;
		}
	}
	else {		//mentes betoltese
		
		fgets(st, MAX,fp);
		if (mentesell(st) == 0) {
			printf("Hibas fajl\n");
			return 0;
		}

		//ellenoriz fv mintajara
		//mappaban a minta, hogy hogyan vannak felosztva az egyes reszek
		rewind(fp);
		fgets(st, MAX * 1, fp);

		i = 0;
		k = 0;
		szam = betu = 0;
		do {
			j = 0;
			for (; st[i] != ';' && st[i] != '\0'; i++) {
				string[j] = st[i];
				j++;
			}
			string[j] = '\0';

			if (k == 0) {		//CSV neve
				fp2 = fopen(string, "r");
				strcpy(fajlnev, string);
				while (!feof(fp2)) {
					fgets(s1, MAX, fp2);			//beolvasom soronként 

					akt = calloc(1, sizeof(lista));			//memóriafoglalás
					if (akt == NULL) {
						printf("Nem siekrult a memoriafoglalas!\n");
						return 0;
					}
					if (elso == NULL) elso = akt;
					else temp->kov = akt;
					akt->kov = NULL;
					akt->volt = 0;	//kezdbeten nem voltunk meg ezen a palyan
					szetszed(s1, akt);
					temp = akt;
				}
			}
			else if (k == 1) {		//aktsorszam
				aktsorszam = atoi(string);
			}
			else if (k == 2) {		//targyak
				if (mut == NULL) mut = calloc(1, sizeof(targyak));		//ha nincs meg targy, letrehozun kegy ures dummy targyat
				//pl.: 3arany1kulcs\0k
				l = m = 0;
				for (c = 0; string[c-1] != '\0'; c++) {
					if ((string[c] >= '0' && string[c] <= '9') || string[c] == '\0') {
						if (szam != 0 && betu != 0) {		//uj targy kezdodik
							s1[l] = '+';
							l++;							
						}
						szam++;
					}
					else if (string[c] >= 'a' && string[c] <= 'z') {
						betu++;
					}
					s1[l] = string[c];
					l++;
				}
				s1[l] = '\0';
				targyhozzaad(mut, s1, 0);
				if (DEBUG == 4) {
					printf("DEBUG: mentes targy hozzaadasa: %s", s1);
					system("pause");
				}
			}
			else if (k == 3) {
				l = m = 0;
				while (string[l-1] != '\0') {
					if (string[l] >= '0' && string[l] <= '9')
					{
						s2[m] = string[l];
						m++;
					}
					else {
						s2[m] = '\0';
						akt = elso;
						while (akt != NULL && akt->sorszam != atoi(s2)) akt = akt->kov;
						if (akt != NULL) {
							akt->volt = 1;
						}
						m = 0;
					}
					l++;
				}
			}

			k++;
			i++;
		} while (st[i] != '\0' && st[i-1] != '\0');

		if (DEBUG == 4){
			printf("DEBUG: mentes targylista: \n");
			debug_targylista(mut);
			system("pause");
		}

		if (!jatek(elso, mut, aktsorszam, fajlnev)) {
			memfelszabadit(elso, mut);
			fclose(fp);
			return 0;
		}
	}
}

int mentesell(char sor[]) {
	//ellenoriz fv mintajara

	int i=0,j=0,k=0,c;
	char string[MAX];
	int szam = 0, betu = 0,x=0;

	do {
		j = 0;
		for (; sor[i] != ';' && sor[i] != '\0'; i++) {
			string[j] = sor[i];
			j++;
		}
		string[j] = '\0';

		if (k == 0) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c] < 'a' || string[c] > 'z') && string[c] != '.')return 0;
				if (string[0] == '.')return 0;
			}
		}
		else if (k == 1) { 
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] <'0' || string[c]>'9')return 0;
			}
		}
		else if (k == 2) {
			//pl.: 3arany1kulcs
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] >= '0' && string[c] <= '9') {
					if (szam != 0 && betu != 0) {
						szam = betu = 0;
					}
					szam++;
					if (betu != 0) return 0;
				}
				else if (string[c] >= 'a' && string[c] <= 'z') {
					betu++;
					if (szam == 0) return 0;
				}
			}
		}
		else if (k == 3) {
			x = strlen(string);
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c] <'0' || string[c]>'9') && string[c] != ',')return 0;
				if (string[0] == ',')return 0;
				if (string[x - 1] < '0' || string[x - 1] > '9') return 0;
			}
		}
				
		k++;
		i++;
	} while (sor[i] != '\0');

	return 1;
}
FILE* fajlmegnyit(char argv[], int* mentes) {
	FILE *fp = NULL;
	int i,j;
	char s[MAX];

	i = 0;
	while (argv[i] != '.' && argv != '\0') i++;
	if (argv[i] == '\0') return fp;	//nem volt pont a fajlnevben, nincs kiterjesztes -> hibas fajl

	i++;
	j = 0;
	while (argv[i] != '\0') {	//fajlkiterjesztes kinyerese
		s[j] = argv[i];
		i++;
		j++;
	}
	s[j] = '\0';

	if (!strcmp(s, "csv"))	*mentes = 0;	//ha .csv, akkor sima fajl
	else if (!strcmp(s, "save")) *mentes = 1;	//ha .save, akkor mentes fajl
	else return fp;

	fp = fopen(argv, "r");

	if (DEBUG == 3) {
		printf("DEBUG: FAJLMEGNYITAS: fp addr: %i, mentes: %d\n", fp, *mentes);
		system("pause");
	}

	return fp;
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
		string[j] = '\0';									//lezárja a stringet ha eléri a ;-t vagy sorvéget


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
		case 11:
			strcpy(akt->hova_op1_voltmar, string);
		}
		k++;
		i++;			//átléptetjük a ;-n, azért kell ide az i++
	} while (s[i] != '\0' && s[i] != '\n');
}

int ellenoriz(char s[]) { //leellenőrzi, hogy az adott helyen megfelelő karakter/szám van-e
	char string[MAX];
	int i = 0, j, k = 0, c;
	char seged[4];
	int szam = 0;
	int betu = 0;
	int plusz_minusz = 0;

	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {	//ciklus, ami ;-ig vagy sorvégig megy 
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';								//lezárja a stringet ha eléri a ;-t vagy sorvéget
		if (DEBUG == 1) printf("DEBUG: %s\n", string);

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
			//pl.: 1kard+21arany-3csont+
			szam = betu = plusz_minusz = 0;
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] >= '0' && string[c] <= '9') {
					szam++;
					if (betu != 0 || plusz_minusz != 0) return 0;
				}
				else if (string[c] >= 'a' && string[c] <= 'z') {
					betu++;
					if (szam == 0 || plusz_minusz != 0) return 0;
				}
				if (string[c] == '+' || string[c] == '-') {
					plusz_minusz++;
					if (szam == 0 || betu == 0) return 0;
					szam = betu = plusz_minusz = 0;
				}
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
				if (DEBUG == 1) printf("DEBUG: op1 %d\n", akt1->sorszam); //csak azért kell, ha hibát keresünk
				return 0; //az op1_hova nem volt talalhato a palya sorszamok kozott
			}

			akt2 = elso;
			while (akt2 != NULL && (akt2->sorszam != akt1->hova_op2)) akt2 = akt2->kov; //ameg nincs vege a listanak ES nem talaltunk ilyen sorszamot
			if (akt2 == NULL) {
				if (DEBUG == 1) printf("DEBUG: op2 %d\n", akt1->sorszam); //csak azért kell, ha hibát keresünk
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

		system("cls");
		printf("\t\t\t%s\n\n", akt->helyszin);								//helyszín kiírása
		printf("%s\n\n", akt->leiras);										//leírás kiírása
		if (akt->hova_op1 != 0) printf("1. %s\n2. %s\n\nVálassz: ", akt->op1_leiras, akt->op2_leiras);			//választási lehetőségek kiírása

}

int opvalasztas() {						//opció választás, ahol az '1'  és a '2'  az elfogadott
	char c;
	do {
		c = _getche();	//bekérjük a konzolablakból a számok, amelyiket választjuk
		putchar('\n');
		if (c == '1') {
			printf("\n");
			return 1;
		}
		else if (c == '2') {
			printf("\n");
			return 2;
		}
		else if (c == 'x') {
			printf("\n");
			return 0;			//x-re kilép a program
		}
		else if (c == 's') {
			printf("\n");
			return -1;			//s-re aktualis allas mentese
		}
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

int targyhozzaad(targyak  *elso, char s[], int voltmar) {
	targyak *akt, *temp;
	akt = elso;
	char szam[50];
	char targy[1000];
	int plusz;
	int i, j;
	int volt;

	if (s[0] == '\0' || voltmar) return 1;	//ures targy (lista elso eleme), vagy voltunk mar a palyan

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
			printf("DEBUG: TARGY ");
		if (plusz) printf("HOZZAADVA: %s %s db\n", targy, szam);
		else printf("LEVONVA: %s %s db\n", targy, szam);
			system("pause");
		}

	}while (s[i] != '\0');

	return 1;
	
}

int jatek(lista *elso, targyak *mut, int aktsorsz, char fajlnev[]) {				//játék függvény (palya elso eleme, targyak elso eleme, aktsorszam)
	lista *akt, *makt;
	int aktsorszam = 1;
	int op;
	FILE *mentesfp;
	char mentes[MAX];
	char mentesvolt[MAX];
	int i, j;
	int jo, h;
	targyak *takt;

	if (mut == NULL) {
		mut = calloc(1, sizeof(targyak));		//ha nincs meg targy, letrehozunk egy ures dummy targyat
		mut->kov = NULL;
	}

	aktsorszam = aktsorsz;	//mentesbol aktsorszam visszaallitasa
	do {
		system("cls");				//felület letörlése
		akt = elso;
		while (akt->sorszam != aktsorszam) akt = akt->kov; 



		kiir(elso, aktsorszam);
		op = opvalasztas();		//1   vagy  2 a visszateres

		if (op == 1) {
			if (akt->szuks_targy[0] == '\0')		// ha nincs a tovabblepesnek feltetele
			{

				if (DEBUG == 2) {
					printf("DEBUG: nincs a tovabblepesnek feltetele\n");
					system("pause");
				}

				if (targyhozzaad(mut, akt->plusz_minusz_targy, akt->volt)) {

					if (akt->volt == 1) {
						printf("%s\n", akt->hova_op1_voltmar);
						_getche();
						aktsorszam = akt->hova_op1;
					}
					else {
					printf("%s\n", akt->mi_tort_op1);
					_getche();
					aktsorszam = akt->hova_op1;   //ha 1-est választjuk oda ugrik ahova az egyes opció után kell


					  //miutan kikerestuk a palyat, beallitjuk a 'volt' valtozot
					akt->volt = 1;
				}
				}
				else {
					//gaz volt a targy hozzaadasanal
					if (DEBUG == 2) {
						printf("DEBUG: baj volt a targy hozzaadasanal\n");
						system("pause");
					}
					if (akt->volt == 1) {
						printf("%s\n", akt->hova_op1_voltmar);
						_getche();
						aktsorszam = akt->hova_op1;
					}
					else {
					printf("%s\n", akt->mi_tort_op1);
					system("pause");
					aktsorszam = akt->hova_op1;

					//miutan kikerestuk a palyat, beallitjuk a 'volt' valtozot
					akt->volt = 1;
				}
			}
			}
			else {		//ha van a tovabblepesnek feltetele
				if (targyell(mut, akt->szuks_targy)) {				//ha megvan a szukseges targy es a kello darabszam
					
					if (!targyhozzaad(mut, akt->plusz_minusz_targy, akt->volt)) {
						if (DEBUG == 2) {
							printf("DEBUG: baj volt a targyhozzaadasnal\n");
							system("pause");
						}

					}
					if (DEBUG == 2) {
						printf("DEBUG: van a tovabblepesnek feltetele, es teljesult\n");
						debug_targylista(mut);
						system("pause");
					}

					if (akt->volt == 1) {
						printf("%s\n", akt->hova_op1_voltmar);
						_getche();
						aktsorszam = akt->hova_op1;
					}
					else {
					printf("%s\n", akt->mi_tort_op1);
						_getche();
					aktsorszam = akt->hova_op1;		//ha 1-est választjuk oda ugrik ahova az egyes opció után kell
					
					//miutan kikerestuk a palyat, beallitjuk a 'volt' valtozot
					akt->volt = 1;
					}
					
				}
				else {			//ha nincs meg a kello targy vagy nincs annyi db belole
					if (DEBUG == 2) {
						printf("DEBUG: van a tovabblepesnek feltetele, de nem teljesult\n");
						debug_targylista(mut);
						system("pause");
					}

					printf("Ehhez az opcióhoz %s szükséges! A másik lehetőséget választod.\n", akt->szuks_targy); //ha kell valamilyen tárgy akkor figyelmeztet rá
					printf("%s\n", akt->mi_tort_op2);
					_getche();
					aktsorszam = akt->hova_op2;
					if (DEBUG == 2) {
						printf("DEBUG: nincs ilyen targy, vagy nincs ennyi db belőle");
						system("pause");
					}
				}
			}
		}
		else if (op == 2) {
			printf("%s\n", akt->mi_tort_op2);
			_getche();
			aktsorszam = akt->hova_op2;  //ha 2-esz választjuk ida ugrik ahova a kettes opció után kell
		}
		else if (op == -1) {			//mentes
		//fajlnev;aktsorszam;targyak;hol_voltunk
			int jo = 0;
			do {
				system("cls");
				printf("\t\t\tJáték mentése\n\n");
				printf("Adja meg a mentés nevét: ");
				h = getline(mentes, MAX - 1);
				if (h == 0) {
					printf("\nHiba: üres sort adott meg.\n");
					_getche();
				}
				else if (!mentesnevell(mentes)) {
					printf("\nHiba: a mentés neve csak betűkből és számokból állhat.\n");
					_getche();
				}
				else if (h == 1 && mentes[0] == 'x') jo = 1;
				else jo = 1;
			} while (jo == 0);

			if (!(h == 1 && mentes[0] == 'x')) {
				sprintf(mentes, "%s.save", mentes);
				mentesfp = fopen(mentes, "w");
				if (mentesfp == NULL) {
					printf("Hiba: nem sikerült a fájlt megnyitni.");
					_getche();
				}
				else {
					fprintf(mentesfp, "%s;", fajlnev);
					fprintf(mentesfp, "%d;", aktsorszam);

					takt = mut;
					while (takt != NULL) {
						if (takt->drb != 0) {		//dummy elem atlepese
							fprintf(mentesfp, "%d%s", takt->drb, takt->nev);
						}
						takt = takt->kov;
					}
					fprintf(mentesfp, ";");
					makt = elso;
					i = j = 0;
					while (makt != NULL) {
						if (makt->volt) {
							mentesvolt[i] = makt->sorszam + 48;
							i++;
							mentesvolt[i] = ',';
							i++;
						}
						makt = makt->kov;
					}
					mentesvolt[i] = '\0';
					if (mentesvolt[strlen(mentesvolt) - 1] == ',') mentesvolt[strlen(mentesvolt) - 1] = '\0';
					fprintf(mentesfp, "%s", mentesvolt);
					fclose(mentesfp);
					printf("\n\nJáték elmentve: %s\n", mentes);
					_getche();
				}
			}
		}
		else return -1; //KILEPES
	} while (aktsorszam != 0);				//azért 0, mert akkor lép ki a játékból
}

int mentesnevell(char s[]) {
	int i;
	for (i = 0; s[i] != '\0'; i++) {
		if ((s[i]<'a' || s[i]>'z') && (s[i]<'0' || s[i]>'9')) return 0;
	}
	return 1;
}

void debug_targylista(targyak *elso) {
	targyak *akt;

	akt = elso;
	printf("DEBUG: TARGYLISTA: ");
	while (akt != NULL) {
		printf("%s %d db, ", akt->nev, akt->drb);
		akt = akt->kov;
	}
}

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

void memfelszabadit(lista *pelso, targyak *telso) {
	lista *pakt, *ptemp;
	targyak *takt, *ttemp;

	pakt = pelso;

	while (pakt != NULL) {
		ptemp = pakt;
		pakt = pakt->kov;
		free(ptemp);
	}

	takt = telso;

	while (takt != NULL) {
		ttemp = takt;
		takt = takt->kov;
		free(ttemp);
	}
}