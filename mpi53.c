#include<stdio.h>
#include <mpi.h>
#include<stddef.h>
#include <stdio.h>
int bsize=9,size,rank;
int a[9]={13,45,1,42,16,7,2,11,9};
typedef struct ele
{
	int b;
	int c;
	int in;
}elem;

void display()
{
	int i;
	printf("\n");
	for(i=0;i<bsize;i++)
	{
		printf("  %d",a[i]);
	}
	printf("\n");
}
void cmpswap(int *b,int *c)
{
	int t;
	if(*b>*c)
	{
		t=*b;
		*b=*c;
		*c=t;
	}
	printf("\nsw::nr1:%d  r2:%d ",*b,*c);
}
void sort()
{
	 int in,l,flag=0,out,j,k,i,b=0,x=0;
//structure declaration
  const int nitems=3;
  int blocklen[3]={1,1,1};
  MPI_Datatype types[3]={MPI_INT,MPI_INT,MPI_INT};
  MPI_Datatype mpi_ele_type;
  MPI_Aint offsets[3];
  offsets[0]=offsetof(elem,b);
  offsets[1]=offsetof(elem,c);
  offsets[2]=offsetof(elem,in);
  MPI_Type_create_struct(nitems,blocklen,offsets,types,&mpi_ele_type);
  MPI_Type_commit(&mpi_ele_type);
	elem obj;
	//end struct delclaration	  		
  if(rank==0)
  {
 	for(j=0;j<bsize;)
  	{
  		printf("\nj::%d\n",j);
		if(j%2==0)
		{
			k=0;
			while(k<bsize-1)
			{
			  	for(i=1;i<size && k<bsize-1;i=i+1)
			  	{
			  		obj.in=k;
			  		obj.b=a[k];
			  		obj.c=a[k+1];
			  		MPI_Send( &obj,1,mpi_ele_type,i,0, MPI_COMM_WORLD);//send obj
			  		printf("sent to :%d  k:%d\n",i,k);
			  		k+=2;
			 	}
			 	k=k-2*(i-1);
			 	for(i=1;i<size&&k<bsize-1;i++)
			 	{
				  	MPI_Status status;
				  	MPI_Recv( &obj,1,mpi_ele_type,MPI_ANY_SOURCE,0, MPI_COMM_WORLD, &status);
				  	k+=2;
				  	a[obj.in]=obj.b;
				  	a[(obj.in)+1]=obj.c;
				  	printf("root rec from :%d  a:%d  b:%d \n",MPI_ANY_SOURCE,a[obj.in],a[obj.in+1]);
			 	}
//			 	printf("\nk:%d",k);
			 }
	 	}
	 	else
	 	{
		  	k=1;
			while(k<bsize)
			{
			
			  	for(i=1;i<size&&k<bsize;i=i+1)
			  	{
			  		obj.in=k;
			  		obj.b=a[k];
			  		obj.c=a[k+1];
			  		MPI_Send( &obj,1,mpi_ele_type,i,0, MPI_COMM_WORLD);
//			  		printf("sent to :%d  k:%d\n",i,k);
				  	MPI_Status status;
				  	k+=2;
			 	}
			 	k=k-2*(i-1);
			 	for(i=1;i<size &&k<bsize;i++)
			 	{
				  	MPI_Status status;
				  	MPI_Recv( &obj,1,mpi_ele_type,MPI_ANY_SOURCE,0, MPI_COMM_WORLD, &status);
				  	k+=2;
				  	a[obj.in]=obj.b;
				  	a[(obj.in)+1]=obj.c;
//				  	printf("root rec from :%d  a:%d  b:%d \n",MPI_ANY_SOURCE,a[obj.in],a[obj.in+1]);
			 	}
			} 	
	 	}
	 	display();
		 	j++;

 	}
  }
  else
  {
  	elem robj;
  	if(rank<=bsize/2)
  	{
  		for(j=0;j<bsize;)
	  	{
	  		l=0;
	  	
	  		printf("\nhi:::j:%d rank:%d",j,rank);
	  		//while((l<(bsize-size) && bsize>=size) || ( l<(size-bsize+1)&& size>bsize) ||
	  		while(bsize-l>0 && rank<=(bsize-l)/2)
	  		{
//	 	  		printf("\nack Rank:%d",rank);
	  			if(rank<=(bsize-l)/2)
	  			{
	  				
				  	MPI_Status status;
				  	MPI_Recv( &robj,1,mpi_ele_type,0,0, MPI_COMM_WORLD, &status);
				 	printf("\n r1:%d  r2:%d \n",robj.b,robj.c); 	
					cmpswap(&robj.b,&robj.c);
				 	printf("received::  rank :%d  i:%d\n",rank,robj.in);
					MPI_Send( &robj,1,mpi_ele_type,0,0, MPI_COMM_WORLD);
					if(size<=bsize)
					{
						l=l+2*(size-1);//chk for more than 9;
//				  		printf("\nack Rank:%d  l:%d",rank,l);

					}
					else
					{
						break;
					}			
				}
			
//				printf("\nl::%d",l);
			}
			j++;
		}
	}
 }
 
}
int main (int argc, char* argv[])
{
 //	char name[len],proc[len];
 	MPI_Status status;
  MPI_Init (&argc, &argv);      /* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  
 sort();
 MPI_Finalize();
 if(rank==0)
 {
 	display();
 }
  return 0;
}
