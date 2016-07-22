/***************************************************************
** This program simulates the Boltzmann distribution of
** quanta at thermal equilibrum in a 3D lattice.
** To run the program need to give two arguments: a seed for
** random number generation, and a desired size for the 
** cubic lattice.
*****************************************************************/



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define output_name "proj2_ext.out"


int findMostQuanta(double*** lattice, int size);
int findVoidSites(double*** lattice, int size);
int findOneSites(double*** lattice, int size);
int findTwoSites(double*** lattice, int size);


int main(int argc, char *argv[])
{	
	int x,y,z,x2,y2,z2,i,e,j,seed,size,nonEquilibrium,mostQuanta,voidSites,invalidInput;
	int oneSites,twoSites,allQuanta;
	double*** lattice;
	double* numbers;

	
	/* check for correct input and also assign command line arguments to variables */
	invalidInput=((sscanf(argv[1],"%d",&seed)!=1)||
	(sscanf(argv[2],"%d",&(size)))!=1);
	if(invalidInput)
	{
		printf("Input is in wrong format.\n");
		exit(EXIT_FAILURE);
	}
	
	/* declare 3D array, and check that pointer to array is not NULL */
	lattice = (double***)malloc((size*sizeof(double*))+(size*size*sizeof(double**))+ 
	(size*size*size*sizeof(double)));
	if(lattice == NULL)
	{
		printf("Can't allocate memory for the array.\n");
		exit(EXIT_FAILURE);
	}
	for(i=0;i<size;++i)
	{
    	lattice[i]=(double**)(lattice+size)+i*size;
		if(lattice[i]==NULL)
		{
			printf("Can't allocate memory for the array.\n");
			exit(EXIT_FAILURE);
		}    	
    	for(e = 0; e < size; ++e) 
    	{
			lattice[i][e]=(double*)(lattice+size+size*size)+i*size*size+e*size;
			if(lattice[i]==NULL)
			{
				printf("Can't allocate memory for the array.\n");
				exit(EXIT_FAILURE);
			}		
		}
	}
	
	/* assign 1 quantum per lattice site randomly to every lattice site */
	srand(seed); 
	allQuanta=size*size*size;
	for (i=0;i<allQuanta;i++) 
	{	
		x=rand()%size;
		y=rand()%size;
		z=rand()%size;
		lattice[x][y][z]++;
	}
	
	/* count void lattice sites in external function */
	voidSites=findVoidSites(lattice,size);
	oneSites=findOneSites(lattice,size);
	twoSites=findTwoSites(lattice,size);


	/* function moves quanta until equilibrium is reached */
	nonEquilibrium=1; 
	while(voidSites<nonEquilibrium)
	{
		/* limits for equilibrium determined by exp. for speed only three term 
		is specified. */
		nonEquilibrium=(voidSites<(allQuanta*0.5))||(oneSites<(allQuanta*0.25))||
		(twoSites<(allQuanta*0.125));
		
		/* generates random lattice coordinates */
		x=rand()%size; 
		y=rand()%size;
		z=rand()%size;
		x2=rand()%size;
		y2=rand()%size;
		z2=rand()%size;
		
		/* moves quanta if initial lattice site is not void, 
		and lattice sites aren't identical */
		if((lattice[x][y][z]!=0)&&((x!=x2)||(y!=y2)||(z!=z2)))
		{
			lattice[x][y][z]--; 
			lattice[x2][y2][z2]++;
			
			/* keeps track of lattice sites with specified quanta numbers */
			if(lattice[x][y][z]==0)
			{
				voidSites++;
			}
			if(lattice[x2][y2][z2]==1)
			{
				voidSites--;
			}
			/*if(lattice[x][y][z]==1)
			{
				oneSites++;
			}
			if(lattice[x2][y2][z2]==2)
			{
				oneSites--;
			}			
			if(lattice[x][y][z]==2)
			{
				twoSites++;
			}
			if(lattice[x2][y2][z2]==3)
			{
				twoSites--;
			}*/
							
		}
	}
	
	/* use of outside function to find size for numbers array */
	mostQuanta=findMostQuanta(lattice,size);


	/* declare array for counting lattice sites with given number of quanta */
	numbers=(double*)malloc(mostQuanta*sizeof(double)); 
	
	/* counting lattice sites with given number of quanta */
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{	
			for(j=0;j<size;j++)
			{
				numbers[(int)lattice[i][e][j]]++;
			}		
		}
	}
	/* free memory */
	free((void*)lattice);
	FILE* output;
	output = fopen(output_name,"a");
	if(output == (FILE*) NULL)
	{
		printf("Couldn't open output file.\n");
		exit(EXIT_FAILURE);
	}
	/* prints out the results */	
	fprintf(output,"v	m(e)\n");
	for(i=0;i<=mostQuanta;i++) 
	{
		fprintf(output,"%d	",i); 
		fprintf(output,"%d\n",(int)numbers[i]);
	}
			
	/* close output file and free memory */
	fclose(output);
	free((void*)numbers);	
	return(0);
}

/* function to find highest number of quanta on one lattice site */
int findMostQuanta(double*** lattice, int size)
{	
	int mostQuanta,i,e,j;
	mostQuanta=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			for(j=0;j<size;j++)
			{
				if(lattice[i][e][j]>mostQuanta)
				{
					mostQuanta=lattice[i][e][j]; 
				}
			}
		}
	}
	return(mostQuanta);
}
/* functions to count specified lattice sites */
int findVoidSites(double*** lattice, int size)
{
	int voidSites,i,j,e;
	voidSites=0;
	i=0;
	j=0;
	e=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			for(j=0;j<size;j++)
			{
				if(lattice[i][e][j]==0)
				{
				voidSites++;
				}
			}
		}
	}
	return(voidSites);
}

int findOneSites(double*** lattice, int size)
{
	int oneSites,i,e,j;
	oneSites=0;
	i=0;
	e=0;
	j=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			for(j=0;j<size;j++)
			{
				if(lattice[i][e][j]==1)
				{
				oneSites++;
				}
			}
		}
	}
	return(oneSites);
}	

int findTwoSites(double*** lattice, int size)
{
	int twoSites,i,e,j;
	twoSites=0;
	i=0;
	e=0;
	j=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			for(j=0;j<size;j++)
			{
				if(lattice[i][e][j]==2)
				{
				twoSites++;
				}
			}
		}
	}
	return(twoSites);
}	
	 