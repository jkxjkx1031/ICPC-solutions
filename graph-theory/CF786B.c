/*
CF786B: 最短路径+线段树

一个简单的想法是，对于每一条v~[l,r]型的边，拆分成v~l,v~(l+1)...v~r共r-l+1条普通边（[l,r]~v型边同理），然后求最短路径即可。
问题是这样的等效建边方法会造成边数过多。可以借助线段树的思路：建立一些辅助点，每个辅助点覆盖了一段编号连续的点，从而每条“点~区间”型边都可以被拆分成不超过logn条普通边。
具体做法是：
建立两棵线段树（设为A和B）。A中边均自顶向下且长度为0，B中边均自底向上且长度为0。两树对应的叶子结点之间分别连长度为0的无向边。
1类计划，直接在叶子结点之间连边。
2类计划，从叶子连边到A的非叶子。
3类计划，从B的非叶子连边到叶子。

实现细节：
由于有2棵线段树，如果想直接在树上跑最短路径的话，就不能用k*2,k*2+1分别给k的左、右儿子编号了。
有一种方案是记录左右儿子的指针。还有一种（我使用的）是将树、图分开，树上结点记录对应的图上结点的编号，这样就能使两棵线段树独立编号了。
*/


#include <stdio.h>
#include <string.h>
#define INF 0x3f3f3f3f3f3f3f3f

typedef long long LL;
int n,q,s,sz,vs,es,seg[2][400010],first[800010],next[6000010],len[6000010],tail[6000010],hs,heap[800010],idx[800010];
LL dist[800010];
void build(int d,int k,int l,int r);
void update(int p,int q,int x,int w,int d,int k,int l,int r);
void addedge(int u,int v,int l,int d);
int pop(void);
void up(int u);
void down(int u);
void swap(int u,int v);

int main(void)
{
    freopen("legacy.in","r",stdin);
    freopen("legacy.out","w",stdout);
    scanf("%d%d%d",&n,&q,&s);
    for(sz=1;sz<n;sz*=2);
    memset(first,0,sizeof(first));
    vs=0;  es=0;
    build(0,1,1,sz);  build(1,1,1,sz);
    int i;
    for(i=1;i<=sz;i++)
    {
        addedge(seg[0][sz-1+i],seg[1][sz-1+i],0,0);
        addedge(seg[0][sz-1+i],seg[1][sz-1+i],0,0);
    }
    int t,u,v,e,l,r,w;
    for(i=1;i<=q;i++)
    {
        scanf("%d",&t);
        if(t==1)
        {
            scanf("%d%d%d",&v,&u,&w);
            addedge(seg[0][sz-1+v],seg[0][sz-1+u],w,0);
        }
        else if(t==2)
        {
            scanf("%d%d%d%d",&v,&l,&r,&w);
            update(l,r,seg[0][sz-1+v],w,0,1,1,sz);
        }
        else
        {
            scanf("%d%d%d%d",&v,&l,&r,&w);
            update(l,r,seg[0][sz-1+v],w,1,1,1,sz);
        }
    }
    memset(dist,0x3f,sizeof(dist));
    hs=vs;
    for(i=1;i<=hs;i++)
    {
        heap[i]=i;  idx[i]=i;
    }
    dist[seg[0][sz-1+s]]=0;
    up(idx[seg[0][sz-1+s]]);
    while(hs)
    {
        u=pop();
        for(e=first[u];e;e=next[e])
        {
            v=tail[e];
            if(dist[u]+len[e]<dist[v])
            {
                dist[v]=dist[u]+len[e];
                up(idx[v]);
            }
        }
    }
    for(i=1;i<=n;i++)
        if(dist[seg[0][sz-1+i]]<INF)
            printf("%I64d ",dist[seg[0][sz-1+i]]);
        else printf("-1 ");
    putchar('\n');
    return 0;
}

void addedge(int u,int v,int l,int d)
{
    if(d)
    {
        int t=u;  u=v;  v=t;
    }
    es++;
    tail[es]=v;  len[es]=l;
    next[es]=first[u];  first[u]=es;
}

void build(int d,int k,int l,int r)
{
    seg[d][k]=++vs;
    if(k>1)
        addedge(seg[d][k/2],seg[d][k],0,d);
    if(l==r) return;
    int m=(l+r)/2;
    build(d,k*2,l,m);  build(d,k*2+1,m+1,r);
}

void update(int p,int q,int x,int w,int d,int k,int l,int r)
{
    if(p<=l&&q>=r)
    {
        addedge(x,seg[d][k],w,d);
        return;
    }
    int m=(l+r)/2;
    if(p<=m) update(p,q,x,w,d,k*2,l,m);
    if(q>m) update(p,q,x,w,d,k*2+1,m+1,r);
}

int pop(void)
{
    int ans=heap[1];
    swap(1,hs--);
    down(1);
    return ans;
}

void up(int u)
{
    if( u>1 && dist[heap[u]]<dist[heap[u/2]] )
    {
        swap(u,u/2);
        up(u/2);
    }
}

void down(int u)
{
    if(u*2>hs) return;
    int v=u*2;
    if( v+1<=hs && dist[heap[v+1]]<dist[heap[v]] ) v++;
    if(dist[heap[v]]<dist[heap[u]])
    {
        swap(u,v);
        down(v);
    }
}

void swap(int u,int v)
{
    int t1=heap[u],t2=heap[v];
    heap[u]=t2;  heap[v]=t1;
    idx[t1]=v;  idx[t2]=u;
}
