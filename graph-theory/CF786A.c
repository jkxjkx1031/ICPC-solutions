/*
CF786A: DP+SPFA    复杂度O(n^2)

如果只有"Win"和"Lose"，直接DP即可。这道题的难点在于对"Loop"的处理。
判定"Loop"的方法是，先尽可能确定非"Loop"的情况，最后剩下无法确定的就是"Loop"。
如果DP，我们必须要先确定一个计算顺序，在这个顺序下，每个状态所能转移到的状态都在这个状态之前被判定（因此，如果某次一个状态被判定为"Loop"，它必定是真-"Loop"）。
然而，这个顺序是很难确定的。如果参考最短路径问题可以发现，dijkstra算法是以“距离升序”为计算顺序的，但是bellman-ford和SPFA算法并没有使用这一计算顺序，而是不断做局部的“松弛操作”，直到没有点的最短距离被更新。
在本题中，我们也可以使用类似的方法：不断地将已确定的状态（非"Loop"）扩展，如果扩展出新的非"Loop"状态，就把它们加入队列。队列为空时，所有的非"Loop"状态都已经确定了。
*/


#include <stdio.h>
#include <string.h>

int n,k[2],s[2][7010],f[2][7010],c[2][7010],pos[14010],b[14010];
char e[2][7010][2][7010];
inline int next(int pos,int d)
{
    return (pos+d-1)%n+1;
}

int main(void)
{
    freopen("berzerk.in","r",stdin);
    freopen("berzerk.out","w",stdout);
    scanf("%d",&n);
    int i,j,l;
    for(i=0;i<=1;i++)
    {
        scanf("%d",&k[i]);
        for(j=1;j<=k[i];j++)
            scanf("%d",&s[i][j]);
    }
    memset(e,0,sizeof(e));
    for(i=0;i<=1;i++)
        for(j=1;j<=n;j++)
            for(l=1;l<=k[i];l++)
                e[i][j][i^1][next(j,s[i][l])]=1;
    memset(c,0,sizeof(c));
    memset(f,-1,sizeof(f));
    f[0][1]=f[1][1]=0;
    int front=1,rear=2,t,p,q;
    b[1]=0;  pos[1]=1;
    b[2]=1;  pos[2]=1;
    while(front<=rear)
    {
        t=b[front];  p=pos[front];
        front++;
        for(q=1;q<=n;q++)
        {
            if( !e[t^1][q][t][p] || f[t^1][q]>=0 ) continue;
            if(!f[t][p])
            {
                f[t^1][q]=1;
                rear++;
                b[rear]=t^1;  pos[rear]=q;
            }
            else
            {
                c[t^1][q]++;
                if(c[t^1][q]==k[t^1])
                {
                    f[t^1][q]=0;
                    rear++;
                    b[rear]=t^1;  pos[rear]=q;
                }
            }
        }
    }
    for(i=0;i<=1;i++)
    {
        for(j=2;j<=n;j++)
            if(f[i][j]==-1) printf("Loop ");
            else if(!f[i][j]) printf("Lose ");
            else printf("Win ");
        putchar('\n');
    }
    return 0;
}
