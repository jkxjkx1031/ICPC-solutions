/*
CF778C: 启发式合并    复杂度O(nlogn)

先计算对于树(trie)上每一个结点u，如果删去所有它与它的孩子结点之间的边，这棵树会减少多少个结点。
方法是固定子树u的最大（结点数最多）的子树不动，把u下其他所有子树中结点逐一插入最大的子树，统计最大的子树中新增的结点数量。
这个算法不会超时，原因是，每个结点最多只会被“重新插入” logn 次。因为一个结点每被插入一次，它所在的子树大小都至少会变为原来2倍。

值得注意的是，每次把结点合并入“最大的子树”后，还必须还原“最大的子树”。
一种方法是在插入完成后“撤销操作”，具体见代码中split()函数。
另一种cf给出的方法是先把所有“非最大”子树在新的内存空间合并起来，再将这棵合并后的树和“最大的子树”比较。这样既不会影响“最大的子树”的结构，“新的内存空间”也可以重复使用。
*/


#include <stdio.h>
#include <string.h>

int n,sz,first[300010],next[600010],tail[600010],ch[600010],d[300010],s[300010],f[300010],g[300010],son[600010][30];
void addedge(int u,int v,char ch);
void build(int u);
void dfs(int u);
int merge(int rt1,int rt2);
void split(int rt1,int rt2);
void clear(int u);

int main(void)
{
    freopen("polyglot.in","r",stdin);
    freopen("polyglot.out","w",stdout);
    scanf("%d",&n);
    int i,j,u,v;
    char x[2];
    memset(first,0,sizeof(first));
    for(sz=0,i=1;i<=n-1;i++)
    {
        scanf("%d%d%s",&u,&v,x);
        addedge(u,v,x[0]);  addedge(v,u,x[0]);
    }
    memset(son,0,sizeof(son));
    memset(d,0,sizeof(d));
    d[1]=1;
    build(1);
    dfs(1);
    memset(g,0,sizeof(g));
    for(i=1;i<=n;i++) g[d[i]]+=f[i];
    int ans=-1,p;
    for(i=1;i<=n;i++)
        if(g[i]>ans)
        {
            ans=g[i];
            p=i;
        }
    printf("%d\n%d\n",n-ans,p);
    return 0;
}

void addedge(int u,int v,char x)
{
    sz++;
    ch[sz]=x-'a';  tail[sz]=v;
    next[sz]=first[u];  first[u]=sz;
}

void build(int u)
{
    int e,v;
    for(e=first[u];e;e=next[e])
    {
        v=tail[e];
        if(d[v]) continue;
        son[u][ch[e]]=v;
        d[v]=d[u]+1;
        build(v);
    }
}

void dfs(int u)
{
    int i,v,maxs=0,son0;
    for(s[u]=1,i=0;i<26;i++)
    {
        v=son[u][i];
        if(!v) continue;
        dfs(v);
        if(s[v]>maxs)
        {
            maxs=s[v];
            son0=v;
        }
        s[u]+=s[v];
    }
    if(!maxs)
    {
        f[u]=0;
        return;
    }
    int t=n;
    for(f[u]=1,i=0;i<26;i++)
    {
        v=son[u][i];
        if( !v || v==son0 ) continue;
        f[u]+=s[v]-merge(son0,v);
    }
    n=t;
    for(i=0;i<26;i++)
    {
        v=son[u][i];
        if( !v || v==son0 ) continue;
        split(son0,v);
    }
}

int merge(int rt1,int rt2)
{
    int ans=0,u,v,i;
    for(i=0;i<26;i++)
    {
        u=son[rt2][i];
        if(!u) continue;
        if(!son[rt1][i])
        {
            son[rt1][i]=++n;
            v=n;
            ans++;
        }
        else v=son[rt1][i];
        ans+=merge(v,u);
    }
    return ans;
}

void split(int rt1,int rt2)
{
    int i,u,v;
    for(i=0;i<26;i++)
    {
        u=son[rt2][i];  v=son[rt1][i];
        if(!u) continue;
        if(v>n)
        {
            clear(v);
            son[rt1][i]=0;
        }
        else if(v) split(v,u);
    }
}

void clear(int u)
{
    int i;
    for(i=0;i<26;i++)
        if(son[u][i])
        {
            clear(son[u][i]);
            son[u][i]=0;
        }
}
