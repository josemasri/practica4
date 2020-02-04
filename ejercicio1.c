#include "functions.h"
#include <stdio.h>

#define N_PROC 10
#define MASTER 0
#define N_NEIGHBORS 2

int main(int argc, char **argv)
{
    // Manejador del grupo global
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /*Para obtener el ID*/
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /*Para obtener el número de procesos*/

    /*Manejador del grupo global*/
    MPI_Group globalGroupManager;
    MPI_Comm_group(MPI_COMM_WORLD, &globalGroupManager);
    // Creando arreglo de grupos
    // Todos los procesos estarán en todos los grupos
    int **groupsArr = getGroups(N_PROC, 3);

    // Crear N_PROC manejadores de grupo
    MPI_Group groupMangers[N_PROC];
    // Crear N_PROC comunicadores de grupo
    MPI_Comm groupCommunicators[N_PROC];
    for (int i = 0; i < N_PROC; i++)
    {
        createGroup(globalGroupManager, 3, groupsArr[i], &groupMangers[i]);
        createCommunicator(MPI_COMM_WORLD, groupMangers[i], &groupCommunicators[i]);
    }
    // Colocar una barrera que todos creen los grupos
    MPI_Barrier(MPI_COMM_WORLD); /*Esperamos a los nodos*/

    // --------------------------
    // Comienza el algoritmo
    // --------------------------

    // Bandera de Mensaje recivido
    int MessageRecived = 0;
    char message;
    // Vecinos (A quienes puedo enviar, de quienes puedo recibir)
    int *neighbors = getNeighbors(world_rank, N_NEIGHBORS);
    
    // Iniciar la inundación con el proceso 0
    if (world_rank == 0)
    {
        message = 'A';
        MessageRecived = 1;
        MPI_Bcast(&message, 1, MPI_CHAR, MASTER, groupCommunicators[0]);
        printf("Soy el MASTER y envie el mensaje: %c", message);
    }
    else
    {
        // Reciving from neighbors and sending to neighbors
        for (size_t i = 0; i < N_NEIGHBORS; i++)
            MPI_Bcast(&message, 1, MPI_CHAR, MASTER, groupCommunicators[neighbors[i]]);

        if (message == 'A')
        {
            MessageRecived = 1;
            MPI_Bcast(&message, 1, MPI_CHAR, MASTER, groupCommunicators[world_rank]);
        }
    }
    printf("Soy el proceso %d y el mensaje es %c", world_rank, message);
    MPI_Finalize();
    return 0;
}
