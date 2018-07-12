/*
HDU5834: 树形DP

跑两遍DFS。

第一遍算：遍历子树后回到根的最大收益；遍历子树后不回到根的最大收益；遍历子树后不回到根的次大收益（分别记录不回到根的最后一次从根出发前往的方向）。

第二遍算：以该点为根遍历整棵树后回到根的最大收益；以该点为根遍历整棵树后不回到根的最大收益；以该点为根遍历整棵树后不回到根的次大收益（分别记录不回到根的最后一次从根出发前往的方向）。
*/


#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
int T,n,d[100010],sz,first[100010],nxt[200010],tail[200010],len[200010],son[100010],bro[100010],p[100010],w[100010],f[100010][3],g[100010][3],id[100010][2],id2[100010][2];

inline int value(int u,int o)
{
    if(o) return max(f[u][1]-w[u],0);
    else return max(f[u][0]-w[u]*2,0);
}

void addedge(int u,int v,int c)
{
    sz++;
    tail[sz]=v;  len[sz]=c;
    nxt[sz]=first[u];  first[u]=sz;
}

void dfs(int u)
{
    int v,e;
    for(son[u]=0,e=first[u];e;e=nxt[e])
    {
        v=tail[e];
        if(son[v]>=0) continue;
        bro[v]=son[u];  son[u]=v;
        p[v]=u;
        w[v]=len[e];
    }
    for(f[u][0]=d[u],v=son[u];v;v=bro[v])
    {
        dfs(v);
        f[u][0]+=value(v,0);
    }
    for(id[u][0]=id[u][1]=0,v=son[u];v;v=bro[v])
        if(!id[u][0] || value(v,1)-value(v,0)>value(id[u][0],1)-value(id[u][0],0))
            id[u][1]=id[u][0],id[u][0]=v;
        else if(!id[u][1] || value(v,1)-value(v,0)>value(id[u][1],1)-value(id[u][1],0))
            id[u][1]=v;
    f[u][1]=f[u][0]+value(id[u][0],1)-value(id[u][0],0);
    f[u][2]=f[u][0]+value(id[u][1],1)-value(id[u][1],0);
}

void dfs2(int u)
{
    int i;
    for(i=0;i<=2;i++)
        g[u][i]=f[u][i]+max(g[p[u]][0]-value(u,0)-w[u]*2,0);
    int t;
    if(id2[p[u]][0]!=u)
        t=f[u][0]+max(g[p[u]][1]-value(u,0)-w[u],0);
    else t=f[u][0]+max(g[p[u]][2]-value(u,0)-w[u],0);
    if(t>g[u][1])
    {
        g[u][2]=g[u][1];  g[u][1]=t;
        id2[u][0]=p[u];  id2[u][1]=id[u][0];
    }
    else if(t>g[u][2])
    {
        g[u][2]=t;
        id2[u][0]=id[u][0];  id2[u][1]=p[u];
    }
    else id2[u][0]=id[u][0],id2[u][1]=id[u][1];
    int v;
    for(v=son[u];v;v=bro[v]) dfs2(v);
}

int main()
{
    freopen("tree.in","r",stdin);
    freopen("tree.out","w",stdout);
    scanf("%d",&T);
    int cas;
    for(cas=1;cas<=T;cas++)
    {
        scanf("%d",&n);
        int i;
        for(i=1;i<=n;i++)
            scanf("%d",&d[i]);
        int u,v,c;
        memset(first,0,sizeof(first));
        for(sz=0,i=1;i<=n-1;i++)
        {
            scanf("%d%d%d",&u,&v,&c);
            addedge(u,v,c);  addedge(v,u,c);
        }
        memset(son,-1,sizeof(son));
        dfs(1);
        dfs2(1);
        printf("Case #%d:\n",cas);
        for(i=1;i<=n;i++)
            printf("%d\n",g[i][1]);
    }
    return 0;
}