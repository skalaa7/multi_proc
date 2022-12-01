#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char * argv[])
{
	int tc=strtol(argv[1],NULL,10);
	int n;
	double sum = 0;
	printf("Number: ");
	std::cin>>n;
	double s = omp_get_wtime();
	#pragma omp parallel for num_threads(tc) reduction(+:sum)
	for(int i = 1; i <= (int)n; i++)
		sum+=(double)i;
		
	s=omp_get_wtime()-s;
	printf("\nSum is %lf\n",sum);
	printf("Executed for %lf ms\n",s*1000);	
	return 0;
}
