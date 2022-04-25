#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Defino una constante que define el numero de elementos totales sobre el que calcular su media
#define N 100

int main(int argc, char* argv[]) {
	//Variables
	int tam, proceso, vectorEnvio[N], status, numeroDatos;
	float media = 0;

	//Inicio OpenMPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &tam);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	//Calculo el numero de datos que recibirá cada proceso tras MPI_Scatter();
	numeroDatos = N / tam;

	//Inicializo los vectores que recibirán y expresarán la media calculada por cada proceso
	float* vectorMedia = (float*)malloc(sizeof(float) * tam);
	int* vectorRecibo = (int*)malloc(sizeof(int) * (N / tam));

	//Los inicializo
	for (int ii = 0; ii < tam; ii++) {
		vectorMedia[ii] = 0;
	}
	for (int ii = 0; ii < (N / tam); ii++) {
		vectorRecibo[ii] = 0;
	}

	//Si soy el PROCESO 0
	if (proceso == 0) {
		//Inicializo el vectorEnvio
		for (int ii = 0; ii < N; ii++) {
			vectorEnvio[ii] = ii;
		}
		
		//Lo imprimo
		cout << "Soy el PROCESO 0 -> Envio:" << endl;
		for (int ii = 0; ii < N; ii++) {
			cout << vectorEnvio[ii] << " ";
		}
		cout << endl;
	}

	//Mando el vector divido según el número de procesos mediante MPI_Scatter
	status = MPI_Scatter(&vectorEnvio[0], numeroDatos, MPI_INT, &vectorRecibo[0], numeroDatos, MPI_INT, 0, MPI_COMM_WORLD);

	//Imprimo el vector recibido (por proceso)
	cout << "Soy el PROCESO " << proceso << " -> Recibo:" << endl;
	for (int ii = 0; ii < numeroDatos; ii++) {
		cout << vectorRecibo[ii] << " ";
	}
	cout << endl;

	//Con el vector recibido, calculo su media
	for (int ii = 0; ii < numeroDatos; ii++) {
		media = media + vectorRecibo[ii];
	}
	media = media / numeroDatos;

	//El proceso 0 recupera las medias calculadas por cada proceso 
	MPI_Gather(&media, 1, MPI_INT, &vectorMedia[0], 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Si soy el proceso 0, muestro las medias obtenidas y calculo la media final
	if (proceso == 0) {
		media = 0;
		for (int ii = 0; ii < tam; ii++) {
			cout << "Media " << ii + 1 << "-> " << vectorMedia[ii] << endl;
			media = media + vectorMedia[ii];
		}
		cout << "MEDIA TOTAL: " << (media / tam) << endl;
	}

	//Finalizo OpenMPI
	MPI_Finalize();
	return 0;
}
