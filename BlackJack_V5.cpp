#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <string.h>
#define MAX_IGRACA 10

typedef struct{
	char ime_igraca[30+1];
	char lozinka_igraca[30+1];
	char spol;
	int igrac_Bal;
	int br_pobjeda;
	int br_gubitka;
	int br_BJ;
} Igrac;

typedef struct{
	int odabereni_igrac;
	int zadnji;
	Igrac elementi[MAX_IGRACA];
} Lista;

typedef struct{
	int dek[52+1];
	int broj_karta_u_deku;
	int izvucena_karta;
	bool kraj;

	int diler_karte[12];
	int diler_bodovi;
	int diler_broj_karta;
	int igrac_karte[12];
	int igrac_bodovi;
	int igrac_broj_karta;
	int igrac_ulog[4];

	Lista igraci;
} Podatci;

void gotoxy(int xpos, int ypos) {
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos; scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput,scrn);
}
void ChangeCursorStatus(bool Visible){

	CONSOLE_CURSOR_INFO *c=new CONSOLE_CURSOR_INFO;
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);

	if (Visible)
	{
		c->bVisible=true;
		c->dwSize=1;//(0) is invild so the default value is set
	}
	else
	{
		c->bVisible=false;
		c->dwSize=1;//it must be assigned to a valid value
	}

	SetConsoleCursorInfo(h,c);
}

void ispisDiamant(){

	_setmode (_fileno(stdout),0x20000);
	wprintf(L"\x2666");
	_setmode (_fileno(stdout),_O_TEXT);

}
void ispisSrce(){

	_setmode (_fileno(stdout),0x20000);
	wprintf(L"\x2665");
	_setmode (_fileno(stdout),_O_TEXT);

}
void ispisDjetelina(){

	_setmode (_fileno(stdout),0x20000);
	wprintf(L"\x2663");
	_setmode (_fileno(stdout),_O_TEXT);

}
void ispisPik(){

	_setmode (_fileno(stdout),0x20000);
	wprintf(L"\x2660");
	_setmode (_fileno(stdout),_O_TEXT);

}

void ispisOkrenutaKarta(){

	_setmode (_fileno(stdout),0x20000);
	wprintf(L"|");
	wprintf(L"\x2592");
	wprintf(L"\x2592");
	wprintf(L"|");
	_setmode (_fileno(stdout),_O_TEXT);

}

void ucitaj(Lista *aigraci){
	FILE *datoteka;

	datoteka = fopen("save", "rb");
	if (datoteka == NULL)
		printf("Problem u otvaranju datoteke!");
	if(datoteka != NULL)
		fread(aigraci,sizeof(*aigraci), 1, datoteka) ;

	fclose(datoteka);
}

void spremi(Lista *aigraci){
	FILE *datoteka;

	datoteka = fopen("save", "wb");
	if (datoteka == NULL)
		printf("Problem u otvaranju datoteke!");
	if(datoteka != NULL)
		fwrite(aigraci,sizeof(*aigraci), 1, datoteka) ;

	fclose(datoteka);
}

void ubaciUListu (int pozicija_ubacivanja, Lista *aigraci) {

	if (aigraci->zadnji >= MAX_IGRACA-1){
		printf("Nema mjesta za novog igraca!");
		getch();
	}

	else{
		Igrac novi;
		int y;
		ChangeCursorStatus(true);
		do{
			gotoxy(0,2);
			printf(" Ime igraca:                              ");
			printf("\n Lozinka igraca:                              ");
			printf("\n Spol igraca [M/Z]:                    ");
			gotoxy(13,2);
			scanf("%s",&novi.ime_igraca);
			gotoxy(17,3);
			scanf("%s",&novi.lozinka_igraca);
			
			do{
				gotoxy(19,4);
				scanf(" %c",&novi.spol);
				if(novi.spol != 'M' && novi.spol != 'Z'){
					gotoxy(0,0);
					printf(" Spol mora biti M ili Z!");
				}			
			}while(novi.spol != 'M' && novi.spol != 'Z');
			
			
			for(y = 1; y <= aigraci->zadnji; y++){
				if(stricmp (novi.ime_igraca,aigraci->elementi[y].ime_igraca) == 0){
					gotoxy(0,0);
					printf(" Ime je vec iskoristeno!");
					break;
				}
			}
		}while(stricmp (novi.ime_igraca,aigraci->elementi[y].ime_igraca) == 0);

		novi.igrac_Bal = 1000;
		novi.br_pobjeda = 0;
		novi.br_gubitka = 0;
		novi.br_BJ = 0;

		ChangeCursorStatus(false);

		for (int pozicija = aigraci->zadnji; pozicija >= pozicija_ubacivanja; pozicija--)
			aigraci->elementi[pozicija+1]= aigraci->elementi[pozicija];
		aigraci->zadnji++;
		aigraci->elementi[pozicija_ubacivanja] = novi;
	}
}

void ispisPocetak(){

	printf("\n======================================\n ");

	ispisDiamant();
	ispisSrce();

	printf(" DOBRODOSLI U IGRU -BLACK JACK- ");

	ispisDjetelina();
	ispisPik();

	printf("\n======================================\n\n");

	printf(" Prije pocetka procitajte pravila igre:\n");
	printf("---------------------------------------------------------------------------------------------------\n");
	printf(" + djelitelj dijeli igracu dvije karte licem gore, a sebi jednu licem gore i jednu licem dolje\n");
	printf(" + cilj igre je postici veci broj bodova od djelitelja, a da igrac ne prijede 21\n");
	printf(" + ako igrac prijede 21, automatski gubi igru\n");
	printf(" + karte s brojevima vrijede onoliko bodova koliki je broj, a decko, dama i kralj vrijede 10 bodova  \n");
	printf(" + As vrijedi 11 bodova ako je zbroj karata 10 ili manji, a 1 bod ako je zbroj karata 11 ili veci\n");
	printf("---------------------------------------------------------------------------------------------------\n\n");

	printf(" pritisnite bilo koju tipku za nastavak...");

	getch();
	system("CLS");

}

void ispisKarta(int brKarte){

	printf("|");

	if(brKarte <= 13){
		ispisPik();

		if(brKarte == 1)
			printf("A");
		else if(brKarte == 11)
			printf("J");
		else if(brKarte == 12)
			printf("Q");
		else if(brKarte == 13)
			printf("K");
		else
			printf("%d",brKarte);
	}

	if(brKarte >= 14 && brKarte <= 26){
		ispisDjetelina();

		if(brKarte == 14)
			printf("A");
		else if(brKarte == 24)
			printf("J");
		else if(brKarte == 25)
			printf("Q");
		else if(brKarte == 26)
			printf("K");
		else
			printf("%d",(brKarte-13));
	}

	if(brKarte >= 27 && brKarte <= 39){
		ispisSrce();

		if(brKarte == 27)
			printf("A");
		else if(brKarte == 37)
			printf("J");
		else if(brKarte == 38)
			printf("Q");
		else if(brKarte == 39)
			printf("K");
		else
			printf("%d",(brKarte-26));
	}

	if(brKarte >= 40){
		ispisDiamant();

		if(brKarte == 40)
			printf("A");
		else if(brKarte == 50)
			printf("J");
		else if(brKarte == 51)
			printf("Q");
		else if(brKarte == 52)
			printf("K");
		else
			printf("%d",(brKarte-39));
	}

	printf("|");
}

void postaviDek(Podatci *aigra){

	for(int x = 1; x <= 52; x++){
		aigra->dek[x-1] = x;
	}

	aigra->dek[53] = 0;

	aigra->broj_karta_u_deku = 52;

}

void izvuciKartu(Podatci *aigra){

	int broj = rand() %(aigra->broj_karta_u_deku);

	aigra->broj_karta_u_deku--;

	aigra->izvucena_karta = aigra->dek[broj];

	while (broj <= aigra->broj_karta_u_deku){
		aigra->dek[broj] = aigra->dek[broj + 1];
		broj++;
	}

}

void dajKartuDileru(Podatci *aigra){

	izvuciKartu(aigra);
	aigra->diler_karte[aigra->diler_broj_karta] = aigra->izvucena_karta;
	aigra->diler_broj_karta++;

}
void dajKartuIgracu(Podatci *aigra){

	izvuciKartu(aigra);
	aigra->igrac_karte[aigra->igrac_broj_karta] = aigra->izvucena_karta;
	aigra->igrac_broj_karta++;

}

void izracun_provjeraBodova(Podatci *aigra){

	aigra->diler_bodovi = 0;
	aigra->igrac_bodovi = 0;

//zbrajanje bodova za dilera
	for(int x = 0; x < aigra->diler_broj_karta; x++){
		if(aigra->diler_karte[x] == 11 || aigra->diler_karte[x] == 12 || aigra->diler_karte[x] == 13 || aigra->diler_karte[x] == 24 || aigra->diler_karte[x] == 25 || aigra->diler_karte[x] == 26
		   || aigra->diler_karte[x] == 37 || aigra->diler_karte[x] == 38 || aigra->diler_karte[x] == 39 || aigra->diler_karte[x] == 50 || aigra->diler_karte[x] == 51 || aigra->diler_karte[x] == 52){

			aigra->diler_bodovi += 10;
		}

		else{
			if(aigra->diler_karte[x] != 1 && aigra->diler_karte[x] != 14 && aigra->diler_karte[x] != 27 && aigra->diler_karte[x] != 40){

				if(aigra->diler_karte[x] <= 13){
					aigra->diler_bodovi += aigra->diler_karte[x];
				}
				if(aigra->diler_karte[x] >= 14 && aigra->diler_karte[x] <= 26){
					aigra->diler_bodovi += (aigra->diler_karte[x]-13);
				}
				if(aigra->diler_karte[x] >= 27 && aigra->diler_karte[x] <= 39){
					aigra->diler_bodovi += aigra->diler_karte[x]-26;
				}
				if(aigra->diler_karte[x] >= 40){
					aigra->diler_bodovi += aigra->diler_karte[x]-39;
				}
			}
		}
 	}
	for(int x = 0; x < aigra->diler_broj_karta; x++){
		if(aigra->diler_karte[x] == 1 || aigra->diler_karte[x] == 14 || aigra->diler_karte[x] == 27 || aigra->diler_karte[x] == 40){
			if(aigra->diler_bodovi <= 10){
				aigra->diler_bodovi += 11;
			}
			else{
				aigra->diler_bodovi += 1;
			}
		}
	}

//zbrajanje bodova za igraca
	for(int x = 0; x < aigra->igrac_broj_karta; x++){
		if(aigra->igrac_karte[x] == 11 || aigra->igrac_karte[x] == 12 || aigra->igrac_karte[x] == 13 || aigra->igrac_karte[x] == 24 || aigra->igrac_karte[x] == 25 || aigra->igrac_karte[x] == 26
		   || aigra->igrac_karte[x] == 37 || aigra->igrac_karte[x] == 38 || aigra->igrac_karte[x] == 39 || aigra->igrac_karte[x] == 50 || aigra->igrac_karte[x] == 51 || aigra->igrac_karte[x] == 52){

			aigra->igrac_bodovi += 10;
		}

		else{
			if(aigra->igrac_karte[x] != 1 && aigra->igrac_karte[x] != 14 && aigra->igrac_karte[x] != 27 && aigra->igrac_karte[x] != 40){

				if(aigra->igrac_karte[x] <= 13){
					aigra->igrac_bodovi += aigra->igrac_karte[x];
				}
				if(aigra->igrac_karte[x] >= 14 && aigra->igrac_karte[x] <= 26){
					aigra->igrac_bodovi += (aigra->igrac_karte[x]-13);
				}
				if(aigra->igrac_karte[x] >= 27 && aigra->igrac_karte[x] <= 39){
					aigra->igrac_bodovi += aigra->igrac_karte[x]-26;
				}
				if(aigra->igrac_karte[x] >= 40){
					aigra->igrac_bodovi += aigra->igrac_karte[x]-39;
				}
			}
		}
	}
	for(int x = 0; x < aigra->igrac_broj_karta; x++){
		if(aigra->igrac_karte[x] == 1 || aigra->igrac_karte[x] == 14 || aigra->igrac_karte[x] == 27 || aigra->igrac_karte[x] == 40){
			if(aigra->igrac_bodovi <= 10){
				aigra->igrac_bodovi += 11;
			}
			else{
				aigra->igrac_bodovi += 1;
			}
		}
	}

//provjera jeli igrac preso 21
	if(aigra->igrac_bodovi >= 21)
		aigra->kraj = true;

}

int izracunajUlog(int aulog[4]){

	int ulog = 0;

	ulog += aulog[0]*10;
	ulog += aulog[1]*50;
	ulog += aulog[2]*200;
	ulog += aulog[3]*500;

	return (ulog);
}

void postaviStrelice(int apozicija, int br_opcija, int ax_poz, int y_poz, int pointeri[]){

	for(int x = 0; x < br_opcija; x++){
		gotoxy(ax_poz,y_poz);
		if(apozicija == x)
			printf(">");
		else
			printf(" ");

		ax_poz += pointeri[x] + 1;
		gotoxy(ax_poz,y_poz);

		if(apozicija == x)
			printf("<");
		else
			printf(" ");

		ax_poz += 2;
	}
}

void ispisUlog(Podatci *aigra){
	system("CLS");

	int strelice, strelice_smijer = 80;
	int pozicija = 0;

	int x_poz = 0;

	for(int x = 0; x < 4; x++)
		aigra->igrac_ulog[x] = 0;

	printf("==========================\n");
	printf("Ulog:");
	printf("\n--------------------------\n");
	printf("Bal :");
	printf("\n==========================\n");

	int pointeri_za_strelice[4] = {2,2,3,3};
	printf("+    +    +     +     +\n");
	printf("| 10 | 50 | 200 | 500 |x\n");
	printf("-    -    -     -     -\n");

	printf("\n- Gore-dole za dodat ili maknuti zeton, a ljevo-desno za promjenu zetona!\n");
	printf("- Pretisnite ENTER za potvrdu!\n");

	do{
		postaviStrelice(pozicija, 4, 1, 6, pointeri_za_strelice);

		gotoxy(24,6);
		printf("%d  ",aigra->igrac_ulog[pozicija]);

		if(strelice_smijer == 80 || strelice_smijer == 72){
			gotoxy(5,1);
			printf(" %d $           ", izracunajUlog(aigra->igrac_ulog));
			gotoxy(5,3);
			printf(" %d $           ", aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal - izracunajUlog(aigra->igrac_ulog));
		}
		do{
			strelice = getch();

			if(strelice == 224){
				strelice_smijer = getch();

				if(strelice_smijer == 72){//gore
					aigra->igrac_ulog[pozicija]++;

					if(izracunajUlog(aigra->igrac_ulog) > aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal)
						aigra->igrac_ulog[pozicija]--;
				}
				if(strelice_smijer == 80){//dole
					if(aigra->igrac_ulog[pozicija] > 0)
						aigra->igrac_ulog[pozicija]--;
				}
				if(strelice_smijer == 75){//ljevo
					if(pozicija > 0)
						pozicija--;
				}
				if(strelice_smijer == 77){//desno
					if(pozicija < 3)
						pozicija++;
				}
			}
		}while(strelice != 224 && strelice != 13);
	}while(strelice != 13);
	gotoxy(0,0);
}

void ispisIgre(Podatci *aigra){

	char ponovi_da_ne;
	char vuci_da_ne = '0';

	int strelice, strelice_smijer;
	int pozicija;

	int x_poz;

	do{
		aigra->diler_broj_karta=0;
		aigra->igrac_broj_karta=0;
		aigra->kraj = false;

		ispisUlog(aigra);

		dajKartuDileru(aigra);
		dajKartuDileru(aigra);
		dajKartuIgracu(aigra);
		dajKartuIgracu(aigra);

		strelice = 0;
		pozicija = 0;

		system("CLS");

		printf("====================================\n");
		printf("[??]Diler: ");
		ispisKarta(aigra->diler_karte[0]);
		ispisOkrenutaKarta();
		printf("\n------------------------------------\n");
		printf("[  ]Igrac: ");
		ispisKarta(aigra->igrac_karte[0]);
		ispisKarta(aigra->igrac_karte[1]);
		printf("\n====================================\n");
		printf("Bal: %d $\tUlog: %d $",aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal - izracunajUlog(aigra->igrac_ulog), izracunajUlog(aigra->igrac_ulog));
		printf("\n====================================\n\n");
		int pointeri_za_strelice1[2] = {4,6};
		printf("\t| Vuci | Ostani |");

		do{
			if(strelice == 13 && pozicija == 1){
				while(aigra->diler_bodovi <= aigra->igrac_bodovi && aigra->diler_bodovi < 17){
					dajKartuDileru(aigra);
					izracun_provjeraBodova(aigra);
					}
				aigra->kraj = true;
			}
			if(strelice == 13 && pozicija == 0)
					dajKartuIgracu(aigra);

			izracun_provjeraBodova(aigra);

			if(aigra->kraj){
				gotoxy(1,1);
				if(aigra->diler_bodovi < 10)
					printf(" ");
				printf("%d",aigra->diler_bodovi);
				gotoxy(11,1);
				for(int x = 0; x < aigra->diler_broj_karta; x++){
				ispisKarta(aigra->diler_karte[x]);
				}
			}

			gotoxy(1,3);
			printf("%d",aigra->igrac_bodovi);

			gotoxy(11,3);
			for(int x = 0; x < aigra->igrac_broj_karta; x++){
				ispisKarta(aigra->igrac_karte[x]);
			}

			if(!aigra->kraj){
				do{
					postaviStrelice(pozicija, 2, 9, 8, pointeri_za_strelice1);

					do{
						strelice = getch();

						if(strelice == 224){
							strelice_smijer = getch();
							if(strelice_smijer == 75){//ljevo
								if(pozicija > 0)
									pozicija--;
							}
							if(strelice_smijer == 77){//desno
								if(pozicija < 1)
									pozicija++;
							}
						}
					}while(strelice != 224 && strelice != 13);
				}while(strelice != 13);
			}

		}while(!aigra->kraj);

		gotoxy(0,9);
	//ispis jeli igrac pobjedio ili nije i dodavanje ili oduzimanje valute
		if(aigra->igrac_bodovi <= 21){
			if(aigra->diler_bodovi < aigra->igrac_bodovi || aigra->diler_bodovi > 21){
				if(aigra->igrac_bodovi == 21){
					aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal += izracunajUlog(aigra->igrac_ulog)*1.5;
					printf("\npobjedili ste !!!BlackJack!!! :O (+ %d $)",2*izracunajUlog(aigra->igrac_ulog)+izracunajUlog(aigra->igrac_ulog)/2);
					aigra->igraci.elementi[aigra->igraci.odabereni_igrac].br_BJ++;
					aigra->igraci.elementi[aigra->igraci.odabereni_igrac].br_pobjeda++;
				}
				else{
					aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal += izracunajUlog(aigra->igrac_ulog);
					printf("\npobjedili ste ! :) (+ %d $)",2*izracunajUlog(aigra->igrac_ulog));
					aigra->igraci.elementi[aigra->igraci.odabereni_igrac].br_pobjeda++;
				}
			}
			else if(aigra->diler_bodovi == aigra->igrac_bodovi)
				printf("\nizjednaceni ! :/ (+ %d $)",izracunajUlog(aigra->igrac_ulog));
			else{
				aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal -= izracunajUlog(aigra->igrac_ulog);
				printf("\nizgubili ste ! :( (- %d $)",izracunajUlog(aigra->igrac_ulog));
				aigra->igraci.elementi[aigra->igraci.odabereni_igrac].br_gubitka++;
			}
		}
		else{
			aigra->igraci.elementi[aigra->igraci.odabereni_igrac].igrac_Bal -= izracunajUlog(aigra->igrac_ulog);
			printf("\nizgubili ste ! :( (- %d $)",izracunajUlog(aigra->igrac_ulog));
			aigra->igraci.elementi[aigra->igraci.odabereni_igrac].br_gubitka++;
		}

		spremi(&aigra->igraci);

		if(aigra->broj_karta_u_deku < 15){
			postaviDek(aigra);
			printf("\n\nDEK JE PROMJESAN!!!");
		}

		strelice = 0;
		pozicija = 0;

		gotoxy(0,8);
		int pointeri_za_strelice2[2] = {12,14};
		printf(" | Igraj ponovo | Spremi i izadi |");

			do{
				postaviStrelice(pozicija, 2, 2, 8, pointeri_za_strelice2);

				strelice = getch();

				if(strelice == 224){
					strelice_smijer = getch();
					if(strelice_smijer == 75){//ljevo
						if(pozicija > 0)
							pozicija--;
					}
					if(strelice_smijer == 77){//desno
						if(pozicija < 1)
							pozicija++;
					}
				}
			}while(strelice != 13);
	}while(strelice == 13 && pozicija == 0);
	gotoxy(0,0);
}

main(){
	Podatci igra;

	ChangeCursorStatus(false);

	srand (time(NULL)); //postavljanje seed-a za slucajan odabir (ovisi o vremenu)

	ispisPocetak();
	postaviDek(&igra);

	igra.igraci.zadnji = 0;

	ucitaj(&igra.igraci);

	igra.igraci.odabereni_igrac = 0;
	igra.igraci.elementi[igra.igraci.odabereni_igrac].igrac_Bal = 100;
	strcpy(igra.igraci.elementi[igra.igraci.odabereni_igrac].ime_igraca, "Gost" );

	int y_poz, pozicija;
	int strelice, strelice_smijer;
	char ime_log[30+1];
	char lozinka_log[30+1];
	do{
		system("CLS");

		pozicija = 0;

		printf("\n      GLAVNI MENU\n");
		printf("=======================");
		printf("\n  Igraj");
		printf("\n  Prijavi se");
		printf("\n  Stvori novog igraca");
		printf("\n  Statistika igraca");
		printf("\n  Izlaz");

		gotoxy(28,0);

		printf("| %s | Bal: %d$ |", igra.igraci.elementi[igra.igraci.odabereni_igrac].ime_igraca, igra.igraci.elementi[igra.igraci.odabereni_igrac].igrac_Bal);

		do{
			y_poz = 3;
			for(int x = 0; x < 5; x++){
				gotoxy(0,y_poz);
				if(pozicija == x)
					printf(">");
				else
					printf(" ");

				y_poz += 1;
			}
			strelice = getch();

			if(strelice == 224){
				strelice_smijer = getch();

				if(strelice_smijer == 72){//gore
					if(pozicija > 0)
						pozicija--;
				}
				if(strelice_smijer == 80){//dole
					if(pozicija < 4)
						pozicija++;
				}
			}
		}while(strelice != 13);

		if(pozicija == 0)
			ispisIgre(&igra);

		else if(pozicija == 1){
			system("CLS");
			ChangeCursorStatus(true);
			printf("\n\n Ime igraca:");
			printf("\n Lozinka igraca:");
			gotoxy(13,2);
			scanf("%s",&ime_log);
			gotoxy(17,3);
			scanf("%s",&lozinka_log);
			ChangeCursorStatus(false);

			for(int x = 1; x <= igra.igraci.zadnji; x++){
				if(stricmp (ime_log,igra.igraci.elementi[x].ime_igraca) == 0 && stricmp (lozinka_log,igra.igraci.elementi[x].lozinka_igraca) == 0)
					igra.igraci.odabereni_igrac = x;
			}
		}
		else if(pozicija == 2){
			system("CLS");
			ubaciUListu(igra.igraci.zadnji+1, &igra.igraci);
			spremi(&igra.igraci);
		}

		else if(pozicija == 3){
			system("CLS");
			printf("\n %s [%c]",igra.igraci.elementi[igra.igraci.odabereni_igrac].ime_igraca, igra.igraci.elementi[igra.igraci.odabereni_igrac].spol);
			printf("\n---------------------------------");
			printf("\n Broj pobjeda: %d",igra.igraci.elementi[igra.igraci.odabereni_igrac].br_pobjeda);
			printf("\n Broj gubitka: %d",igra.igraci.elementi[igra.igraci.odabereni_igrac].br_gubitka);
			printf("\n Broj dobivenih BlackJack-a: %d",igra.igraci.elementi[igra.igraci.odabereni_igrac].br_BJ);
			
			if(ime_log,igra.igraci.elementi[igra.igraci.odabereni_igrac].br_gubitka == 0)
				printf("\n Omjer gubitka i pobjeda: 100%% pobjeda");
			else
				printf("\n Omjer gubitka i pobjeda: %.2f",(igra.igraci.elementi[igra.igraci.odabereni_igrac].br_pobjeda * 1.0)/(ime_log,igra.igraci.elementi[igra.igraci.odabereni_igrac].br_gubitka * 1.0));
			getch();
		}

	}while(pozicija != 4);

	spremi(&igra.igraci);

}
