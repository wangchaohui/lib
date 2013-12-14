#include <stdio.h>

#define RED 0
#define BLACK 1

typedef int W;

typedef struct bn bn;

struct bn{
	bn *left,*right,*p;
	char color;
	W key;
	/*satellite*/
	int n;
};

typedef struct{
	bn *root,nil;
}bst;

bn b[2700000];
int tot,d[4001][4];

void left_rotate(bst *T,bn *x)
{
	bn *y=x->right;
	x->right=y->left;
	if(y->left!=&T->nil)
		y->left->p=x;
	y->p=x->p;
	if(x->p==&T->nil)
		T->root=y;
	else if(x==x->p->left)
		x->p->left=y;
	else
		x->p->right=y;
	y->left=x;
	x->p=y;
}

void right_rotate(bst *T,bn *x)
{
	bn *y=x->left;
	x->left=y->right;
	if(y->right!=&T->nil)
		y->right->p=x;
	y->p=x->p;
	if(x->p==&T->nil)
		T->root=y;
	else if(x==x->p->left)
		x->p->left=y;
	else
		x->p->right=y;
	y->right=x;
	x->p=y;
}

bn* RB_serch(bst *T,W k)
{
	bn *x=T->root;
	while(x!=&T->nil && k!=x->key)	
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

void RB_insert_fixup(bst *T,bn *z)
{
	bn *y;
	while(z->p->color==RED)
		if(z->p==z->p->p->left)
		{
			y=z->p->p->right;
			if(y->color==RED)
			{
				z->p->color=BLACK;
				y->color=BLACK;
				z->p->p->color=RED;
				z=z->p->p;
			}
			else
			{
				if(z==z->p->right)
				{
					z=z->p;
					left_rotate(T,z);
				}
				z->p->color=BLACK;
				z->p->p->color=RED;
				right_rotate(T,z->p->p);
			}
		}
		else
		{
			y=z->p->p->left;
			if(y->color==RED)
			{
				z->p->color=BLACK;
				y->color=BLACK;
				z->p->p->color=RED;
				z=z->p->p;
			}
			else
			{
				if(z==z->p->left)
				{
					z=z->p;
					right_rotate(T,z);
				}
				z->p->color=BLACK;
				z->p->p->color=RED;
				left_rotate(T,z->p->p);
			}
		}
		T->root->color=BLACK;
}

void RB_insert(bst *T,bn *z)
{
	bn *x=T->root,*y=&T->nil;
	while(x!=&T->nil)
	{
		y=x;
		if(z->key<x->key)
			x=x->left;
		else
			x=x->right;
	}
	z->p=y;
	if(y==&T->nil)
		T->root=z;
	else if(z->key<y->key)
		y->left=z;
	else
		y->right=z;
	z->left=&T->nil;
	z->right=&T->nil;
	z->color=RED;
	RB_insert_fixup(T,z);
}

void RB_delete_fixup(bst *T,bn *x)
{
	bn *w;
	while(x!=T->root && x->color==BLACK)
		if(x==x->p->left)
		{
			w=x->p->right;
			if(w->color==BLACK)
			{
				x->p->color=RED;
				left_rotate(T,x->p);
				w=x->p->right;
			}
			if(w->left->color==BLACK && w->right->color==BLACK)
			{
				w->color=RED;
				x=x->p;
			}
			else
			{
				if(w->right->color==BLACK)
				{
					w->left->color=BLACK;
					w->color=RED;
					right_rotate(T,w);
					w=x->p->right;
				}
				w->color=x->p->color;
				x->p->color=BLACK;
				w->right->color=BLACK;
				left_rotate(T,x->p);
				x=T->root;
			}
		}
		else
		{
			w=x->p->left;
			if(w->color==BLACK)
			{
				x->p->color=RED;
				right_rotate(T,x->p);
				w=x->p->left;
			}
			if(w->right->color==BLACK && w->left->color==BLACK)
			{
				w->color=RED;
				x=x->p;
			}
			else
			{
				if(w->left->color==BLACK)
				{
					w->right->color=BLACK;
					w->color=RED;
					left_rotate(T,w);
					w=x->p->left;
				}
				w->color=x->p->color;
				x->p->color=BLACK;
				w->left->color=BLACK;
				right_rotate(T,x->p);
				x=T->root;
			}
		}
		x->color=BLACK;
}

bn* RB_delete(bst *T,bn *z)
{
	bn *x,*y;
	if(z->left==&T->nil || z->right==&T->nil)
		y=z;
	else
		y=tree_successor(z);
	if(y->left!=&T->nil)
		x=y->left;
	else
		x=y->right;
	x->p=y->p;
	if(y->p==&T->nil)
		T->root=x;
	else if(y==y->p->left)
		y->p->left=x;
	else
		y->p->right=x;
	if(y!=z)
	{
		z->key=y->key;
		/*satellite*/
		z->n=y->n;
	}
	if(y->color==BLACK)
		RB_delete_fixup(T,x);
	return y;
}

void RB_init(bst *T)
{
	tot=0;T->root=&T->nil;T->nil.color=BLACK;
}

int main()
{
	int n,i,j,t;
	__int64 c=0;
	bst T;
	bn *x;
	RB_init(&T);
	scanf("%d",&n);
	for(i=1;i<=n;i++)
		scanf("%d %d %d %d",&d[i][0],&d[i][1],&d[i][2],&d[i][3]);
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
		{
			t=d[i][0]+d[j][1];
			x=RB_serch(&T,t);
			if(x==&T.nil)
			{
				x=&b[++tot];
				x->key=t;x->n=1;
				RB_insert(&T,x);
			}
			else
				x->n++;
		}
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
		{
			t=d[i][2]+d[j][3];
			x=RB_serch(&T,-t);
			if(x!=&T.nil)
				c+=x->n;
		}
	printf("%I64d\n",c);
	return 0;
}
