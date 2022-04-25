#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {

    MPI_Init(NULL, NULL);

    int world_rank;
    int world_size;
    int array[100];

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_rank == 0) {
        cout << "EJERCICIO 2" << endl;
        cout << "Soy el proceso: " << world_rank << " - Numero de procesos: " << world_size << endl;
        for (int ii = 0; ii < 100; ii++) {
            array[ii] = ii;
        }
        MPI_Bcast(&array, 100, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else {
        cout << "Soy el proceso: " << world_rank << endl;

        MPI_Bcast(&array, 100, MPI_INT, 0, MPI_COMM_WORLD);

        //MPI_Recv(&array, 100, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int ii = 0; ii < 100; ii++) {
            cout << "Soy el proceso: " << world_rank << " - ii:" << ii << "-" << array[ii] << "   " << endl;
        }
    }
    MPI_Finalize();
}