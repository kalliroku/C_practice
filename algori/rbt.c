/*
1. 모든 노드는 빨간색 또는 검은색
2. 루트 노드는 검은색
3. 리프 노드는 검은색
4. 빨간 노드의 자식들은 모두 검은색
5. 루트 노드와 모든 리프 노드 사이에 있는 검은 노드의 수는 전부 동일
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

#ifndef RBTree
#define NULL 0
#define NIL (&nil_RBT)
#define RBTree 1

typedef int ElementType;

typedef struct RBTNode {
	struct RBTNode* parent;
	struct RBTNode* left;
	struct RBTNode* right;

	int color; //0 : red, 1 : black
	int count;

	ElementType data;
} RBTNode;

RBTNode nil_RBT;

RBTNode* RBT_CreateNode(ElementType);
void RBT_Destroy(RBTNode*);
RBTNode* RBT_SearchNode(RBTNode*, ElementType);
RBTNode* RBT_SearchMin(RBTNode*);
RBTNode* RBT_SearchMax(RBTNode*);
void RBT_Print(RBTNode*, int, int);
void RBT_RotateL(RBTNode**, RBTNode*);
void RBT_RotateR(RBTNode**, RBTNode*);
void RBT_InsertFind(RBTNode**, RBTNode*);
void RBT_InsertAfter(RBTNode**, RBTNode*);
void RBT_Insert(RBTNode**, RBTNode*);
void RBT_Print(RBTNode*, int, int);
RBTNode* RBT_RemoveNode(RBTNode**, ElementType);
void RBT_RemoveAfter(RBTNode**, RBTNode*);
#endif

RBTNode* RBT_CreateNode(ElementType newdata) {
	//newdata를 받아 새로운 노드 생성(검은색, 모든 링크는 NULL)
	RBTNode* newnode = (RBTNode*)calloc(1, sizeof(RBTNode));
	newnode->data = newdata;
	newnode->color = 1;
	newnode->count = 1;

	return newnode;
}

void RBT_Destroy(RBTNode* tree) {
	//트리 전체를 삭제
	if (tree->left != NIL) RBT_Destroy(tree->left);
	if (tree->right != NIL) RBT_Destroy(tree->right);
	tree->left = NIL;
	tree->right = NIL;
	free(tree);
}

RBTNode* RBT_SearchNode(RBTNode* tree, ElementType target) {
	if (tree == NIL || tree == NULL) return NULL;
	if (tree->data > target) return RBT_SearchNode(tree->left, target);
	else if (tree->data < target) return RBT_SearchNode(tree->right, target);
	else return tree;
}

RBTNode* RBT_SearchMin(RBTNode* tree) {
	if (tree == NIL) return NIL;
	if (tree->left == NIL) return tree;
	else return RBT_SearchMin(tree->left);
}

RBTNode* RBT_SearchMax(RBTNode* tree) {
	if (tree == NIL) return NIL;
	if (tree->right == NIL) return tree;
	else return RBT_SearchMax(tree->right);
}

void RBT_Print(RBTNode* node, int depth, int blackcount) {
	//트리를 출력함
	char c = 'T';
	int v = -1;
	if (node == NULL || node == NIL) return;
	if (node->color) blackcount++;
	if (node->parent != NULL) {
		v = node->parent->data;
		if (node->parent->left == node) c = 'L';
		else c = 'R';
	}
	for (int i = 0; i < depth; i++) printf(" ");
	printf("%d %s [%c, %d] ", node->data, node->color ? "BLACK" : "RED", c, v);
	if (node->left == NIL && node->right == NIL) printf("---------- %d\n", blackcount);
	else printf("\n");
	RBT_Print(node->left, depth + 1, blackcount);
	RBT_Print(node->right, depth + 1, blackcount);
}

void RBT_RotateL(RBTNode** root, RBTNode* parent) {
	//parent의 왼쪽 노드를 중심으로 트리를 회전시킴
	RBTNode* rchild = parent->right;
	parent->right = rchild->left;

	if (rchild->left != NIL) {
		rchild->left->parent = parent;
	}
	rchild->parent = parent->parent;

	if (parent->parent == NULL) (*root) = rchild;
	else {
		if (parent == parent->parent->right) parent->parent->right = rchild;
		else parent->parent->left = rchild;
	}
	rchild->left = parent;
	parent->parent = rchild;
}

void RBT_RotateR(RBTNode** root, RBTNode* parent) {
	//parent의 오른쪽 노드를 중심으로 트리를 회전시킴
	RBTNode* lchild = parent->left;
	parent->left = lchild->right;

	if (lchild->right != NIL) {
		lchild->right->parent = parent;
	}
	lchild->parent = parent->parent;

	if (parent->parent == NULL) (*root) = lchild;
	else {
		if (parent == parent->parent->left) parent->parent->left = lchild;
		else parent->parent->right = lchild;
	}
	lchild->right = parent;
	parent->parent = lchild;
}

void RBT_InsertFind(RBTNode** tree, RBTNode* newnode) {
	//새로 삽입할 노드가 들어갈 위치를 찾아 삽입함
	if (*tree == NULL) *tree = newnode;
	if ((*tree)->data < newnode->data) {
		if ((*tree)->right == NIL) {
			(*tree)->right = newnode;
			newnode->parent = *tree;
		}
		else RBT_InsertFind(&(*tree)->right, newnode);
	}
	else if ((*tree)->data > newnode->data) {
		if ((*tree)->left == NIL) {
			(*tree)->left = newnode;
			newnode->parent = *tree;
		}
		else RBT_InsertFind(&(*tree)->left, newnode);
	}
}

void RBT_InsertAfter(RBTNode** root, RBTNode* x) {
	//트리에 노드를 삽입하며 무너진 규칙을 회복함
	while (x != *root && x->parent->color == 0) {
		if (x->parent == x->parent->parent->left) {
			//부모 노드가 왼쪽인 경우
			RBTNode* uncle = x->parent->parent->right; //삼촌 노드
			if (uncle->color == 0) {
				//삼촌 노드가 빨간색인 경우
				x->parent->color = 1;
				uncle->color = 1;
				x->parent->parent->color = 0;

				x = x->parent->parent;
			}
			else {
				//삼촌 노드가 검은색인 경우
				if (x == x->parent->right) {
					x = x->parent;
					RBT_RotateL(root, x);
				}
				x->parent->color = 1;
				x->parent->parent->color = 0;
				RBT_RotateR(root, x->parent->parent);
			}
		}
		else {
			//부모 노드가 오른쪽인 경우
			RBTNode* uncle = x->parent->parent->left; //삼촌 노드
			if (uncle->color == 0) {
				//삼촌 노드가 빨간색인 경우
				x->parent->color = 1;
				uncle->color = 1;
				x->parent->parent->color = 0;

				x = x->parent->parent;
			}
			else {
				//삼촌 노드가 검은색인 경우
				if (x == x->parent->left) {
					x = x->parent;
					RBT_RotateR(root, x);
				}
				x->parent->color = 1;
				x->parent->parent->color = 0;
				RBT_RotateL(root, x->parent->parent);
			}
		}
	}

	(*root)->color = 1;
}

void RBT_Insert(RBTNode** tree, RBTNode* newnode) {
	//새로 생성된 노드를 받은 다음 트리에 삽입함
	RBTNode* node = RBT_SearchNode(*tree, newnode->data);
	if (node != NULL) {
		node->count++;
		return;
	}
	RBT_InsertFind(tree, newnode);
	newnode->color = 0;
	newnode->left = NIL;
	newnode->right = NIL;
	RBT_InsertAfter(tree, newnode);
}

RBTNode* RBT_RemoveNode(RBTNode** root, ElementType data) {
	RBTNode* removed = NULL;
	RBTNode* successor = NULL;
	RBTNode* target = RBT_SearchNode((*root), data);
	
	if (target == NULL) return NULL;
	if (target->count > 1) {
		(target->count)--;
		return NULL;
	}
	if (target->left == NIL || target->right == NIL) removed = target;
	else {
		removed = RBT_SearchMin(target->right);
		target->data = removed->data;
	}

	if (removed->left != NIL) successor = removed->left;
	else successor = removed->right;

	successor->parent = removed->parent;
	if (removed->parent == NULL) (*root) = successor;
	else {
		if (removed == removed->parent->left) removed->parent->left = successor;
		else removed->parent->right = successor;
	}

	if (removed->color) RBT_RemoveAfter(root, successor);

	return removed;
}

void RBT_RemoveAfter(RBTNode** root, RBTNode* successor) {
	RBTNode* sibling = NULL;

	while (successor->parent != NULL && successor->color) {
		if (successor == successor->parent->left) {
			sibling = successor->parent->right;

			if (sibling->color == 0) {
				sibling->color = 1;
				successor->parent->color = 0;
				RBT_RotateL(root, successor->parent);
			}
			else {
				if (sibling->left->color && sibling->right->color) {
					sibling->color = 0;
					successor = successor->parent;
				}
				else {
					if (sibling->left->color == 0) {
						sibling->left->color = 1;
						sibling->color = 0;

						RBT_RotateR(root, sibling);
						sibling = successor->parent->right;
					}
					sibling->color = successor->parent->color;
					successor->parent->color = 1;
					sibling->right->color = 1;
					RBT_RotateL(root, successor->parent);
					successor = *root;
				}
			}
		}
		else {
			sibling = successor->parent->left;

			if (sibling->color == 0) {
				sibling->color = 1;
				successor->parent->color = 0;
				RBT_RotateR(root, successor->parent);
			}
			else {
				if (sibling->left->color && sibling->right->color) {
					sibling->color = 0;
					successor = successor->parent;
				}
				else {
					if (sibling->right->color == 0) {
						sibling->right->color = 1;
						sibling->color = 0;

						RBT_RotateL(root, sibling);
						sibling = successor->parent->left;
					}
					sibling->color = successor->parent->color;
					successor->parent->color = 1;
					sibling->left->color = 1;
					RBT_RotateR(root, successor->parent);
					successor = *root;
				}
			}
		}
	}

	successor->color = 1;
}

int main(void) {
    int t, k, n;
    char o;
    nil_RBT.color = 1;
    scanf("%d", &t);
    for (int tt = 0; tt < t; tt++) {
        RBTNode* root = NULL;
        RBTNode* node = NULL;
        scanf("%d", &k);
        for (int i = 0; i < k; i++) {
            scanf(" %c %d", &o, &n);
            if (o == 'I') {
                RBT_Insert(&root, RBT_CreateNode(n));
            }
            else if (n < 0) {
                if (root == NULL) continue;
                node = RBT_RemoveNode(&root, RBT_SearchMin(root)->data);
                if (node) free(node);
            }
            else {
                if (root == NULL) continue;
                node = RBT_RemoveNode(&root, RBT_SearchMax(root)->data);
                if (node) free(node);
            }
            if (root == NIL) root = NULL;
        }
        if (root == NULL) printf("EMPTY\n");
        else {
            printf("%d %d\n", RBT_SearchMax(root)->data, RBT_SearchMin(root)->data);
        }
        if (root != NULL) RBT_Destroy(root);
    }
    return 0;
}
