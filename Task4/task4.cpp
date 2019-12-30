#include <stdio.h>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

int main()
{
        srand(time(NULL));
        int m=500,n=500,p=500;
        int num_threads=14;
        int a=0;
        int bpoint=10;
        double A[m][n],B[n][p],C[m][p],Cp[m][p];
        //Генерация массива
        for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
                A[i][j]=rand()%(bpoint-a)+a;

        for (int i=0;i<n;i++)
        for (int j=0;j<p;j++)
                B[i][j]=rand()%(bpoint-a)+a;


        //Перемножение матриц в последовательном режиме
        double start=omp_get_wtime();
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
                C[i][j]=0;

        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
        for (int k=0;k<n;k++)
                C[i][j]+=A[i][k]*B[k][j];
        double nonparallel=omp_get_wtime()-start;
        printf("Non parallel %f\n",nonparallel);





        //Перемножение матриц в параллельном режиме с ленточным разделением
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
                Cp[i][j]=0;

        #pragma omp parallel for num_threads(num_threads)
        for (int i=0;i<m;i++)
        for (int j=0,j1=i % p;j<p;j++,j1--)
        {
                if (j1<0) j1+=p;
                for(int k=0;k<n;k++)
                        Cp[i][j1]+=A[i][k]*B[k][j1];
        }
        double end=omp_get_wtime();
        printf("Tape separation  %f  ,  %f\n",end-start,nonparallel/(end-start));

        bool isEqual=true;
        int row=0;
        int column=0;
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
                if (abs(Cp[i][j]-C[i][j])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        column=j;
                        break;
                }
        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i %i\n",row,column);



        //Перемножение матриц в параллельном режиме с блочным разделением
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
                Cp[i][j]=0.0;

        int num_bloks_n=n/num_threads;
        int ost_n=n%num_threads;
        int *arr_n=new int[num_threads+1];
        arr_n[0]=0;


        /*int num_of_blocks_m=1;
        int num_bloks_m=m/num_of_blocks_m;
        int ost_m=m%num_of_blocks_m;
        int *arr_m=new int[num_of_blocks_m+1];
        arr_m[0]=0;

        int num_of_blocks_p=1;
        int num_bloks_p=p/num_of_blocks_p;
        int ost_p=p%num_of_blocks_p;
        int *arr_p=new int[num_of_blocks_p+1];
        arr_p[0]=0;*/

        for (int i=1;i<=ost_n;i++)
                arr_n[i]=i*(num_bloks_n+1);

        for (int i=1;i<num_threads-ost_n+1;i++)
                arr_n[ost_n+i]=ost_n*(num_bloks_n+1)+i*num_bloks_n;

        /*for (int i=1;i<=ost_m;i++)
                arr_m[i]=i*(num_bloks_m+1);

        for (int i=1;i<num_of_blocks_m-ost_m+1;i++)
                arr_m[ost_m+i]=ost_m*(num_bloks_m+1)+i*num_bloks_m;

        for (int i=1;i<=ost_p;i++)
                arr_p[i]=i*(num_bloks_p+1);

        for (int i=1;i<num_of_blocks_p-ost_p+1;i++)
                arr_p[ost_p+i]=ost_p*(num_bloks_p+1)+i*num_bloks_p;*/

        #pragma omp parallel for num_threads(num_threads)
        for (int block_n=0;block_n<num_threads;block_n++)
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
        for (int k=arr_n[block_n];k<arr_n[block_n+1];k++)
                #pragma omp atomic
                 Cp[i][j]+=A[i][k]*B[k][j];
        end=omp_get_wtime();
        printf("Block division  %f  ,  %f\n",end-start,nonparallel/(end-start));

        isEqual=true;
        row=0;
        column=0;
        for (int i=0;i<m;i++)
        for (int j=0;j<p;j++)
                if (abs(Cp[i][j]-C[i][j])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        column=j;
                        break;
                }
        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i %i\n",row,column);

        return 0;
}

