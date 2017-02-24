/*
BZOJ1059: 二分图匹配    复杂度O(VE)

一种很常用的二分图建图方法：每一行对应左侧点集中一个节点（行节点），每一列对应右侧点集中一个节点（列节点），如果(i,j)位置为1，就在行节点i和列节点j之间连一条边。
然后用匈牙利算法求最大匹配即可。如果最大匹配=n，输出Yes，否则输出No。
*/


#include <stdio.h>
#include <string.h>

int T,n,e[210][210],vis[210],from[210];
int match(int u);

int main(void)
{
    freopen("matrix.in","r",stdin);
    freopen("matrix.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d",&n);
        memset(e,0,sizeof(e));
        int i,j,t;
        for(i=1;i<=n;i++)
            for(j=1;j<=n;j++)
            {
                scanf("%d",&t);
                if(t) e[i][j]=1;
            }
        memset(from,0,sizeof(from));
        int ans=0;
        for(i=1;i<=n;i++)
        {
            memset(vis,0,sizeof(vis));
            if(match(i)) ans++;
        }
        if(ans==n) printf("Yes\n");
        else printf("No\n");
    }
    return 0;
}

int match(int u)
{
    int i;
    for(i=1;i<=n;i++)
        if( e[u][i] && !vis[i] )
        {
            vis[i]=1;
            if( !from[i] || match(from[i]) )
            {
                from[i]=u;
                return 1;
            }
        }
    return 0;
}
