/*
CF86C：DP+AC自动机

定义状态f[i][u][o]，其中i表示目标字符串排到了第i位，
u表示AC自动机的所有状态中，所对应的前缀是当前已排好的目标字符串的后缀，且长度最长的节点的编号，
o表示目标字符串的第i位之前（包括第i位），有多少位字符还没有被完整的模式串覆盖。

注意几个细节：
1.题目问的是有多少种不同的“目标字符串”，而不是有多少种“覆盖方法”。后者也可以用类似方法做，但是转移规则不同。
2.fail指针要用BFS求，不能用DFS。
3.把trie上根节点的fail值设为0会比设为1方便很多（如果根节点编号是1）。
*/


#include <stdio.h>
#include <string.h>
#define M 1000000009

typedef long long LL;
int n,m,sz,son[110][4],fail[110],term[110],par[110],l[110],q[110],dig[110];
LL f[1010][110][20];
LL dp(int i,int u,int o);

int main(void)
{
    freopen("gene.in","r",stdin);
    freopen("gene.out","w",stdout);
    scanf("%d%d",&n,&m);
    memset(son,0,sizeof(son));
    memset(term,0,sizeof(term));
    sz=1;  l[1]=0;
    int i,j,u,v,t,d;
    char str[20];
    for(i=1;i<=m;i++)
    {
        scanf("%s",str);
        for(u=1,j=0;str[j];j++)
        {
            if(str[j]=='A') t=0;
            else if(str[j]=='C') t=1;
            else if(str[j]=='G') t=2;
            else t=3;
            if(!son[u][t])
            {
                l[++sz]=l[u]+1;
                son[u][t]=sz;
            }
            par[son[u][t]]=u;
            u=son[u][t];
        }
        term[u]=1;
    }
    memset(fail,-1,sizeof(fail));
    fail[1]=0;
    for(i=0;i<4;i++)
        fail[son[1][i]]=1;
    int front=1,rear=1;
    q[1]=1;
    while(front<=rear)
    {
        u=q[front];  d=dig[front];
        front++;
        if(fail[u]<0)
            for( fail[u]=1,v=fail[par[u]] ; v ; v=fail[v] )
                if(son[v][d])
                {
                    fail[u]=son[v][d];
                    break;
                }
        for(i=0;i<4;i++)
            if(son[u][i])
            {
                rear++;
                q[rear]=son[u][i];  dig[rear]=i;
            }
    }
    memset(f,-1,sizeof(f));
    printf("%I64d\n",dp(0,1,0));
    return 0;
}

LL dp(int i,int u,int o)
{
    if(f[i][u][o]>=0) return f[i][u][o];
    else if(i==n)
        return f[i][u][o] = o ? 0 : 1;
    f[i][u][o]=0;
    int j,v,w,t;
    for(j=0;j<4;j++)
    {
        for( w=0,v=u ; v&&l[v]>=o ; v=fail[v] )
            if(son[v][j])
            {
                w=son[v][j];
                break;
            }
        if(w)
        {
            for(t=w;l[t]>=o+1;t=fail[t])
                if(term[t]) break;
            f[i][u][o] = ( f[i][u][o] + dp(i+1,w,l[t]<o+1 ? o+1 : 0) )%M;
        }
    }
    return f[i][u][o];
}
