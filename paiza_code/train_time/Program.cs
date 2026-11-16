using System;
public class times
{
    public int L;
    public int R;
    
    public times(int l, int r)
    {
        this.L=l;
        this.R=r;
    }
    
    public static int is_ok(times T_1, times T_2)
    {
        if((T_1.L-T_2.L)*(T_1.R-T_2.R)>0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

class Program
{
    
    static void mergesort(times[] t_list, int head, int tail)
    {
        if(head==tail){return;}
        else
        {
            int mid=head+(tail-head)/2;
            mergesort(t_list, head, mid);
            mergesort(t_list, mid+1, tail);
            merge(t_list, head, mid, tail);
            
        }
    }
    
    static void merge(times[] t_list, int head, int mid, int tail)
    {
        times[] temp = new times[tail-head+1];
        int i=head;//左がどこまで進んだか
        int j=mid+1;//右がどこまで進んだか
        int k=0;//新たな配列がどこまで埋まったか
        
        //比較して小さい順に追加する
        while(i<=mid && j<=tail)
        {
            if(t_list[i].L<t_list[j].L)
            {
                temp[k++]=t_list[i++];
            }
            else{temp[k++]=t_list[j++];}
        }
        if(i>mid)
        {
            while(j<=tail){temp[k++]=t_list[j++];}
        }
        else{while(i<=mid){temp[k++]=t_list[i++];}}
        
        for(int n=0;n<k;++n)
        {
            t_list[head+n]=temp[n];
        }
    }
    
    static void Main()
    {
        // 自分の得意な言語で
        // Let's チャレンジ！！
        int judge=1;
        int N = int.Parse(Console.ReadLine());
        times[] t_list = new times[N];
        int[] line=new int[2];
        for(int i=0;i<N;++i)
        {
            line=Array.ConvertAll(Console.ReadLine().Split(), int.Parse);
            t_list[i]=new times(line[0], line[1]);
        }
        
        mergesort(t_list, 0, N-1);
        for(int j=1;j<N;++j)
        {
            judge*=times.is_ok(t_list[j-1], t_list[j]);
        }
        
        if(judge==1){Console.WriteLine("Yes");}
        else{Console.WriteLine("No");}
        
        
    }
}