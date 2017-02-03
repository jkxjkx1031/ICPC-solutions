/*
BZOJ1023：DFS+树形DP+单调队列DP    复杂度O(N)

这道题提供了一种基本的处理仙人掌的方法。
可以把仙人掌视为通过树形结构连接起来的“基环外向树”。
每个环上的深度最小节点（最高点）作为“基环外向树”的输出端向父亲节点输出信息，其他节点作为输入端接受儿子节点输入的信息。
这道题中的“信息”是f[u]，即每棵以u为根的子树中，以u为起点的最大链长。

这道题中，对于每个u，先计算不允许走环上边时的f[u]，同时更新子树上的直径（这里要用树形DP的方法算每棵子树上的直径）。
待环上所有点都遍历过后，再加进环上边（变成基环外向树），更新基环外向树上直径（用DP算，单调队列优化到O(N)），
并更新环上最高点的f值（环上其他点的f值后面用不到了）。

参考：http://ydcydcy1.blog.163.com/blog/static/21608904020131493113160/
*/


#include <stdio.h>
#include <math.h>
#include <string.h>
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

int n,m,ans,T,sz,first[50010],next[200010],tail[200010],dfn[50010],low[50010],par[50010],dep[50010],f[50010],c[100010],mq[100010];
void addedge(int u,int v);
void dfs(int u);
void dp(int u,int v);

int main(void)
{
    freopen("cactus.in","r",stdin);
    freopen("cactus.out","w",stdout);
    scanf("%d%d",&n,&m);
    int i,j,k,u,v;
    memset(first,0,sizeof(first));
    for(sz=0,i=1;i<=m;i++)
    {
        scanf("%d",&k);
        scanf("%d",&u);
        for(j=2;j<=k;j++)
        {
            scanf("%d",&v);
            addedge(u,v);  addedge(v,u);
            u=v;
        }
    }
    memset(dfn,0,sizeof(dfn));
    T=0;  par[1]=0;  dep[1]=1;
    ans=0;
    dfs(1);
    printf("%d\n",ans);
    return 0;
}

void addedge(int u,int v)
{
    tail[++sz]=v;
    next[sz]=first[u];  first[u]=sz;
}

void dfs(int u)
{
    dfn[u]=low[u]=++T;  f[u]=0;
    int e,v;
    for(e=first[u];e;e=next[e])
    {
        v=tail[e];
        if(v==par[u]) continue;
        if( dfn[v] && dfn[v]<dfn[u] )
            low[u]=MIN(low[u],low[v]);
        else if(!dfn[v])
        {
            par[v]=u;  dep[v]=dep[u]+1;
            dfs(v);
            low[u]=MIN(low[u],low[v]);
            if(low[v]==dfn[v])
            {
                ans=MAX(ans,f[u]+f[v]+1);
                f[u]=MAX(f[u],f[v]+1);
            }
        }
    }
    for(e=first[u];e;e=next[e])
    {
        v=tail[e];
        if( dfn[v]>dfn[u] && par[v]!=u ) dp(u,v);
    }
}

void dp(int u,int v)
{
    int s=dep[v]-dep[u]+1,i,p;
    for(i=1,p=v;i<=s;i++,p=par[p])
        c[i]=c[s+i]=f[p];
    int front=1,rear=0;
    for(i=1;i<=s/2;i++)
    {
        while( front<=rear && c[i]-i>=c[mq[rear]]-mq[rear] ) rear--;
        mq[++rear]=i;
    }
    for(i=s/2+1;i<=s/2+s;i++)
    {
        ans=MAX(ans,c[i]+c[mq[front]]+i-mq[front]);
        if(mq[front]==i-s/2) front++;
        while( front<=rear && c[i]-i>=c[mq[rear]]-mq[rear] ) rear--;
        mq[++rear]=i;
    }
    for(i=1;i<s;i++)
        f[u]=MAX(f[u],c[i]+MIN(s-i,i));
}
