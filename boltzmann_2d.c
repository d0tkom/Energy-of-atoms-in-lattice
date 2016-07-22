/***************************************************************
** This program simulates the Boltzmann distribution of
** quanta at thermal equilibrum in a 2D lattice.
** To run the program need to give two arguments: a seed for
** random number generation, and a desired size for the 
** square lattice.
*****************************************************************/



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define output_name "proj2.out"


int findMostQuanta(double** lattice, int size);
int findVoidSites(double** lattice, int size);
int findOneSites(double** lattice, int size);
int findTwoSites(double** lattice, int size);
int findThreeSites(double** lattice, int size);

int main(int argc, char *argv[])
{	
	int x,y,x2,y2,i,e,mostQuanta,voidSites,nonEquilibrium,invalidInput;
	int oneSites,twoSites,threeSites,allQuanta;
	int seed,size;
	double** lattice;
	double* numbers;

	
	/* check for correct input, and assign command line arguments to variables */
	invalidInput=((argc!=3)||(sscanf(argv[1],"%d",&seed)!=1)||
	(sscanf(argv[2],"%d",&(size)))!=1);
	if(invalidInput)
	{
		printf("Input is in wrong format.\n");
		exit(EXIT_FAILURE);
	}

	/* declare 2D array, and check that pointer to array is not NULL */
	lattice=(double**)malloc(size*sizeof(double*));
	if(lattice == NULL)
	{
		printf("Can't allocate memory for the array.\n");
		exit(EXIT_FAILURE);
	}
	for (i=0; i<size; i++)
	{
		lattice[i]=(double*)malloc(size*sizeof(double));
		if(lattice[i]==NULL)
		{
			printf("Can't allocate memory for the array.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* assign number of quanta in the system to a variable */
	allQuanta=size*size;
	/* assign 1 quantum per lattice site randomly to every lattice site */
	srand(seed); 
	for (i=0;i<allQuanta;i++) 
	{	
		x=rand()%size;
		y=rand()%size;
		lattice[x][y]++;
	}
	/* count lattice sites with specified quanta number */
	voidSites=findVoidSites(lattice,size);
	oneSites=findOneSites(lattice,size);
	twoSites=findTwoSites(lattice,size);
	threeSites=findThreeSites(lattice,size);

	/* function moves quanta until equilibrium is reached */
	nonEquilibrium = 1;
	while(nonEquilibrium)
	{
		/* terms for equilibrium determined by exp. 
		for higher precision four terms are specified */
		nonEquilibrium=(voidSites<(allQuanta*0.5))||(oneSites<(allQuanta*0.25))||
		(twoSites<(allQuanta*0.125))||(threeSites<(allQuanta*0.0625));
		
		/* generates random lattice coordinates */
		x=rand()%size; 
		y=rand()%size;
		x2=rand()%size;
		y2=rand()%size;
		
		/* moves quanta if initial lattice site is not void, 
		and lattice sites aren't identical */
		if((lattice[x][y]!=0)&&((x!=x2)||(y!=y2)))
		{
			lattice[x][y]--; 
			lattice[x2][y2]++;
			
			/* keeps track of lattice sites with specified quanta numbers */
			if(lattice[x][y]==0)
			{
				voidSites++;
			}
			if(lattice[x2][y2]==1)
			{
				voidSites--;
			}
			if(lattice[x][y]==1)
			{
				oneSites++;
			}
			if(lattice[x2][y2]==2)
			{
				oneSites--;
			}			
			if(lattice[x][y]==2)
			{
				twoSites++;
			}
			if(lattice[x2][y2]==3)
			{
				twoSites--;
			}
			if(lattice[x][y]==3)
			{
				threeSites++;
			}
			if(lattice[x2][y2]==4)
			{
				threeSites--;
			}							
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
			numbers[(int)lattice[i][e]]++; 		
		}
	}
	
	/* free memory from lattice*/
	for (i=0; i<size; i++)
	{
		free((void*)lattice[i]);
	}
	free((void*)lattice);
	
	FILE* output;
	/* open file */
	output = fopen(output_name,"a");
	if(output == (FILE*) NULL)
	{
		printf("Couldn't open output file.\n");
		exit(EXIT_FAILURE);
	}	
	/* validate that pointer to output file is not NULL */

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
int findMostQuanta(double** lattice, int size)
{	
	int mostQuanta,i,e;
	mostQuanta=0;
	for(i=0;i<size-1;i++)
	{
		for(e=0;e<size-1;e++)
		{
			if(lattice[i][e]>mostQuanta)
			{
				mostQuanta=lattice[i][e]; 
			}
		}
	}
	return(mostQuanta);
}
/* functions to count specified lattice sites */
int findVoidSites(double** lattice, int size)
{
	int voidSites,i,e;
	voidSites=0;
	i=0;
	e=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			if(lattice[i][e]==0)
			{
			voidSites++;
			}
		}
	}
	return(voidSites);
}	

int findOneSites(double** lattice, int size)
{
	int oneSites,i,e;
	oneSites=0;
	i=0;
	e=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			if(lattice[i][e]==1)
			{
			oneSites++;
			}
		}
	}
	return(oneSites);
}	

int findTwoSites(double** lattice, int size)
{
	int twoSites,i,e;
	twoSites=0;
	i=0;
	e=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			if(lattice[i][e]==2)
			{
			twoSites++;
			}
		}
	}
	return(twoSites);
}	

int findThreeSites(double** lattice, int size)
{
	int threeSites,i,e;
	threeSites=0;
	i=0;
	e=0;
	for(i=0;i<size;i++)
	{
		for(e=0;e<size;e++)
		{
			if(lattice[i][e]==2)
			{
			threeSites++;
			}
		}
	}
	return(threeSites);
}	