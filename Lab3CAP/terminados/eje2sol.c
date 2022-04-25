#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define N 10  //Longitud vector

int main(int argc, char* argv[]){

	int size, rank, i, from, to, ndat, tag, VA[N], status;
	MPI_Status info;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	//Inicializo vector
	for (i=0; i<N; i++) {
		VA[i] = 0;
	}
	
	//Si soy el maestro
	if(rank==0){
		for (i=0; i<N; i++) {
			VA[i] = i;
		}
		printf("Vector a enviar:\n");
		for(i=0; i<N;i++){
			printf("%d  ",VA[i]);
		}
		printf("\n");
	//Si soy esclavo
	}else{
		printf("Proceso %d: VA antes de recibir datos: \n",rank);
		for (i=0; i<N; i++) {
			printf("%d  ",VA[i]);
		}
		printf("\n");
	}
	
	status = MPI_Bcast(&VA[0], N, MPI_INT, 0,MPI_COMM_WORLD);
	
	if(status!=MPI_SUCCESS){
		printf("Error broadcasting...\n");
		exit(1);
	}
	if(rank!=0){
		printf("Proceso %d recibe VA \nVA = ",rank);
		for (i=0; i<N; i++) {
			printf("%d  ",VA[i]);
		}
			printf("\n\n");
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}