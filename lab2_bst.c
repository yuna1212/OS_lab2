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

// ¹ÂÅØ½º ÃÊ±âÈ­
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// °øÀ¯ ÀÚ¿ø: shared tree
lab2_tree* shared_tree = lab2_tree_create();

// key value´Â 0ºÎÅÍ ¼ø¼­´ë·Î ÁÜ.
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
    } side_is; // ?´ë–??¸ë“œê°€ ë¶€ëª¨ì˜ ?¤ë¥¸ìª½ì¸ì?, ?¼ìª½?¸ì? ?•?˜ê¸?
    
    lab2_node* parent_node;
    lab2_node* now_node;
    side_is nodeSide_is = UNKNOWN;
    
    // ?¸ë“œ ?„ì¹˜ ì°¾ê¸°
    parent_node = NULL;
    now_node = tree->root;
    while (node != now_node) {
        // ë§Œ??ì§€ê¸ˆ ?ˆ?” ?¸ë“œê°€ NULL?´ë¼ë©´ ì°¾ëŠ” ê°’???†?” ê²ƒ?´ë‹?
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return;
        }
        // ì°¾ëŠ” ê°’???” ?¬ë‹¤ë©´ ?¤ë¥¸ìª½ìœ¼ë? ?´ë™
        if (node->key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nodeSide_is = RIGHT;
        }
        // ì°¾ëŠ” ê°’???” ?‘?¤ë©´ ?¼ìª½?¼ë? ?´ë™
        else if (node->key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nodeSide_is = LEFT;
        }
    }

    // nodeSide_isê°€ UNKNOWN?´ë¼ëŠ” ê²ƒ?€ ì°¾ëŠ” ?¸ë“œê°€ ?¸ë¦¬?˜ ë£¨íŠ¸ë¼ëŠ” ê²ƒ ?˜ë¯? ê°€??ê¼­?€ê¸??¸ë“œ ?­?œ?˜ê³ ?¶ìŒ!!
    
    // now?” ?????†??.
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
	// now?” ?????˜?˜ ?ˆ??
    else if (node->left == NULL || node->right == NULL) {
        switch (nodeSide_is) {
        case LEFT:
            // now?˜ ?¼ìª½ ?????ˆ?”ê²ƒ?´ë‹?
            if (node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now?˜ ?¤ë¥¸ìª??????ˆ?”ê²ƒ?´ë‹?
            else if (node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;
        
        case RIGHT:
            // now?˜ ?¼ìª½ ?????ˆ?”ê²ƒ?´ë‹?
            if (node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now?˜ ?¤ë¥¸ìª??????ˆ?”ê²ƒ?´ë‹?
            else if (node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree?˜ ë£¨íŠ¸ë? ?­?œ?´ì•¼í•˜ë¯€ë¡œ tree->rootë¥??¤ìŒ ?¸ë“œë¡œ ê°€ë¦¬í‚¤ê? ?´ì•¼í•?
             // now?˜ ?¼ìª½ ?????ˆ?”ê²ƒ?´ë‹?
            if (node->left != NULL) {
                tree->root = now_node->left;
            }
            // now?˜ ?¤ë¥¸ìª??????ˆ?”ê²ƒ?´ë‹?
            else if (node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
        
    }

    // now?” ?????˜ ???ˆ??
    else if (node->left != NULL && node->right != NULL) {
        lab2_node* rightTerminal_node; // now?˜ ?¼ìª½?˜ ê°€???¤ë¥¸ìª??¸ë“œ
        lab2_node* parent_terminal = NULL; // rightTerminal_node?˜ ë¶€ëª?

        // now?˜ ?¼ìª½???˜ ê°€???¤ë¥¸ìª??¸ë“œ ??‰
        rightTerminal_node = now_node->left;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // ?´ë?ë¶„??ì¢€ ?·ê?ë¦¬ê¸´ ??.
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

    // nownodeë¥??´ì œ?˜ê³  ë¹„?Œì£¼ê¸°!
    free(now_node);
    now_node = NULL;

}


/*
* °øÀ¯ Æ®¸®¿¡ Á¢±Ù. Á¢±Ù ½º·¹µå´Â ÀÎÀÚ·Î ¹Ş¾Æ. Á¢±ÙÇØ¼­ ¿¬»êÇÏ´Â °÷Àº CS
*/
void access_sharedtree(thread_arg* thrd) {
    //¾²·¹µå ±¸Á¶Ã¼ ¹Ş±â
    thread_arg* th_arg = (thread_arg*)arg;

    // ¹İº¹ È½¼ö´Â ¹ŞÀº ¾²·¹µåÀÇ ¹İº¹ È½¼ö·Î
    int num_iterations = th_arg->num_iterations;

    // ¹ŞÀº ¾²·¹µå µ¿±âÈ­ ÄÑ¸é 1. °æÀï»óÅÂÀÌ¸é 0
    int is_sync = th_arg->is_sync;

    int i = 0;
    int op_mode = -1; // 1: insert 2: remove

    // ¾²·¹µå°£ Å©¸®Æ¼ÄÃ ¼½¼Ç ÁøÀÔ ¼ø¼­ Á¤ÇØÁà¾ß ÇÏ´Â °æ¿ì: ¶ôÀ» ÀÌ¿ëÇÔ
    if (is_sync) {
        
        // ¾²·¹µåÀÇ ¹İº¹ È½¼ö¸¸Å­ ·çÇÁ
        for (i = 0; i < num_iterations; i++) {
            // ¹«½¼ ¿¬»ê ÇÒÁö Á¤ÇØÁÖ±â
            if (i < num_iterations / 2)
                op_mode = 1;
            else
                op_mode = 2;

            // ½ºÀ§Ä¡¹® ¾È¿¡ mutex¸¦ ³Ö´Â°Ô ³ªÀ»±î...?¼º´ÉÃø¸é¿¡¼­ ºñ±³¸¦ ¾ÈÇÏ´Ï±î ÂÉ±İ ´õ ¼º´ÉÀÌ ³ªÀ»°Í °°±ä ÇÑµ¥....
            switch (op_mode) {
            case 0: // Æ®¸®¿¡ ´õÇÏ±â
                pthread_mutex_lock(&mutex);
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                pthread_mutex_unlock(&mutex);
                break;
            case 1: // Æ®¸®¿¡¼­ »©±â -> »¬¶§ Å° °ª ¾øÀ¸¸é °Á ¾Æ¹« ÀÌ»ó ¾øÀÌ ¸®ÅÏ..ÀÌ°æ¿ì °Á searchµÊ.
                pthread_mutex_lock(&mutex);
                lab2_node_remove(shared_tree, rand() % key_value);
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
    }
    else { // ¾²·¹µå°¡ ÇÏ³ª¸¸ Á¢±ÙÇÏ·Á°í ÇÑ´Ù¸é: ¶ôÀº ÇÊ¿ä ¾øÀ½
        // ¾²·¹µåÀÇ ¹İº¹ È½¼ö¸¸Å­ ·çÇÁ
        for (i = 0; i < num_iterations; i++) {
            // ¹«½¼ ¿¬»ê ÇÒÁö Á¤ÇØÁÖ±â
            if (i < num_iterations / 2)
                op_mode = 1;
            else
                op_mode = 2;

            // ½ºÀ§Ä¡¹® ¾È¿¡ mutex¸¦ ³Ö´Â°Ô ³ªÀ»±î...?¼º´ÉÃø¸é¿¡¼­ ºñ±³¸¦ ¾ÈÇÏ´Ï±î ÂÉ±İ ´õ ¼º´ÉÀÌ ³ªÀ»°Í °°±ä ÇÑµ¥....
            switch (op_mode) {
            case 0: // Æ®¸®¿¡ ´õÇÏ±â  
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                break;
            case 1: // Æ®¸®¿¡¼­ »©±â -> »¬¶§ Å° °ª ¾øÀ¸¸é °Á ¾Æ¹« ÀÌ»ó ¾øÀÌ ¸®ÅÏ..ÀÌ°æ¿ì °Á searchµÊ.
                lab2_node_remove(shared_tree, rand() % key_value);    
                break;
            }
        }
    }
}

// ¶ô ½ÃÇèÇØº¸±â: ¾²·¹µå ¸î°³ ¸¸µé°ÇÁö, ÇÏ³ªÀÇ ¾²·¹µå¿¡¼­ ¿¬»êÀº ¸î¹ø ¹İº¹ÇÒ°ÇÁö, ½º·¹µå´Â CS¿¡ µ¿½Ã¿¡ ÁøÀÔÇÒ ¼ö ÀÖ´ÂÁö ¸Å°³º¯¼ö·Î ¹ŞÀ½
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
        pthread_create(&pthreads[i], NULL, access_sharedtree, &arg); // ¿¹Á¦ ÄÚµå´Â ¸¶Áö¸· ÀÎÀÚ º¸ÀÌµå·Î ³Ñ±â°í ÇÔ¼ö ³»¿¡¼­ ´Ù½Ã Çüº¯È¯ ÇÏ´Âµ¥ ±»ÀÌ..?
    }

    // ¿øÇÏ´Â °³¼ö ¸¸Å­ÀÇ ½º·¹µå¸¦ ¸¸µé¾îÁà¼­ ÀÌÁ¦ ¼ø¼­ ¸ÂÃç¾ß ÇÔ.
    // Ã³À½ ¸¸µé¾îÁø ¾²·¹µåºÎÅÍ ¼ø¼­´ë·Î ½ÃÀÛÇÏ°Ô ÇÔ.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }

    // ½º·¹µå °³¼ö, ¹İº¹ È½¼ö, °æÀï»óÅÂ¿´´ÂÁö ¾Æ´Ï¾ú´ÂÁö Àü´ŞÇØ¼­ Ãâ·ÂÇÏÀÚ
    print_result(num_threads, num_iterations, is_sync);

    return 0; // ¸®ÅÏ ±»ÀÌ ÇÊ¿äÇÑ°¡...½Í´Ù...

}

