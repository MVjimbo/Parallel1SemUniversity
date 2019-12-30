#include <iostream>
#include <map>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <omp.h>
using namespace std;
const char * ar[] = {"a", "b", "c", "d", "e", "f", "g", "h", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
        "u", "v", "w", "x", "y", "z"};
int main() {
        srand(time(NULL));
        int num_threads=8;
        int n=500;
        string fs="ja";//Строка, которую ищем
        //string  s = "qwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwerjauiopqwertyuiop";//Строка, в котрой ищем
        string s="";
        for(int i=0; i < n; i++) s += ar[rand() % (sizeof ar/sizeof (char *))];//Рандомное заполнение строки
        int lengthS=s.length();

        map <char,int> mp;
        for (int i=0;i<fs.length();i++)
        {
                char c=fs[i];
                mp[c]=i;
        }

        //Поиск первого вхождения строки
        int num_blocks=num_threads;
        int num_blocks_count=lengthS/num_blocks;
        int ost_count=lengthS%num_blocks;
        int *arr=new int[num_blocks+1];
        arr[0]=0;
        for (int i=1;i<=ost_count;i++)
                arr[i]=i*(num_blocks_count+1);
        for (int i=1;i<num_blocks-ost_count+1;i++)
                arr[ost_count+i]=ost_count*(num_blocks_count+1)+i*num_blocks_count;

        for (int i=1;i<=num_blocks;i++)
                while(mp.find(s[arr[i]-1])!=mp.end())
                        arr[i]--;

        int *find_in_threads=new int[num_blocks];//Массив для сохранения результатов для каждого потока

        #pragma omp parallel for num_threads(num_threads)
        for (int i=0;i<num_blocks;i++)
        {
                string str=string(s,arr[i],arr[i+1]-arr[i]);
                find_in_threads[omp_get_thread_num()]=str.find(fs);
        }

        int result=-1;
        for (int i=0;i<num_blocks;i++)
                if ((result==-1) && (find_in_threads[i]!=-1))
                {
                        result=find_in_threads[i]+arr[i];
                        break;
                }
        if ((result==-1) && (arr[num_blocks]!=lengthS-1))
        {
                string str=string(s,arr[num_blocks],lengthS-arr[num_blocks]);
                result=str.find(fs);
                if (result!=-1) result+=arr[num_blocks];
        }

        cout<<result<<endl;

        return 0;
}

