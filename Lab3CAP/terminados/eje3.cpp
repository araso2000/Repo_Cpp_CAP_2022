#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]) {

    MPI_Status info;

    MPI_Init(&argc, &argv);

    int world_rank;
    int world_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int arraySend[100];
    //int *arrayReceive = (int*) malloc((100 / (world_size - 1)) * sizeof(int));
    int arrayReceive[100];

    if (world_rank == 0) {
        cout << "EJERCICIO 3" << endl;
        cout << "Soy el proceso: " << world_rank << " - Numero de procesos: " << world_size << endl;
        for (int ii = 0; ii < 100; ii++) {
            arraySend[ii] = ii;
        }
        cout << "Vector a enviar" << endl;
        for (int ii = 0; ii < 100; ii++) {
            cout << arraySend[ii] << "  ";
        }
        cout << endl;
    }

    int status = MPI_Scatter(&arraySend, (100 / world_size), MPI_INT, &arrayReceive, 100, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "ESTADO: " << status << endl;

    if (status != MPI_SUCCESS) {
        cout << "ERROR" << endl << endl;
        exit(1);
    }

    if (world_rank != 0) {
        cout << "Soy el proceso: " << world_rank << endl;

        for (int ii = 0; ii < (100/world_size); ii++) {
            cout << "Soy el proceso: " << world_rank << " - ii:" << ii << "-" << arrayReceive[ii] << "   " << endl;
        }
    }
    
    MPI_Finalize();
}