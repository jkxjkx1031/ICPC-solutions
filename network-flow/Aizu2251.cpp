/*
Aizu2251: Floyd+二分图匹配  复杂度O(L^3)

首先跑一遍Floyd，求出对于任意两个任务x,y，一个人能不能在完成x之后完成y。
于是可以得到一张点数为L的DAG。

接下来需要求这个DAG的最小链覆盖。方法是拆点+二分图匹配。最后答案是 顶点数(L)-最大匹配。
注意最小链覆盖是允许链相互重叠的，因此得到DAG之后，需要对每个顶点u，建立有向边到它的每个后继顶点，从而使得求最小链覆盖的时候可以“跳过”那些重叠点。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

int n,m,l,dist[110][110],p[1010],t[1010],sz,first[1010],next[1000010],tail[1000010],from[1010],vis[1010];
void addedge(int u,int v);
int match(int u);
int hungary();

int main()
{
    freopen("christmas.in","r",stdin);
    freopen("christmas.out","w",stdout);
    for(;;)
    {
        scanf("%d%d%d",&n,&m,&l);
        if(!n) break;
        int i,j,k,u,v,d;
        memset(dist,0x3f,sizeof(dist));
        for(i=1;i<=m;i++)
        {
            scanf("%d%d%d",&u,&v,&d);
            dist[u+1][v+1]=dist[v+1][u+1]=d;
        }
        for(i=1;i<=n;i++) dist[i][i]=0;
        for(k=1;k<=n;k++)
            for(i=1;i<=n;i++)
                for(j=1;j<=n;j++)
                    dist[i][j]=std::min(dist[i][j],dist[i][k]+dist[k][j]);
        for(i=1;i<=l;i++)
            scanf("%d%d",&p[i],&t[i]);
        memset(first,0,sizeof(first));
        for(sz=0,i=1;i<=l;i++)
            for(j=1;j<=l;j++)
                if(i!=j && t[i]+dist[p[i]+1][p[j]+1]<=t[j]) addedge(i,j);
        printf("%d\n",l-hungary());
    }
    return 0;
}

void addedge(int u,int v)
{
    tail[++sz]=v;
    next[sz]=first[u];  first[u]=sz;
}

int hungary()
{
    int ret=0,i;
    memset(from,0,sizeof(from));
    for(i=1;i<=l;i++)
    {
        memset(vis,0,sizeof(vis));
        if(match(i)) ret++;
    }
    return ret;
}

int match(int u)
{
    int v,e;
    for(e=first[u];e;e=next[e])
    {
        v=tail[e];
        if(vis[v]) continue;
        vis[v]=1;
        if(!from[v] || match(from[v]))
        {
            from[v]=u;
            return 1;
        }
    }
    return 0;
}
