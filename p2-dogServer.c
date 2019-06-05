#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<pthread.h>

#define MAXINPUT 256
#define BACKLOG 32
#define PORT 3535

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <ShellApi.h>
#endif

void clearScreen()
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("cls");
#endif
}

int REGISTROS;
int HASHSIZE = 1999;
int lastHashIndex[2000];
int medicalCreated = 0;
int confirmSignal = 1;

struct DogType
{
	char name[32];
	char type[32];
	int age;
	char breed[16];
	int height;
	float weight;
	char sex;
	bool deleted;
	int index;
	int prevHashIndex;
	int medicalHistoryID;
};

struct ThreadParameters{
	int socketDescriptor;
	struct sockaddr_in socketStruct;
	char ip[INET_ADDRSTRLEN];
};

int writeRegister(void *ap, int position)
{

	struct DogType *dato;
	dato = ap;
	FILE *f;

	f = fopen("dataDogs.dat", "ab");
	int d = fseek(f, position * sizeof(struct DogType), SEEK_SET);
	if (f == NULL)
	{
		perror("Could not open a file");
		exit(-1);
	}
	int r = fwrite(dato, sizeof(struct DogType), 1, f);

	if (r == 0)
	{
		perror("Could not write Struct");
		exit(-1);
	}
	fclose(f);
	return 0;
}

void findByIndex(struct DogType *ap, int index, FILE *f)
{

	struct DogType reg;

	int d = fseek(f, 0 * sizeof(struct DogType), SEEK_SET);
	if (d == -1)
	{
		printf("error al mover al index\n");
	}
	d = fseek(f, index * sizeof(struct DogType), SEEK_SET);
	if (d == -1)
	{
		printf("error al mover al index\n");
	}

	int r = fread(&reg, sizeof(struct DogType), 1, f);

	if (r == 0){
		perror("Could no read structure");
		exit(-1);
	}

	 d = fseek(f, 0 * sizeof(struct DogType), SEEK_SET);
	if (d == -1)
	{
		printf("error al mover al index\n");
	}

	strcpy(ap->name, reg.name);
	strcpy(ap->type, reg.type);
	ap->age = reg.age;
	strcpy(ap->breed, reg.breed);
	ap->height = reg.height;
	ap->weight = reg.weight;
	ap->sex = reg.sex;
	ap->deleted = reg.deleted;
	ap->index = reg.index;
	ap->prevHashIndex = reg.prevHashIndex;
	ap->medicalHistoryID = reg.medicalHistoryID;
}

int countRecords(FILE *f)
{
	int r;
	r = fseek(f, 0 * sizeof(struct DogType), SEEK_SET);
	if (r == -1)
	{
		printf("error al mover al index\n");
	}
	struct DogType perro = {"", "", 0, "", 0, 0.0, 'f', 0, 0};
	int count = 0;
	while (fread(&perro, sizeof(struct DogType), 1, f) != 0)
	{
		count++;
	}
	return count;
}

unsigned int calculateHash(const char *word)
{
	unsigned int hash = 0, c;

	for (size_t i = 0; word[i] != '\0'; i++)
	{
		c = (unsigned char)word[i];
		hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
	}
	return hash % HASHSIZE;
}

void writeHash()
{
	int status = remove("hash.dat");
	if (status == 0)
	{
		printf("Hash file deleted\n");
	}

	FILE *f;
	f = fopen("hash.dat", "ab+");

	if (f == NULL)
	{
		perror("Could not open file");
		exit(-1);
	}

	int r = fwrite(lastHashIndex, sizeof(lastHashIndex), 1, f);

	if (r == 0)
	{
		perror("Could not write Struct");
		exit(-1);
	}
	fclose(f);
}

void readHash()
{
	
	FILE *f;
	f = fopen("hash.dat", "rb");
	if (f == NULL)
	{
		perror("Could not open file");
		exit(-1);
	}

	int r = fread(lastHashIndex, sizeof(lastHashIndex), 1, f);

	if (r == 0)
	{
		perror("Could not read Struct hash");
		exit(-1);
	}
	fclose(f);
}

int eraseFunction(int sizeOfRegisters, int indexToDelete)
{
	FILE *f;
	FILE *fp2;
	int returnValue = 0;
	for (int i = 0; i < 2000; i++)
	{
		lastHashIndex[i] = -1;
	}

	struct DogType reg;
	fp2 = fopen("dataDogsCopy.dat", "ab+");

	if (fp2 == NULL)
	{
		perror("Could not open a file");
		exit(-1);
	}

	f = fopen("dataDogs.dat", "ab+");
	if (f == NULL)
	{
		perror("Could not open a file");
		exit(-1);
	}
	///BORRADO DE HISTORIA MEDICA

	struct DogType aux;

	findByIndex(&aux, indexToDelete, f);

	int iDMedical = aux.medicalHistoryID;

	char fileNameNumber[256];
	char path[256] = "Historias_clinicas/";
	sprintf(fileNameNumber, "%d", iDMedical);
	strcat(path, fileNameNumber);
	strcat(path, ".txt");
	int yt = fseek(f, 0, SEEK_SET);

	for (int i = 0; i < indexToDelete; i++)
	{
		int g = fread(&reg, sizeof(struct DogType), 1, f);
		reg.index = i;
		int hash = calculateHash(reg.name);
		reg.prevHashIndex = lastHashIndex[hash];
		lastHashIndex[hash] = i;
		int gg = fwrite(&reg, sizeof(struct DogType), 1, fp2);
		int seek = fseek(f, 0, SEEK_CUR);
	}

	if (fp2 == NULL)
	{
		perror("Could not open a file");
		exit(-1);
	}

	if (!(indexToDelete == (sizeOfRegisters - 1)))
	{

		int seek = fseek(f, ((indexToDelete + 1) * sizeof(struct DogType)), SEEK_SET);

		for (int j = (indexToDelete + 1); j < sizeOfRegisters; j++)
		{
			int g = fread(&reg, sizeof(struct DogType), 1, f);
			reg.index = (j - 1);
			int hash = calculateHash(reg.name);
			reg.prevHashIndex = lastHashIndex[hash];
			lastHashIndex[hash] = (j - 1);
			int gg = fwrite(&reg, sizeof(struct DogType), 1, fp2);
		}
	}

	fclose(f);
	fclose(fp2);

	if (remove("dataDogs.dat") == 0)
	{
		returnValue = 1;
	}
	else
	{
		returnValue = -1;
	}

	if (remove(path) == 0)
	{
		returnValue = 2;
	}
	else
	{
		returnValue = 0;
	}

	int err = rename("dataDogsCopy.dat", "dataDogs.dat");

	if (err == -1)
	{
		puts("Error al renombrar el archivo");
	}

	writeHash();
	readHash();

	REGISTROS--;
	return returnValue;
}

void findByName(int* sockID, char name[32], FILE *f){

	int hash = calculateHash(name);
	int lastIndex = lastHashIndex[hash];
	int count = 0;
	int t;
	struct DogType reg;

	if (lastIndex != -1){
		while(lastIndex != -1){
			findByIndex(&reg, lastIndex, f);
			if (strcmp(reg.name, name) == 0){
				count++;
			}
			lastIndex = reg.prevHashIndex;
		}

		t = send(sockID, &count, sizeof(int), 0);
		lastIndex = lastHashIndex[hash];
		if(t == -1){
			perror("Error Enviando Count");
		}

		while (lastIndex != -1)
		{
			findByIndex(&reg, lastIndex, f);
			if (strcmp(reg.name, name) == 0){
				t = send(sockID, &reg, sizeof(struct DogType), 0);
				if(t == -1){
					perror("Error Enviando estructura");
				}
			}
			lastIndex = reg.prevHashIndex;
		}
	}else{
		count = 0;
		t = send(sockID, &count, sizeof(int), 0);
		lastIndex = lastHashIndex[hash];
		if(t == -1){
			perror("Error Enviando Count");
		}
	}
}

void writeInt(int *write)
{
	FILE *f;
	f = fopen("count.dat", "w+");

	if (f == NULL)
	{
		perror("Could not open file");
		exit(-1);
	}

	int r = fwrite(write, sizeof(write), 1, f);

	if (r == 0)
	{
		perror("Could not write Struct");
		exit(-1);
	}
	fclose(f);
}

int readInt()
{
	FILE *f;
	int aux;
	if ((access("count.dat", F_OK) != -1))
	{
		f = fopen("count.dat", "r");

		if (f == NULL)
		{
			perror("Could not open file number");
			exit(-1);
		}
		int r = fread(&aux, sizeof(aux), 1, f);
		if (r == 0)
		{
			perror("Could not read Struct int");
			exit(-1);
		}
		return aux;
	}
	else
	{
		return 0;
	}
}

void executeOption(int* sockId, int menuOption, char *ipstr){

	FILE *log;

	log = fopen("serverDogs.log", "ab+");
	if(log == NULL){
		perror("Error abriendo log file");
	}

	time_t timeinfo = time(NULL);
	struct tm tm = *localtime(&timeinfo);
	char date[80];
	char time[80];
	strftime(date, 80 ,"%Y%m%d", &tm);	

	switch(menuOption){
		case 1:
			readHash();

			int v = send(sockId, &REGISTROS, sizeof(int), 0);
			if(v == -1){
				perror("Error Enviando confirmacion ");
			}

			struct DogType newReg;
			
			v = recv(sockId, &newReg, sizeof(struct DogType), 0);
			if(v ==-1){
				perror("Error recibiendo struct ");
			}

			int newReghash = calculateHash(newReg.name);
			newReg.prevHashIndex = lastHashIndex[newReghash];
			lastHashIndex[newReghash] = newReg.index;

			writeRegister(&newReg, newReg.index);
			REGISTROS++;

			writeHash();
			readHash();

			strftime(time, 80 ,"%H%M%S", &tm);	
			fprintf(log, "[%sT%s] Cliente[%s][insercion][%d]\n", date, time, ipstr, REGISTROS);
			break;

		case 2:
			printf("");

			v = send(sockId, &REGISTROS, sizeof(int), 0);
			if(v == -1){
				perror("Error Enviando confirmacion ");
			}

			v = recv(sockId, &confirmSignal, sizeof(int), 0);
			if(v ==-1){
				perror("Error recibiendo struct ");
			}

			if(confirmSignal == 1){
				int data2, hist;
				struct DogType searchedReg;
				v =  recv(sockId, &data2, sizeof(int), 0);
				if(v ==-1){
					perror("Error recibiendo index ");
				}
				FILE *f;
				f = fopen("dataDogs.dat", "rb+");

				if (f == NULL){
					perror("Could not open a file");
					exit(-1);
				}

				findByIndex(&searchedReg, data2, f);

				strftime(time, 80 ,"%H%M%S", &tm);	
				fprintf(log, "[%sT%s] Cliente[%s][lectura][%d]\n", date, time, ipstr, (data2 + 1));

				v = send(sockId, &searchedReg, sizeof(struct DogType), 0);
				if(v ==-1){
					perror("Error Enviando estructura ");
				}
				close(f);

				v =  recv(sockId, &hist, sizeof(int), 0);
				if(v ==-1){
					perror("Error recibiendo Opcion historia clinica");
				}
				fclose(f);
				if(hist == 1){
					int id = sockId;
					v = send(sockId, &id, sizeof(int), 0);
					if(v ==-1){
						perror("Error Enviando estructura ");
					}
					medicalCreated = readInt();
					int number = data2;
					if (searchedReg.medicalHistoryID == -1){
							FILE *h;
							h = fopen("dataDogs.dat", "rb+");
							//printf("Medical %d, data2 %d\n", medicalCreated, data2);
							data2 = medicalCreated;
							searchedReg.medicalHistoryID = medicalCreated;
							medicalCreated++;
							writeInt(&medicalCreated);

							int d = fseek(h, 0 * sizeof(struct DogType), SEEK_SET);
							if (d == -1)
							{
								printf("error al regresar al inicio \n");
							}

							d = fseek(h, number * sizeof(struct DogType), SEEK_SET);
							if (d == -1)
							{
								printf("error al mover al index\n");
							}
							int r = fwrite(&searchedReg, sizeof(struct DogType), 1, h);
							
							if (r == 0)
							{
								perror("Could not write Struct");
								exit(-1);
							}
							d = fseek(h, 0 * sizeof(struct DogType), SEEK_SET);
							if (d == -1)
							{
								printf("error al regresar al inicio \n");
							}
							fclose(h);
							
							if (f == NULL)
							{
								perror("Could not open a file");
								exit(-1);
							}

							medicalCreated = readInt();
						}
						else{
							data2 = searchedReg.medicalHistoryID;
						}
						char fileName[256] = "Historias_clinicas/";
						char fileNameNumber[256];
						

						sprintf(fileNameNumber, "%d", data2);
						
						strcat(fileName, fileNameNumber);
						strcat(fileName, ".txt");
						if (!(access(fileName, F_OK) != -1))
						{

							FILE *g = fopen(fileName, "w");

							if (g == NULL){
								perror("Creen la carpeta primero impedidos\n");
								exit(1);
							}

							fprintf(g, "---------------------------------------------------------------------------\n");
							fprintf(g, "Datos del paciente\n");
							fprintf(g, "\nname: %s\n", searchedReg.name);
							fprintf(g, "type: %s\n", searchedReg.type);
							fprintf(g, "age: %d\n", searchedReg.age);
							fprintf(g, "breed: %s\n", searchedReg.breed);
							fprintf(g, "height: %d\n", searchedReg.height);
							fprintf(g, "weight: %.2f\n", searchedReg.weight);
							fprintf(g, "sex: %c\n", searchedReg.sex);
							fprintf(g, "---------------------------------------------------------------------------\n");
							fprintf(g, "Historia clinica: ");

							fclose(g);
						}

						FILE *fp = fopen(fileName, "ab+");

						if(fp == NULL){
							perror("File");
							return 2;
						}

						while(1){
							char sendbuffer[1024] = "";
							int b = fread(sendbuffer, 1, sizeof(sendbuffer), fp);
							if( b > 0){
								v = send(sockId, sendbuffer, b, 0);
								if(v ==-1){
									perror("Error Enviando estructura ");
								}
							}
							if(b < 1024){
								if(feof(fp)){
									int flag = -1;
								}
								break;
							}
						}
						
						fclose(fp);

						int b = 0;
						char recBuf[1024] = "";
						FILE* t = fopen(fileName, "w");
						if(t == NULL){
							perror("Abriendo archivo");
						}

						do{
							b = recv(sockId, recBuf, 1024, 0);
							fprintf(t, "%s", recBuf);
						}while(b == 1024);

						fclose(t);
				}

			}
			
			break;
		case 3:
			printf("");

			v = send(sockId, &REGISTROS, sizeof(int), 0);
			if(v == -1){
				perror("Error Enviando confirmacion ");
			}

			v = recv(sockId, &confirmSignal, sizeof(int), 0);
			if(v ==-1){
				perror("Error recibiendo struct ");
			}

			if(confirmSignal == 1){
				int data3;

				v =  recv(sockId, &data3, sizeof(int), 0);
				if(v ==-1){
					perror("Error recibiendo index ");
				}

				int functionStatus = eraseFunction(REGISTROS, data3);
				v = send(sockId, &functionStatus, sizeof(int), 0);
				if(v == -1){
					perror("Error Enviando confirmacion ");
				}

				strftime(time, 80 ,"%H%M%S", &tm);	
				fprintf(log, "[%sT%s] Cliente[%s][borrado][%d]\n", date, time, ipstr, (data3 + 1));
			}
			
			break;
		case 4:
			printf("");
			char data4[256];
			v =  recv(sockId, data4, sizeof(data4), 0);
			if(v ==-1){
				perror("Error recibiendo nombre");
			}
			FILE *g;
			g = fopen("dataDogs.dat", "rb+");

			if (g == NULL){
				perror("Could not open a file");
				exit(-1);
			}
			findByName(sockId, data4, g);
			close(g);

			strftime(time, 80 ,"%H%M%S", &tm);	
			fprintf(log, "[%sT%s] Cliente[%s][busqueda][%s]\n", date, time, ipstr, data4);

			break;
		case 5:
			break;
		default:
			printf("Default\n");
			break;
	}

	fclose(log);
}

void *connection_handler(void *client){
	
	struct ThreadParameters client1 =  *(struct ThreadParameters *)client;
	int fd1 = client1.socketDescriptor;
	int b;
	FILE *f;
	f = fopen("dataDogs.dat", "rb+");

	if (f == NULL){
		perror("Could not open a file");
		exit(-1);
	}

	REGISTROS = countRecords(f);
	close(f);

	printf("---------------------------------------------------------------------------\n");
	printf("CONEXION ESTABLECIDA\n");
	printf("---------------------------------------------------------------------------\n");

	int r = send(fd1, &REGISTROS, sizeof(REGISTROS), 0);
	if(r == -1 ){
		perror("Error al enviar cantidad de registros");
	}
	
	while(1){		
		r = recv(fd1, &b, sizeof(b), 0);
		if(r == -1){
			perror("Error al recibir opcion");
		}

		executeOption(fd1, b, client1.ip);
		if(b == 5){
			break;
		}
	}

	writeInt(&medicalCreated);
	fclose(f);
	close(fd1);
	
}

int main(){

	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("CARGANDO DATOS ...\n");
	printf("---------------------------------------------------------------------------\n");
	medicalCreated = readInt();
	readHash();

	
	struct sockaddr_in server, client1;
	size_t tama, tamaClient;
	int r;
	int fd, fd1;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	if(fd == -1){
		perror("error al crear socket");
	}

	//Parametros de la estructura sockaddr_in server
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(server.sin_zero, 8);

	//Bind del server
	tama = sizeof( struct sockaddr_in);
	r = bind(fd, (struct sockaddr_in*)&server, tama);

	if(r == -1){
		perror("error en bind");
	}

	//Listen del server
	r = listen(fd, BACKLOG);
	if(r == -1){
		perror("error en listen");
	}

	printf("ESPERANDO CONEXION\n");

	//Conexion con un cliente 
	tamaClient = 0;
	pthread_t thread_id;
	while(fd1  = accept(fd, (struct sockaddr_in*)&client1, &tamaClient)){
		struct ThreadParameters *client =  malloc(sizeof(struct ThreadParameters));

		char ipstr[INET_ADDRSTRLEN];
		inet_ntop( AF_INET, &client1.sin_addr, ipstr, INET_ADDRSTRLEN );
		//printf("ip: %s\n", ipstr);
		
		client->socketDescriptor = fd1;
		client->socketStruct = client1;
		strcpy(client->ip,  ipstr);
		
		if( pthread_create( &thread_id , NULL , connection_handler , (void*) client) < 0){
			perror("could not create thread");
		}
		

	}
	close(fd);
	return 0;
}