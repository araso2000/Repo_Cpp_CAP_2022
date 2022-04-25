#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]) {

	int tam, proceso, position;
	int primero, segundo,tercero;
	int buf[10];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &tam);
	MPI_Comm_rank(MPI_COMM_WORLD, &proceso);

	if (proceso == 0) {
		//Soy el proceso MASTER
		cout << "Introduce el primer numero INT:" << endl;
		cin >> primero;
		cout << "Introduce el segundo numero INT:" << endl;
		cin >> segundo;
		cout << "Introduce el tercer numero INT:" << endl;
		cin >> tercero;

		position = 0;
		MPI_Pack(&primero, 1, MPI_INT, buf, 10 * sizeof(int), &position, MPI_COMM_WORLD);
		MPI_Pack(&segundo, 1, MPI_INT, buf, 10 * sizeof(int), &position, MPI_COMM_WORLD);
		MPI_Pack(&tercero, 1, MPI_INT, buf, 10 * sizeof(int), &position, MPI_COMM_WORLD);

		MPI_Bcast(&buf, 1, MPI_PACKED, 0, MPI_COMM_WORLD);
	}
	else {
		//Soy el proceso SLAVE
		MPI_Bcast(&buf, 1, MPI_PACKED, 0, MPI_COMM_WORLD);

		position = 0;
		MPI_Unpack(&buf, 10 * sizeof(int), &position, &primero, 1, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(&buf, 10 * sizeof(int), &position, &segundo, 1, MPI_INT, MPI_COMM_WORLD);
		MPI_Unpack(&buf, 10 * sizeof(int), &position, &tercero, 1, MPI_INT, MPI_COMM_WORLD);

		cout << "Proceso: " << proceso << " - Primero INT: " << primero << endl;
		cout << "Proceso: " << proceso << " - Primero INT: " << segundo << endl;
		cout << "Proceso: " << proceso << " - Primero INT: " << tercero << endl;
	}

	//Finalizo OpenMPI
	MPI_Finalize();
	return 0;
}
