#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define nLines 9 //Número de filas de los textos cifrados
#define nCharsPerLine 33 //Número de caracteres en cada fila de los textos cifrados
#define nRotors 2 //Número de rotores de enigma para cifrar y descifrar
#define total nLines*nCharsPerLine //Total de letras en el texto
#define hilos 10	//Numero de hilos procesando

int arraySend[total];
int arrayReceive[total/hilos];
int textoCifrado[hilos][nCharsPerLine];
int textoDescifrado[hilos][nCharsPerLine];
int resultado[total];
int resultadoFinal[nLines][nCharsPerLine];

int ciphered[nLines][nCharsPerLine] = { //[9][33] - 2 rotores
	{65,63,59,89,50,117,131,56,142,132,131,64,153,133,154,139,146,145,160,80,161,162,86,172,162,161,94,183,163,176,178,187,120},
	{57,65,57,87,48,115,129,54,140,130,129,62,134,139,150,139,72,158,148,143,164,82,150,171,170,168,175,94,161,169,165,175,182},
	{53,57,53,125,128,46,132,122,121,54,123,137,136,130,76,66,152,142,141,74,152,151,151,154,168,86,172,162,157,178,96,164,182},
	{61,70,60,122,129,124,138,57,143,133,132,65,135,134,158,151,87,77,163,153,152,85,159,168,173,171,95,181,171,166,187,105,194},
	{55,58,54,108,120,116,132,51,137,127,126,59,144,139,134,136,149,140,155,158,89,79,165,155,154,87,172,163,166,164,173,167,101},
	{61,58,56,129,119,114,135,53,126,142,124,143,131,148,67,153,143,142,75,159,148,146,159,162,170,89,170,163,95,174,168,179,117},
	{57,65,57,87,48,130,128,123,124,129,129,62,141,155,68,146,145,144,145,78,145,160,152,86,160,169,170,173,178,98,184,181,104},
	{67,64,60,133,123,122,55,103,132,132,135,149,8252,152,71,128,140,161,146,153,95,85,157,168,173,93,179,169,172,184,103,183,180},
	{57,56,54,114,117,131,49,116,131,123,57,124,137,139,65,151,141,140,73,153,150,150,153,167,168,87,173,170,93,162,176,176,170},
};

void printNumbersAsString(int lines[nLines][nCharsPerLine])
{
	for (int idx = 0; idx < nLines; idx++)
	{
		char line[nCharsPerLine + 1];
		for (int idx2 = 0; idx2 < nCharsPerLine; idx2++)
		{
			line[idx2] = lines[idx][idx2];
		}
		line[nCharsPerLine] = '\0';
		printf("%s\n", line);
	}
}

int* decipher(int line[], int key)
{
	int rawData[nCharsPerLine];

	for (int idx = 0; idx < nCharsPerLine; idx++)
	{
		rawData[idx] = line[idx];
	}

	int* rotorKeys = (int*)malloc(sizeof(int) * nRotors);
	int remainder = key;
	for (int idx = 0; idx < nRotors; idx++)
	{
		int divisor = pow(10, (nRotors - (1 + idx)));
		rotorKeys[idx] = (int)(remainder / divisor);
		remainder = (int)(remainder % divisor);
	}


	for (int rotorIdx = 0; rotorIdx < nRotors; rotorIdx++)
	{
		int displacement = rotorKeys[rotorIdx];
		for (int idx = 0; idx < nCharsPerLine; idx++)
		{
			rawData[idx] = rawData[idx] - displacement++;
		}
	}

	free(rotorKeys);
	return rawData;
}

void enigma()
{
	//int deciphered[nLines][nCharsPerLine];
	for (int idx = 0; idx < nLines; idx++)
	{
		for (int lineKey = (int)pow(10, nRotors - 1); lineKey < (int)pow(10, nRotors); lineKey++)
		{
			int* p_deciphered = decipher(ciphered[idx], lineKey);

			char decipheredLine[nCharsPerLine];
			for (int idx = 0; idx < nCharsPerLine; idx++)
			{
				decipheredLine[idx] = p_deciphered[idx];
			}

			char stringKey[nRotors + 1];
			sprintf_s(stringKey, "%d", lineKey);
			if (!strncmp(stringKey, decipheredLine, nRotors))
			{
				for (int idx2 = 0; idx2 < nCharsPerLine; idx2++)
				{
					textoDescifrado[idx][idx2] = decipheredLine[idx2];
				}
				printf("Descifrada linea %d con clave %d\n", idx, lineKey);
				break;
			}
		}
	}
}

int main(int argc, char* argv[]) {

		unsigned t0, t1;
		t0 = clock();

		MPI_Status info;

		MPI_Init(&argc, &argv);

		int world_rank;
		int world_size;

		MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &world_size);


		if (world_rank == 0) {
			cout << "Texto a descifrar: " << endl;
			printNumbersAsString(ciphered);

			//Añadimos toda la matriz en fila al vector
			int temp = 0;
			for (int ii = 0; ii < nLines; ii++) {
				for (int jj = 0; jj < nCharsPerLine; jj++) {
					arraySend[temp] = ciphered[ii][jj];
					temp++;
				}
			}
		}

		int status = MPI_Scatter(&arraySend[0], total / hilos, MPI_INT, &arrayReceive[0], total / hilos, MPI_INT, 0, MPI_COMM_WORLD);
		//cout << "ESTADO: " << status << endl;

		if (status != MPI_SUCCESS) {
			cout << "ERROR" << endl << endl;
			exit(1);
		}

		int temp = 0;
		for (int ii = 0; ii < hilos; ii++) {
			for (int jj = 0; jj < nCharsPerLine; jj++) {
				textoCifrado[ii][jj] = arrayReceive[temp];
				temp++;
			}
		}

		enigma();

		int hola[total / hilos];
		int temp1 = 0;
		for (int ii = 0; ii < hilos; ii++) {
			for (int jj = 0; jj < nCharsPerLine; jj++) {
				hola[temp1] = textoDescifrado[ii][jj];
				temp++;
			}
		}

		MPI_Gather(&hola[0], total / hilos, MPI_INT, &resultado[0], total, MPI_INT, 0, MPI_COMM_WORLD);

		temp = 0;
		for (int ii = 0; ii < nLines; ii++) {
			for (int jj = 0; jj < nCharsPerLine; jj++) {
				resultadoFinal[ii][jj] = resultado[temp];
				temp++;
			}
		}

		if (world_rank == 0) {
			printNumbersAsString(resultadoFinal);

			t1 = clock();

			double time = (double(t1 - t0) / CLOCKS_PER_SEC);
			cout << "Tiempo de ejecucion (en segundos): " << time << endl;
		}		
		MPI_Finalize();
}