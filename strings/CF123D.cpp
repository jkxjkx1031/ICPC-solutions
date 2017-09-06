/*
CF123D: 后缀数组+单调栈

建立好高度数组，之后用单调栈像等高线一样把高度数组分成一层一层，再分别计算每一层的贡献即可。
*/


#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;
typedef long long LL;
char str[100010];
LL n,l[100010],r[100010],ofs,stk[100010],sa[100010],rk[100010],tmp[100010],hei[100010];
vector<LL> ll[100010],rr[100010];
void build_sa(char *str,LL sa[]);
void build_hei(char *str,LL sa[],LL hei[]);

int main()
{
    freopen("string.in","r",stdin);
    freopen("string.out","w",stdout);
    scanf("%s",str);
    n=strlen(str);
    build_sa(str,sa);
    build_hei(str,sa,hei);
    LL i,k;
    for(k=0,i=1;i<=n-1;i++)
    {
        while(k && hei[stk[k]]>=hei[i]) k--;
        l[i]=(k ? stk[k]+1 : 1);
        stk[++k]=i;
    }
    for(k=0,i=n-1;i>=1;i--)
    {
        while(k && hei[stk[k]]>=hei[i]) k--;
        r[i]=(k ? stk[k]-1 : n-1);
        stk[++k]=i;
    }
    for(i=1;i<=n-1;i++)
        if(hei[i] && (ll[hei[i]].empty() || l[i]!=ll[hei[i]].back()))
        {
            ll[hei[i]].push_back(l[i]);  rr[hei[i]].push_back(r[i]);
        }
    LL ans=0,p,q;
    for(hei[0]=hei[n]=0,i=1;i<=n-1;i++)
        while(!ll[i].empty())
        {
            p=ll[i].back();  q=rr[i].back();
            ans+=(q-p+2)*(q-p+3)/2*(i-max(hei[p-1],hei[q+1]));
            ll[i].pop_back();  rr[i].pop_back();
        }
    for(i=1;i<=n;i++)
        ans+=n-sa[i]+1-max(hei[i],hei[i-1]);
    printf("%I64d\n",ans);
    return 0;
}

bool cmp(LL i,LL j)
{
    if(rk[i]!=rk[j]) return rk[i]<rk[j];
    LL ri=(i+ofs<=n ? rk[i+ofs] : -1),rj=(j+ofs<=n ? rk[j+ofs] : -1);  // rank>=0
    return ri<rj;
}

void build_sa(char *str,LL sa[])
{
    LL i;
    for(i=1;i<=n;i++)
    {
        sa[i]=i;
        rk[i]=str[i-1];
    }
    for(ofs=1;ofs<=n;ofs*=2)
    {
        sort(sa+1,sa+n+1,cmp);
        for(tmp[sa[1]]=1,i=2;i<=n;i++)
            tmp[sa[i]]=tmp[sa[i-1]]+(cmp(sa[i-1],sa[i]) ? 1 : 0);
        for(i=1;i<=n;i++) rk[i]=tmp[i];
    }
}

void build_hei(char *str,LL sa[],LL hei[])
{
    LL h=0,i,j;
    for(i=1;i<=n;i++)
    {
        if(rk[i]==n) continue;
        j=sa[rk[i]+1];
        if(h) h--;
        for(;j+h<=n && i+h<=n;h++)
            if(str[j+h-1]!=str[i+h-1]) break;
        hei[rk[i]]=h;
    }
}
