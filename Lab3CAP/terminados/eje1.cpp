#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(){

    MPI_Init(NULL, NULL);

    int world_rank;
    int world_size;
    int array[100];

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int divisionNormal = 100 / world_size;
    int* arrayTemp = (int*)malloc(sizeof(int) * divisionNormal);

    if (world_rank == 0) {
	  cout << "EJERCICIO 1" << endl;
        cout << "Soy el proceso: " << world_rank << " - Numero de procesos: " << world_size << endl;
        for (int ii = 0; ii < 100; ii++) {
            array[ii] = ii;
        }

        for (int ii = 1; ii < world_size; ii++) {
            int contador = 0;
            for (int jj = ((ii * divisionNormal) - divisionNormal); jj < divisionNormal; jj++) {
                arrayTemp[contador] = array[jj];
                contador++;
            }
            MPI_Send(&arrayTemp, divisionNormal, MPI_INT, ii, 0, MPI_COMM_WORLD);
        }
    }else {
        cout << "Soy el proceso: " << world_rank << endl;

        MPI_Recv(&array, divisionNormal, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int ii = 0; ii < 100; ii++) {
            cout << "Soy el proceso: " << world_rank << " - ii:" << ii << "-" << array[ii] << "   " << endl;
        }
    }
    MPI_Finalize();
}
