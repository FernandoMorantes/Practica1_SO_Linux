#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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



int main(){

    clearScreen();

	//Creacion del socket
    struct sockaddr_in server;
	struct DogType buffer;
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
		perror("error en bind");
	}

	//Test de recibimiento de datos
	r = recv(fd, &buffer, sizeof(buffer), 0);
    printf("---------------------------------------------------------------------------\n");
    printf("SOCKET TEST: \n\n");
	printf("name: %s\n",buffer.name);
	printf("type: %s\n", buffer.type);
	printf("age: %d\n", buffer.age);
	printf("breed: %s\n", buffer.breed);
	printf("height: %d\n", buffer.height);
	printf("weight: %.2f\n", buffer.weight);
	printf("sex: %c\n", buffer.sex);
	printf("index: %d\n", buffer.index);
	printf("prev hash index: %d\n\n", buffer.prevHashIndex);
    printf("---------------------------------------------------------------------------\n");
	close(fd);	

    pauseShell();
    clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("CARGANDO PROGRAMA ...\n");
	printf("---------------------------------------------------------------------------\n");

    /*
	medicalCreated = readInt();
	readHash();
	FILE *f;
	f = fopen("dataDogs.dat", "rb+");

	if (f == NULL)
	{
		perror("Could not open a file");
		exit(-1);
	}

	REGISTROS = countRecords(f);
    */

	clearScreen();
	printf("\nBIENVENIDO USUARIO\n");

	int menuOption = executeMenu();

	while (menuOption != 5)
	{
		clearScreen();
		switch (menuOption)
		{
		case 1:
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
				scanf("%[^\n]%*c", regInput);
			} while (!validateRegValue(6, regInput));

			float weight;
			sscanf(regInput, "%f", &weight);
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

			int newReghash = calculateHash(newReg->name);
			newReg->prevHashIndex = lastHashIndex[newReghash];
			lastHashIndex[newReghash] = newReg->index;
			pauseShell();

			//writeRegister(newReg, newReg->index);
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
			printf("prev hash index: %d\n\n", newReg->prevHashIndex);

			//writeHash();
			//readHash();
			pauseShell();
			break;

		case 2:
            /*
			fclose(f);
			f = fopen("dataDogs.dat", "rb+");

			if (f == NULL)
			{
				perror("Could not open a file");
				exit(-1);
			}
            */
			printf("---------------------------------------------------------------------------\n");
			printf("VER REGISTRO\n");
			printf("---------------------------------------------------------------------------\n");
			printf("NUMERO DE REGISTROS: %d\n", REGISTROS);
			printf("---------------------------------------------------------------------------\n\n");

			char numberInput[MAXINPUT];
			int regNumber;
			do
			{
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

			if (regNumber > 0 && regNumber <= REGISTROS)
			{

				struct DogType searchedReg;
				//findByIndex(&searchedReg, (regNumber - 1), f);
				clearScreen();
				printf("---------------------------------------------------------------------------\n\n");
				printf("Registro %d\n", regNumber);
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

				do
				{
					printf("Desea ver la historia clinica (Y/N): ");
					gets(clinicHystoryOption);

					if (strlen(clinicHystoryOption) != 1)
					{
						printf("Comando no valido\n\n");
					}
					else
					{
						if (clinicHystoryOption[0] == 'Y' || clinicHystoryOption[0] == 'y' || clinicHystoryOption[0] == 'N' || clinicHystoryOption[0] == 'n')
						{
							break;
						}
						else
						{
							printf("Comando no valido\n\n");
						}
					}

				} while (true);
                
                /*
				int number = (regNumber - 1);
				if (clinicHystoryOption[0] == 'Y' || clinicHystoryOption[0] == 'y')
				{
					if (searchedReg.medicalHistoryID == -1)
					{
						regNumber = medicalCreated;
						searchedReg.medicalHistoryID = medicalCreated;
						medicalCreated++;

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
					else
					{
						regNumber = searchedReg.medicalHistoryID;
					}
					char fileName[256] = "Historias_clinicas/";
					char fileNameNumber[256];

					sprintf(fileNameNumber, "%d", regNumber);
					strcat(fileName, fileNameNumber);
					strcat(fileName, ".txt");

					if (!(access(fileName, F_OK) != -1))
					{

						FILE *g = fopen(fileName, "w");

						if (g == NULL)
						{
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
					openFile(fileName);
				}
                */
				clearScreen();
			}
			else
			{
				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("EL REGISTRO NUMERO %d NO EXISTE\n", regNumber);
				printf("---------------------------------------------------------------------------\n");
			}

			pauseShell();
			break;

		case 3:
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

            /*
			if (regDeleteNumber > 0 && regDeleteNumber <= REGISTROS)
			{

				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("BORRANDO REGISTRO ...\n");
				printf("---------------------------------------------------------------------------\n");

				fclose(f);
				eraseFunction(REGISTROS, (regDeleteNumber - 1));

				f = fopen("dataDogs.dat", "ab+");

				if (f == NULL)
				{
					perror("Could not open a file");
					exit(-1);
				}
			}
			else
			{
				clearScreen();
				printf("---------------------------------------------------------------------------\n");
				printf("EL REGISTRO NUMERO %d NO EXISTE\n", regDeleteNumber);
				printf("---------------------------------------------------------------------------\n");
			}
            */

			pauseShell();
			break;

		case 4:
			printf("---------------------------------------------------------------------------\n");
			printf("BUSCAR REGISTRO\n");
			printf("---------------------------------------------------------------------------\n\n");
			printf("Ingrese el nombre a busar en los registros (Max 32 caracteres): ");

			//readHash();
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
			tolowerCase(nameInput);
			//findByName(nameInput, f);
			pauseShell();
			break;

		case 5:
			break;

		default:
			printf("---------------------------------------------------------------------------\n");
			printf("OPCION NO VALIDA\n");
			printf("---------------------------------------------------------------------------\n");

			pauseShell();
			break;
		}
		clearScreen();
		menuOption = executeMenu();
	}
	//writeInt(&medicalCreated);
	clearScreen();
	printf("---------------------------------------------------------------------------\n");
	printf("Finalizando programa ...\n");
	printf("---------------------------------------------------------------------------\n");
    //fclose(f);
	pauseShell();

    return 0;
}