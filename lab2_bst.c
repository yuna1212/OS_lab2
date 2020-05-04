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

// 뮤텍스 초기화
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 공유 자원: shared tree
lab2_tree* shared_tree = lab2_tree_create();

// key value는 0부터 순서대로 줌.
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
    } side_is; // ?대�??몃뱶媛� 遺�紐⑥쓽 ?ㅻⅨ履쎌씤�?, ?쇱そ?몄? ?�?섍�?
    
    lab2_node* parent_node;
    lab2_node* now_node;
    side_is nodeSide_is = UNKNOWN;
    
    // ?몃뱶 ?꾩튂 李얘린
    parent_node = NULL;
    now_node = tree->root;
    while (node != now_node) {
        // 留�??吏�湲� ?�?� ?몃뱶媛� NULL?대씪硫� 李얜뒗 媛�???�?� 寃�?대�?
        if (now_node == NULL) {
            printf("[Error]No node to delete..Nothing matches the node put by parameter.\n");
            return;
        }
        // 李얜뒗 媛�???� ?щ떎硫� ?ㅻⅨ履쎌쑝�? ?대룞
        if (node->key > now_node->key) {
            parent_node = now_node;
            now_node = parent_node->right;
            nodeSide_is = RIGHT;
        }
        // 李얜뒗 媛�???� ?�?ㅻ㈃ ?쇱そ?쇰? ?대룞
        else if (node->key < now_node->key) {
            parent_node = now_node;
            now_node = parent_node->left;
            nodeSide_is = LEFT;
        }
    }

    // nodeSide_is媛� UNKNOWN?대씪�뒗 寃�?� 李얜뒗 ?몃뱶媛� ?몃━?� 猷⑦듃�씪�뒗 寃� ?섎�? 媛�??瑗�?�湲??몃뱶 ?�?�?섍퀬?띠쓬!!
    
    // now?� ?�?�???�??.
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
	// now?� ?�?�???�?� ?�??
    else if (node->left == NULL || node->right == NULL) {
        switch (nodeSide_is) {
        case LEFT:
            // now?� ?쇱そ ?�?�???�?붽쾬?대�?
            if (node->left != NULL) {
                parent_node->left = now_node->left;
            }
            // now?� ?ㅻⅨ履??�?�???�?붽쾬?대�?
            else if (node->right != NULL) {
                parent_node->left = now_node->right;
            }
            break;
        
        case RIGHT:
            // now?� ?쇱そ ?�?�???�?붽쾬?대�?
            if (node->left != NULL) {
                parent_node->right = now_node->left;
            }
            // now?� ?ㅻⅨ履??�?�???�?붽쾬?대�?
            else if (node->right != NULL) {
                parent_node->right = now_node->right;
            }
            break;

        case UNKNOWN:
            // tree?� 猷⑦듃�? ?�?�?댁빞�븯誘�濡� tree->root瑜??ㅼ쓬 ?몃뱶濡� 媛�由ы궎�? ?댁빞��?
             // now?� ?쇱そ ?�?�???�?붽쾬?대�?
            if (node->left != NULL) {
                tree->root = now_node->left;
            }
            // now?� ?ㅻⅨ履??�?�???�?붽쾬?대�?
            else if (node->right != NULL) {
                tree->root = now_node->right;
            }
            break;
        }
        
    }

    // now?� ?�?�???� ???�??
    else if (node->left != NULL && node->right != NULL) {
        lab2_node* rightTerminal_node; // now?� ?쇱そ?� 媛�???ㅻⅨ履??몃뱶
        lab2_node* parent_terminal = NULL; // rightTerminal_node?� 遺�紐?

        // now?� ?쇱そ?�?�?� 媛�???ㅻⅨ履??몃뱶 ?�?�
        rightTerminal_node = now_node->left;
        while (rightTerminal_node->right != NULL) {
            parent_terminal = rightTerminal_node;
            rightTerminal_node = parent_terminal->right;
        }

        // ?대?遺�??醫� ?룰?由ш릿 ??.
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

    // nownode瑜??댁젣?섍퀬 鍮�?뚯＜湲�!
    free(now_node);
    now_node = NULL;

}


/*
* 공유 트리에 접근. 접근 스레드는 인자로 받아. 접근해서 연산하는 곳은 CS
*/
void access_sharedtree(thread_arg* thrd) {
    //쓰레드 구조체 받기
    thread_arg* th_arg = (thread_arg*)arg;

    // 반복 횟수는 받은 쓰레드의 반복 횟수로
    int num_iterations = th_arg->num_iterations;

    // 받은 쓰레드 동기화 켜면 1. 경쟁상태이면 0
    int is_sync = th_arg->is_sync;

    int i = 0;
    int op_mode = -1; // 1: insert 2: remove

    // 쓰레드간 크리티컬 섹션 진입 순서 정해줘야 하는 경우: 락을 이용함
    if (is_sync) {
        
        // 쓰레드의 반복 횟수만큼 루프
        for (i = 0; i < num_iterations; i++) {
            // 무슨 연산 할지 정해주기
            if (i < num_iterations / 2)
                op_mode = 1;
            else
                op_mode = 2;

            // 스위치문 안에 mutex를 넣는게 나을까...?성능측면에서 비교를 안하니까 쪼금 더 성능이 나을것 같긴 한데....
            switch (op_mode) {
            case 0: // 트리에 더하기
                pthread_mutex_lock(&mutex);
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                pthread_mutex_unlock(&mutex);
                break;
            case 1: // 트리에서 빼기 -> 뺄때 키 값 없으면 걍 아무 이상 없이 리턴..이경우 걍 search됨.
                pthread_mutex_lock(&mutex);
                lab2_node_remove(shared_tree, rand() % key_value);
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
    }
    else { // 쓰레드가 하나만 접근하려고 한다면: 락은 필요 없음
        // 쓰레드의 반복 횟수만큼 루프
        for (i = 0; i < num_iterations; i++) {
            // 무슨 연산 할지 정해주기
            if (i < num_iterations / 2)
                op_mode = 1;
            else
                op_mode = 2;

            // 스위치문 안에 mutex를 넣는게 나을까...?성능측면에서 비교를 안하니까 쪼금 더 성능이 나을것 같긴 한데....
            switch (op_mode) {
            case 0: // 트리에 더하기  
                lab2_node* newnode = lab2_node_create(key_value++);
                lab2_node_insert(shared_tree, newnode);
                break;
            case 1: // 트리에서 빼기 -> 뺄때 키 값 없으면 걍 아무 이상 없이 리턴..이경우 걍 search됨.
                lab2_node_remove(shared_tree, rand() % key_value);    
                break;
            }
        }
    }
}

// 락 시험해보기: 쓰레드 몇개 만들건지, 하나의 쓰레드에서 연산은 몇번 반복할건지, 스레드는 CS에 동시에 진입할 수 있는지 매개변수로 받음
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
        pthread_create(&pthreads[i], NULL, access_sharedtree, &arg); // 예제 코드는 마지막 인자 보이드로 넘기고 함수 내에서 다시 형변환 하는데 굳이..?
    }

    // 원하는 개수 만큼의 스레드를 만들어줘서 이제 순서 맞춰야 함.
    // 처음 만들어진 쓰레드부터 순서대로 시작하게 함.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }

    // 스레드 개수, 반복 횟수, 경쟁상태였는지 아니었는지 전달해서 출력하자
    print_result(num_threads, num_iterations, is_sync);

    return 0; // 리턴 굳이 필요한가...싶다...

}

