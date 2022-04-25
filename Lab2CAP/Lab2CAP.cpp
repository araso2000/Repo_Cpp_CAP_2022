#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

bool esPrimo(int numero);

int main() {
    int numbersOtro[10];
    int numbers[10];
    int world_rank;
    int world_size;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Request request;
    MPI_Status status;
    bool repeat = true;

    if (world_rank == 0) {
        while (1) {
            if (repeat) {
                MPI_Irecv(numbersOtro, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
            }
            repeat = false;

            int contador = 0;
            for (int ii = 0; contador == 10; ii++) {
                if (esPrimo(ii)) {
                    numbers[contador] = ii;
                    contador++;
                }
            }
            cout << "Numeros primos proceso O" << endl;
            for (int ii = 0; ii < 10; ii++) {
                cout << numbers[ii] << "   ";
            }
            cout << endl;

            int flag = 0;
            MPI_Test(&request, &flag, &status);
            if (flag == 1) {
                cout << "Numeros primos proceso 1" << endl;
                for (int ii = 0; ii < 10; ii++) {
                    cout << numbersOtro[ii] << "   ";
                }
                cout << endl;
                repeat = true;
            }
        }

    }else if (world_rank == 1) {
        while (1) {
            int contador = 0;
            for (int ii = 0; contador == 10; ii++) {
                if (esPrimo(ii)) {
                    numbers[contador] = ii;
                    contador++;
                }
            }
            if (repeat) {
                MPI_Isend(numbers, 10, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
            }
            repeat = false;

            int flag = 0;
            MPI_Test(&request, &flag, &status);
            if (flag == 1) {
                repeat = true;
            }
            
        }
    }
    MPI_Finalize();
}

bool esPrimo(int numero) {
    if (numero % 2 == 0) {
        return(true);
    }
    else {
        return(false);
    }
}
