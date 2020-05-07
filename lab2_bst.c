/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32182609, 32184160
*	    Student name : YANG HYEEUN, CHONG YUNA
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
 * It visits all nodes of BST in in-order.
 * It returns the total number of nodes of BST. 
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
* This function creates empty Binary Search Tree.
* Return the created Binary Search Tree.
*/
lab2_tree* lab2_tree_create() {
    lab2_tree* tree = (lab2_tree*)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    pthread_mutex_init(&tree->lock, NULL);
    return tree;
}


/*
* This function creates a new node and set a key value as given argument.
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
* This function inserts a node to a Binary Search Tree.
* If it succeeds to insert a node, it returns 0.
* If it fails to insert a node, it returns -1.
*/
int lab2_node_insert(lab2_tree* tree, lab2_node* new_node) {
    // It's unable to insert a node to the tree.
    if (tree == NULL || new_node == NULL){
	    return LAB2_ERROR;
    }
    
    lab2_node* now = tree->root;
    // If the tree is empty, new_node becomes root of the tree.
    if (now == NULL) {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (1) {
        // Find the location to insert new node and insert it.
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
    return LAB2_SUCCESS;
}

/*
 * This Function insert nodes to the BST in fine-garined manner.
 */
int lab2_node_insert_fg(lab2_tree* tree, lab2_node* new_node) {
    // It's unable to insert a node to the tree.  
    if (tree == NULL || new_node == NULL){
            return LAB2_ERROR;
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
        // Find the location to insert new node and insert it.
        if (new_node->key <= now->key) {
            if (now->left) {
		pthread_mutex_lock(&(now->left->mutex));
                now = now->left;
		pthread_mutex_unlock(&now->mutex);
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
		pthread_mutex_lock(&(now->right->mutex));
                now = now->right;
		pthread_mutex_unlock(&now->mutex);
            }
            else {
		pthread_mutex_lock(&now->mutex);
                now->right = new_node;
		pthread_mutex_unlock(&now->mutex);
                break;
            }
        }
    }
    return LAB2_SUCCESS;
}


/*
 * This function inserts a node to the BST in coarse-garined manner.
 */
int lab2_node_insert_cg(lab2_tree* tree, lab2_node* new_node) {
    // It's unable to insert a node to the tree. 
    if (tree == NULL || new_node == NULL){
            return -1;
    }

    lab2_node* now = tree->root;
    // Lock the mutex variable "lock" of the tree.
    // Only one thread can access the tree.
    pthread_mutex_lock(&tree->lock);

    // If the tree is empty, new_node becomes root of the tree.
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
    // Unlock the mutex variable "lock" of the tree.
    // Other threads can access the tree.
    pthread_mutex_unlock(&tree->lock);
    return 0;
}

/*
 * This function removes the node from the tree which has the same key value passed by argument.  
 */
int lab2_node_remove(lab2_tree* tree, int key) {
    
    // It indicates that the node is a left or right child of the parent node.
    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; 

    lab2_node* now_node = tree->root; // The node which it is visited
    lab2_node* parent_node = NULL; // The parent node of the visited node

    sideis nownode_is = UNKNOWN;

    // Find the node who has same key value with argument key value.
    // Escape while loop when now_node has the same key value with argument key value.  
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_ERROR;
        }

        // If the target key value is bigger than key value of now_node, move to right side. 
        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        // If the target key value is smaller than key value of now_node, move to left side. 
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }
    // nodeSide_is == UNKNOWN means that the node to remove is the root of the tree. 

    // The situation that now_node has no child.
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

    // The situation that now_node has one child.
    else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            // now_node has left side child.
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now_node has right side child.
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            // now_node has left side child.
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now_node has right side child.
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // replace the root of tree with child node.
            // now_node has left side child.
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            // now_node has right side child.
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
    }
    // now_node has both left side child and right side child.
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; // It indicates the the rightmost child node of the left child node.
        lab2_node* parent_terminal; // Parent of the rightTerminal_node

        // Find the rightmost child of the left child of now_node.
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        switch (nownode_is) {
        case LEFT_CHILD:
            // Disconnect the rightTerminal_node and the parent of the rightTerminal_node.
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
            // Move the rightTerminal_node to root of the tree.
            parent_node->left = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case RIGHT_CHILD:
            // Disconnect the rightTerminal_node and the parent of the rightTerminal_node.
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
            // Move the rightTerminal_node to root of the tree.           
            parent_node->right = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
            break;

        case UNKNOWN:
           // Disconnect the rightTerminal_node and the parent of the rightTerminal_node.
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
            // Move the rightTerminal_node to root of the tree.  
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;

            break;
        }

    }
    // Delete now_node.
    lab2_node_delete(now_node);
}


/*
 * This function removes node from the BST in fine-grained manner.
 */
int lab2_node_remove_fg(lab2_tree* tree, int key) {
    //printf("thread is %ud\n", pthread_self());

    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; 

    pthread_mutex_lock(&tree->root->mutex);
    lab2_node* now_node = tree->root; 
    lab2_node* parent_node = NULL; 
    lab2_node* key_node; 

    sideis nownode_is = UNKNOWN;

    // Find the node who has same key value with argument key value.
    // Escape while loop when now_node has the same key value with argument key value.
    // Repeat locking and unlocking while visiting nodes.
    while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return -1;
        }

        if (key > now_node->key) {
            if (nownode_is != UNKNOWN)
                pthread_mutex_unlock(&parent_node->mutex);
            parent_node = now_node;
            pthread_mutex_lock(&parent_node->right->mutex);
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        else if (key < now_node->key) {
            if (nownode_is != UNKNOWN)
                pthread_mutex_unlock(&parent_node->mutex);
            parent_node = now_node;
            pthread_mutex_lock(&parent_node->left->mutex);
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }

   // Delete the node that is the root of the tree
   if(nownode_is == UNKNOWN){     
      // The situation that now_node has no child.
      if (now_node->left == NULL && now_node->right == NULL){
         tree->root = NULL;
         lab2_node_delete(now_node);
         return 0;
      }
      
      // If now_node has child, lock on the mutex variable of child node.
      if (now_node->left != NULL)
         pthread_mutex_lock(&now_node->left->mutex);
      else if (now_node->right != NULL)
         pthread_mutex_lock(&now_node->right->mutex);
    
      // The situation that now_node has one child.
      if (now_node->left == NULL || now_node->right == NULL){
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            pthread_mutex_unlock(&now_node->left->mutex);
            }
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            pthread_mutex_unlock(&now_node->right->mutex);
            }
         lab2_node_delete(now_node);
         return 0;
      }
     // now_node has both left side child and right side child.
        lab2_node* rightTerminal_node; 
        lab2_node* parent_terminal; 
        // As for the right child, it doesn't need locking.
        pthread_mutex_unlock(&now_node->right->mutex); 
        
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        // Find the rightmost child of the left child of now_node.
        while (rightTerminal_node->right != NULL) {
            pthread_mutex_lock(&rightTerminal_node->right->mutex);
            if(parent_terminal != now_node)
            pthread_mutex_unlock(&parent_terminal->mutex);
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }
        
        pthread_mutex_unlock(&rightTerminal_node->mutex);
        // Complete to find
      
        if (parent_terminal != now_node) {
                parent_terminal->right = NULL;
                if (rightTerminal_node->left != NULL) {
               pthread_mutex_lock(&rightTerminal_node->left->mutex);
                    parent_terminal->right = rightTerminal_node->left;
               pthread_mutex_unlock(&rightTerminal_node->left->mutex);
                }
       }
            else {
                now_node->left = NULL;
                if (rightTerminal_node->left != NULL) {
                    parent_terminal->right = rightTerminal_node->left;
                }
            }
            tree->root = rightTerminal_node;
            rightTerminal_node->right = now_node->right;
            rightTerminal_node->left = now_node->left;
      lab2_node_delete(now_node);
      return 0;
      
   }
  // The node to delete is not the root of the tree
    // If now_node has child, lock on the mutex variable of child node. 
    if (now_node->left != NULL)
        pthread_mutex_lock(&now_node->left->mutex);
    else if (now_node->right != NULL)
        pthread_mutex_lock(&now_node->right->mutex);

    // The situation that now_node has no child.
    if (now_node->left == NULL && now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            parent_node->left = NULL;
            break;
        case RIGHT_CHILD:
            parent_node->right = NULL;
            break;
        }
    }
    // The situation that now_node has one child.
    else if (now_node->left == NULL || now_node->right == NULL) {
        sideis child_of_nownode_is;
        switch (nownode_is) {
        case LEFT_CHILD:
            if (now_node->left != NULL) {
                child_of_nownode_is = LEFT_CHILD;
                parent_node->left = now_node->left;
            }
            else if (now_node->right != NULL) {
                child_of_nownode_is = RIGHT_CHILD;
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            if (now_node->left != NULL) {
                child_of_nownode_is = LEFT_CHILD;
                parent_node->right = now_node->left;
            }
            else if (now_node->right != NULL) {
                child_of_nownode_is = RIGHT_CHILD;
                parent_node->right = now_node->right;
            }
            break;

        }
        //unlock the mutex variable of child node.
        if (child_of_nownode_is == LEFT_CHILD)
            pthread_mutex_unlock(&now_node->left->mutex);
        else if (child_of_nownode_is == RIGHT_CHILD)
            pthread_mutex_unlock(&now_node->right->mutex);
    }
    // now_node has both left side child and right side child.
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; 
        lab2_node* parent_terminal; 
        pthread_mutex_unlock(&now_node->right->mutex);
        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        // Find the rightmost child of the left child of now_node.
        while (rightTerminal_node->right != NULL) {
            pthread_mutex_lock(&rightTerminal_node->right->mutex);
            pthread_mutex_unlock(&parent_terminal->mutex);
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }
        // Complete to find
        pthread_mutex_unlock(&parent_terminal->mutex);
        pthread_mutex_unlock(&rightTerminal_node->mutex);

        switch (nownode_is) {
        case LEFT_CHILD:
            if (parent_terminal != now_node) {
                parent_terminal->right = NULL;
                
                if (rightTerminal_node->left != NULL) {
                    parent_terminal->right = rightTerminal_node->left;
                }
            }
            else {
                now_node->left = NULL;

                if (rightTerminal_node->left != NULL)
                    parent_terminal->left = rightTerminal_node->left;
            }

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
        }

    }

    pthread_mutex_unlock(&now_node->mutex);
    pthread_mutex_unlock(&parent_node->mutex);

    lab2_node_delete(now_node);
}

/*
 * This function removes node from the BST in coarse-grained manner.
 */
int lab2_node_remove_cg(lab2_tree* tree, int key) {
    typedef enum side_is {
        UNKNOWN,
        LEFT_CHILD,
        RIGHT_CHILD
    } sideis; 
    // Lock the mutex variable "lock" of the tree.
    // Only one thread can access the tree.
    pthread_mutex_lock(&tree->lock);
    lab2_node* now_node = tree->root; 
    lab2_node* parent_node = NULL;
    lab2_node* key_node; 

    sideis nownode_is = UNKNOWN;
    
    // Find the node who has same key value with argument key value.
    // Escape while loop when now_node has the same key value with argument key value. 
     while (now_node->key != key) {
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return LAB2_ERROR;
        }

        if (key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nownode_is = RIGHT_CHILD;
        }
        else if (key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nownode_is = LEFT_CHILD;
        }
    }
    // The situation that now_node has no child.
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
    // The situation that now_node has one child.
     else if (now_node->left == NULL || now_node->right == NULL) {
        switch (nownode_is) {
        case LEFT_CHILD:
            if (now_node->left != NULL) {
                parent_node->left = now_node->left;
            }
            else if (now_node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;

        case RIGHT_CHILD:
            if (now_node->left != NULL) {
                parent_node->right = now_node->left;
            }
            else if (now_node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            if (now_node->left != NULL) {
                tree->root = now_node->left;
            }
            else if (now_node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
    }
   // now_node has both left side child and right side child.
    else if (now_node->left != NULL && now_node->right != NULL) {
        lab2_node* rightTerminal_node; 
        lab2_node* parent_terminal; 

        rightTerminal_node = now_node->left;
        parent_terminal = now_node;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        switch (nownode_is) {
        case LEFT_CHILD:
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
    // Unlock the mutex variable "lock" of the tree.
    // Other threads can access the tree.
    pthread_mutex_unlock(&tree->lock);
    lab2_node_delete(now_node);
}







/*
 * This function deletes the lab2_tree passed by argument.
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
 * This function deletes the lab2_node passed by argument. 
 */
void lab2_node_delete(lab2_node* node) {
    free(node);
    node = NULL;
}


