#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <ctime>


int main()
{
        srand(time(NULL));
        int m=500,n=500,p=500;
        int b=100;
        int A[m][n];
        int B[n][p];
        for (int i=0;i<m;i+=1)
        for (int j=0;j<n;j+=1)
        {
                A[i][j]=rand()%b;
        }
         for (int i=0;i<n;i+=1)
        for (int j=0;j<p;j+=1)
        {
                B[i][j]=rand()%b;
        }


        int C[m][p];
        int Cone[m][p];
        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                C[i][j]=0;

        clock_t start=clock();
        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
        for (int k=0;k<n;k+=1)
                C[i][j]+=A[i][k]*B[k][j];
        clock_t end=clock();
        double time=(double)(end-start)/CLOCKS_PER_SEC;
        printf("Standart %f \n", time);

        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
		Cone[i][j]=0;

        double wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
        for (int k=0;k<n;k+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        double wend=omp_get_wtime();
        printf("ijk %f \n",wend-wstart);


        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]=0;

        wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int i=0;i<m;i+=1)
        for (int k=0;k<n;k+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        wend=omp_get_wtime();
        printf("ikj %f \n",wend-wstart);


        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]=0;

        wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int j=0;j<p;j+=1)
        for (int i=0;i<m;i+=1)
        for (int k=0;k<n;k+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        wend=omp_get_wtime();
        printf("jik %f \n",wend-wstart);


	
        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]=0;

        wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int j=0;j<p;j+=1)
        for (int k=0;k<n;k+=1)
        for (int i=0;i<m;i+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        wend=omp_get_wtime();
        printf("jki %f \n",wend-wstart);


        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]=0;

        wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int k=0;k<n;k+=1)
        for (int j=0;j<p;j+=1)
        for (int i=0;i<m;i+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        wend=omp_get_wtime();
        printf("kji %f \n",wend-wstart);


        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]=0;

        wstart=omp_get_wtime();
        #pragma omp parallel for
        for (int k=0;k<n;k+=1)
        for (int i=0;i<m;i+=1)
        for (int j=0;j<p;j+=1)
                Cone[i][j]+=A[i][k]*B[k][j];
        wend=omp_get_wtime();
	 printf("kij %f \n",wend-wstart);

        return 0;
}

