
#define swap(a,b){t=a;a=b;b=t;}
#define maxn 50000

int lc[maxn+1];

void heapsort(int *heap,int (*CFun)(int,int));
void buildheap(int *heap,int (*CFun)(int,int));
void heapify(int *heap,int i,int (*CFun)(int,int));
void shiftup(int *heap,int i,int (*CFun)(int,int));
void heapadd(int *heap,int i,int (*CFun)(int,int));
int extract(int *heap,int (*CFun)(int,int));
int CFun(int x,int y);

/******* ******* *******

carefully

buildheap in heapsort sometimes is not must

lc for decrease-key use

******* ******* *******/

void heapsort(int *heap,int (*CFun)(int,int))
{
	int i,t,l=heap[0];
	buildheap(heap,CFun);
	for(i=heap[0];i>=2;i--)
	{
		swap(heap[1],heap[i]);
		heap[0]--;
		heapify(heap,1,CFun);
	}
	heap[0]=l;
}

void buildheap(int *heap,int (*CFun)(int,int))
{
	int i;
	for(i=heap[0]/2;i>=1;i--)
		heapify(heap,i,CFun);
}

void heapify(int *heap,int i,int (*CFun)(int,int))
{
	int min=2*i,n=heap[0],t;
	while(min<=n)
	{
		if(min+1<=n && CFun(heap[min],heap[min+1])<0)min++;
		if(CFun(heap[i],heap[min])>=0)return;
		swap(heap[i],heap[min]);
		lc[heap[i]]=i;
		lc[heap[min]]=min;
		i=min;min*=2;
	}
}

int CFun(int x,int y)
{
	if(x>y)return 1;/*left is better*/
	return -1;/*right is better*/
}

void shiftup(int *heap,int i,int (*CFun)(int,int))
{
	int t,ii;
	while(i>1)
	{
		ii=i/2;
		if(CFun(heap[i],heap[ii])<=0)return;
		swap(heap[i],heap[ii]);
		lc[heap[i]]=i;
		lc[heap[ii]]=ii;
		i=ii;
	}
}

void heapadd(int *heap,int i,int (*CFun)(int,int))
{
	heap[++heap[0]]=i;
	lc[i]=heap[0];
	shiftup(heap,heap[0],CFun);
}

void heapdel(int *heap,int i,int (*CFun)(int,int))
{
	lc[heap[i]]=-1;
	if(--heap[0]>=i)
	{
		heap[i]=heap[heap[0]+1];
		lc[heap[i]]=i;
		heapify(i);
		shiftup(i);
	}
}

int extract(int *heap,int (*CFun)(int,int))
{
	int m=heap[1];
	heapdel(heap,1,CFun);
	return m;
}