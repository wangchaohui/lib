#include <stdio.h>

typedef unsigned int W;

typedef struct bn bn;

struct bn{
	bn *left,*right,*p;
	W key;
	/*satellite*/
};

typedef struct{
	bn *root;
}bst;

bn b[100001];
int tot;

bn* tree_serch(bn *x,W k)
{
	while(x!=NULL && k!=x->key)	
		if(k<x->key)
			x=x->left;
		else
			x=x->right;
	return x;
}

bn* tree_min(bn *x)
{
	while(x->left!=NULL)
		x=x->left;
	return x;
}

bn* tree_max(bn *x)
{
	while(x->right!=NULL)
		x=x->right;
	return x;
}

bn* tree_successor(bn *x)
{
	bn *y;
	if(x->right!=NULL)
		return tree_min(x->right);
	while((y=x->p)!=NULL && x==y->right)
		x=y;
	return x;
}

bn* tree_predecessor(bn *x)
{
	bn *y;
	if(x->left!=NULL)
		return tree_max(x->left);
	while((y=x->p)!=NULL && x==y->left)
		x=y;
	return x;
}

void tree_insert(bst *T,bn *z)
{
	bn *x,*y=NULL;
	x=T->root;
	while(x!=NULL)
	{
		y=x;
		if(z->key<x->key)
			x=x->left;
		else
			x=x->right;
	}
	z->p=y;
	if(y==NULL)
		T->root=z;
	else if(z->key<y->key)
		y->left=z;
	else
		y->right=z;
}

bn* tree_delete(bst *T,bn *z)
{
	bn *x,*y;
	if(z->left==NULL || z->right==NULL)
		y=z;
	else
		y=tree_successor(z);
	if(y->left!=NULL)
		x=y->left;
	else
		x=y->right;
	if(x!=NULL)
		x->p=y->p;
	if(y->p==NULL)
		T->root=x;
	else if(y==y->p->left)
		y->p->left=x;
	else
		y->p->right=x;
	if(y!=z)
	{
		z->key=y->key;
		/*satellite*/
	}
	return y;
}

int main()
{
	int n,m,i;
	unsigned int t;
	bst T;
	bn *x;
	scanf("%d %d",&n,&m);
	tot=0;T.root=NULL;
	for(i=1;i<=n;i++)
	{
		scanf("%u",&t);
		x=tree_serch(T.root,t);
		if(x==NULL)
		{
			x=&b[++tot];
			x->key=t;x->left=x->right=NULL;x->n=1;
			tree_insert(&T,x);
		}
		else
			x->n++;
	}
	for(i=1;i<=m;i++)
	{
		scanf("%u",&t);
		x=tree_serch(T.root,t);
		if(x==NULL)
			puts("0");
		else
			printf("%d\n",x->n);
	}
	return 0;
}
