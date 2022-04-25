#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int prob();

int main() {
	MPI_Init(NULL, NULL);

	int yosoy,world_size,pelota,puntos,pelota_backup,suspuntos;
	yosoy = world_size = pelota = puntos = pelota_backup = suspuntos = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &yosoy);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	srand(time(NULL)*yosoy);
	int mirandom = (rand() % 101);
	int surandom = 0;

	int envio, recibo;
	if (yosoy == 0) {
		envio = recibo = 1;
	}
	else {
		envio = recibo = 0;
	}

	MPI_Send(&mirandom, 1, MPI_INT, envio, 0, MPI_COMM_WORLD);
	MPI_Recv(&surandom, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	cout << "Yo soy: " << yosoy << " y mi random es: " << mirandom << " y su random es: " << surandom << endl << endl;
	
	int rondas = 1;
	while (rondas!=10 || puntos>=90) {
		cout << "Ronda " << rondas << endl << endl;
		pelota = 0;
		int fin = 0;
		while (fin != 1) {
			if (mirandom > surandom) {
				if (prob() == 1) {
					pelota = -1;
					fin = 1;
				}
				cout << "Soy " << yosoy << " - Mando pelota (" << pelota << ")\n\n";
				MPI_Send(&pelota, 1, MPI_INT, envio, 0, MPI_COMM_WORLD);
				MPI_Recv(&pelota, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				cout << "Soy " << yosoy << " - Recibo pelota (" << pelota << ")\n\n";
				if (pelota == -1) {
					cout << "El otro proceso (yo soy: " << yosoy << ") ha perdido la pelota\n\n";
					puntos = puntos + pelota_backup;
					fin = 1;
				}
				else {
					pelota_backup = pelota;
				}
				pelota++;
			}
			else {
				MPI_Recv(&pelota, 1, MPI_INT, envio, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				cout << "Soy " << yosoy << " - Recibo pelota (" << pelota << ")\n\n";
				if (pelota == -1) {
					cout << "El otro proceso (yo soy: " << yosoy << ") ha perdido la pelota\n\n";
					puntos = puntos + pelota_backup;
					fin = 1;
				}
				else {
					pelota_backup = pelota;
				}
				pelota++;
				if (prob() == 1) {
					pelota = -1;
					fin = 1;
				}
				cout << "Soy " << yosoy << " - Mando pelota (" << pelota << ")\n\n";
				MPI_Send(&pelota, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD);
			}
		}
		rondas++;
		cout << "Puntos del proceso " << yosoy << ": " << puntos << endl << endl;

	}
	

	cout << "Puntos FINALES de " << yosoy << ": " << puntos << endl;

	MPI_Send(&puntos, 1, MPI_INT, envio, 0, MPI_COMM_WORLD);
	MPI_Recv(&suspuntos, 1, MPI_INT, recibo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	if (yosoy == 0) {
		if (puntos < suspuntos) {
			cout << "Gana P1" << endl;
			cout << "Diferencia de puntos: " << suspuntos - puntos;
		}
		else if (puntos>suspuntos) {
			cout << "Gana P0";
			cout << "Diferencia de puntos: " << puntos - suspuntos;
		}
		else {
			cout << "Empate";
		}
	}

	

	MPI_Finalize();
}

int prob() {
	if ((rand() % 1001) < 101) {
		return(1);
	}else {
		return(0);
	}
}