/*
HDU6158: 笛卡尔定理

解笛卡尔定理的方程即可（四圆两两相切，已知其中三圆半径，便可求第四圆半径）。
该方程是一个一元二次方程，利用求根公式可以得到关于半径的递推式。
求根公式一共给出两根，n=2,3时两根相等（两圆对称），其余时候其中一根等于第n-4个圆的半径。
利用韦达定理，可以将一阶非线性递推转化为二阶线性递推，从而提高精度。

注意到圆的半径收敛速度很快，因此当圆的半径足够小时就不必继续递推。由于本题非spj，注意调节EPS值。
*/


#include <cstdio>
#include <cstring>

const double PI=3.14159265359,EPS=1e-15;
int T,r1,r2,n;
inline double sqr(double x)
{
    return x*x;
}

int main()
{
    freopen("designer.in","r",stdin);
    freopen("designer.out","w",stdout);
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d%d",&r1,&r2,&n);
        if(r1<r2)
        {
            int t=r1;  r1=r2;  r2=t;
        }
        double t01=1.0/r1,t02=1.0/r2,t1,t2=1.0/(r1-r2),t3,ans=sqr(r1-r2),s;
        int i;
        for(i=2;i<=n;i+=2)
        {
            t3=(i<4 ? t02+t2-t01 : (t02+t2-t01)*2-t1);
            s=sqr(1.0/t3);
            if(s<EPS) break;
            ans+=s*(i<n ? 2 : 1);
            t1=t2;  t2=t3;
        }
        printf("%.5f\n",ans*PI);
    }
    return 0;
}
