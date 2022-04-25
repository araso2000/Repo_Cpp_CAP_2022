#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define N 10  //Longitud vector

int main(int argc, char* argv[]) {

	int size, rank, i, from, to, ndat, tag, VA[N], status;
	MPI_Status info;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int* VB = (int*)malloc(sizeof(int)*N / size);
	//Inicializo vector

	for (i = 0; i < N; i++) {
		VB[i] = 0;
	}

	//Si soy el maestro
	if (rank == 0) {
		for (i = 0; i < N; i++) {
			VA[i] = i;
		}
		printf("Vector a repartir:\n");
		for (i = 0; i < N; i++) {
			printf("%d  ", VA[i]);
		}
		printf("\n");
	}
	printf("Proceso %d: VB antes de recibir datos: \n", rank);
	for (i = 0; i < N / size; i++) {
		printf("%d  ", VB[i]);
	}
	printf("\n");

	status = MPI_Scatter(&VA[0], N / size, MPI_INT, &VB[0], N / size, MPI_INT, 0, MPI_COMM_WORLD);

	if (status != MPI_SUCCESS) {
		printf("Error scatting...\n");
		exit(1);
	}
	printf("Proceso %d recibe VB \nVB = ", rank);
	for (i = 0; i < N / size; i++) {
		printf("%d  ", VB[i]);
	}
	printf("\n\n");
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}