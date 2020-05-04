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

// ���ؽ� �ʱ�ȭ
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// ���� �ڿ�: shared tree
lab2_tree* shared_tree = lab2_tree_create();

// key value�� 0���� ������� ��.
int key_value = 0;
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *
 *  @param lab2_tree *tree  : bst to print in-order.
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree* tree) {
    // You need to implement lab2_node_print_inorder function.  
    if (tree->root != NULL) {
        lab2_tree* subtree = (lab2_tree*)malloc(sizeof(lab2_tree));
        *subtree = *tree;
        if (subtree->root->left != NULL) {
            subtree->root = tree->root->left;
            lab2_node_print_inorder(subtree);
        }
        printf("The key is %d\n", tree->root->key);
        *subtree = *tree;
        if (subtree->root->right != NULL) {
            subtree->root = tree->root->right;
            lab2_node_print_inorder(subtree);
        }
        free(subtree);
        return 0;
    }
    else
        return 1;
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
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert.
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree* tree, lab2_node* new_node) {
    // You need to implement lab2_node_insert_fg function.
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
    lab2_node* deletenode;
    lab2_node* nownode = tree->root;
    lab2_node* p_replacing, * replacing;
    while (1) {
        // This node deletes itself.
        if (nownode->key == key) {
            lab2_node_delete(tree,nownode);
            return 0;
        }

        // Didn't find the node to delete yet.
        // The key node is in rightside because the key is bigger than a key of nownode.
        else if (key > nownode->key) {
            nownode = nownode->right;
        }

        // The key node is in leftside because the key is smaller than a key of nownode.
        else if (key < nownode->key) {
            nownode = nownode->left;
        }

        // Anything matches the key.
        if (nownode == NULL) {
            printf("Any node matches the key.\n");
            return -1;
        }
    }
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
        lab2_node_delete(tree, tree->root);
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
void lab2_node_delete(lab2_tree* tree, lab2_node* node) {
    if (node == NULL) {
        printf("[Error]Impossible to delete NULL from tree..\n");
        return;
    }
    typedef enum side_is {
        UNKNOWN,
        LEFT,
        RIGHT
    } side_is; 
    
    lab2_node* parent_node;
    lab2_node* now_node;
    side_is nodeSide_is = UNKNOWN;
    
    
    parent_node = NULL;
    now_node = tree->root;
    while (node != now_node) {
    
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return;
        }
    
        if (node->key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nodeSide_is = RIGHT;
        }
    
        else if (node->key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nodeSide_is = LEFT;
        }
    }

   
    if (node->left == NULL && node->right == NULL) {
        switch (nodeSide_is) {
        case LEFT:
            parent_node->left = NULL;
            break;
        case RIGHT:
            parent_node->right = NULL;
            break;
        case UNKNOWN:
            tree->root = NULL;
            break;
        }
    }
	
    else if (node->left == NULL || node->right == NULL) {
        switch (nodeSide_is) {
        case LEFT:
    
            if (node->left != NULL) {
                parent_node->left = now_node->left;
            }
    
            else if (node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;
        
        case RIGHT:
    
            if (node->left != NULL) {
                parent_node->right = now_node->left;
            }
    
            else if (node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
    
            if (node->left != NULL) {
                tree->root = now_node->left;
            }
    
            else if (node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
        
    }

    
    else if (node->left != NULL && node->right != NULL) {
        lab2_node* rightTerminal_node; 
        lab2_node* parent_terminal = NULL; 

        
        rightTerminal_node = now_node->left;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        
        switch (nodeSide_is) {
        case LEFT:
            if (parent_terminal != NULL)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;

            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT:
            if (parent_terminal != NULL)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;

            parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
            if (parent_terminal != NULL)
                parent_terminal->right = NULL;
            else
                now_node->left = NULL;
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;
        }
    }

    free(now_node);
    now_node = NULL;

}


/*
* ���� Ʈ���� ����. ���� ������� ���ڷ� �޾�. �����ؼ� �����ϴ� ���� CS
*/
void access_sharedtree(thread_arg* thrd) {
    //������ ����ü �ޱ�
    thread_arg* th_arg = (thread_arg*)arg;

    // �ݺ� Ƚ���� ���� �������� �ݺ� Ƚ����
    int num_iterations = th_arg->num_iterations;

    // ���� ������ ����ȭ �Ѹ� 1. ��������̸� 0
    
    int is_sync = th_arg->is_sync;

    

    typedef enum operation {
        UNKNOWN,
        INSERT,
        REMOVE
    } operation;

    int i = 0;
    operation op_mode = UNKNOWN; 

    // �����尣 ũ��Ƽ�� ���� ���� ���� ������� �ϴ� ���: ���� �̿���
    if (is_sync) {
        
        // �������� �ݺ� Ƚ����ŭ ����
        for (i = 0; i < num_iterations; i++) {
            // ���� ���� ���� �����ֱ�
            if (i < num_iterations / 2)
                op_mode = INSERT;
            else
                op_mode = REMOVE;

            // ����ġ�� �ȿ� mutex�� �ִ°� ������...?�������鿡�� �񱳸� ���ϴϱ� �ɱ� �� ������ ������ ���� �ѵ�....
            switch (op_mode) {
            case INSERT: // Ʈ���� ���ϱ�
                pthread_mutex_lock(&mutex);
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                pthread_mutex_unlock(&mutex);
                break;
            case REMOVE: // Ʈ������ ���� -> ���� Ű �� ������ �� �ƹ� �̻� ���� ����..�̰�� �� search��.
                pthread_mutex_lock(&mutex);
                lab2_node_remove(shared_tree, rand() % key_value);
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
    }
    else { // �����尡 �ϳ��� �����Ϸ��� �Ѵٸ�: ���� �ʿ� ����
        // �������� �ݺ� Ƚ����ŭ ����
        for (i = 0; i < num_iterations; i++) {
            // ���� ���� ���� �����ֱ�
            if (i < num_iterations / 2)
                op_mode = INSERT;
            else
                op_mode = REMOVE;

            // ����ġ�� �ȿ� mutex�� �ִ°� ������...?�������鿡�� �񱳸� ���ϴϱ� �ɱ� �� ������ ������ ���� �ѵ�....
            switch (op_mode) {
            case INSERT: // Ʈ���� ���ϱ�  
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                break;
            case REMOVE: // Ʈ������ ���� -> ���� Ű �� ������ �� �ƹ� �̻� ���� ����..�̰�� �� search��.
                lab2_node_remove(shared_tree, rand() % key_value);    
                break;
            }
        }
    }
}

// �� �����غ���: ������ � �������, �ϳ��� �����忡�� ������ ��� �ݺ��Ұ���, ������� CS�� ���ÿ� ������ �� �ִ��� �Ű������� ����
int mutex_test(int num_threads, int num_iterations, int is_sync) {
    pthread_t* pthread = NULL;
    int i;
    long double result = 0.0;
    thread_arg arg;
    arg.is_sync = is_sync;
    arg.num_iterations = num_iterations;

    pthreads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    memeset(pthreads, 0x0, sizeof(pthread_t) * num_threads);

    for (i = 0; i < num_threads; i++) {
        pthread_create(&pthreads[i], NULL, access_sharedtree, &arg); // ���� �ڵ�� ������ ���� ���̵�� �ѱ�� �Լ� ������ �ٽ� ����ȯ �ϴµ� ����..?
    }

    // ���ϴ� ���� ��ŭ�� �����带 ������༭ ���� ���� ����� ��.
    // ó�� ������� ��������� ������� �����ϰ� ��.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }

    // ������ ����, �ݺ� Ƚ��, ������¿����� �ƴϾ����� �����ؼ� �������
    print_result(num_threads, num_iterations, is_sync);

    return 0; // ���� ���� �ʿ��Ѱ�...�ʹ�...

}

