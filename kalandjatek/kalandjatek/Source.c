#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000


typedef struct list {			//lista, ahova beolvasom majd a dolgokat
	int sorszam;
	char helyszin[MAX];
	char leiras[MAX];
	char op1_leiras[MAX];
	char op2_leiras[MAX];
	char szuks_targy_op1[MAX];
	char szuks_targy_op2[MAX];
	char plusz_minusz_targy_op1[MAX];
	char plusz_minusz_targy_op2[MAX];
	int hova_op1;
	int hova_op2;
	char mi_tort_op1[MAX];
	char mi_tort_op2[MAX];
	struct lista *kov;
}lista;

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

int getline(char s[], int n) {
	int c, i;
	for (i = 0; i<n && (c = getchar()) != EOF&&c != '\n'; i++) s[i] = c;
	s[i] = '\0';
	while (c != EOF&&c != '\n') c = getchar();
	return i;
}

void main() {
	FILE *fp;				//filepointer
	char st[MAX];			//string, ahova lementem a dolgokat a csv-ből
	lista *elso = NULL, *akt = NULL, *temp = NULL;

	fp = fopen("beadandov3.0.csv", "r");

	if (fp == NULL) {
		printf("Hiba a fajl megnyitasakor! \n");
		return 0;
	}

	/*ellenőrzés ide kell majd */
	while (!feof(fp)) {
		fgets(st, MAX, fp);
		if (ellenoriz(st) == 0) {
			printf("Rossz volt a csv formatuma \n");
			return 0;
		}
	}

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
	
	akt = elso;
	while (akt != NULL) {
		printf("sorszam: %d\n", akt->sorszam);
		printf("helyszin: %s\n", akt->helyszin);
		printf("leiras: %s\n", akt->leiras);
		printf("op1 leiras: %s\n", akt->op1_leiras);
		printf("op2 leiras: %s\n", akt->op2_leiras);
		printf("szukseges targy 1: %s\n", akt->szuks_targy_op1);
		printf("szukseges targy 2: %s\n", akt->szuks_targy_op2);
		printf("plusz/minusz targy op1: %s\n", akt->plusz_minusz_targy_op1);
		printf("plusz/minusz targy op2: %s\n", akt->plusz_minusz_targy_op2);
		printf("hova op1: %d\n", akt->hova_op1);
		printf("hova op2: %d\n", akt->hova_op2);
		printf("mi tortenik op1: %s\n", akt->mi_tort_op1);
		printf("mi tortenik op2: %s\n", akt->mi_tort_op2);
		printf("----------------------------uj sor---------------------\n");
		akt = akt->kov;
	}


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
			strcpy(akt->szuks_targy_op1, string);
		case 6:
			strcpy(akt->szuks_targy_op2, string);
		case 7:
			strcpy(akt->plusz_minusz_targy_op1, string);
		case 8:
			strcpy(akt->plusz_minusz_targy_op2, string);
		case 9:
			akt->hova_op1 = atoi(string);
		case 10:
			akt->hova_op2 = atoi(string);
		case 11:
			strcpy(akt->mi_tort_op1, string);
		case 12:
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

	printf("%s\n", s);

	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {	//ciklus, ami ;-ig vagy sorvégig megy 
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';								//lezárja a stringet ha eléri a ;-t vagy sorvéget
		printf("%s\n", string);

		if (k == 0) {
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] <'0' || string[c]>'9') return 0;
			}
			printf("sorszam\n");
		}
		else if (k == 1) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("%c,helyszin\n", string[c]);
		}
		else if (k == 2) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("leiras\n");
		}
		else if (k == 3) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("op1 leiras\n");
		}
		else if (k == 4) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("op2 leiras\n");
		}
		else if (k == 5) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("szukseges targy 1 \n");
		}
		else if (k == 6) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("szukseges targy 2 \n");
		}
		else if (k == 7) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("plusz minusz targy op1 \n");
		}
		else if (k == 8) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("plusz minusz targy op2 \n");
		}
		else if (k == 9) {
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] <'0' || string[c]>'9') return 0;
			}
			printf("hova op1 \n");
		}
		else if (k == 10) {
			for (c = 0; string[c] != '\0'; c++) {
				if (string[c] <'0' || string[c]>'9') return 0;
			}
			printf("hova op2 \n");
		}
		else if (k == 11) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("mi tort op1 \n");
		}
		else if (k == 12) {
			for (c = 0; string[c] != '\0'; c++) {
				if ((string[c]< '0' || string[c]> '9') && string[c] != '.' && string[c] != ',' && string[c] != ' ' && !magyarbetu(string[c])) return 0;
			}
			printf("mi tort op2 \n");
		}
	
		k++;
		i++;
	} while (s[i] != '\0' && s[i] != '\n');

	return 1;
}