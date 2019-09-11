#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>


int place(int board[], int row ,int column);
void queen(int board[],int row, int c, int col, int origrow, int origcol);
void callq(int column, int N);
int count;
int global_count;


int main(int argc, char *argv[]) {

    if(argc < 1)
	printf("Not enough arguments\n USAGE: mpirun -np num queenpb\n N must be equal to np");

    int N = atoi(argv[1]);
    int i;
    int id;
    int p;

    MPI_Init (&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    double t1 =  MPI_Wtime();
    callq(id+1, N);

    MPI_Barrier(MPI_COMM_WORLD);

    //add all individual counts to the total
    MPI_Reduce (&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(!id){
		double t2 = - t1 + MPI_Wtime();
        	printf("Time spent: %lf sec\n", t2);
		printf("Total Solutions: %d\n",  global_count);
	}


    MPI_Finalize();
    return 0;
}



int place(int board[], int row,int column)
{

    int i;

    for(i = 1; i <= row - 1; ++i)
    {

       //checking column and diagonal conflicts
       if(board[i] == column)
           return 0;
       else if(abs(board[i]-column) == abs(i-row))
           return 0;
 }

    return 1;
}

//function to check for proper positioning of queen
void queen(int board[], int row, int c, int col, int origrow, int origcol)
{
    int column;

    for(column = col; column <= c; ++column)
    {
        if(row == origrow && column == origcol + 1)  
            return;

        if(place(board, row, column))
        {
            board[row] = column; //no conflicts so place queen
            if(row == c)	//if bottom of the board is reached, then a solution has been found
   	        count++;
            else //if queen is placed, recursively call the function 
                queen(board, row+1, c, 1, origrow, origcol);
        }
    }
}

//function for threads to call 
void callq(int column, int N){

    int board[20];
    queen(board, 1, N, column, 1, column);

}
