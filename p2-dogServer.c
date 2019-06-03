#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

void pauseShell()
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	printf("Presione cualquier tecla para continuar...");
	int c = getchar();
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("pause");
#endif
}

void openFile(char fileName[256])
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	char path[256] = "gedit";
	strcat(path, fileName);
	system(path);
#endif

#if defined(_WIN32) || defined(_WIN64)
	ShellExecute(NULL, NULL, fileName, NULL, NULL, SW_SHOW);
#endif
}

int REGISTROS;
int HASHSIZE = 1999;
int lastHashIndex[2000];
int medicalCreated = 0;

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

void tolowerCase(char *str)
{
	unsigned char *p = (unsigned char *)str;

	while (*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}
}

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
	int d = fseek(f, index * sizeof(struct DogType), SEEK_SET);
	if (d == -1)
	{
		printf("error al mover al index\n");
	}

	int r = fread(&reg, sizeof(struct DogType), 1, f);
	if (r == 0)
	{
		perror("Could no read structure");
		exit(-1);
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
	/*int d = fseek(f, 0 * sizeof(struct DogType), SEEK_SET);
	if (d == -1)
	{
		printf("error al mover al index\n");
	}
*/
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

int validateMenuInput(char input[MAXINPUT])
{

	int length = strlen(input);
	for (int i = 0; i < length; i++)
	{
		if (!isdigit(input[i]))
		{
			printf("Input no valido\n\n");
			return 0;
			exit(1);
		}
	}
	return 1;
}

int validateInteger(char input[MAXINPUT])
{

	int length = strlen(input);
	for (int i = 0; i < length; i++)
	{
		if (!isdigit(input[i]))
		{
			return 0;
			exit(1);
		}
	}
	return 1;
}

int validateFloat(char input[MAXINPUT])
{

	double value;
	char *endptr;
	value = strtod(input, &endptr);
	if ((*endptr == '\0') || (isspace(*endptr) != 0))
		return 1;
	else
		return 0;
}

int executeMenu()
{

	int selectedOption;
	printf("---------------------------------------------------------------------------\n");
	printf("MENU PRINCIPAL\nSeleccione la el numero de la opcion que desea ejecutar\n\n");
	printf("1 - Ingresar registro\n");
	printf("2 - Ver registro\n");
	printf("3 - Borrar registro\n");
	printf("4 - Buscar registro\n");
	printf("5 - Salir\n");
	printf("---------------------------------------------------------------------------\n");

	char menuInput[MAXINPUT] = "";
	do
	{
		printf("Opcion: ");
		gets(menuInput);
	} while (!validateMenuInput(menuInput));

	sscanf(menuInput, "%d", &selectedOption);
	return selectedOption;
}

int validateRegValue(int type, char input[MAXINPUT])
{

	switch (type)
	{
	case 1:
		if (strlen(input) <= 32)
		{
			return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	case 2:
		if (strlen(input) <= 32)
		{
			return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	case 3:
		if (validateInteger(input))
		{
			int value;
			sscanf(input, "%d", &value);
			if (value < 2147483648 && value >= 0)
				return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}

		break;

	case 4:
		if (strlen(input) <= 16)
		{
			return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	case 5:
		if (validateInteger(input))
		{
			int value;
			sscanf(input, "%d", &value);
			if (value < 2147483648 && value >= 0)
				return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	case 6:
		if (validateFloat(input))
		{
			float value;
			sscanf(input, "%lf", &value);
			if (value < 3.40282e+38 && value >= 0.0)
				return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	case 7:
		if (strlen(input) == 1 && (strcmp(input, "H") == 0 || strcmp(input, "M") == 0))
		{
			return 1;
		}
		else
		{
			printf("El dato ingresado no es valido\n\n");
			return 0;
		}
		break;

	default:
		return 1;
		break;
	}
}

int eraseFunction(int sizeOfRegisters, int indexToDelete)
{
	FILE *f;
	FILE *fp2;
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

	clearScreen();
	if (remove("dataDogs.dat") == 0)
	{
		printf("---------------------------------------------------------------------------\n");
		printf("EL REGISTRO FUE ELIMINADO CORRECTAMENTE\n");
		printf("---------------------------------------------------------------------------\n");
	}
	else
	{
		printf("---------------------------------------------------------------------------\n");
		printf("ERROR: NO SE PUDO ELIMINAR EL REGISTRO\n");
		printf("---------------------------------------------------------------------------\n");
	}

	if (remove(path) == 0)
	{
		printf("LA HISTORIA CLINICA FUE ELIMINADA CORRECTAMENTE\n");
		printf("---------------------------------------------------------------------------\n");
	}
	else
	{
		printf("NO SE ENCONTRO HISTORIA CLINICA\n");
		printf("---------------------------------------------------------------------------\n");
	}

	int err = rename("dataDogsCopy.dat", "dataDogs.dat");

	if (err == -1)
	{
		puts("Error al renombrar el archivo");
	}

	writeHash();
	readHash();

	REGISTROS--;
	return 0;
}

void findByName(int* sockID, char name[32], FILE *f){
	int hash = calculateHash(name);
	int lastIndex = lastHashIndex[hash];
	int count = 0;
	int t;
	struct DogType reg;

	if (lastIndex == -1)
	{
		printf("---------------------------------------------------------------------------\n");
		printf("NO EXISTEN REGISTROS CON EL NOMBRE %s\n", name);
		printf("---------------------------------------------------------------------------\n\n");
	}
	else{
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
		if (count != 0)
		{
			printf("---------------------------------------------------------------------------\n");
			printf("HAY %d REGISTRO/S CON EL NOMBRE %s\n", count, name);
			printf("---------------------------------------------------------------------------\n\n");
		}
		else
		{
			printf("---------------------------------------------------------------------------\n");
			printf("NO EXISTEN REGISTROS CON EL NOMBRE %s\n", name);
			printf("---------------------------------------------------------------------------\n\n");
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
void executeOption(int* sockId, int menuOption){
	switch(menuOption){
		case 1:
			readHash();

			struct DogType newReg;
			
			int v = recv(sockId, &newReg, sizeof(struct DogType), 0);
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
			break;

		case 2:
			printf("");
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
			v = send(sockId, &searchedReg, sizeof(struct DogType), 0);
			if(v ==-1){
				perror("Error Enviando estructura ");
			}
			close(f);
			v =  recv(sockId, &hist, sizeof(int), 0);
			if(v ==-1){
				perror("Error recibiendo Opcion historia clinica");
			}
			printf("historia  %d\n", hist);
			if(hist == 1){
				printf("Si historia clinica\n");
				int number = data2;
				if (searchedReg.medicalHistoryID == -1){
						printf("Medical %d, data2 %d\n", medicalCreated, data2);
						data2 = medicalCreated;
						searchedReg.medicalHistoryID = medicalCreated;
						medicalCreated++;
						printf("Medical %d, data2 %d\n", medicalCreated, data2);
						int d = fseek(f, number * sizeof(struct DogType), SEEK_SET);
						if (d == -1)
						{
							printf("error al mover al index\n");
						}
						int r = fwrite(&searchedReg, sizeof(struct DogType), 1, f);
						printf("escritos %d\n", r);
						if (r == 0)
						{
							perror("Could not write Struct");
							exit(-1);
						}
						d = fseek(f, 0 * sizeof(struct DogType), SEEK_SET);
						if (d == -1)
						{
							printf("error al regresar al inicio \n");
						}
						fclose(f);
						f = fopen("dataDogs.dat", "rb+");
						if (f == NULL)
						{
							perror("Could not open a file");
							exit(-1);
						}
					}
					else{
						data2 = searchedReg.medicalHistoryID;
					}
					char fileName[256] = "Historias_clinicas/";
					char fileNameNumber[256];

					sprintf(fileNameNumber, "%d", data2);
					strcat(fileName, fileNameNumber);
					strcat(fileName, ".txt");
					printf("path %s\n", fileName);
					if (!(access(fileName, F_OK) != -1))
					{

						FILE *g = fopen(fileName, "w");

						if (g == NULL){
							printf("Error opening file!\n");
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
							printf("Enviando\n");
							v = send(sockId, sendbuffer, b, 0);
							if(v ==-1){
								perror("Error Enviando estructura ");
							}
						}
						if(b < 1024){
							if(feof(fp)){
								int flag = -1;
								printf("Fin archivo");
								
							}
							break;
						}
					}
					

					fclose(fp);
					printf("Termino\n\n");
					
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
			break;
		case 3:
			printf("");
			int data3;
			v =  recv(sockId, &data3, sizeof(int), 0);
			if(v ==-1){
				perror("Error recibiendo index ");
			}
			eraseFunction(REGISTROS, data3);
			int confirm = REGISTROS;
			v = send(sockId, &confirm, sizeof(int), 0);
			if(v == -1){
				perror("Error Enviando confirmacion ");
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
			break;
		case 5:
			printf("\nentro a caso 5\n");
			break;
		default:
			printf("Desfault\n");
			break;
	}
}


int main(){

	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("CARGANDO PROGRAMA ...\n");
	printf("---------------------------------------------------------------------------\n");
	printf("leyendo int\n");
	medicalCreated = readInt();
	readHash();
	printf("Arbiendo archivo\n");
	FILE *f;
	f = fopen("dataDogs.dat", "rb+");

	if (f == NULL){
		perror("Could not open a file");
		exit(-1);
	}

	REGISTROS = countRecords(f);
	close(f);
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
	printf("Bind\n");
	r = bind(fd, (struct sockaddr_in*)&server, tama);

	if(r == -1){
		perror("error en bind");
	}

	//Listen del server
	printf("Listen\n");
	r = listen(fd, BACKLOG);
	if(r == -1){
		perror("error en listen");
	}

	printf("Waiting\n");

	//Conexion con un cliente 
	tamaClient = 0;
	//fd1 = accept(fd, (struct sockaddr_in*)&client1, &tamaClient);
	int b;
	fd1  = accept(fd, (struct sockaddr_in*)&client1, &tamaClient);
	if(fd1 <= 0){
		perror("Error en accept");
	}
	/*
	while(recv(fd1, &b, sizeof(b), 0)){
		
		if(r ==-1){
			perror("Error recibiendo opcion ");
		}else if(b == -1){
			break;
		}else{
			printf(" %d\n",b);
		}
	}
	*/

	clearScreen();
	printf("\nBIENVENIDO USUARIO\n");

	r = send(fd1, &REGISTROS, sizeof(REGISTROS), 0);
	if(r == -1 ){
		perror("Error al enviar cantidad de registros");
	}
	
	while(1){
		r = recv(fd1, &b, sizeof(b), 0);
		if(r == -1){
			perror("Error al recibir opcion");
		}
		
		printf("Opcion selecionada %d\n", b);
		executeOption(fd1, b);
		if(b == 5){
			break;
		}
	}

	

	writeInt(&medicalCreated);
	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("Finalizando programa ...\n");
	printf("---------------------------------------------------------------------------\n");
	fclose(f);
	close(fd1);
	close(fd);
	pauseShell();

	return 0;
}