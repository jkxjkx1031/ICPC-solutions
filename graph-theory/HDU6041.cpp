/*
HDU6041: dfs+堆

注意到输入的图是一棵仙人掌，因此原图的每一个生成树都是通过分别删掉仙人掌每个环上的一条边得到的（输入为树的情况需要特判）。

dfs找出所有环后，问题转化为经典的“多路归并”问题：有m个集合，每次从每个集合中各拿出一个数进行求和，问前k大的和分别是多少？
方法是每次将前两个集合合并，用堆实现（见《训练指南》例题），并且注意“启发式”合并，使得堆的大小总是等于正在合并的较小集合的大小。
复杂度是O(k*集合个数*log(集合平均大小))。

关于STL使用的注意事项：vector.clear()调用后不会释放内存，如果需要释放内存可以使用deque容器。
*/


#include <cstdio>
#include <cstring>
#include <deque>
#include <algorithm>

using namespace std;
typedef long long LL;
struct HEAPNODE
{
    int val,idx;
}heap[1010];
int n,m,k,sz,first[1010],nxt[4010],tail[4010],len[4010],cnt,dep[1010],par[1010],parl[1010],c[1010],h;
deque<int> s[1010];
void addedge(int u,int v,int l);
bool cmp(int x,int y);
void dfs(int u);
void merge(int x,int y);
void push(int val,int idx);
HEAPNODE pop();
void up(int u);
void down(int u);
void swap(int u,int v);

int main()
{
    freopen("curse.in","r",stdin);
    freopen("curse.out","w",stdout);
    int cas=0;
    while(~scanf("%d%d",&n,&m))
    {
        int i,u,v,l;
        memset(first,0,sizeof(first));
        for(sz=0,i=1;i<=m;i++)
        {
            scanf("%d%d%d",&u,&v,&l);
            addedge(u,v,l);  addedge(v,u,l);
        }
        scanf("%d",&k);
        cnt=0;
        memset(dep,0,sizeof(dep));
        par[1]=0;  dep[1]=1;
        dfs(1);
        LL sum=0,e;
        for(i=1;i<=n;i++)
            for(e=first[i];e;e=nxt[e])
                sum+=len[e];
        sum/=2;
        unsigned int ans=0;
        for(i=1;i<cnt;i++) merge(i,i+1);
        if(cnt)
            for(i=0;i<c[cnt];i++)
                ans+=(sum-s[cnt][i])*(i+1);
        else ans=sum;
        printf("Case #%d: %u\n",++cas,ans);
    }
    return 0;
}

void addedge(int u,int v,int l)
{
    sz++;
    tail[sz]=v;  len[sz]=l;
    nxt[sz]=first[u];  first[u]=sz;
}

bool cmp(int x,int y)
{
    return x>y;
}

void dfs(int u)
{
    int t,v,e;
    for(e=first[u];e;e=nxt[e])
    {
        v=tail[e];
        if(v==par[u]) continue;
        else if(!dep[v])
        {
            par[v]=u;  parl[v]=len[e];
            dep[v]=dep[u]+1;
            dfs(v);
        }
        else if(dep[v]<dep[u])
        {
            s[++cnt].clear();
            for(t=u;t!=v;t=par[t])
                s[cnt].push_back(parl[t]);
            s[cnt].push_back(len[e]);
            c[cnt]=s[cnt].size();
            sort(s[cnt].begin(),s[cnt].end(),cmp);
        }
    }
}

void merge(int x,int y)
{
    int i;
    for(h=0,i=0;i<c[y];i++)
        push(s[y][i]+s[x][0],0);
    s[y].clear();
    c[y]=0;
    HEAPNODE t;
    while(c[y]<k && h)
    {
        t=pop();
        s[y].push_back(t.val);
        c[y]++;
        if(t.idx<c[x]-1)
            push(t.val-s[x][t.idx]+s[x][t.idx+1],t.idx+1);
    }
    s[x].clear();
    c[x]=0;
}

void push(int val,int idx)
{
    h++;
    heap[h].val=val;  heap[h].idx=idx;
    up(h);
}

HEAPNODE pop()
{
    HEAPNODE res=heap[1];
    heap[1]=heap[h--];
    down(1);
    return res;
}

void up(int u)
{
    if(u>1 && heap[u].val>heap[u/2].val)
    {
        swap(u,u/2);
        up(u/2);
    }
}

void down(int u)
{
    if(u*2>h) return;
    int v=u*2;
    if(v+1<=h && heap[v+1].val>heap[v].val) v++;
    if(heap[v].val>heap[u].val)
    {
        swap(u,v);
        down(v);
    }
}

void swap(int u,int v)
{
    HEAPNODE t=heap[u];  heap[u]=heap[v];  heap[v]=t;
}
