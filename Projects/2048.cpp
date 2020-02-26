// 2048.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
//system("cls");
int _tmain(int argc, _TCHAR* argv[])
{
	
	//declare
	void print(int[][4],int score,int max);//把地图打印出来
	int move(int[][4],char,int*,int*);//移动(包含更新地图,更新分数),回传0代表输入错误
	int tell(int[][4]);//判断是否还能动,不能动回传0
	void create(int[][4]);//随机生成新节点
	char tellorient(int map[][4],int score,float x,float y,float z,int w,int ban[4],int times[]);
	//variation
	srand(time(NULL));
	int times[31][15]={0,0,66,5,7,91,55,65,78,96,27,60,84,24,19,28};
	//int times[31][15];
	float x,y,z;
	int w;
	int map[4][4]={0};
	int score=0,max=0,t,son;
	char a;
	int aver=20000;
	int round=0,fround=0,roundcount=0,sum=0;
	//body
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			map[i][j]=0;
		}
	}
	while(1){
		//遺傳初始化
		/*if(fround<=30){
			for(int i=0;i<15;i++)times[30][i]=(rand()%100);
			roundcount=30;
		}else{
			times[rand()%30][rand()%15]=rand()%100;
			int dad1=rand()%30,dad2=rand()%30,dad3=rand()%30;
			for(int i=0;i<5;i++){
				times[30][i*3+0]=times[dad1][i*3+0];
				times[30][i*3+1]=times[dad2][i*3+1];
				times[30][i*3+2]=times[dad3][i*3+2];
			}
			roundcount=30;
		}*/
		//
		x=310.0;
		y=0.5;
		z=30.0;
		w=4.0;
		create(map);
		create(map);
		do{
			print(map,score,max);
			do{
				int ban[4]={0,0,0,0};
				printf("w(up)a(left)s(down)d(right)n(new_game)q(quit)=");
				//a=getchar();
				a=tellorient(map,score,x,y,z,w,ban,times[roundcount]); // 把上面那行换成下面这行可改为自动操控模式
				fflush(stdin);
			}while((t=move(map,a,&score,&max))==0);
			if(t==-1)return 0;
			system("cls");
			create(map);
		}while(tell(map));
		system("cls");
		print(map,score,max);
		printf("GAME OVER\nscore=%d\n",score);
		printf("q(quit)n(new_game)=");
		a=getchar();
		fflush(stdin);
		//遺傳代碼
		/*if(score>aver){
			if(fround>30){
				son=rand()%30;
				for(int i=0;i<15;i++)times[son][i]=times[30][i];
			}else{
				for(int i=0;i<15;i++)times[fround][i]=times[30][i];
			}
			printf("round=%d score=%d\n",fround,score);
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					if(i==0&&j==0)printf("   ");
					else printf("%3d",times[(fround<=30?fround:son)][i*4+j-1]);
				}
				printf("\n");
			}
			fround++;
		}
		a=(fround<1000?'n':'q');*/
		//
		if(a=='q')break;
		else{
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					map[i][j]=0;
				}
			}
			if(score>max)max=score;
			score=0;
			system("cls");
		}
		//printf("\n");
	}
	printf("\n%d",sum/50);
	return 0;
}
//智能体移动
char agentmove(int map[4][4],float x,float y,float z,int w,int times[]){
	void create(int map[][4]);
	int move(int map[][4],char a,int *score,int* max);
	int count(int map[][4],int score,float x,float y,float z,float w,int times[]);
	int tell(int map[][4]);
	char alpha[4]={'w','a','s','d'};
	int pup=0,pdown=0,pleft=0,pright=0;
	for(int i=0;i<4;i++){
		//建立新地图
		int newmap[4][4],t=0,m=0,point=0;
		for(int k=0;k<4;k++)
			for(int l=0;l<4;l++)
				newmap[k][l]=map[k][l];
		//第一步
		if(move(newmap,alpha[i],&t,&m)==0)continue;
		create(newmap);
		if(tell(newmap)==0)point=count(newmap,0,x,y,z,w,times)*(-5);
		//给分
		switch(i){
		case 0:
			pup+=(count(newmap,0,x,y,z,w,times)+point);		
			break;
		case 1:
			pleft+=(count(newmap,0,x,y,z,w,times)+point);
			break;
		case 2:
			pdown+=(count(newmap,0,x,y,z,w,times)+point);
			break;
		case 3:
			pright+=(count(newmap,0,x,y,z,w,times)+point);
			break;
		};
	}
	int maxi=(pup>pdown?pup:pdown);
	maxi=(maxi>pleft?maxi:pleft);
	maxi=(maxi>pright?maxi:pright);
	if(maxi==pup)return 'w';
	if(maxi==pdown)return 's';
	if(maxi==pleft)return 'a';
	if(maxi==pright)return 'd';
}
//判断方向函数
char tellorient(int map[][4],int score,float x,float y,float z,int w,int ban[4],int times[]){
	int count(int map[][4],int score,float x,float y,float z,float w,int times[]);
	int tell(int map[][4]);
	int move(int map[][4],char a,int *score,int* max);
	void create(int map[][4]);
	int pup=0,pdown=0,pleft=0,pright=0;
	char alpha[4]={'w','a','s','d'};
	for(int i=0;i<4;i++){
		if(ban[i]==0){
			for(int j=0;j<100;j++){
				//建立新地图
				int newmap[4][4],tempscore=score,maximum;
				for(int k=0;k<4;k++)
					for(int l=0;l<4;l++)
						newmap[k][l]=map[k][l];
				//第一步
				if(move(newmap,alpha[i],&tempscore,&maximum)==0)break;
				create(newmap);
				//后十步
				for(int k=0;k<w;k++){
					if(move(newmap,agentmove(newmap,x,y,z,w,times),&tempscore,&maximum)==0)break;
					create(newmap);
					if(tell(newmap)==0){
						tempscore*=(-z);
						break;
					}
				}
				//给分
				switch(i){
				case 0:
					pup+=count(newmap,tempscore,x,y,z,w,times);
					break;
				case 1:
					pleft+=count(newmap,tempscore,x,y,z,w,times);
					break;
				case 2:
					pdown+=count(newmap,tempscore,x,y,z,w,times);
					break;
				case 3:
					pright+=count(newmap,tempscore,x,y,z,w,times);
					break;
				};
			}
		}
	}
	int maxi=(pup>pdown?pup:pdown);
	maxi=(maxi>pleft?maxi:pleft);
	maxi=(maxi>pright?maxi:pright);
	int ans=-1;
	if(maxi==pup)ans=0;
	if(maxi==pdown)ans=2;
	if(maxi==pleft)ans=1;
	if(maxi==pright)ans=3;
	//建立新地图
	int newmap[4][4],tempscore=score,maximum;
		for(int k=0;k<4;k++)
			for(int l=0;l<4;l++)
				newmap[k][l]=map[k][l];
	if(move(newmap,alpha[ans],&tempscore,&maximum)==0){
		ban[ans]=1;
		return tellorient(newmap,tempscore,x,y,z,w,ban,times);
	}else{
		return alpha[ans];
	}
}
float pow(float x,int y){
	float ans=x;
	for(int i=0;i<y;i++){
		ans*=x;
	}
	return ans;
}
//算分函数(x左上角权重y挥发系数z代理人死掉扣分w差值过大扣分)
int count(int map[][4],int score,float x,float y,float z,float w,int a[]){
	//权重分加分
	float pow(float,int);
	float sum=score;
	sum+=(map[0][0]*x);
	sum+=(map[0][1]*x*pow(y,a[0]));
	sum+=(map[0][2]*x*pow(y,a[1]));
	sum+=(map[0][3]*x*pow(y,a[2]));
	sum+=(map[1][0]*x*pow(y,a[3]));
	sum+=(map[1][1]*x*pow(y,a[4]));
	sum+=(map[1][2]*x*pow(y,a[5]));
	sum+=(map[1][3]*x*pow(y,a[6]));
	sum+=(map[2][0]*x*pow(y,a[7]));
	sum+=(map[2][1]*x*pow(y,a[8]));
	sum+=(map[2][2]*x*pow(y,a[9]));	
	sum+=(map[2][3]*x*pow(y,a[10]));
	sum+=(map[3][0]*x*pow(y,a[11]));
	sum+=(map[3][1]*x*pow(y,a[12]));
	sum+=(map[3][2]*x*pow(y,a[13]));
	sum+=(map[3][3]*x*pow(y,a[14]));
	return sum;
}
//移动(包含更新地图,更新分数),回传0代表输入错误
int move(int map[][4],char a,int *score,int* max){
	void create(int[][4]);
	int flag=0;
	switch(a){
		case 'q':
			return -1;
			break;
		case 'n':
			if(*score>*max)*max=*score;
			*score=0;
			for(int i=0;i<4;i++){
				for(int j=0;j<4;j++){
					map[i][j]=0;
				}
			}
			create(map);
			break;
		case 'a': //left
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[i][k]==0&&map[i][k+1]!=0){
								flag=1;
								int t=map[i][k];
								map[i][k]=map[i][k+1];
								map[i][k+1]=t;
							}
						}
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(map[i][j]==map[i][j+1]){
						flag=1;
						map[i][j]*=2;
						map[i][j+1]=0;
						*score+=map[i][j];
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[i][k]==0&&map[i][k+1]!=0){
								int t=map[i][k];
								map[i][k]=map[i][k+1];
								map[i][k+1]=t;
							}
						}
					}
				}
			}
			if(flag==0)return 0;
			break;
		case 'w': //up
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[k][i]==0&&map[k+1][i]!=0){
								flag=1;
								int t=map[k][i];
								map[k][i]=map[k+1][i];
								map[k+1][i]=t;
							}
						}
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(map[j][i]==map[j+1][i]){
						flag=1;
						map[j][i]*=2;
						map[j+1][i]=0;
						*score+=map[j][i];
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[k][i]==0&&map[k+1][i]!=0){
								int t=map[k][i];
								map[k][i]=map[k+1][i];
								map[k+1][i]=t;
							}
						}
					}
				}
			}
			if(flag==0)return 0;
			break;
		case 's': //down
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[3-k][i]==0&&map[2-k][i]!=0){
								flag=1;
								int t=map[3-k][i];
								map[3-k][i]=map[2-k][i];
								map[2-k][i]=t;
							}
						}
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(map[3-j][i]==map[2-j][i]){
						flag=1;
						map[3-j][i]*=2;
						map[2-j][i]=0;
						*score+=map[3-j][i];
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[3-k][i]==0&&map[2-k][i]!=0){
								int t=map[3-k][i];
								map[3-k][i]=map[2-k][i];
								map[2-k][i]=t;
							}
						}
					}
				}
			}
			if(flag==0)return 0;
			break;
		case 'd': //right
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[i][3-k]==0&&map[i][2-k]!=0){
								flag=1;
								int t=map[i][2-k];
								map[i][2-k]=map[i][3-k];
								map[i][3-k]=t;
							}
						}
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int j=0;j<3;j++){
					if(map[3-i][j]==map[2-i][j+1]){
						map[3-i][j]*=2;
						map[2-i][j+1]=0;
						*score+=map[i][j];
					}
				}
			}
			for(int i=0;i<4;i++){
				for(int i=0;i<4;i++){//每一行
					for(int j=0;j<4;j++){
						for(int k=0;k<4-j-1;k++){
							if(map[i][3-k]==0&&map[i][2-k]!=0){
								flag=1;
								int t=map[i][2-k];
								map[i][2-k]=map[i][3-k];
								map[i][3-k]=t;
							}
						}
					}
				}
			}
			if(flag==0)return 0;
			break;
		default:
			return 0;
			break;
	}
	return 1;
}
//判断是否还能动,不能动回传0
int tell(int map[][4]){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(map[i][j]==0)return 1;
		}
	}
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(i>0){
				if(map[i-1][j]==map[i][j])return 1;
			}
			if(j>0){
				if(map[i][j-1]==map[i][j])return 1;				
			}
			if(i<3){
				if(map[i+1][j]==map[i][j])return 1;
			}
			if(j<3){
				if(map[i][j+1]==map[i][j])return 1;
			}
		}
	}
	return 0;
}
//随机生成新节点
void create(int map[][4]){
	//srand(time(NULL));
	while(1){
		int b;
		int a=(rand()%16);
		b=(rand()%5);
		b=(b==4?4:2);
		if(map[a/4][a%4]==0){
			map[a/4][a%4]=b;
			break;
		}
	}
}
//把地图打印出来
void print(int a[][4],int score,int max){
	printf("score=%d  max=%d\n",score,max);
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			if(a[i][j]==0)printf("    ");
			else printf("%4d",a[i][j]);
		}
		printf("\n");
	}
}
