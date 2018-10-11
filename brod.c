#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_MISIONARA 5
#define MAX_KANIBALA 6
#define MAX_VRIJEME 4

#define KANIBAL 11
#define MISIONAR 12

struct my_msgbuf {
	long mtype;
	char mtext[200];
};

int msqid;
int msqid_putnici;

void retreat(int failure) 
{
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}

void retreat_putnici(int failure) 
{
    if (msgctl(msqid_putnici, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }
    exit(0);
}

void kanibal(int oznaka) {
	struct my_msgbuf buf;
	//int msqid;
	//int msqid_putnici;
	key_t key = getuid();
	key_t key_putnici = 123456;
	char text[] = "Ja sam kanibal";
	int zastavica = oznaka; 			// 		0 - kanibal   1 - misionar
	long kombinacija;
	
	int trenutno_kanibala = 0;
	int trenutno_misionara = 0;
	
	if ((msqid = msgget(key, 0600 | IPC_CREAT)) == -1) { // red poruka za razmjenu znacke
        perror("msgget");
        exit(1);
    }
	
	if ((msqid_putnici = msgget(key_putnici, 0600 | IPC_CREAT)) == -1) { // poruke koje ce ostali procesi slati brodu
        perror("msgget");
        exit(1);
    }
    
    sigset(SIGINT, retreat);
    sigset(SIGINT, retreat_putnici);
	
	while(1) {	 // cekaj na poruku
        if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Kanibal primio poruku: %s\n", buf.mtext);
		
		kombinacija = buf.mtype;
		switch(kombinacija) {
			case 1:
				trenutno_kanibala = 0;
				trenutno_misionara = 0;
				break;
			case 2:
				trenutno_kanibala = 0;
				trenutno_misionara = 1;
				break;
			case 3:
				trenutno_kanibala = 1;
				trenutno_misionara = 0;
				break;
			case 4:
				trenutno_kanibala = 1;
				trenutno_misionara = 1;
				break;
			case 5:
				trenutno_kanibala = 0;
				trenutno_misionara = 2;
				break;
			case 6:
				trenutno_kanibala = 2;
				trenutno_misionara = 0;
				break;
		}
		//printf("Trenutno u brodu- MISIONARA: %d   KANIBALA: %d\n", trenutno_misionara, trenutno_kanibala);
		
		if((trenutno_kanibala + trenutno_misionara) == 0 || (trenutno_kanibala + trenutno_misionara) == 1 || ((trenutno_kanibala + trenutno_misionara) == 2 
			&& ((trenutno_kanibala == 2) || (trenutno_misionara) == 2))) {		// kanibal moze uci u brod
				
				trenutno_kanibala++;
				
				// spremi tekst i tip poruke
				char text[] = "Kanibal ulazi u čamac";
				memcpy(buf.mtext, text, strlen(text)+1);
				buf.mtype = KANIBAL;	// tip znacke
				
				// posalji znacku natrag brodu
				printf("Kanibal: ulazim u čamac\n");
				if (msgsnd(msqid_putnici, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1)
					perror("msgsnd");

			
			break;
		}
		
		else {	// nema dozvolu za ulazak u camac
				// spremi tekst i tip poruke
				char text[] = "Kanibal ne može ući u čamac, šalje dalje značku";
				memcpy(buf.mtext, text, strlen(text)+1);
				buf.mtype = kombinacija;	// tip znacke
				
				// posalji znacku sljedecem procesu
				printf("Kanibal: šaljem dalje značku\n");
				usleep(500);
				if (msgsnd(msqid, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1)
					perror("msgsnd");
		}
    }
	
}

void misionar(int oznaka) {
	struct my_msgbuf buf;
	//int msqid;
	//int msqid_putnici;
	key_t key = getuid();
	key_t key_putnici = 123456;
	char text[] = "Ja sam misionar";
	int zastavica = oznaka; 			// 		0 - kanibal   1 - misionar
	long kombinacija;

	int trenutno_kanibala = 0;
	int trenutno_misionara = 0;
	
	if ((msqid = msgget(key, 0600 | IPC_CREAT)) == -1) { // red poruka za razmjenu znacke
        perror("msgget");
        exit(1);
    }
	
	if ((msqid_putnici = msgget(key_putnici, 0600 | IPC_CREAT)) == -1) { // poruke koje ce ostali procesi slati brodu
        perror("msgget");
        exit(1);
    }
    
    sigset(SIGINT, retreat);
    sigset(SIGINT, retreat_putnici);
	
	while(1) {	 // cekaj na poruku
        if (msgrcv(msqid, (struct msgbuf *)&buf, sizeof(buf)-sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }
        printf("Misionar primio poruku: %s\n", buf.mtext);
		
		kombinacija = buf.mtype;
		switch(kombinacija) {
			case 1:
				trenutno_kanibala = 0;
				trenutno_misionara = 0;
				break;
			case 2:
				trenutno_kanibala = 0;
				trenutno_misionara = 1;
				break;
			case 3:
				trenutno_kanibala = 1;
				trenutno_misionara = 0;
				break;
			case 4:
				trenutno_kanibala = 1;
				trenutno_misionara = 1;
				break;
			case 5:
				trenutno_kanibala = 0;
				trenutno_misionara = 2;
				break;
			case 6:
				trenutno_kanibala = 2;
				trenutno_misionara = 0;
				break;
		}
		
		//printf("Trenutno u brodu- MISIONARA: %d   KANIBALA: %d\n", trenutno_misionara, trenutno_kanibala);
		
		if(trenutno_kanibala + trenutno_misionara < 3 
			&& (trenutno_kanibala == 0 || trenutno_kanibala == 1)) {		// misionar moze uci u brod
				
				trenutno_misionara++;
				
				// spremi tekst i tip poruke
				char text[] = "Misionar ulazi u čamac";
				memcpy(buf.mtext, text, strlen(text)+1);
				buf.mtype = MISIONAR;	// tip znacke
				
				// posalji znacku natrag brodu
				printf("Misionar: ulazim u čamac\n");
				if (msgsnd(msqid_putnici, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1)
					perror("msgsnd");
			
			break;
		}
		
		else {	// nema dozvolu za ulazak u camac
				// spremi tekst i tip poruke
				char text[] = "Misionar ne može ući u čamac, šalje dalje značku";
				memcpy(buf.mtext, text, strlen(text)+1);
				buf.mtype = kombinacija;	// tip znacke
				
				// posalji znacku sljedecem procesu
				printf("Misionar: šaljem dalje značku\n");
				usleep(500);
				if (msgsnd(msqid, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1)
					perror("msgsnd");
		}
    }
	
}

int main(void) {
	struct my_msgbuf buf;
	//int msqid;
	//int msqid_putnici;
	key_t key = getuid();
	key_t key_putnici = 123456;
	char text[] = "Ja sam brod";
	int zastavica = 0; 			// 		0 - kanibal   1 - misionar
	long kombinacija = 1;
	int i, brojac = 0;
	srand(time(NULL));
	char random;
	
	int max_kanibala = 0;
	int max_misionara = 0;
	
	int broj_kanibala = 0;
	int broj_misionara = 0;

	int trenutno_kanibala = 0;
	int trenutno_misionara = 0;
	
	int pid = getpid();
	
	printf("\n Unesite koliko maksimalno želite MISIONARA: ");
	scanf("%d", &max_misionara);
	
	printf("\n Unesite koliko maksimalno želite KANIBALA: ");
	scanf("%d", &max_kanibala);
	
	if(max_kanibala == 0 || max_misionara == 0) {
		printf("\n Zadali ste nedozvoljeni broj kanibala ili misionara! \n\n Gasim program!");
		return(0);
	}
	
	// stvori redove poruka
	if ((msqid = msgget(key, 0600 | IPC_CREAT)) == -1) { // red poruka za razmjenu znacke
        perror("msgget");
        exit(1);
    }
    
    printf("\nBrod: Stvorio sam red poruka za značku! \n");
    
    if ((msqid_putnici = msgget(key_putnici, 0600 | IPC_CREAT)) == -1) { // poruke koje ce ostali procesi slati brodu
        perror("msgget");
        exit(1);
    }
    
    printf("Brod: Stvorio sam red poruka za putnike!\n\n");
    sleep(2);

	// stvori procese kanibale i misionare
	for (i = 0; i < max_kanibala + max_misionara; i++) {
		if(getpid() != pid) break;
		
		random = rand() % 2;
		if(random == 0 && broj_kanibala < max_kanibala || broj_misionara >= max_misionara) {
			zastavica = 0; // potrebno stvoriti kanibala
			broj_kanibala++;
		}
		else {
			zastavica = 1; // potrebno stvoriti misionara
			broj_misionara++;
		}
		switch (fork()) {
			case -1:
				printf("Ne mogu kreirati novi proces\n");
				break;
			case 0:
				if(zastavica == 0) {
					printf("stvoren KANIBAL\n");
					sleep(2);
					kanibal(0);	// pozovi funkciju kanibal
					break;
				}
				else {
					printf("stvoren MISIONAR\n");
					sleep(2);
					misionar(1);		// pozovi funkciju misionar
					break;
				}
		}
		if(getpid() != pid) break;
	}
	if(getpid() == pid) {
		sleep(2);
		printf("\nBrod: Šaljem značku prvom procesu\n");
		printf("Brod: ČEKAM NA PRVI UKRCAJ! \n\n");
		while(1) { 	// beskonacna petlja brod koji ceka
		
			// spremi tekst i tip poruke
			char text[] = "Dobio sam značku";
			memcpy(buf.mtext, text, strlen(text)+1);
			buf.mtype = kombinacija;	// tip znacke
			
			// posalji znacku
			
			if (msgsnd(msqid, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1)
				perror("msgsnd");
			
			while(1) {
				
				if(trenutno_kanibala + trenutno_misionara != 3) {
					if (msgrcv(msqid_putnici, (struct msgbuf *)&buf, sizeof(buf)-sizeof(long), 0, IPC_NOWAIT) != -1) {
						
						if(buf.mtype == KANIBAL)
							trenutno_kanibala++;
						else if(buf.mtype == MISIONAR)
							trenutno_misionara++;
							
						printf("\nBrod primio poruku: %s \n", buf.mtext);
						printf("Trenutno u brodu ---- MISIONARA: %d   KANIBALA: %d\n\n", trenutno_misionara, trenutno_kanibala);
						
						
						// racunanje kombinacije tipa poruke za slanje preko znacke
						if(trenutno_kanibala==0) {
							if(trenutno_misionara ==0)
								kombinacija = 1;
							else if(trenutno_misionara==1)
								kombinacija = 2;
							else if(trenutno_misionara == 2)
								kombinacija = 5;
						}
						else if(trenutno_kanibala == 1) {
							if(trenutno_misionara ==0)
								kombinacija = 3;
							else if(trenutno_misionara==1)
								kombinacija = 4;
						}
						else if(trenutno_kanibala == 2)
							kombinacija = 6;
						
						if(trenutno_kanibala + trenutno_misionara != 3) {
							char text[] = "Značka proslijeđena";
							memcpy(buf.mtext, text, strlen(text)+1);
							buf.mtype = kombinacija;	// tip znacke
						
							if (msgsnd(msqid, (struct msgbuf *)&buf, strlen(text) + 1, 0) == -1) // posalji novu znacku
								perror("msgsnd");
						}
					}
				}
				 
				 sleep(1);
				 brojac++;
				 if(brojac == MAX_VRIJEME) {
					 brojac = 0;
					 break;
				 }
			}
			
			if(trenutno_kanibala + trenutno_misionara != 0) {
				// camac prevozi
				printf("\nPREVEZENI:  ");
				for(i=0; i<trenutno_misionara; i++) {
					printf("  misionar");
				}
				for(i=0; i<trenutno_kanibala; i++) {
					printf("  kanibal");
				}
				printf("\n\n\n\n");
				
				trenutno_misionara = 0;
				trenutno_kanibala = 0;
				kombinacija = 1;
			}
			else
				break;
		}
	}
	else
		return 0;
	
	printf("\n\nBrod: čekam već 4 sekunde, GOTOV SAM !!!!!!\n\n");
	msgctl(msqid, IPC_RMID, NULL);	// obrisi red poruka
	msgctl(msqid_putnici, IPC_RMID, NULL);	// obrisi red poruka
	return 0;
}



