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
    pthread_mutex_init(&tree->lock, NULL);
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
    pthread_mutex_init(&node->mutex, NULL);
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
	//if (new_node->key == now->key){
		//printf("duplicate key..\n");
	//	return -1;
	//}
        if (new_node->key <= now->key) {
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
    if (tree == NULL || new_node == NULL){
            return -1;
    }

    lab2_node* now = tree->root;
 // If the tree is empty, new_node becomes root of the tree.
    if (now == NULL) {
	pthread_mutex_lock(&tree->lock);
        tree->root = new_node;

	pthread_mutex_unlock(&tree->lock);
	return 0;
    }
    while (1) {
        if (new_node->key <= now->key) {
            if (now->left) {
                now = now->left;
            }
            else {
		pthread_mutex_lock(&now->mutex);
                now->left = new_node;
                pthread_mutex_unlock(&now->mutex); 
                break;
            }
        }
        else {
            if (now->right) {
                now = now->right;
            }
            else {
		pthread_mutex_lock(&now->mutex);
                now->right = new_node;
		pthread_mutex_unlock(&now->mutex);
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
    if (tree == NULL || new_node == NULL){
            return -1;
    }

    lab2_node* now = tree->root;
    // If the tree is empty, new_node becomes root of the tree.
    pthread_mutex_lock(&tree->lock);
    if (now == NULL) {
        tree->root = new_node;
    }
    else {
        while (1) {
            if (new_node->key <= now->key) {
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
    }
    pthread_mutex_unlock(&tree->lock);
    return 0;
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
    } sideis; // Ÿî¶² ³ëµå°¡ ºÎžðÀÇ ¿Àž¥ÂÊÀÎÁö, ¿ÞÂÊÀÎÁö Á€ÇÏ±â

    lab2_node* now_node = tree->root; // ¹æ¹®ÇÏŽÂ ³ëµå
    lab2_node* parent_node = NULL; // ¹æ¹®ÇÏŽÂ ³ëµåÀÇ ºÎžð ³ëµå
    lab2_node* key_node; // »èÁŠÇÒ ³ëµå

    sideis nownode_is = UNKNOWN;

    // Å°°ª °¡Áö°íÀÖŽÂ ³ëµå Ã£±â: ¹Ýº¹¹® ³ª°¡žé nownodeŽÂ ÀÔ·ÂÅ°¿Í °°ÀºÅ° °¡ÁöŽÂ°Å.
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_ERROR;
        }

        // Ã£ŽÂ°Ô Žõ Å©ŽÙžé ¿Àž¥ÂÊÀž·Î ÀÌµ¿
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        // Ã£ŽÂ °ªÀÌ Žõ ÀÛŽÙžé ¿ÞÂÊÀž·Î ÀÌµ¿
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }
    // nodeSide_is°¡ UNKNOWNÀÌ¶óŽÂ °ÍÀº Ã£ŽÂ ³ëµå°¡ Æ®ž®ÀÇ ·çÆ®¶óŽÂ °Í ÀÇ¹Ì. °¡Àå ²ÀŽë±â ³ëµå »èÁŠÇÏ°íœÍÀœ!!

    // nowŽÂ ÀÚœÄÀÌ ŸøŽÙ..
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

    // nowŽÂ ÀÚœÄÀÌ ÇÏ³ª ÀÖŽÙ.
    else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            // nowÀÇ ¿ÞÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // nowÀÇ ¿Àž¥ÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            // nowÀÇ ¿ÞÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // nowÀÇ ¿Àž¥ÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // treeÀÇ ·çÆ®žŠ »èÁŠÇØŸßÇÏ¹Ç·Î tree->rootžŠ ŽÙÀœ ³ëµå·Î °¡ž®Å°°Ô ÇØŸßÇÔ.
             // nowÀÇ ¿ÞÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // nowÀÇ ¿Àž¥ÂÊ ÀÚœÄÀÌ ÀÖŽÂ°ÍÀÌŽÙ
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
    }
    // nowŽÂ ÀÚœÄÀÌ µÑ ŽÙ ÀÖŽÙ
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // nowÀÇ ¿ÞÂÊÀÇ °¡Àå ¿Àž¥ÂÊ ³ëµå
        lab2_node* parent_terminal; // rightTerminal_nodeÀÇ ºÎžð.

        // nowÀÇ ¿ÞÂÊÀÚœÄÀÇ °¡Àå ¿Àž¥ÂÊ ³ëµå Åœ»ö
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // ÀÌºÎºÐÀÌ Á» Çò°¥ž®±ä ÇØ..
        switch (nownode_is) {
        case LEFT_CHILD:
            // ž»ŽÜ ³ëµå¿Í ±× ºÎžð ³ëµåžŠ ¿¬°á ²÷ŽÂŽÙ.
            if (parent_terminal != now_node) {
                parent_terminal->right = NULL;

                if (rightTerminal_node->left != NULL) {
                    parent_terminal->right = rightTerminal_node->left;
                }
            } 
            else {
                now_node->left = NULL;
                if (rightTerminal_node->left != NULL) {
                    parent_terminal->left = rightTerminal_node->left;
                }
            }    
            // ž»ŽÜ ³ëµåžŠ ÀÌµ¿ÇÑŽÙ.

            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT_CHILD:
            if (parent_terminal != now_node) {
                parent_terminal->right = NULL;

                if (rightTerminal_node->left != NULL) {
                    parent_terminal->right = rightTerminal_node->left;
                }
            }
            else {
                now_node->left = NULL;
                if (rightTerminal_node->left != NULL) {
                    parent_terminal->left = rightTerminal_node->left;
                }
            }
            
	    parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
            if (parent_terminal != now_node) {
                parent_terminal->right = NULL;

                if (rightTerminal_node->left != NULL) {
                    parent_terminal->right = rightTerminal_node->left;
                }
            }
            else {
                now_node->left = NULL;
                if (rightTerminal_node->left != NULL) {
                    parent_terminal->left = rightTerminal_node->left;
                }
            }

	    tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;

            break;
        }

    }
    // nownodežŠ ÇØÁŠÇÏ°í ºñ¿öÁÖ±â!
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
        lab2_node_remove(tree, tree->root->key);
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


