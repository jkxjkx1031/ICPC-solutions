/*
BZOJ1046: LIS    复杂度O(nlogn+n*m)

先预处理出以每个数为开头的LIS长度（LIS数组），然后对于每个询问，把数列扫一遍就好了。

注意，本题要求的是下标的字典序，不是值的字典序。
如果要按元素值的字典序输出，可以在预处理完LIS数组后，对数列以(ai,i)为关键字重新排序。
可以证明，原数列中存在的上升子序列在新数列中仍然存在（但是新数列中存在的上升子序列原数列中不一定存在）。
然后就可以用与本题类似的方法处理了。在判定后继的时候代码需要做一些小改动。

参考：http://www.cnblogs.com/Sdchr/p/6130221.html
*/


#include <stdio.h>
#include <string.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))
#define INF 9000000000000000000

typedef long long LL;
int n,m,lis[10010];
LL a[10010],f[10010];

int main(void)
{
    freopen("sequence.in","r",stdin);
    freopen("sequence.out","w",stdout);
    scanf("%d",&n);
    int i,j;
    for(i=1;i<=n;i++)
        scanf("%lld",&a[i]);
    for(i=1;i<=n;i++) f[i]=-INF;
    int l,r,mid;
    for(i=n;i>=1;i--)
    {
        l=0;  r=n;
        while(l<r)
        {
            mid=(l+r+1)/2;
            if(f[mid]>a[i]) l=mid;
            else r=mid-1;
        }
        lis[i]=l+1;
        f[l+1]=MAX(f[l+1],a[i]);
    }
    scanf("%d",&m);
    int t;
    LL last;
    for(i=1;i<=m;i++)
    {
        scanf("%d",&t);
        for(last=-INF,j=1;t&&j<=n;j++)
            if(a[j]>last&&lis[j]>=t)
            {
                t--;
                if(t) printf("%lld ",a[j]);
                else printf("%lld\n",a[j]);
                last=a[j];
            }
        if(t) printf("Impossible\n");
    }
    return 0;
}
