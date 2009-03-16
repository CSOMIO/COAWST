
#include "mpiP.h"



/*
 * COLLECTIVE
 */


FORT_NAME( mpi_barrier , MPI_BARRIER )(int *comm, int *ierror)
{
  *ierror=MPI_Barrier( *comm );
}


int MPI_Barrier(MPI_Comm comm )
{
  return(MPI_SUCCESS);
}


/*********/


FORT_NAME( mpi_bcast , MPI_BCAST )(void *buffer, int *count, int *datatype,
				   int *root, int *comm, int *ierror )
{
  *ierror=MPI_Bcast(buffer, *count, *datatype, *root, *comm);
}



int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype,
	      int root, MPI_Comm comm )
{
  if (root!=0)
    {
      fprintf(stderr,"MPI_Bcast: bad root = %d\n",root);
      abort();
    }


  return(MPI_SUCCESS);
}


/*********/


FORT_NAME( mpi_gather , MPI_GATHER )
                       (void *sendbuf, int *sendcount, int *sendtype,
			void *recvbuf, int *recvcount, int *recvtype,
			int *root, int *comm, int *ierror)
{
  *ierror=MPI_Gather( sendbuf, *sendcount, *sendtype,
		      recvbuf, *recvcount, *recvtype,
		      *root, *comm);
}


int MPI_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
	       void* recvbuf, int recvcount, MPI_Datatype recvtype,
	       int root, MPI_Comm comm)
{
  if (root!=0)
    {
      fprintf(stderr,"MPI_Gather: bad root = %d\n",root);
      abort();
    }

  memcpy(recvbuf,sendbuf,sendcount*sendtype);

  return(MPI_SUCCESS);
}

/*********/



FORT_NAME( mpi_gatherv , MPI_GATHERV )
                        ( void *sendbuf, int *sendcount, int *sendtype,
			  void *recvbuf, int *recvcounts, int *displs,
			  int *recvtype, int *root, int *comm, int *ierror)
{
  *ierror=MPI_Gatherv( sendbuf, *sendcount, *sendtype,
		       recvbuf, recvcounts, displs,
		       *recvtype, *root, *comm);
}


int MPI_Gatherv(void* sendbuf, int sendcount, MPI_Datatype sendtype, 
		void* recvbuf, int *recvcounts, int *displs,
		MPI_Datatype recvtype, int root, MPI_Comm comm)
{
  int offset;

  if (root!=0)
    {
      fprintf(stderr,"MPI_Gatherv: bad root = %d\n",root);
      abort();
    }

  offset=displs[0]*recvtype;
  memcpy( (char *)recvbuf+offset, sendbuf, recvcounts[0] * recvtype);

  return(MPI_SUCCESS);
}



/*********/


FORT_NAME( mpi_allgather , MPI_ALLGATHER )
                          ( void *sendbuf, int *sendcount, int *sendtype,
			    void *recvbuf, int *recvcount, int *recvtype,
			    int *comm, int *ierror)
{
  *ierror=MPI_Allgather( sendbuf, *sendcount, *sendtype,
			 recvbuf, *recvcount, *recvtype,
			 *comm );
}


int MPI_Allgather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
		  void* recvbuf, int recvcount, MPI_Datatype recvtype,
		  MPI_Comm comm)
{

  memcpy(recvbuf,sendbuf,sendcount * sendtype);

  return(MPI_SUCCESS);

}

/*********/


FORT_NAME( mpi_scatterv , MPI_SCATTERV )
                         ( void *sendbuf, int *sendcounts, int *displs,
			   int *sendtype, void *recvbuf, int *recvcount,
			   int *recvtype, int *root, int *comm, int *ierror)
{
  *ierror=MPI_Scatterv(sendbuf, sendcounts, displs,
		       *sendtype, recvbuf, *recvcount,
		       *recvtype, *root, *comm);
}
		       
		       

int MPI_Scatterv(void* sendbuf, int *sendcounts, int *displs, 
		 MPI_Datatype sendtype, void* recvbuf, int recvcount, 
		 MPI_Datatype recvtype, int root, MPI_Comm comm)
{
  int offset;

  if (root!=0)
    {
      fprintf(stderr,"MPI_Scatterv: bad root = %d\n",root);
      abort();
    }

  offset=displs[0]*sendtype;
  memcpy(recvbuf,(char *)sendbuf+offset,sendcounts[0] * sendtype);
  
  return(MPI_SUCCESS);
}



/*********/


FORT_NAME( mpi_reduce , MPI_REDUCE )
                       ( void *sendbuf, void *recvbuf, int *count,
			 int *datatype, int *op, int *root, int *comm,
			 int *ierror)
{
  *ierror=MPI_Reduce(sendbuf, recvbuf, *count,
		     *datatype, *op, *root, *comm);
}



int MPI_Reduce(void* sendbuf, void* recvbuf, int count, 
	       MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)

{
  if (root!=0)
    {
      fprintf(stderr,"MPI_Reduce: bad root = %d\n",root);
      abort();
    }

  memcpy(recvbuf,sendbuf,count * datatype);

  return(MPI_SUCCESS);
}


/*********/


FORT_NAME( mpi_allreduce , MPI_ALLREDUCE )
                          ( void *sendbuf, void *recvbuf, int *count,
			    int *datatype, int *op, int *comm, int *ierror)
{
  *ierror=MPI_Allreduce(sendbuf, recvbuf, *count,
			*datatype, *op, *comm);

}


int MPI_Allreduce(void* sendbuf, void* recvbuf, int count, 
		  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{

  memcpy(recvbuf,sendbuf,count * datatype);

  return(MPI_SUCCESS);

}


