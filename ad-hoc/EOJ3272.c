/*
EOJ3272: 压位    复杂度O(n*max{a[i]}/64)

详见http://acm.ecnu.edu.cn/blog/entry/33/
*/


#include <stdio.h>
#include <string.h>
#define MAX(X,Y) ((X)>(Y) ? (X) : (Y))

typedef unsigned long long ULL;
int T,n,m,a[50010];
ULL b[10010];
ULL high(ULL n,ULL len);
ULL low(ULL n,ULL len);

int main(void)
{
    freopen("nuclear.in","r",stdin);
    freopen("nuclear.out","w",stdout);
    scanf("%d",&T);
    int cas;
    for(cas=1;cas<=T;cas++)
    {
        scanf("%d",&n);
        memset(b,0,sizeof(b));
        int i,j;
        for(m=0,i=1;i<=n;i++)
        {
            scanf("%d",&a[i]);
            b[a[i]/64]|=(ULL)1<<(a[i]%64);
            m=MAX(m,a[i]/64);
        }
        printf("Case %d: ",cas);
        int k,d;
        for(i=1;i<=n;i++)
        {
            for(j=0;j<=m;j++)
            {
                k=j+a[i]/64;  d=a[i]%64;
                if(k>m) continue;
                if(a[i]/64==j)
                    b[j]-=(ULL)1<<(a[i]%64);
                if((low(b[k+1],d) & high(b[j],d)) > 0
                   || (high(b[k],64-d) & low(b[j],64-d)) > 0) break;
                if(a[i]/64==j)
                    b[j]+=(ULL)1<<(a[i]%64);
            }
            if(j<=m) break;
        }
        if(i<=n) printf("NO\n");
        else printf("YES\n");
    }
    return 0;
}

ULL low(ULL n,ULL len)
{
    if(!len) return 0;
    else if(len==64) return n;
    else return n&(((ULL)1<<len)-1);
}

ULL high(ULL n,ULL len)
{
    if(!len) return 0;
    else if(len==64) return n;
    else return n>>(64-len);
}
