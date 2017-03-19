/*
CF785E: 块状数组

交换任意两个数后，新增和减少的逆序对必定和这两个数有关。
因此需要解决的问题就是：求给定区间内不超过x的数共有多少个？

这是一个可以用块状数组解决的经典问题！（可以见《挑战程设》）

大致方法是将数列划分成大约sqrt(n)个长度大约为sqrt(n)的区间（假设区间长度为b），维护每个区间的有序化版本。
修改时，维护修改元素所在区间的有序性。复杂度O(b)。
查询时，对于不完整的区间顺序遍历，完整的区间二分查找。复杂度O((n/b)*logb)。

由于这道题的查询复杂度略高于修改复杂度，因此可以把b略微调大，使b约等于sqrt(nlogn)。
*/


#include <stdio.h>
#include <string.h>
#define N0 1877

typedef long long LL;
int n,m,q,a[200010],b[200010];
void update(int pos,int val);
int count(int pos);
int query(int pos,int val);

int main(void)
{
    freopen("permutation.in","r",stdin);
    freopen("permutation.out","w",stdout);
    scanf("%d%d",&n,&q);
    m=(n-1)/N0+1;
    int i;
    for(i=1;i<=n;i++) a[i]=b[i]=i;
    LL ans=0;
    int x,y,l,r,t;
    for(i=1;i<=q;i++)
    {
        scanf("%d%d",&l,&r);
        if(l>r)
        {
            t=l;  l=r;  r=t;
        }
        if(l<r&&a[l]<a[r]) ans--;
        else if(l<r) ans++;
        ans-=count(l)+count(r);
        x=a[l];  y=a[r];
        update(l,y);  update(r,x);
        ans+=count(l)+count(r);
        printf("%I64d\n",ans);
    }
    return 0;
}

void update(int pos,int val)
{
    int k=(pos-1)/N0+1,i,j,t;
    for(i=1;i<=N0;i++)
        if(b[(k-1)*N0+i]==a[pos])
        {
            b[(k-1)*N0+i]=val;
            break;
        }
    for( j=(k-1)*N0+i ; j<n && j<k*N0 && b[j]>b[j+1] ; j++ )
    {
        t=b[j];  b[j]=b[j+1];  b[j+1]=t;
    }
    for( j=(k-1)*N0+i ; j>(k-1)*N0+1 && b[j]<b[j-1] ; j-- )
    {
        t=b[j];  b[j]=b[j-1];  b[j-1]=t;
    }
    a[pos]=val;
}

int count(int pos)
{
    return query(pos,n)-query(pos,a[pos])+query(n,a[pos]-1)-query(pos,a[pos]-1);
}

int query(int pos,int val)
{
    int k=(pos-1)/N0+1,i,ans=0,l,r,m;
    for(i=1;i<=(pos-1)%N0+1;i++)
        if(a[(k-1)*N0+i]<=val) ans++;
    for(i=1;i<k;i++)
    {
        l=0;  r=N0;
        while(l<r)
        {
            m=(l+r+1)/2;
            if(b[(i-1)*N0+m]<=val) l=m;
            else r=m-1;
        }
        ans+=l;
    }
    return ans;
}
