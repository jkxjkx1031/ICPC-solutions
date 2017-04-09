/*
CF788C: 等式变形+最小环    复杂度O(n^2)

把题目的要求写成数学等式：(a1+a2+...+am)/m=n，
即a1+a2+...+am=nm，
即(a1-n)+(a2-n)+...+(am-n)=0.

可以建立一个整数数轴，ai-n>0的项对应指向数轴正方向的有向边，ai-n<0的项对应指向负方向的边。将这些边加到数轴的每一个点上。
于是，原问题就转化为，从原点出发，找一个最小环再回到原点。由于边都是无权的，所以可以用BFS解决。
值得注意的是，可以把遍历到的点的范围限定在0~2000内。这是因为如果有一个环会用到2000+的点，那么必定也可以在走到1000+的时候先往回走。同理，环上也可以不用到负数点。
还有一个细节：最小环也可能是0点上的自环，需要特判一下。
*/


#include <stdio.h>
#include <string.h>
#define MIN(X,Y) ((X)<(Y) ? (X) : (Y))

int n,k,b[1010],e[2010][2010],q[2010],dis[2010];

int main(void)
{
    freopen("mixing.in","r",stdin);
    freopen("mixing.out","w",stdout);
    scanf("%d%d",&n,&k);
    memset(b,0,sizeof(b));
    int i,j,a;
    for(i=1;i<=k;i++)
    {
        scanf("%d",&a);
        b[a]=1;
    }
    if(b[n])
    {
        printf("1\n");
        return 0;
    }
    memset(e,0,sizeof(e));
    for(i=0;i<=1000;i++)
        if(b[i] && i-n>0)
            for(j=0;j+i-n<=2000;j++)
                e[j][j+i-n]=1;
        else if(b[i] && i-n<0)
            for(j=2000;j+i-n>=0;j--)
                e[j][j+i-n]=1;
    memset(dis,-1,sizeof(dis));
    dis[0]=0;
    q[1]=0;
    int front=1,rear=1,u,v;
    while(front<=rear)
    {
        u=q[front++];
        for(v=0;v<=2000;v++)
        {
            if(dis[v]>=0 || !e[u][v]) continue;
            dis[v]=dis[u]+1;
            q[++rear]=v;
        }
    }
    int ans=3000;
    for(i=1;i<=2000;i++)
        if(e[i][0] && dis[i]>=0)
            ans=MIN(ans,dis[i]+1);
    if(ans==3000) printf("-1\n");
    else printf("%d\n",ans);
    return 0;
}
