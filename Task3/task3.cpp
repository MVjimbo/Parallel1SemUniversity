#include <stdio.h>
#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

int main()
{
        srand(time(NULL));
        int m=1000,n=1000;
        int num_threads=12;
        int a=0;
        int bpoint=100;
        double A[m][n];
        double x[n];
        //Генерация массива и вектора
        for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
                A[i][j]=rand()%(bpoint-a)+a;
        for (int j=0;j<n;j++)
                x[j]=rand()%(bpoint-a)+a;

        double b[m];
        double bp[m];

        //Перемножение в последовательном режиме
        double start=omp_get_wtime();
        for (int i=0;i<m;i++)
                b[i]=0;

        for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
                b[i]+=A[i][j]*x[j];
        double nonparallel=omp_get_wtime()-start;
        printf("Non parallel %f\n",nonparallel);





        //Перемножение в параллельном режиме с разделением по строкам
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
                bp[i]=0.0;

        #pragma omp parallel for num_threads(num_threads)
        for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
                bp[i]+=A[i][j]*x[j];
        double end=omp_get_wtime();
        printf("Rows division %f  ,  %f\n",end-start,nonparallel/(end-start));

        bool isEqual=true;
        int row=0;
        for (int i=0;i<m;i++)
                if (abs(bp[i]-b[i])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        break;
                }

        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i\n",row);




        //Перемножение в параллельном режиме с разделением по столбцам
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
                bp[i]=0;
        double **mass=new double*[num_threads];
        for (int i=0;i<num_threads;i++)
                mass[i]=new double[m];
        #pragma omp parallel num_threads(num_threads)
        {
        #pragma omp for
        for (int j=0;j<n;j++)
        for (int i=0;i<m;i++)
                mass[omp_get_thread_num()][i]+=A[i][j]*x[j];
        }
        for (int j=0;j<num_threads;j++)
        for (int i=0;i<m;i++)
                bp[i]+=mass[j][i];
        end=omp_get_wtime();
        printf("Column division %f  ,  %f\n",end-start,nonparallel/(end-start));

        isEqual=true;
        row=0;
        for (int i=0;i<m;i++)
                if (abs(bp[i]-b[i])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        break;
                }
        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i\n",row);




        //Перемножение в параллельном режиме с блочным разделением по строкам
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
                bp[i]=0.0;

        int num_bloks_rows=m/num_threads;
        int ost_rows=m%num_threads;
        int *arr=new int[num_threads+1];
        arr[0]=0;
        for (int i=1;i<=ost_rows;i++)
                arr[i]=i*(num_bloks_rows+1);

        for (int i=1;i<num_threads-ost_rows+1;i++)
                arr[ost_rows+i]=ost_rows*(num_bloks_rows+1)+i*num_bloks_rows;

        #pragma omp parallel for num_threads(num_threads)
        for (int block=0;block<num_threads;block++)
                for(int i=arr[block];i<arr[block+1];i++)
                        for (int j=0;j<n;j++)
                                bp[i]+=A[i][j]*x[j];
        end=omp_get_wtime();
        printf("Block division(rows) %f  ,  %f\n",end-start,nonparallel/(end-start));

        isEqual=true;
        row=0;
        for (int i=0;i<m;i++)
                if (abs(bp[i]-b[i])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        break;
                }

        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i\n",row);




        //Перемножение в параллельном режиме с блочным разделением по столбцам
        start=omp_get_wtime();
        for (int i=0;i<m;i++)
                bp[i]=0;

        int num_bloks_columns=n/num_threads;
        int ost_columns=n%num_threads;
        int *arr1=new int[num_threads+1];
        arr1[0]=0;

        double **mass2=new double*[num_threads];
        for (int i=0;i<num_threads;i++)
                mass2[i]=new double[m];

        for (int i=1;i<=ost_columns;i++)
                arr1[i]=i*(num_bloks_columns+1);

        for (int i=1;i<num_threads-ost_columns+1;i++)
                arr1[ost_columns+i]=ost_columns*(num_bloks_columns+1)+i*num_bloks_columns;
        #pragma omp parallel for num_threads(num_threads)
        for (int block=0;block<num_threads;block++)
                for(int j=arr1[block];j<arr1[block+1];j++)
                        for (int i=0;i<m;i++)
                                mass2[omp_get_thread_num()][i]+=A[i][j]*x[j];

        for (int j=0;j<num_threads;j++)
        for (int i=0;i<m;i++)
                bp[i]+=mass2[j][i];
        end=omp_get_wtime();
        printf("Block division(columns) %f  ,  %f\n",end-start,nonparallel/(end-start));

        isEqual=true;
        row=0;
        for (int i=0;i<m;i++)
                if (abs(bp[i]-b[i])>0.00001)
                {
                        isEqual=false;
                        row=i;
                        break;
                }

        if (isEqual)
                printf("OK\n");
        else
                printf("Not equal %i\n",row);

        return 0;
}

