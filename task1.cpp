#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

int main()
{
        srand(time(NULL));
        int n=1000;
        int a=-1000;
        int b=1000;
        int arr[n][n];
        for (int i=0;i<n;i+=1)
        for (int j=0;j<n;j+=1)
        {
                arr[i][j]=rand()%b+a;
        }
        int min=b+a;
        int max=a;
        for (int i=0;i<n;i+=1)
        {
                min=b+a;
                for (int j=0;j<n;j++)
                        min=std::min(min,arr[i][j]);
                max=std::max(min,max);
        }
        printf("\n");
        printf("%i ",max);
        min=b+a;
        max=a;
	#pragma omp parallel for reduction(max: max)
        for (int i=0;i<n;i+=1)
        {
                int min1=b+a;
                for (int j=0;j<n;j++)
                        min1=std::min(min1,arr[i][j]);
                max=std::max(min1,max);
        }
        printf("%i ",max);
        return 0;
}

