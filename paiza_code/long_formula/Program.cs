using System;
class Program
{
    static void Main()
    {
        // 自分の得意な言語で
        // Let's チャレンジ！！
        bool is_allone;
        int N = int.Parse(Console.ReadLine());
        int answer=0;
        if(N==1){Console.WriteLine(0);}
        else
        {
            int[] sums=new int[(N+1)/2];
            int[] fugo=new int[(N+1)/2];
            string str=Console.ReadLine();
            for(int i=0;i<(N+1)/2;i++)
            {
                if(i==0)fugo[i]=1;//最初にプラスを追加
                else{fugo[i]=(str[2*i-1]=='+')? 1:-1;}
            }
            
            for(int j=0;j<(N+1)/2;++j)
            {
                if(j==0)sums[j]=1;
                else sums[j]=sums[j-1]+fugo[j];
                if(sums[j]==0&&answer<2*j+1){answer=2*j+1;}
            }
            
            for(int k=0;k<(N+1)/2;++k)
            {
                for(int l=k+1;l<(N+1)/2;++l)
                {
                    sums[l]-=fugo[k];
                    if(sums[l]==0 && answer<2*(l-k)-1)
                    {
                        answer=2*(l-k)-1;
                    }
                }
            }
            
            
            
            Console.WriteLine(answer);
            
        }
    }
}