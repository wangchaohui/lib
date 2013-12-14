#include <stdio.h>
#include <string.h>

int state[8][256];

void FSM()
{
	int i,j;
	for(i=0;i<=7;i++)for(j=0;j<256;j++)state[i][j]=-1;
	for(i='0';i<='9';i++)
	{
		state[0][i]=1;//
		state[1][i]=1;//1
		state[2][i]=3;//.
		state[3][i]=3;//.1
		state[4][i]=6;//E
		state[5][i]=6;//E-
		state[6][i]=6;//E1
	}
	state[0][' ']=0;
	state[0]['-']=1;
	state[1]['.']=2;
	state[1]['E']=4;
	state[3]['E']=4;
	state[4]['-']=5;
	state[1][' ']=7;
	state[3][' ']=7;
	state[6][' ']=7;
	state[7][' ']=7;
}

int main()
{
	int p,i,st,l;
	char s[256];
	FSM();
	scanf("%d ",&p);
	while(p--)
	{
		gets(s);
		l=strlen(s);
		st=0;
		for(i=0;i<l && st!=-1;i++)st=state[st][s[i]];
		if(st==1 || st==3 || st==6 || st==7)
			puts("YES");
		else
			puts("NO");
	}
	return 0;
}
