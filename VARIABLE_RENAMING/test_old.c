#include<stdio.h>
void foo(int,int);
int fact(int);
int power(int,int);
int main()
{
	int temp1=5,temp2=10;
	foo(temp2,temp1);
	foo(fact(temp2),pow(temp2,temp1));
}
void foo(int x,int y)
{
	printf("%d %d\n",x,y);
}
int fact(int n)
{
	if(n <=1)
		return 1;
	return n*fact(n-1);
}
int power(int m,int n)
{
	if(n==1)
	return m;
	return m*power(m,n-1);
}
