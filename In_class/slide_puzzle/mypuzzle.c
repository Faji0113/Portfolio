#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define side_length 3 //一辺の長さ
#define N (side_length*side_length) //タイルの枚数(空白を含む)
#define loop 30 //最初に崩すときに何回動かすか
#define direction 4//動かせる方向は4種類
//探索手法のパラメータ
#define ids -1 //IDS
#define h_0 0  //幅優先探索
#define h_1 1  //ハミング距離
#define h_2 2  //マンハッタン距離

typedef struct location {int x; int y;}loc; //各数字のタイルの座標。空白が0

typedef struct Node
{
    loc* n;
    struct Node* p;//親ノードのポインタ
    int g;//移動コスト
    int h;//ヒューリスティック
}Node;

//ノードの線形リスト
typedef struct list
{
    Node* node;
    struct list* next;

}list;

void moveleft(loc* board); //空白を左へ
void moveright(loc* board); //空白を右へ
void moveup(loc* board); //空白を上へ
void movedown(loc* board); //空白を下へ

loc* make_puzzle(); //初期盤面を作る

void printboard(loc* board);

list* create_list(loc* board, int param);//初期盤面をコピーした初期ノードを格納した線形リスト
bool is_goal(loc* board);//盤面がゴールと一致しているかどうか
bool is_same(loc* board1, loc* board2);//同じ盤面かを決める

int hfunc(loc* board, int param);//ヒューリスティック関数

//各探索手法の性能を表示
void print_stats(const char *name,bool is_succeeded, int expanded, int max_open, int solve, double t);

//ノードの線形リストのメモリ解放
void delete_list(list* aim)
{
    while(aim)
    {
        list* t=aim->next;
        free(aim->node->n);
        free(aim->node);
        free(aim);
        aim=t;
    }
}

//子ノードを生成しそのアドレスを返す. 動かせないなら解放してNULL
Node* expand(Node* parent,int dir,int param)
{
    Node* child=malloc(sizeof(Node));
    child->n=malloc(sizeof(loc)*N);
    memcpy(child->n,parent->n,sizeof(loc)*N);
    switch(dir)
    {
        case 0: moveleft(child->n);break;
        case 1: moveright(child->n);break;
        case 2: moveup(child->n);break;
        case 3: movedown(child->n);break;
    }
    
    if(!is_same(child->n,parent->n))
    {
        child->g=parent->g+1;
        child->p=parent;
        child->h=hfunc(child->n,param);
        return child;
    }

    else{free(child->n);free(child);return NULL;}
}
//リストからノードを削除
void pop(Node* node,list** L)
{
    list* current=*L;
    list* previous=NULL;
    while(current)
    {
        if(current->node==node)
        {
            if(previous==NULL)*L=current->next;
            else {previous->next=current->next;}

            free(current);
            return;
        }
        previous=current;
        current=current->next;
    }
    return;
}
//param==-1のときは先頭にpush, param!=-1のときはg+hが小さい順になるようにノードをリストに追加
void push(Node* node, list** L, int param)
{
    list* new=malloc(sizeof(list));
    new->node = node;
    new->next = NULL;

    if(param == -1)
    {
        new->next = *L;
        *L = new;
        return;
    }

    // g+hが小さい順に挿入
    list* current = *L;
    list* previous = NULL;
    while(current)
    {
        if(node->g + node->h <= current->node->g + current->node->h) break;
        previous = current;
        current = current->next;
    }
    if(previous == NULL) *L = new;
    else previous->next = new;
    new->next = current;
}

//クローズドリスト(探索済みのノードのリスト)に入っているか判定
bool in_closed(Node* node,list* closed)
{
    while(closed)
    {
        if(!is_same(node->n,closed->node->n)){closed=closed->next;continue;}
        return true;
    }return false;
}

void IDS(loc* board)
{
    int expanded_node = 0, max_open = 0, open_size = 0, solve = 0;
    bool is_succeeded=false;
    clock_t check,end;
    clock_t start = clock();
    //深さ制限を変えながら探索
    for(int d=0;;++d)
    {
        list* open=create_list(board, ids);
        open_size=1;
        list* used=NULL;
        Node* parent=NULL;
        check=clock();
        //タイムアウト処理
        if((double)(check-start)/CLOCKS_PER_SEC >= 60)
        {
            printf("60秒以上経過しました。");
            solve=-1;
            delete_list(open);
            goto finish;
        }
        //最大オープンサイズの更新
        if(open_size>max_open)max_open=open_size;
        
        //正解判定
        if(is_goal(open->node->n))
        {
            end=clock();
            is_succeeded=true;
            solve=open->node->g;
            delete_list(open);
            goto finish;
        }

        //openリスト(まだ展開してないノードのリスト)に入っているものを順に探索
        while(open)
        {
            parent=open->node;
            pop(parent,&open);
            --open_size;
            push(parent, &used, ids);
            if(parent->g>=d)
            {
                continue;
            }//深さ制限
            
            //上下左右の子ノード生成
            for(int dir=0;dir<direction;++dir)
            {
                check=clock();
                //タイムアウトチェック
                if((double)(check-start)/CLOCKS_PER_SEC >= 60)
                {
                    printf("60秒以上経過しました。");
                    delete_list(open);
                    delete_list(used);
                    solve=-1;
                    goto finish;
                }

                Node* child=expand(parent,dir,ids);
                //子ノード生成に成功した時の処理
                if(child!=NULL)
                {
                    ++expanded_node;
                    //子ノードがゴール
                    if(is_goal(child->n))
                    {
                        end=clock();
                        is_succeeded=true;
                        solve=child->g;
                        Node* goal=child;
                        printf("\n====IDSの最短ルートを逆順で表示====\n");
                        while(child)
                        {
                            printboard(child->n);
                            printf("\n");
                            child=child->p;
                        }
                        delete_list(open);
                        delete_list(used);
                        free(goal->n);free(goal);
                        goto finish;
                    }
                    //ゴールでなければオープンリストへ
                    else
                    {
                        {
                            push(child,&open,ids);
                            ++open_size;
                            if(max_open<open_size)max_open=open_size;
                        }
                    }
                }
                else{continue;}
            }
            
        }
        delete_list(open);
        delete_list(used);
        continue;

    }
    finish: ;
    double t=(is_succeeded)?(double)(end-start)/CLOCKS_PER_SEC:60;
    print_stats("IDS",is_succeeded,expanded_node,max_open,solve,t);
}

void A_star(loc* board, int param)
{
    int expanded_node = 0, max_open = 0, open_size = 0, solve = 0;
    bool is_succeeded=false;
    const char* name = (param==h_0)?"A*(h0)":(param==h_1)?"A*(h1)":"A*(h2)";
    clock_t start = clock();
    clock_t check;
    list* open=create_list(board,param);
    ++open_size;
    ++max_open;
    list* closed=NULL;
    Node* parent=NULL;
    Node* child=NULL;
    while(open)
    {
        check=clock();
        if((double)(check-start)/CLOCKS_PER_SEC >= 60)
        {
            printf("60秒以上経過しました。");
            solve=-1;
            goto timeout;
        }
        parent=open->node;
        if(is_goal(parent->n)){is_succeeded=true; break;}
        for(int dir=0;dir<direction;++dir)
        {
            check=clock();
            if((double)(check-start)/CLOCKS_PER_SEC >= 60)
            {
                printf("60秒以上経過しました。");
                solve=-1;
                goto timeout;
            }
            child=expand(parent,dir,param);
            if(child!=NULL)
            {
                ++expanded_node;
                list* previous=NULL; 
                list* current=closed;
                if(in_closed(child,closed)){free(child->n);free(child);continue;}
                
                previous=NULL;current=open;
                while(current)
                {
                    check=clock();
                    if((double)(check-start)/CLOCKS_PER_SEC >= 60)
                    {
                        printf("60秒以上経過しました。");
                        solve=-1;
                        free(child->n);free(child);
                        goto timeout;
                    }
                    if(is_same(current->node->n,child->n))
                    {
                        if(child->g+child->h < current->node->g+current->node->h)
                        {
                            Node* old=current->node;
                            pop(current->node, &open);
                            push(child, &open,param);
                            free(old->n);free(old);
                            if(open_size>max_open)max_open=open_size;
                            goto nextloop;
                        }
                        else
                        {
                            free(child->n);free(child);goto nextloop;
                        }
                    }previous=current;current=current->next;
                }
                push(child,&open,param);++open_size;
                if(open_size>max_open)max_open=open_size;

            }
            nextloop: 
            continue;
        }
        push(parent,&closed,param);pop(parent,&open);--open_size;
    }

    clock_t end=clock();
    solve=parent->g;
    printf("\n====%sの最短ルートを逆順で表示====\n",name);
    while(parent)
    {
        printboard(parent->n);
        printf("\n");
        parent=parent->p;
    }
    timeout:
    delete_list(open);delete_list(closed);
    double t=(is_succeeded)?(double)(end-start)/CLOCKS_PER_SEC:60;
    print_stats(name,is_succeeded,expanded_node,max_open,solve,t);
}

int main(void)
{
    srand(time(NULL));
    loc* board=make_puzzle();
    printf("===初期盤面===\n");
    printboard(board);
    IDS(board);
    A_star(board,h_0);
    A_star(board,h_1);
    A_star(board,h_2);
    free(board);
    return 0;
}

void moveleft(loc* board)
{
    if(board[0].x!=0)
    {
        for(int i=1;i<N;++i)
        {
            if(board[0].x-1==board[i].x && board[0].y==board[i].y)
            {
                loc t=board[0]; board[0]=board[i]; board[i]=t;
                break;
            }
        }
    }
    return;
}

void moveright(loc* board)
{
    if(board[0].x!=side_length-1)
    {
        for(int i=1;i<N;++i)
        {
            if(board[0].x+1==board[i].x && board[0].y==board[i].y)
            {
                loc t=board[0]; board[0]=board[i]; board[i]=t;
                break;
            }
        }
    }
    return;
}

void moveup(loc* board)
{
    if(board[0].y!=0)
    {
        for(int i=1;i<N;++i)
        {
            if(board[0].x==board[i].x && board[0].y-1==board[i].y)
            {
                loc t=board[0]; board[0]=board[i]; board[i]=t;
                break;
            }
        }
    }
    return;
}

void movedown(loc* board)
{
    if(board[0].y!=side_length-1)
    {
        for(int i=1;i<N;++i)
        {
            if(board[0].x==board[i].x && board[0].y+1==board[i].y)
            {
                loc t=board[0]; board[0]=board[i]; board[i]=t;
                break;
            }
        }
    }
    return;
}

loc* make_puzzle()
{
    loc* board=malloc(sizeof(loc)*N);
    for(int i=0;i<N;++i)
    {
        int goal_x=(i==0)? side_length-1 : (i-1)%side_length;
        int goal_y=(i==0)? side_length-1 : (i-1)/side_length;
        board[i].x=goal_x; board[i].y=goal_y;
    }
    for(int k=0;k<loop;++k)
    {
        switch(rand()%4)
        {
            case 0:moveleft(board);break;
            case 1:moveright(board);break;
            case 2:moveup(board);break;
            case 3:movedown(board);break;
            default:break;
        }
    }
    
    return board;
}

void printboard(loc* board)
{
    int result[N];int i=0;
    while(i<N)
    {
        result[board[i].x+board[i].y*side_length]=i;
        ++i;
    }
    
    for(int k=0;k<N;++k)
    {
        printf("%d",result[k]);
        if(k%side_length==side_length-1)printf("\n");
    }
}

int hfunc(loc* board, int param)
{
    int h=0;
    switch(param)
    {
        case -1:return 0;
        case 0:return 0;
        case 1:
            for(int i=0;i<N;++i)
            {
                int goal_x=(i==0)? side_length-1:(i-1)%side_length;
                int goal_y=(i==0)? side_length-1:(i-1)/side_length;
                if(board[i].x!=goal_x||board[i].y!=goal_y)++h;
            }
            return h;
        case 2:
            for(int k=0;k<N;++k)
            {
                int goal_x=(k==0)? side_length-1:(k-1)%side_length;
                int goal_y=(k==0)? side_length-1:(k-1)/side_length;
                h+=abs(board[k].x-goal_x)+abs(board[k].y-goal_y);
            }
            return h;
        default: return 0;
    }
}

list* create_list(loc* board, int param)
{
    loc* copy=malloc(sizeof(loc)*N);
    memcpy(copy,board,sizeof(loc)*N);
    Node* tail=malloc(sizeof(Node));
    Node t={copy,NULL,0,hfunc(copy,param)};
    *tail=t;
    list* open=malloc(sizeof(list));
    list a={tail, NULL};
    *open=a;
    return open;
}

void print_stats(const char *name,bool is_succeeded, int expanded, int max_open, int solve, double t)
{
    printf("\n%s\n", name);
    if(is_succeeded)printf("succeeded\n");
    else printf("failed\n");
    printf("展開ノード数 : %d\n", expanded);
    printf("最大OPENサイズ : %d\n", max_open);
    printf("手数 : %d\n", solve);
    printf("実行時間 : %.4f秒\n", t);
}

bool is_goal(loc* board)
{
    for(int i=0;i<N;++i)
    {
        int goal_x=(i==0)?side_length-1:(i-1)%side_length;
        int goal_y=(i==0)?side_length-1:(i-1)/side_length;
        if(board[i].x != goal_x||board[i].y!=goal_y)return false;
    }
    return true;
}

bool is_same(loc* board1, loc* board2)
{
    for(int i=0;i<N;++i)
    {
        if(board1[i].x!=board2[i].x || board1[i].y!=board2[i].y)return false;
    }
    return true;
}