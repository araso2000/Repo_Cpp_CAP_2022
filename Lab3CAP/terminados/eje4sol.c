#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define N 10

int main(int argc, char* argv[]) {

	//Variables de mi programa
	int size, rank, i, from, to, ndat, tag, VA[N], data[N], status;
	MPI_Status info;

	//Inicializo MPI
	MPI_Init(&argc, &argv);

	//Calculo cu?ntos procesos hay
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//Calculo qu? proceso soy
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Inicializo vector a 0
	for (i = 0; i < N; i++) {
		VA[i] = 0;
	}

	int* VB = (int*)malloc(sizeof(int) * N / size);
	//Inicializo vector

	for (i = 0; i < N; i++) {
		VB[i] = 0;
	}

	//Si soy maestro, inicializo los elementos del vector para que guarden su posici?n (i)
	if (rank == 0) {
		for (i = 0; i < N; i++) {
			VA[i] = i;
		}
	}
	printf("Proceso %d: VA antes de recibir datos: \n", rank);
	//Para todos: imprimo el vector
	for (i = 0; i < N; i++) {
		printf("%d ", VA[i]);
	}
	printf("\n");
	//Calculamos cu?ntos datos corresponden a cada proceso
	ndat = N / size;
	//Dividimos el vector en partes, y las distribuimos entre los procesos 
	status = MPI_Scatter(&VA[0], ndat, MPI_INT, &VB[0], ndat, MPI_INT, 0, MPI_COMM_WORLD);

	//Cada proceso multiplica por 2 su parte del vector
	for (i = 0; i < ndat; i++) {
		VB[i] = 2 * VB[i];
	}
	printf("Proceso %d: VA despu?s de recibir los datos y multiplicarlos por 2: \n", rank);
	//Cada proceso imprime su parte del vector
	for (i = 0; i < ndat; i++) {
		printf("%d ", VB[i]);
	}
	printf("\n");
	if (rank == 0) {
		for (i = 0; i < N; i++) {
			data[i] = 0;
		}
	}
	//Uso gather para recolectar los valores nuevos en un nuevo vector
	MPI_Gather(&VB[0], ndat, MPI_INT, &data[0], ndat, MPI_INT, 0, MPI_COMM_WORLD);

	//El maestro imprime el vector final
	if (rank == 0) {
		for (i = 0; i < N; i++) {
			printf("%d ", data[i]);
		}
		printf("\n");
	}
	//Cierro MPI
	MPI_Finalize();
	return 0;
}