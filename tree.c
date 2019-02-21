#include <stdio.h>
#include <stdlib.h>

/////
//queue
struct qnode{
	struct qnode * next;
	void * udata;
};
/////
//树节点
struct tnode{
	struct tnode *l;
	struct tnode *r;
	int data;
	int height; //balance
	void * udata;
};

typedef struct tnode tnode;
void insert(tnode*);
void lcr(tnode*);
void clr(tnode*);
void lrc(tnode*);
void randTree(tnode*);
int isFullBinTree(tnode*);
int isFullBTInner(tnode*);

void insert(tnode *r){
	
}

//遍历
void lcr(tnode *node){
	if(node == 0)
		return;
	lcr(node->l);
	printf("%d,",node->data);
	lcr(node->r);
}

void clr(tnode *node){
	if(node ==0)
		return;
	printf("%d,",node->data);
	clr(node->l);
	clr(node->r);
}

void lrc(tnode *node){
	if(node == 0)
		return;
	lrc(node->l);
	lrc(node->r);
	printf("%d,",node->data);
}

//生成
static int maxData = 1000;
void randTree(tnode *root){
	int i = 0;
	tnode *l = malloc(sizeof(tnode));
	tnode *r = malloc(sizeof(tnode));
	l->data = rand() % maxData;
	r->data = rand() % maxData;
	root->l = l;
	root->r = r;	
}

void fillTree(tnode *p, int d){
	d--;
	if(d < 0)
		return;
	randTree(p);
	fillTree(p->l,d);
	fillTree(p->r,d);
}

//测试
static const IS_NOT_FULL = -1;
static const IS_FULL = 0;
static const IS_NULL = 1;
static const INNER_DIFF = 2;
int isFullBinTree(tnode* root){
	int res;
	if(root != 0){
		res = isFullBTInner(root);
		if(res == INNER_DIFF)
			return IS_NOT_FULL;
		return IS_FULL;
	}
	return IS_NULL;
}

int isFullBTInner(tnode *node){
	if(node->l == 0 && node->r != 0)
		return INNER_DIFF;
	if(node->l != 0 && node->r == 0)
		return INNER_DIFF;
	if(node->l == 0 && node->r == 0)
		return IS_NULL;

	int lres = isFullBTInner(node->l);
	int rres = isFullBTInner(node->r);
		
	if(lres == rres == IS_NULL)
		return IS_NULL;
	return INNER_DIFF;
}

///////////////////////
//二分查找树

tnode* insertBST(tnode*,int);
tnode* insertBST(tnode *node,int data){
	if(node == 0){
		node = malloc(sizeof(node));
		bzero(node,sizeof(node));
		node->data = data;
	}else if(node->data < data){
		node->r = insertBST(node->r,data);
	}else if(node->data > data){
		node->l = insertBST(node->l,data);
	}else{
		//same
	}
	return node;
}

tnode* searchBST(tnode*,int);
tnode* searchBST(tnode* node, int data){
	if(node == 0)
		return 0;
	if(node->data > data){
		return searchBST(node->l, data);
	}else if(node->data < data){
		return searchBST(node->r, data);
	}else if(node->data == data){
		return node;
	}else{
		return 0;	
	}
}

//范围搜索
//只要是二叉树都可以使用
void rangeSearch(tnode*,int,int);
void rangeSearch(tnode *node,int lo,int hi){
	if(node == 0)
		return;
	
	//lcr = BT sorted output
	//ASC
	if(node->data > lo){
		rangeSearch(node->l, lo, hi);
	}
	if(node->data >= lo && node->data <= hi) //loo,[lo~hi],hii
	{
		//enqueue
		printf("%d,",node->data);
	}
	if(node->data < hi){
		rangeSearch(node->r, lo, hi);
	}
}

void reverRangeSearch(tnode*,int,int);
void reverRangeSearch(tnode* node,int lo,int hi){
	//rcl DESC
	if(node == 0)
		return;

	if(node->data < hi){
		reverRangeSearch(node->r, lo, hi);
	}
	if(node->data <= hi && node->data >= lo){
		//enqueue
		printf("%d,\n",node->data);
	}
	if(node->data > lo){
		reverRangeSearch(node->l, lo, hi);
	}
}
tnode* minBST(tnode*);
tnode* minBST(tnode* node){
	if(node == 0)
		return 0;
	
	if(node->l != 0)	
		return minBST(node->l);
	return node;
}

tnode* maxBST(tnode*);
tnode* maxBST(tnode* node){
	if(node == 0)
		return 0;
	if(node->r != 0)
		return maxBST(node->r);
	return node;
}

tnode* delBST(tnode*,int);
tnode* delBST(tnode* node,int data){
	//lcr
	//c comp l / r
	printf("node->data[%d] == data[%d]\n",node->data,data);
	printf("node->l[%lx] ->r[%lx]\n",node->l,node->r);
	if(node == 0)
		return 0;
	
	if(node->data > data){
		node->l = delBST(node->l, data);
		return node;
	}else if(node->data == data){
		if(node->r != 0){
			printf("up right\n");
			tnode *rmin = minBST(node->r);
			printf("rmin %d\n",rmin->data);
			delBST(node->r, rmin->data);
			rmin->l = node->l;
			rmin->r = node->r;
			free(node);
			return rmin;
		}else if(node->l != 0){
			printf("up left\n");
			tnode *lmax = maxBST(node->l);
			delBST(node->l, lmax->data);
			lmax->l = node->l;
			lmax->r = node->r;
			free(node);
			return lmax;
		}
		free(node);
		//l=r=0
		return 0;
	}else if(node->data < data){
		node->r = delBST(node->r, data);
		return node;
	}
	return node;
}


////////////
//AVL
//平衡:子树的深度保持一致
//因为左右子树的最大高度差为1,所以不会出现在二叉查找树中 连续插入越来越大或小的节点后,造成越来越像链表,越来越不平衡,时间复杂度逐渐变为O(N)的情况.删除也同样(取决于删除算法,在删除一个有2个child的节点后,倾向于取左节点的最大值,还是取右节点的最小值,来替代当前被删除节点的空位.
//特点是,增,删,之后需要使用旋转操作来保持平衡
//还需要每个节点存储高度信息(注意:不是深度,深度是针对整棵树而言)

/*
// descrpted
void rightRotate(tnode * node){
	tnode* l;
	l = node->l;
	if(l == 0)
		return;
	node->l = l->r;
	l->r = node;
}

void leftRotate(tnode * node){
	tnode* r;
	r = node->r;
	if(r == 0)
		return;
	node->r = r->l;
	r->l = node;
}
*/



//  k2     ->    k1
//k1               k2
tnode * rotateL(tnode * k2){
	tnode * k1;
	k1 = k2->l;
	if(k1 != 0){
		k2->l = k1->r;
		k1->r = k2;
	}
	k2->height = max(height(k2->l),height(k2->r)) + 1;
	k1->height = max(height(k1->l),height(k1->r)) + 1;
	return k1;
}

//k1    ->    k2
//  k2      k1
tnode * rotateR(tnode * k1){
	tnode * k2;
	k2 = k1->r;
	if(k2 != 0){
		k1->r = k2->l;
		k2->l = k1;
	}
	k1->height = max(height(k1->l),height(k1->r)) + 1;
	k2->height = max(height(k2->l),height(k2->r)) + 1;
	return k2;
}

//       k3  ->     k3    ->   k2
//   k1           k2         k1  k3
//     k2       k1
tnode * doubleRotateRL(tnode *k3){
	k3->l = rotateR(k3->l);
	return rotateL(k3);
}

//    k1      ->      k1    ->      k2
//        k3           k2         k1  k3
//      k2              k3
tnode * doubleRotateLR(tnode *k1){
	k1->r = rotateL(k1->r);
	return rotateR(k1);
}

int height(tnode *t){
	if(t == 0)
		return -1;
	else
		return t->height;
	
}

int max(int a, int b){
	return a>b?a:b;
}

//非惰性删除
tnode * delAVL(tnode *t,int data){
	if(t == 0)
		return 0;
	
	if(data < t->data){
		t->l = delAVL(t->l, data);
	}else if(data > t->data){
		t->r = delAVL(t->r, data);
	}else if(data == t->data){
		int hl = height(t->l);
		int hr = height(t->r);
		if(hl == hr == -1){
			t = 0;
			return 0;
		}else{
			tnode *tmp;
			if(hl > hr || hl == hr){
				tmp = maxBST(t->l);
				t->l = delAVL(t->l, tmp->data);
			}else{
				tmp = minBST(t->r);
				t->r = delAVL(t->r, tmp->data);
			}
			tmp->l = t->l;
			tmp->r = t->r;
			t = tmp;
		}
	}
	
	//rotate
	if(height(t->l) - height(t->r) == 2){
		if(t->l->l != 0)
			t = rotateL(t);
		else
			t = doubleRotateRL(t);
	}else if(height(t->r) - height(t->l) == 2){
		if(t->r->r != 0)
			t = rotateR(t);
		else
			t = doubleRotateLR(t);
	}
	
	t->height = max(height(t->l), height(t->r)) + 1;
	return t;
	//最小,最大
	//平衡
	//高度
}

tnode * insertAVL(tnode *t,int data){
	if(t == 0){
		t = malloc(sizeof(tnode));
		t->data = data;
		t->l=0;
		t->r=0;
		t->height=0;
	}else if(data < t->data){
		t->l = insertAVL(t->l,data);
		if(height(t->l) - height(t->r) == 2){
			if(data < t->l->data)
				t = rotateL(t);
			else
				t = doubleRotateRL(t);
		}
	}else if(data > t->data){
		t->r = insertAVL(t->r,data);
		if(height(t->r) - height(t->l) == 2){
			if(data > t->r->data)
				t = rotateR(t);
			else
				t = doubleRotateLR(t);
		}
	}
	
	t->height = max(height(t->l),height(t->r)) + 1;
	return t;
}


///////////
void showAll(tnode*);
void showAll(tnode* node){
	printf("\nclr\n");
	clr(node);
	printf("\nlcr\n");
	lcr(node);
	printf("\nlrc\n");
	lrc(node);
}

///////////////////////

void showTree();
void showBST();
void showAVL();

void main(){
	srand(348394);
	//bin tree's clr lcr lrc
	//showTree();
	
	//BST
	//showBST();
	
	//AVL
	showAVL();
}

void showAVL(){
	tnode * t;
	t = (tnode *)0;
	int datas[] = {8,4,2,1,9,5,10};
	int len = sizeof(datas) / sizeof(datas[0]);
	int i = 0;
	printf("1\n");
	while(i < len){
		printf("2 %d\n",i);
		t = insertAVL(t,datas[i]);
		i++;
	}
	printf("3\n");
	showAll(t);
}

void showBST(){
	int arr[] = {5,7,8,6,1,3};
	int len = sizeof(arr) / sizeof(arr[0]);
	int i = 0;
	tnode *root = malloc(sizeof(tnode));
	bzero(root,sizeof(root));
	root->data = 4;
	while(i<len){
		insertBST(root,arr[i++]);
	}
	showAll(root);

	tnode *res = searchBST(root,6);
	printf("\nsearch 6 %lx:%d\n",res,res->data);
	
	res = searchBST(root,2);
	printf("\nsearch 2 %lx\n",res);

	printf("\nrangesearch 2-6\n");
	rangeSearch(root, 2,6);

	printf("\nreverRangesearch 2-6\n");
	reverRangeSearch(root, 2,6);

	printf("\ndel node [5]\n");
	tnode * resNode = delBST(root, 5);
	printf("del FINISH\n");
	showAll(resNode);
}



void showTree(){
	tnode * r = malloc(sizeof(tnode));
	r->data = rand() % maxData;
	fillTree(r, 2);
	
	printf("\nlcr\n");
	lcr(r);
	printf("\nclr\n");	
	clr(r);
	printf("\nlrc\n");
	lrc(r);

	printf("\nisFullBinTree?\n");
	int isfbt = isFullBinTree(r);
	printf("%d\n",isfbt);

	tnode * r2 = malloc(sizeof(tnode));
	bzero(r2,sizeof(tnode));
	r2->l = malloc(sizeof(tnode));
	bzero(r2->l,sizeof(tnode));	

	printf("\nisFullBinTree?\n");
	isfbt = isFullBinTree(r2);
	printf("%d\n",isfbt);


}