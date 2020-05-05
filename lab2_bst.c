/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id :
*	    Student name :
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"



/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *
 *  @param lab2_tree *tree  : bst to print in-order.
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree* tree, int num) {
    // You need to implement lab2_node_print_inorder function.
    if (tree->root != NULL) {
        lab2_tree* subtree = (lab2_tree*)malloc(sizeof(lab2_tree));
        *subtree = *tree;
        if (subtree->root->left != NULL) {
            subtree->root = tree->root->left;
            num=lab2_node_print_inorder(subtree, num);
        }
        //printf("The key is %d\n", tree->root->key);
        num++;
        *subtree = *tree;
        int num2 = 0;
        if (subtree->root->right != NULL) {
            subtree->root = tree->root->right;
            num2=lab2_node_print_inorder(subtree, num2);
        }
        free(subtree);
        return num+num2;
    }
    else
        return num;
}

/*
* Create empty Binary Search Tree.
* Return the created Binary Search Tree.
*/
lab2_tree* lab2_tree_create() {
    lab2_tree* tree = (lab2_tree*)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    return tree;
}


/*
* Create a new node and set a key value as given argument.
* Return the created BST node.
*/
lab2_node* lab2_node_create(int key) {
    lab2_node* node = (lab2_node*)malloc(sizeof(lab2_node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
* Insert a node to a Binary Search Tree.
* If it succeeds to insert a node, it returns 0.
* If it fails to insert a node, it returns -1.
*/
int lab2_node_insert(lab2_tree* tree, lab2_node* new_node) {
    if (tree == NULL || new_node == NULL){
	    return -1;
    }
    
    lab2_node* now = tree->root;
    // If the tree is empty, new_node becomes root of the tree.
    if (now == NULL) {
        tree->root = new_node;
        return 0;
    }
    while (1) {
        if (new_node->key < now->key) {
            if (now->left) {
                now = now->left;
            }
            else {
                now->left = new_node;
                break;
            }
        }
        else {
            if (now->right) {
                now = now->right;
            }
            else {
                now->right = new_node;
                break;
            }
        }
    }
    return 0;
}

/*
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert.
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree* tree, lab2_node* new_node) {
    // You need to implement lab2_node_insert_fg function.
    lab2_node* now = tree->root;

    // If the tree is empty, new_node becomes root of the tree.
    if (now == NULL) {
        tree->root = new_node;
        return 0;
    }
    while (1) {
        // It isn't possible to insert node that has duplicate key value.
        if (new_node->key == now->key) {
            printf("Duplicate key value...\n");
            return -1;
        }

        if (new_node->key < now->key) {
            if (now->left) {
                now = now->left;
            }
            else {
                now->left = new_node;
                break;
            }
        }
        else {
            if (now->right) {
                now = now->right;
            }
            else {
                now->right = new_node;
                break;
            }
        }
    }
    return 0;
}

/*
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert.
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree* tree, lab2_node* new_node) {
    // You need to implement lab2_node_insert_cg function.
}

/*
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete.
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree* tree, int key) {
    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; // 어떤 노드가 부모의 오른쪽인지, 왼쪽인지 정하기

    lab2_node* now_node = tree->root; // 방문하는 노드
    lab2_node* parent_node = NULL; // 방문하는 노드의 부모 노드
    lab2_node* key_node; // 삭제할 노드

    sideis nownode_is = UNKNOWN;

    // 키값 가지고있는 노드 찾기: 반복문 나가면 nownode는 입력키와 같은키 가지는거.
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_FAIL;
        }

        // 찾는게 더 크다면 오른쪽으로 이동
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        // 찾는 값이 더 작다면 왼쪽으로 이동
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }
    // nodeSide_is가 UNKNOWN이라는 것은 찾는 노드가 트리의 루트라는 것 의미. 가장 꼭대기 노드 삭제하고싶음!!

    // now는 자식이 없다..
    if (now_node->left == NULL && now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            parent_node->left = NULL;
            break;
        case RIGHT_CHILD:
            parent_node->right = NULL;
            break;
        case UNKNOWN:
            tree->root = NULL;
            break;
        }
    }

    // now는 자식이 하나 있다.
    else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree의 루트를 삭제해야하므로 tree->root를 다음 노드로 가리키게 해야함.
             // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
    }
    // now는 자식이 둘 다 있다
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // now의 왼쪽의 가장 오른쪽 노드
        lab2_node* parent_terminal; // rightTerminal_node의 부모.

        // now의 왼쪽자식의 가장 오른쪽 노드 탐색
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // 이부분이 좀 헷갈리긴 해..
        switch (nownode_is) {
        case LEFT_CHILD:
            // 말단 노드와 그 부모 노드를 연결 끊는다.
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;

            // 말단노드의 왼쪽자식은 있을경우 그거 옮겨줌.
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;

            // 말단 노드를 이동한다.

            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT_CHILD:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;

            break;
        }

    }
    // nownode를 해제하고 비워주기!
    lab2_node_delete(now_node);
}


/*
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete.
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree* tree, int key) {
    // You need to implement lab2_node_remove_fg function.
     // You need to implement lab2_node_remove_cg function.
    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; // 어떤 노드가 부모의 오른쪽인지, 왼쪽인지 정하기

   
    lab2_node* now_node = tree->root; // 방문하는 노드
    lab2_node* parent_node = NULL; // 방문하는 노드의 부모 노드
    lab2_node* key_node; // 삭제할 노드

    sideis nownode_is = UNKNOWN;

    // 키값 가지고있는 노드 찾기: 반복문 나가면 nownode는 입력키와 같은키 가지는거.
    // 락 걸고 찾고 걸고 찾고....
    pthread_mutex_lock(&now_node->mutex));
    if(parent->node !=NULL)
        pthread_mutex_lock(&parent_node->mutex));
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_FAIL;
        }

        // 찾는게 더 크다면 오른쪽으로 이동
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        // 찾는 값이 더 작다면 왼쪽으로 이동
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }

    

    // nodeSide_is가 UNKNOWN이라는 것은 찾는 노드가 트리의 루트라는 것 의미. 가장 꼭대기 노드 삭제하고싶음!!

    // now는 자식이 없다..
    if (now_node->left == NULL && now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            parent_node->left = NULL;
            break;
        case RIGHT_CHILD:
            parent_node->right = NULL;
            break;
        case UNKNOWN:
            tree->root = NULL;
            break;
        }
    }

    // now는 자식이 하나 있다.
    else if (now_node->left == NULL || now_node->right == NULL) {
        int has_leftchild; // 1이면 왼쪽 자식 갖고있는거임
        // 있는 자식에 락부터 걸어주고..
        if (now_node->left != NULL) {
            pthread_mutex_lock(&now_node->left->mutex));
            has_leftchild = 1;
        }
        else {
            pthread_mutex_lock(&now_node->right->mutex));
            has_leftchild = 0;
        }

        switch (nownode_is) {
        case LEFT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree의 루트를 삭제해야하므로 tree->root를 다음 노드로 가리키게 해야함.
             // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }

        // 다 끝났으니까 락 풀어주고
        if(has_leftchild)
            pthread_mutex_unlock(&now_node->left->mutex));
        else
            pthread_mutex_lock(&now_node->right->mutex));
    }
    // now는 자식이 둘 다 있다
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // now의 왼쪽의 가장 오른쪽 노드
        lab2_node* parent_terminal; // rightTerminal_node의 부모.

        // now의 왼쪽자식의 가장 오른쪽 노드 탐색
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // 말단 노드와 그 부모 노드 락 걸기
        pthread_mutex_lock(&rightTerminal_node->mutex)); // 락을 하고 검사해야할까?
        pthread_mutex_lock(&parent_terminal_node->mutex));

        // 이부분이 좀 헷갈리긴 해..
        switch (nownode_is) {
        case LEFT_CHILD:
            // 말단 노드와 그 부모 노드를 연결 끊는다.
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;

            // 말단노드의 왼쪽자식은 있을경우 그거 옮겨줌.
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;

            // 말단 노드를 이동한다.

            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT_CHILD:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;

            break;
        }
        // 말단 노드와 그 부모 노드 락 풀기
        pthread_mutex_unlock(&rightTerminal_node->mutex)); // 락을 하고 검사해야할까?
        pthread_mutex_unlock(&parent_terminal_node->mutex));
    }
    
    // nownode를 해제하고 비워주기!
    lab2_node_delete(now_node);

    // 락 풀기
    // pthread_mutex_lock(&now_node->mutex)); // 이미 없어진 노드에 락을 푸는게 무슨 의미야?
    pthread_mutex_unlock(&parent_node->mutex));
}


/*
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete.
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree* tree, int key) {
    // You need to implement lab2_node_remove_cg function.
    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; // 어떤 노드가 부모의 오른쪽인지, 왼쪽인지 정하기

    // 락 잡기
    pthread_mutex_lock(&(tree->root->mutex));
    lab2_node* now_node = tree->root; // 방문하는 노드
    lab2_node* parent_node = NULL; // 방문하는 노드의 부모 노드
    lab2_node* key_node; // 삭제할 노드

    sideis nownode_is = UNKNOWN;

    // 키값 가지고있는 노드 찾기: 반복문 나가면 nownode는 입력키와 같은키 가지는거.
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_FAIL;
        }

        // 찾는게 더 크다면 오른쪽으로 이동
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        // 찾는 값이 더 작다면 왼쪽으로 이동
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }
    // nodeSide_is가 UNKNOWN이라는 것은 찾는 노드가 트리의 루트라는 것 의미. 가장 꼭대기 노드 삭제하고싶음!!

    // now는 자식이 없다..
    if (now_node->left == NULL && now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            parent_node->left = NULL;
            break;
        case RIGHT_CHILD:
            parent_node->right = NULL;
            break;
        case UNKNOWN:
            tree->root = NULL;
            break;
        }
    }

    // now는 자식이 하나 있다.
    else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree의 루트를 삭제해야하므로 tree->root를 다음 노드로 가리키게 해야함.
             // now의 왼쪽 자식이 있는것이다
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // now의 오른쪽 자식이 있는것이다
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
    }
    // now는 자식이 둘 다 있다
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // now의 왼쪽의 가장 오른쪽 노드
        lab2_node* parent_terminal; // rightTerminal_node의 부모.

        // now의 왼쪽자식의 가장 오른쪽 노드 탐색
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // 이부분이 좀 헷갈리긴 해..
        switch (nownode_is) {
        case LEFT_CHILD:
            // 말단 노드와 그 부모 노드를 연결 끊는다.
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            
            // 말단노드의 왼쪽자식은 있을경우 그거 옮겨줌.
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;

            // 말단 노드를 이동한다.
            
            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT_CHILD:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
            if (parent_terminal != now_node)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            if (rightTerminal_node->left != NULL)
                parent_terminal->right = rightTerminal_node->left;
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;

            break;
        }

    }
    pthread_mutex_unlock(&(tree->root->mutex));
    // nownode를 해제하고 비워주기!
    lab2_node_delete(now_node);
}


/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete.
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree* tree) {

    if (tree->root != NULL) {
        int key = tree->root->key;
        lab2_node_remove(tree, key);
        lab2_tree_delete(tree);
    }
    else
        return;
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove.
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node* node) {
    free(node);
    node = NULL;
}



