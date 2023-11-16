#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll ans=0;
int n,m,a[2005][2005],sum[2005][2005],up[2005][2005];
inline int get(int x,int y,int p,int q){
	return sum[p][q]+sum[x-1][y-1]-sum[x-1][q]-sum[p][y-1];
}
inline int read(){
	int x=0;
	char c=getchar();
	while(c<'0'||c>'9'){
		c=getchar();
	}
	while(c>='0'&&c<='9'){
		x=(x<<3)+(x<<1)+(c^48);
		c=getchar();
	}
	return x;
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	memset(up,-1,sizeof(up));
	n=read(),m=read();
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			a[i][j]=read();
		}
	}
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			sum[i][j]=a[i][j]+sum[i-1][j]+sum[i][j-1]-sum[i-1][j-1];
			//cout<<sum[i][j]<<' ';
		}
		//cout<<endl;
	}
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			int t=up[i-1][j-1];
			if(a[i][j]){
				if(t!=-1&&get(i-t-1,j-t-1,i,j)==(t+2)*(t+2)){
					up[i][j]=up[i-1][j-1]+1;
					//cout<<i<<' '<<j<<' '<<up[i][j]<<endl;
				}else{
					up[i][j]=0;
				}
			}
		}
	}
	for(int i=1;i<=n;i++){
		for(int j=1;j<=m;j++){
			if(a[i][j]!=0&&a[i+1][j]!=1&&a[i][j+1]!=1&&a[i+1][j+1]!=1){
				for(int t=0;t<=up[i][j];t++){
					for(int k=i+1;k<=n;k++){
						int l=j,r=m,mid=0;
						while(l<r){
							mid=(l+r+1)>>1;
							if(get(i-t,j-t,k,mid)==(t+1)*(t+1)){
								l=mid;
							}else{
								r=mid-1;
							}	
						}
						ans+=l-j;
						if(l==j) break;
					} 				
				}				
			}
		}
	}
	cout<<ans;
	return 0;
}
