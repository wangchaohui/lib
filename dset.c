

struct dset{
	int node;
	int rank;
	int pi;
	/*satellite*/
	int n;
	int sum;
};

struct dset ds[100001];

void makeset(int x)
{
	ds[x].pi=x;
	ds[x].rank=0;
	/*satellite*/
	ds[x].n=1;
	ds[x].sum=d[x];
}

int find(int x)
{
	if(x!=ds[x].pi)
		ds[x].pi=find(ds[x].pi);
	return ds[x].pi;
}

void link(int y,int x)
{
	ds[y].pi=x;
	/*satellite*/
	ds[x].n+=ds[y].n;
	ds[x].sum+=ds[y].sum;
}

int sunion(int x,int y)
{
	int rx=find(x),ry=find(y);
	if(rx==ry)return rx;
	if(ds[rx].rank>ds[ry].rank)
	{
		link(ry,rx);
		return rx;
	}
	else
	{
		link(rx,ry);
		if(ds[rx].rank==ds[ry].rank)
			ds[ry].rank++;
		return ry;
	}
}