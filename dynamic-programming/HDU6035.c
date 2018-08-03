/*
HDU6035: 树形dp+计数原理  复杂度O(n)

主要思路就是把原问题转化为 求每种颜色的点在多少条链上出现过至少一次，于是又可转化为 求每种颜色的点在多少条链上没有出现。
本题的树形dp技巧性略强，具体实现见代码。
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef long long LL;
int n,c[200010],sz,first[200010],next[400010],tail[400010],last[200010],vis[200010],anc[200010],s[200010],t[200010],sum[200010];
LL ans;
void addedge(int u,int v);
void dfs(int u);
#define BUF_SZ 100000
char buf[BUF_SZ+10];
inline char nc(void)
{
    static char *pr=buf,*pend=buf;
    if(pr==pend)
    {
        pr=buf;
        pend=pr+fread(buf,1,BUF_SZ,stdin);
        if(pr==pend) return EOF;
        else return *pr++;
    }
    return *pr++;
}
inline int readint(int *x)
{
    static char ch;
    ch=nc();
    while(ch!=EOF && (ch<'0' || ch>'9')) ch=nc();
    if(ch==EOF) return 0;
    for(*x=0;ch>='0' && ch<='9';ch=nc())
        *x=*x*10+ch-'0';
    return 1;
}

int main(void)
{
    freopen("tree.in","rb",stdin);
    freopen("tree.out","w",stdout);
    int cas=0;
    while(readint(&n))
    {
        printf("Case #%d: ",++cas);
        int i;
        for(i=1;i<=n;i++) readint(&c[i]);
        memset(first,0,sizeof(first));
        int u,v;
        for(sz=0,i=1;i<=n-1;i++)
        {
            readint(&u);  readint(&v);
            addedge(u,v);  addedge(v,u);
        }
        srand((unsigned int)time(NULL));
        memset(vis,0,sizeof(vis));
        memset(last,0,sizeof(last));
        ans=(LL)n*n*(n-1)/2;
        int rt=rand()%n+1;
        dfs(rt);
        for(i=1;i<=n;i++) sum[i]=n;
        for(i=1;i<=n;i++)
            if(!anc[i]) sum[c[i]]-=s[i];
        for(i=1;i<=n;i++)
            ans-=(LL)sum[i]*(sum[i]-1)/2;
        printf("%lld\n",ans);
    }
    return 0;
}

void addedge(int u,int v)
{
    tail[++sz]=v;
    next[sz]=first[u];  first[u]=sz;
}

void dfs(int u)
{
    vis[u]=1;
    anc[u]=last[c[u]];
    last[c[u]]=u;
    int e,v;
    for(s[u]=1,t[u]=0,e=first[u];e;e=next[e])
    {
        v=tail[e];
        if(vis[v]) continue;
        dfs(v);
        ans-=((LL)s[v]-t[u])*(s[v]-t[u]-1)/2;
        s[u]+=s[v];
        t[u]=0;
    }
    if(anc[u]) t[anc[u]]+=s[u];
    last[c[u]]=anc[u];
}
