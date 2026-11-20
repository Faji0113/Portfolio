using System;
public class book
{
    public int page;
    public int date;
    
    public book(int p, int d){this.page=p;this.date=d;}
}
class Program
{
    static int compare2(int a, int b){return (a>b)? a:b;}
    
    static book max2(book b_1, book b_2, int i, int j, int M, book[] lib)
    {
        int p_1 = b_1.page + lib[i-1].page;
        int d_1 = b_1.date +lib[i-1].date;
        if(p_1>b_2.page)
        {
            return new book(p_1, d_1);
            
        }
        else if(p_1==b_2.page){return (d_1>b_2.date)? b_2:new book(p_1, d_1);}
        else{return b_2;}
        
    }
    
    
    static void Main()
    {
        // 自分の得意な言語で
        // Let's チャレンジ！！
        var line = Console.ReadLine().Split();
        int N = int.Parse(line[0]);//本の数
        int M = int.Parse(line[1]);//残り日数
        
        book[] lib=new book[N];
        for(int k=0;k<N;++k)
        {
            int[] status = Array.ConvertAll(Console.ReadLine().Split(), int.Parse);
            lib[k] = new book(status[0], status[1]);
        }
        
        book[][] nap=new book[N+1][];
        for(int x=0;x<=N;++x)
        {
            nap[x] = new book[M+1];
        }
        book zeros = new book(0, 0);
        Array.Fill(nap[0], zeros);
        //ナップザック問題i*j, i...使えるもの, j...容量上限
        for(int i=1;i<=N;i++)
        {
            nap[i][0]=zeros;
            for(int j=1;j<=M;j++)
            {
                if(j>=lib[i-1].date)
                {
                    nap[i][j]=max2(nap[i-1][j-lib[i-1].date],nap[i-1][j], i, j, M, lib);
                    
                }
                else{nap[i][j]=nap[i-1][j];}
            }
        }
        Console.WriteLine(nap[N][M].page);
    }
}