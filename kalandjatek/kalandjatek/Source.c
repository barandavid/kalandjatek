#include <stdio.h>

#define MAX 1000

void szetszed(char s[], lista *akt);

int getline(char s[], int n) {
	int c, i;
	for (i = 0; i<n && (c = getchar()) != EOF&&c != '\n'; i++) s[i] = c;
	s[i] = '\0';
	while (c != EOF&&c != '\n') c = getchar();
	return i;
}


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
	char hova_op1[MAX];
	char hova_op2[MAX];
	char mi_tort_op1[MAX];
	char mi_tort_op2[MAX];
	struct list *kov;
}lista;


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


		printf("%s", st);
	}
	


}

void szetszed(char s[], lista *akt) {
	char string[MAX];
	int i=0,j;
	do {
		j = 0;
		for (; s[i] != '\0' && s[i] != '\n' && s[i] != ';'; i++) {
			string[j] = s[i];
			j++;
			if (s[i] == ';' || s[i] == '\0' || s[i] == '\n'){
				string[j] = '\0';
			}


		}

	} while (s[i] != '\0' && s[i] != '\n');
}

