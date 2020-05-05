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


// key value�� 0���� ������� ��.
int key_value = 0;
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
    lab2_node* now_node = tree->root;
    lab2_node* p_replacing, * replacing;
    lab2_node* parent_node = NULL;
    lab2_node* key_node;


    typedef enum side_is {
        UNKNOWN,
        LEFT,
        RIGHT
    } sideis; // � ��尡 �θ��� ����������, �������� ���ϱ�

    sideis nodeSide_is = UNKNOWN;

    // Ű�� �������ִ� ��� ã��: �ݺ��� ������ nownode�� �Է�Ű�� ����Ű �����°�.
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return;
        }

        // ã�°� �� ũ�ٸ� ���������� �̵�
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nodeSide_is = RIGHT;
        }
        // ã�� ���� �� �۴ٸ� �������� �̵�
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nodeSide_is = LEFT;
        }
    }
    key_node = now_node;
    // nodeSide_is�� UNKNOWN�̶�� ���� ã�� ��尡 Ʈ���� ��Ʈ��� �� �ǹ�. ���� ����� ��� �����ϰ����!!

    // now�� �ڽ��� ����..
    if (key_node->left == NULL && key_node->right == NULL) {
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

    // now�� �ڽ��� �ϳ� �ִ�.
    else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nodeSide_is) {
        case LEFT:
            // now�� ���� �ڽ��� �ִ°��̴�
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now�� ������ �ڽ��� �ִ°��̴�
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT:
            // now�� ���� �ڽ��� �ִ°��̴�
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now�� ������ �ڽ��� �ִ°��̴�
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree�� ��Ʈ�� �����ؾ��ϹǷ� tree->root�� ���� ���� ����Ű�� �ؾ���.
             // now�� ���� �ڽ��� �ִ°��̴�
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // now�� ������ �ڽ��� �ִ°��̴�
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }

    }
    // now�� �ڽ��� �� �� �ִ�
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // now�� ������ ���� ������ ���
        lab2_node* parent_terminal = NULL; // rightTerminal_node�� �θ�.

        // now�� �����ڽ��� ���� ������ ��� Ž��
        rightTerminal_node = now_node->left;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // �̺κ��� �� �򰥸��� ��..
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

    // nownode�� �����ϰ� ����ֱ�!
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


