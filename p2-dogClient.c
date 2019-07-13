#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>


#define PORT 3535
#define MAXINPUT 256

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
	printf("Presione cualquier tecla para continuar...\n");
	int ch;
    struct termios oldt, newt;
    tcgetattr ( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
#endif

#if defined(_WIN32) || defined(_WIN64)
	system("pause");
#endif
}

void openFile(char fileName[256])
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	char path[256] = "gedit ";
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
int errorSignal = -1;
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

void tolowerCase(char *str)
{
	unsigned char *p = (unsigned char *)str;

	while (*p)
	{
		*p = tolower((unsigned char)*p);
		p++;
	}
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



int main(){

	clearScreen();
	
	struct sockaddr_in server;
	int fd, fd1;
	size_t tama;
	int r;
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1){
		perror("error al crear socket");
	}

	//Parametros de la estructura sockaddr_in server
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(server.sin_zero, 8);
	tama = sizeof( struct sockaddr_in);

	//Conexion con el servidor desde el cliente
	r = connect(fd, (struct sockaddr_in*)&server, tama);

	if(r == -1){
		printf("---------------------------------------------------------------------------\n");
		printf("NO SE PUDO ESTABLECER UNA CONEXION CON EL SERVIDOR\n");
		printf("---------------------------------------------------------------------------\n");
		exit(0);
	}
	int regs;

	r = recv(fd, &regs, sizeof(int), 0);
	if(r == -1 ){
		printf("---------------------------------------------------------------------------\n");
		printf("NO SE PUDO LEER LOS DATOS DEL SERVIDO CORRECTAMENTE\n");
		printf("---------------------------------------------------------------------------\n");
		exit(0);
	}else{
		REGISTROS = regs;

		printf("---------------------------------------------------------------------------\n");
		printf("CONEXION CON EL SERVIDOR ESTABLECIDA\n");
		printf("---------------------------------------------------------------------------\n");
		printf("SE RECIBIERON %d REGISTROS\n", REGISTROS);
		printf("---------------------------------------------------------------------------\n");
		pauseShell();
		clearScreen();
		printf("\nBIENVENIDO USUARIO\n");
	}

	int menuOption = executeMenu();
	while (menuOption != 5)
	{	
		clearScreen();
		switch (menuOption)
		{
		case 1:
			printf("");

			r = send(fd, &menuOption, sizeof(int), 0);
			if(r ==-1){
				perror("Error Enviando opcion de menu ");
			}

			printf("Espere un momento porfavor ...");

			r =  recv(fd, &REGISTROS, sizeof(int), 0);
			if(r ==-1){
				perror("Error recibiendo index ");
			}

			clearScreen();
			printf("---------------------------------------------------------------------------\n");
			printf("INGRESAR REGISTRO\n");
			printf("---------------------------------------------------------------------------\n\n");

			char regInput[MAXINPUT];

			do
			{
				printf("Ingrese el NOMBRE del animal (Max 32 caracteres): ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(1, regInput));

			char name[32];
			strcpy(name, regInput);
			tolowerCase(name);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese el TIPO de animal (Max 32 caracteres): ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(2, regInput));

			char type[32];
			strcpy(type, regInput);
			tolowerCase(type);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese la EDAD del animal (anos): ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(3, regInput));

			int age;
			sscanf(regInput, "%d", &age);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese la RAZA del animal: ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(4, regInput));

			char breed[16];
			strcpy(breed, regInput);
			tolowerCase(breed);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese la ESTATURA del animal (En centimetros): ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(5, regInput));

			int height;
			sscanf(regInput, "%d", &height);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese el PESO del animal (En kilogramos): ");
				fgets(regInput, MAXINPUT, stdin);
			} while (!validateRegValue(6, regInput));

			float weight;
			weight = strtod(regInput, NULL);
			printf("---------------------------------------------------------------------------\n");

			do
			{
				printf("Ingrese el SEXO del animal (H o M): ");
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(7, regInput));
			printf("---------------------------------------------------------------------------\n");

			char sex = regInput[0];

			struct DogType *newReg = malloc(sizeof(struct DogType));

			strcpy(newReg->name, name);
			strcpy(newReg->type, type);
			newReg->age = age;
			strcpy(newReg->breed, breed);
			newReg->height = height;
			newReg->weight = weight;
			newReg->sex = sex;
			newReg->deleted = false;
			newReg->index = REGISTROS;
			newReg->medicalHistoryID = -1;
			
			pauseShell();

			r = send(fd, newReg, sizeof(struct DogType), 0);
			if(r ==-1){
				perror("Error Enviando opcion de menu ");
			}
			
			REGISTROS++;
			clearScreen();
			printf("---------------------------------------------------------------------------\n");
			printf("EL REGISTRO HA SIDO CREADO CORRECTAMENTE\n");
			printf("---------------------------------------------------------------------------\n\n");
			printf("RESUMEN:\n\n");
			printf("Registro numero %d\n", ((newReg->index) + 1));
			printf("name: %s\n", newReg->name);
			printf("type: %s\n", newReg->type);
			printf("age: %d\n", newReg->age);
			printf("breed: %s\n", newReg->breed);
			printf("height: %d\n", newReg->height);
			printf("weight: %.2lf\n", newReg->weight);
			printf("sex: %c\n", newReg->sex);

			pauseShell();
			break;

		case 2:
			printf("");

			r = send(fd, &menuOption, sizeof(int), 0);
			if(r ==-1){
				perror("Error Enviando opcion de menu ");
			}

			printf("Espere un momento porfavor ...");

			r =  recv(fd, &REGISTROS, sizeof(int), 0);
			if(r ==-1){
				perror("Error recibiendo index ");
			}
			
			clearScreen();
			printf("---------------------------------------------------------------------------\n");
			printf("VER REGISTRO\n");
			printf("---------------------------------------------------------------------------\n");
			printf("NUMERO DE REGISTROS: %d\n", REGISTROS);
			printf("---------------------------------------------------------------------------\n\n");

			char numberInput[MAXINPUT];
			int regNumber;
			do{
				printf("Ingrese el numero de registro que desea ver: ");
				gets(numberInput);
				if (!validateInteger(numberInput))
				{
					printf("El numero ingresado no es valido\n\n");
				}
				else
				{
					break;
				}
			} while (true);

			sscanf(numberInput, "%d", &regNumber);

			if (regNumber > 0 && regNumber <= REGISTROS){
				confirmSignal = 1;
				r = send(fd, &confirmSignal, sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando opcion de menu ");
				}
				struct DogType searchedReg;

				regNumber--;
				r = send(fd, &regNumber , sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando indice");
				}
				r = recv(fd, &searchedReg, sizeof(struct DogType), 0);
				if(r ==-1){
					perror("Error recibiendo Estructura");
				}

				clearScreen();
				printf("---------------------------------------------------------------------------\n\n");
				printf("Registro %d\n", (regNumber + 1));
				printf("\nname: %s\n", searchedReg.name);
				printf("type: %s\n", searchedReg.type);
				printf("age: %d\n", searchedReg.age);
				printf("breed: %s\n", searchedReg.breed);
				printf("height: %d\n", searchedReg.height);
				printf("weight: %.2lf\n", searchedReg.weight);
				printf("sex: %c\n", searchedReg.sex);
				printf("hash: %d\n", calculateHash(searchedReg.name));
				printf("prev hash index: %d\n\n", searchedReg.prevHashIndex);
				printf("medical id: %d\n\n", searchedReg.medicalHistoryID);
				printf("---------------------------------------------------------------------------\n");

				char clinicHystoryOption[MAXINPUT] = " ";

				do{
					printf("Desea ver la historia clinica (Y/N): ");
					gets(clinicHystoryOption);

					if (strlen(clinicHystoryOption) != 1){
						printf("Comando no valido\n\n");
					}
					else{
						if (clinicHystoryOption[0] == 'Y' || clinicHystoryOption[0] == 'y' || clinicHystoryOption[0] == 'N' || clinicHystoryOption[0] == 'n'){
							break;
						}
						else{
							printf("Comando no valido\n\n");
						}
					}
				} while (true);

				int c = 1;
                if (clinicHystoryOption[0] == 'Y' || clinicHystoryOption[0] == 'y'){

					r = send(fd, &c , sizeof(int), 0);
					if(r == -1){
						perror("Error Enviando opcion Historia clinica");
					}
				
					int idThread;
					int v = recv(fd, &idThread, sizeof(int), 0);
					if(v == -1){
						perror("Error recibiendo");
					}

					char fileId[256];
					char path[] = "temporal";
					sprintf(fileId, "%d", idThread);
					strcat(path, fileId);
					strcat(path, ".txt");
					
										
					int b = 0;
					FILE* t;
					char recBuf[1024] = "";

					t = fopen(path, "ab+");
					
					if(t == NULL){
						perror("Abriendo archivo");
					}

					
					do{
						b = recv(fd, recBuf, 1024, 0);
						fprintf(t, "%s", recBuf);
					}while(b == 1024);

					fclose(t);
					openFile(path);
					pauseShell();
					FILE *fp = fopen(path, "ab+");
					char rbuff[1024] = "";
					if(fp == NULL){
						perror("File");
						return 2;
					}
					while(1){
						char sendbuffer[1024] = {0};
						int b = fread(sendbuffer, 1, sizeof(sendbuffer), fp);
						if( b > 0){
							printf("Enviando\n");
							int v = send(fd, sendbuffer, b, 0);
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
					
					int status = remove(path);
					if (status == 0)
					{
						printf("Temporal file deleted\n");
					}
					
				}else{
					c = 0;
					r = send(fd, &c , sizeof(int), 0);
					if(r == -1){
						perror("Error Enviando opcion Historia clinica");
					}
					pauseShell();
				}
                
				clearScreen();
			}
			else{
				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("EL REGISTRO NUMERO %d NO EXISTE\n", regNumber);
				printf("---------------------------------------------------------------------------\n");
				pauseShell();
				
				confirmSignal = -1;
				r = send(fd, &confirmSignal, sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando opcion de menu ");
				}

				clearScreen();
			}

			break;

		case 3:
			printf("");

			r = send(fd, &menuOption, sizeof(int), 0);
			if(r ==-1){
				perror("Error Enviando opcion de menu ");
			}

			printf("Espere un momento porfavor ...");

			r =  recv(fd, &REGISTROS, sizeof(int), 0);
			if(r ==-1){
				perror("Error recibiendo index ");
			}

			clearScreen();
			printf("---------------------------------------------------------------------------\n");
			printf("BORRAR REGISTRO\n");
			printf("---------------------------------------------------------------------------\n");
			printf("NUMERO DE REGISTROS: %d\n", REGISTROS);
			printf("---------------------------------------------------------------------------\n\n");


			char deleteInput[MAXINPUT];
			int regDeleteNumber;

			do
			{
				printf("Ingrese el numero de registro que desea borrar: ");
				gets(deleteInput);
				if (!validateInteger(deleteInput))
				{
					printf("El numero ingresado no es valido\n\n");
				}
				else
				{
					break;
				}
			} while (true);

			sscanf(deleteInput, "%d", &regDeleteNumber);

           
			if (regDeleteNumber > 0 && regDeleteNumber <= REGISTROS)
			{
				confirmSignal = 1;
				r = send(fd, &confirmSignal, sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando opcion de menu ");
				}
				regDeleteNumber--;
				r = send(fd, &regDeleteNumber, sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando numero de registro ");
				} 

				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("BORRANDO REGISTRO ...\n");
				printf("---------------------------------------------------------------------------\n");
				int eraseStatus;
				r = recv(fd, &eraseStatus, sizeof(int), 0);
				if(r ==-1){
					perror("Error recibiendo Confirmacion");
				}

				clearScreen();

				if(eraseStatus == 1){
					printf("---------------------------------------------------------------------------\n");
					printf("EL REGISTRO FUE ELIMINADO CORRECTAMENTE\n");
					printf("---------------------------------------------------------------------------\n");
				}else if(eraseStatus == -1){
					printf("---------------------------------------------------------------------------\n");
					printf("ERROR: NO SE PUDO ELIMINAR EL REGISTRO\n");
					printf("---------------------------------------------------------------------------\n");
				}else if(eraseStatus == 2){
					printf("---------------------------------------------------------------------------\n");
					printf("EL REGISTRO FUE ELIMINADO CORRECTAMENTE\n");
					printf("---------------------------------------------------------------------------\n");
					printf("LA HISTORIA CLINICA FUE ELIMINADA CORRECTAMENTE\n");
					printf("---------------------------------------------------------------------------\n");
				}else{
					printf("---------------------------------------------------------------------------\n");
					printf("EL REGISTRO FUE ELIMINADO CORRECTAMENTE\n");
					printf("---------------------------------------------------------------------------\n");
					printf("NO SE ENCONTRO HISTORIA CLINICA\n");
					printf("---------------------------------------------------------------------------\n");
				}
				
			}
			else
			{
				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("EL REGISTRO NUMERO %d NO EXISTE\n", regDeleteNumber);
				printf("---------------------------------------------------------------------------\n");
				confirmSignal = -1;
				r = send(fd, &confirmSignal, sizeof(int), 0);
				if(r ==-1){
					perror("Error Enviando opcion de menu ");
				}
			}

			pauseShell();
			break;

		case 4:
			printf("");

			printf("Espere un momento porfavor ...");

			r = send(fd, &menuOption, sizeof(int), 0);
			if (r == -1)
			{
				perror("Error Enviando opcion de menu ");
			}

			clearScreen();
			printf("---------------------------------------------------------------------------\n");
			printf("BUSCAR REGISTRO\n");
			printf("---------------------------------------------------------------------------\n\n");
			printf("Ingrese el nombre a busar en los registros (Max 32 caracteres): ");

			char nameInput[MAXINPUT];
			do
			{
				gets(nameInput);
				if (strlen(nameInput) <= 32)
				{
					break;
				}
				else
				{
					printf("El nombre ingresado no es valido\n\n");
				}
			} while (true);

			clearScreen();

			printf("---------------------------------------------------------------------------\n");
			printf("BUSCANDO COINCIDENCIAS ...\n");
			printf("---------------------------------------------------------------------------\n");

			tolowerCase(nameInput);
			
			r = send(fd, nameInput, sizeof(nameInput), 0);
			if(r ==-1){
				perror("Error Enviando Nombre ");
			}
			int count;
			r = recv(fd, &count, sizeof(int), 0);
			if(r ==-1){
				perror("Error recibiendo Count");
			}

			if(count == 0){
				printf("---------------------------------------------------------------------------\n");
				printf("NO EXISTEN REGISTROS CON EL NOMBRE %s\n", nameInput);
				printf("---------------------------------------------------------------------------\n\n");
			}else{
				struct DogType reg;
				for(int i = 0; i < count; i++){
					r = recv(fd, &reg, sizeof(struct DogType), 0);
					if(r ==-1){
						perror("Error recibiendo Estructura");
					}else{
						printf("---------------------------------------------------------------------------\n\n");
						printf("Informacion del registro: %d\n", (reg.index + 1));
						printf("\nname: %s\n", reg.name);
						printf("type: %s\n", reg.type);
						printf("age: %d\n", reg.age);
						printf("breed: %s\n", reg.breed);
						printf("height: %d\n", reg.height);
						printf("weight: %.2lf\n", reg.weight);
						printf("sex: %c\n\n", reg.sex);
					}
				}
				printf("---------------------------------------------------------------------------\n");
				printf("HAY %d REGISTRO/S CON EL NOMBRE %s\n", count, nameInput);
				printf("---------------------------------------------------------------------------\n\n");
			}

			pauseShell();
			break;

		case 5:
			break;

		default:
			printf("---------------------------------------------------------------------------\n");
			printf("OPCION NO VALIDA\n");
			printf("---------------------------------------------------------------------------\n");

			r = send(fd, &errorSignal, sizeof(int), 0);
			if(r ==-1){
				perror("Error Enviando señal de error ");
			}

			pauseShell();
			break;
		}
		clearScreen();
		menuOption = executeMenu();
	}
	r = send(fd, &menuOption, sizeof(int), 0);
	if(r ==-1){
		perror("Error Enviando opcion de menu ");
	}
	//writeInt(&medicalCreated);
	close(fd);
	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("Finalizando programa ...\n");
	printf("---------------------------------------------------------------------------\n");
    //fclose(f);
	pauseShell();

    return 0;
}