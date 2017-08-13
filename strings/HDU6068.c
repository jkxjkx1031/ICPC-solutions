/*
HDU6068: KMP+DP  复杂度O(nm+km)

对于剩余的前半串，答案用KMP+前缀和很容易求出。
后半串需要使用DP的思想（类似于AC自动机上的DP）：假定已经匹配到模式串的第i个位置，母串的第j个位置，之后共能完成匹配多少次？

预处理出前半部分的前缀和数组，和后半部分的dp数组，每次询问用O(m)时间求和即可。
具体的求和公式见官方题解：http://bestcoder.hdu.edu.cn/blog/page/2/
*/


#include <stdio.h>
#include <string.h>

typedef long long LL;
int T,n,m,q,match[50010],fail[110];
LL cnt[110][50010],pref[50010],suf[110][50010];
char str[50010],s[50010],t[110];

int main(void)
{
    freopen("quotation.in","r",stdin);
    freopen("quotation.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d%d%*c",&n,&m,&q);
        int i,j,k;
        gets(str);
        for(i=1;i<=n;i++) s[i]=str[i-1];
        gets(str);
        for(i=1;i<=m;i++) t[i]=str[i-1];
        t[m+1]=0;
        for(fail[0]=fail[1]=0,i=2;i<=m;i++)
        {
            for(j=fail[i-1];j;j=fail[j])
                if(t[j+1]==t[i])
                {
                    fail[i]=j+1;
                    break;
                }
            if(!j) fail[i]=(t[1]==t[i] ? 1 : 0);
        }
        memset(pref,0,sizeof(pref));
        for(match[0]=0,i=1;i<=n;i++)
        {
            for(j=match[i-1];j;j=fail[j])
                if(t[j+1]==s[i])
                {
                    match[i]=j+1;
                    break;
                }
            if(!j) match[i]=(t[1]==s[i] ? 1 : 0);
            if(match[i]==m) pref[i]++;
        }
        for(i=1;i<=2;i++)
            for(j=1;j<=n;j++)
                pref[j]+=pref[j-1];
        for(i=0;i<=m;i++)
            for(cnt[i][0]=0,j=1;j<=n;j++)
                cnt[i][j]=cnt[i][j-1]+(match[j]==i ? 1 : 0);
        for(i=0;i<=m;i++) suf[i][n+1]=0;
        for(i=n;i>=1;i--)
            for(j=0;j<=m;j++)
            {
                for(k=j;k;k=fail[k])
                    if(t[k+1]==s[i]) break;
                if(!k) k=(t[1]==s[i] ? 1 : 0);
                else k++;
                suf[j][i]=suf[k][i+1];
                if(k==m) suf[j][i]++;
            }
        for(i=0;i<=m;i++)
            for(j=n;j>=1;j--)
                suf[i][j]+=suf[i][j+1];
        int l,r;
        LL ans;
        while(q--)
        {
            scanf("%d%d",&l,&r);
            for(ans=pref[l]*(n-r+1),i=0;i<=m;i++)
                ans+=suf[i][r]*cnt[i][l];
            printf("%lld\n",ans);
        }
    }
    return 0;
}
