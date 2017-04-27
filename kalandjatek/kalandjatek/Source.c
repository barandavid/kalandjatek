#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000




typedef struct list {
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


void szetszed(char s[], struct list *akt);

int getline(char s[], int n) {
	int c, i;
	for (i = 0; i<n && (c = getchar()) != EOF&&c != '\n'; i++) s[i] = c;
	s[i] = '\0';
	while (c != EOF&&c != '\n') c = getchar();
	return i;
}

void main() {
	FILE *fp;
	char st[MAX];
	lista *elso = NULL, *akt = NULL, *temp = NULL;

	fp = fopen("beadandov3.0.csv", "r");

	if (fp == NULL) {
		printf("Hiba a fajl megnyitasakor! \n");
		return 0;
	}

	while (!feof(fp)) {
		fgets(st, MAX, fp);

		akt = calloc(1, sizeof(lista));
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

void szetszed(char s[], struct list *akt) {
	char string[MAX];
	int i=0,j,k=0;
	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {
			string[j] = s[i];
			j++;
		}
		string[j] = '\0';/*
		printf("%s\n", string);*/
		
		switch (k){
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
		i++;
	} while (s[i] != '\0' && s[i] != '\n');
	/*printf("----------------------------uj sor---------------------\n");*/
}