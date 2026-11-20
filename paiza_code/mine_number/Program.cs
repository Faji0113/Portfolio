using System;

class Program
{
    static int find(long l, long[] count, int limit)
    {
        int head=1;
        int tail=limit;
        if(count[head]>l){return 0;}
        if(count[tail]<=l){return tail;}
        while(head+1<tail)
        {
            int mid=head+(tail-head)/2;
            if(count[mid]>l)
            {
                tail=mid;
            }
            if(count[mid]<=l)
            {
                head=mid;
            }
            
        }
        return head;
        
    }
    
    static void Main()
    {
        long num=0;//掘れた鉱石の数
        var line = Console.ReadLine().Split();
        int N = int.Parse(line[0]);//鉱石の種類
        int D = int.Parse(line[1]);//日数
        
        long[] jewels=new long[N];//この要素を足して人数にできるだけ近い数を作ればOK
        long[] labor =new long[D];
        jewels = Array.ConvertAll(Console.ReadLine().Split(), long.Parse);
        labor =Array.ConvertAll(Console.ReadLine().Split(), long.Parse);
        Array.Sort(jewels);
        
        int M=0;//労働者数の最大値
        foreach(int x in labor) if(x>M) M=x;
        long[] count=new long[N+1];//N種類の鉱石を掘れる人数の最小値
        Array.Fill(count, 0);
        //種類ごとに更新
        for(int n=0;n<N;++n)
        {
            count[n+1]=count[n]+jewels[n];
            
        }
        
        
        //日ごとに鉱石数を更新
        for(int i=0;i<D;i++)
        {
            num+=find(labor[i], count, N);
        }
        Console.WriteLine(num);
    }
}