#include <iostream>
#include <stdlib.h>
#include <omp.h>
using namespace std;

#define STEPS	20
#define SIZE	20
#define MOVE	0.1

#if defined(_OPENMP)
void lock(int i, bool *locks) {
	for (bool locked = false; locked == false; /*NOP*/) {
		#pragma omp critical (LockRegion)
		{
			locked = !locks[i-1] && !locks[i] && !locks[i+1];
			if (locked) {
				locks[i-1] = true; locks[i] = true; locks[i+1] = true;
			}
		}
	}
}
void unlock(int i, bool *locks) {
	#pragma omp critical (LockRegion)
	{
		locks[i-1] = false; locks[i] = false; locks[i+1] = false;
	}
}
#endif

bool **CreateMesh(int I, int J) {
	bool **Mesh = new bool *[I];
	for (int i = 0; i < I; i++) {
		Mesh[i] = new bool[J];
		for (int j = 0; j < J; j++) Mesh[i][j] = false;
	}
	return(Mesh);
}

int main(int argc, char **argv) {
	srand48(8767134);
	bool *locks = new bool[SIZE + 2];
	for (int i = 0; i < SIZE + 2; i++) locks[i] = false;
	bool **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
	bool **MeshB = CreateMesh (SIZE + 2, SIZE + 2);

	for (int i = 1; i <= SIZE; i++) {
		for (int j = 1; j <= SIZE; j++) {
			if (drand48() < 0.05) {
				MeshA[i][j] = true;
			}
		}
	}

	for (int n = 0; n < STEPS; n++) {
		for (int i = 1; i <= SIZE; i++) {
			for (int j = 1; j <= SIZE; j++) if (MeshA[i][j] == true) { 
				MeshA[i][j] = false;
				double move = drand48();
				if (move < 1.0*MOVE) {
					MeshB[i-1][j] = true;
				} else if (move < 2.0*MOVE) {
					MeshB[i+1][j] = true;
				} else if (move < 3.0*MOVE) {
					MeshB[i][j-1] = true;
				} else if (move < 4.0*MOVE) {
					MeshB[i][j+1] = true;
				} else {
					MeshB[i][j] = true;
				}
			} 
		}
		swap(MeshA, MeshB);
		
		for(int p = 0; p <= SIZE; p++) {
			for(int q = 0; q <= SIZE; q++) {
				if(MeshA[p][q] == true) {
					cout<<"X | ";
				} else {
					cout<<"0 | ";
				}
			}
			cout<<"\n";
		}
		cout<<"\n\n\n\n\n\n\n\n\n";
	}
}

