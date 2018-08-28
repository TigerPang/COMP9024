//
//  main.c
//  last verson
//
//  Created by Jack on 2018/5/19.
//  Copyright © 2018年 Jack. All rights reserved.
//


/**********************************************
 This Assignment is the most difficult one so far.
 I did not have enough time to finish all the tasks.
 The functions including insertion, deletion, RemoveMin and Min are working fine.
 Although I have the right logic and algorithm, I did not have enough time to debug and fix the errors for the TaskScheduler function.
 If I had enough time, I think I can fix the bug.
 ***********************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for heap nodes
typedef struct HeapNode {
    // each node stores the priority (key), name, execution time,
    //  release time and deadline of one task
    int key; //key of this item
    int TaskName;  // task name
    int Etime; //execution time
    int Rtime; // release time
    int Dline; // deadline
    struct HeapNode *parent; //pointer to parent
    struct HeapNode *left; //pointer to left child
    struct HeapNode *right; //pointer to right child
} HeapNode;

//data type for a priority queue (heap)
typedef struct Heap{ //this is heap header
    int  size;      // count of items in the heap
    HeapNode *LastNode; // last node pointer
    HeapNode *root; // pointer to the root of heap
} Heap;

// create a new heap node to store an item (task)
HeapNode *newHeapNode(int k, int n, int c, int r, int d, HeapNode *L, HeapNode *R, HeapNode *P)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
    // L, R, P: pointers to left child, right child and parent, respectively
    HeapNode *new;
    new = malloc(sizeof(HeapNode));
    assert(new != NULL);
    new->key = k;
    new->TaskName = n;
    new->Etime = c;
    new->Rtime = r;
    new->Dline = d;
    new->left = L; // left child
    new->right = R; // righ child
    new->parent = P; // parent
    return new;
}

// create a new empty heap-based priority queue
Heap *newHeap()
{ // this function creates an empty heap-based priority queue
    Heap *T;
    T = malloc(sizeof(Heap));
    assert (T != NULL);
    T->size = 0;
    T->LastNode=NULL;
    T->root = NULL;
    return T;
}

/*
 This function will work as heapify for both insertion and deletion.
 This heap is a min heap, it is also a complete Binary tree, so for a key for a newly inserted node if it is smaller than the root->key, we swap the node with the root. So the root of the heap is always the smallest among other nodes.
 
 This is always the last step of the insertion.
 
 The most important part for this part is: if we move one node, we need to update this node's parent, children, and also do the same to the node which is swapped. We need to make sure every step is correct.
 
 Finally, update the last node.
 */

void Heapfy (Heap *T, HeapNode *Node)
{
    HeapNode *np = Node->parent;
    
    if (np->parent) {
        if (np == np->parent->right) {
            np->parent->right = Node;
        }
        else{
            np->parent->left = Node;
        }
    }
    else {
        T->root = Node;
    }
    Node->parent = np->parent;
    int flag = -1;
    if (Node == np->right) {
        flag = 1;
    }
    else{
        flag = 0;
    }
    HeapNode *copy;
    if (Node == np->right) {
        copy = np->left;
    }
    else{
        copy = np->right;
    }
    np->left = Node->left;
    if (np->left) {
        np->left->parent = np;
    }
    
    np->right = Node->right;
    if (np->right) {
        np->right->parent = np;
    }
    
    if (flag == 0) {
        Node->left = np;
    }
    else{
        Node->right = np;
    }
    np->parent = Node;
    if (flag == 0) {
        Node->right = copy;
    }
    else{
        Node->left = copy;
    }
    
    if (copy != NULL) {
        copy->parent = Node;
    }
    if (Node == T->LastNode) {
        T->LastNode = np;
    }
}

/*
 To save some space coding in the deletion part, I created this function to help me with swap nodes.
 */

void Swap (Heap *T, HeapNode *Node1, HeapNode *Node2)
{
    if (Node1->parent != NULL) {
        if (Node1 == Node1->parent->right) {
            Node1->parent->right = Node2;
        }
        else{
            Node1->parent->left = Node2;
        }
    }
    else {
        T->root = Node2;
    }
    Node2->parent = Node1->parent;
    Node2->left = Node1->left;
    if (Node2->left != NULL) {
        Node2->left->parent = Node2;
    }
    
    Node2->right = Node1->right;
    if (Node2->right != NULL) {
        Node2->right->parent = Node2;
    }
}


/*
 The time complexity for Insertion is log(n);
 1
 / \
 2   3
 /\  /\
 4  5 6 7
 Above is a tree, it can be seen as a small sub of a big tree.
 
 If we can implement this small tree, then we have covered all possiblities of the insertion.
 
 The last node can only be at 4, 5, 6, 7.
 
 For example:
 When we do insertion, if we have already have a left child, then we can only insert the new node to the right. If this is not the case, then we need to find the left most node to insert the new node. Then we update the last node.
 */

void Insert(Heap *T, int k, int n, int c, int r, int d)
{
    /*create a new node if T is empty. and update the last node.*/
    HeapNode *node = newHeapNode(k, n, c, r, d, NULL, NULL, NULL);
    if (T->root == NULL) {
        T->root = node;
        T->LastNode = node;
        return;
    }
    
    /* I used a reference node to travel the whole tree to find the next last node.*/
    HeapNode *ref = T->LastNode;
    while (ref->parent && ref == ref->parent->right) {
        ref = ref->parent;
    }
    if (ref->parent != NULL) {
        if (ref->parent->right) {
            ref = ref->parent->right;
            while (ref->left) {
                ref = ref->left;
            }
        }
        else {
            ref = ref->parent;
        }
    }
    else {
        while (ref->left != NULL) {
            ref = ref->left;
        }
    }
    // this step we set the new node as the last node,  then we link the new node with the reference node.
    T->LastNode = node;
    if (ref->left != NULL) {
        ref->right = node;
    }
    else{
        ref->left = node;
    }
    node->parent = ref;
    node->left = NULL;
    node->right = NULL;
    T->size++;
    // do the heapify
    while (node->parent && node->parent->key > node->key) {
        Heapfy(T, node);
    }
}


/* Make an copy of a node
 */
HeapNode *copy(HeapNode *n)
{
    HeapNode *new = newHeapNode(n->key, n->TaskName, n->Etime, n->Rtime, n->Dline, n->left, n->right, n->parent);
    return new;
    
}

/* The time complexity for RemoveMin() is O(log(n));
 Because of the min heap, we need to remove the root.
 The theory behiind it is that we replace the root with the last root, then we heapify. Update the last node.
 At the begining, we start at the last node, then if the current node is the left kind, and not the root, then move the current node to the next left node. At the end reach the right most node.
 As requested, we copy a the root then return it at the end.
 */

HeapNode *RemoveMin (Heap *T)
{
    if (T->root->parent == NULL && T->root->left == NULL && T->root->right == NULL) {
        T->root = NULL;
        T->LastNode= NULL;
        return NULL;
    }
    HeapNode *copy0 = copy(T->root);
    HeapNode *cur = T->LastNode;
    T->size --;
    while (cur->parent && cur == cur->parent->left) {
        cur = cur->parent;
    }
    if (cur->parent) {
        cur = cur->parent->left;
        while (cur->right) {
            cur = cur->right;
        }
    } else {
        while (cur->right != NULL) {
            cur = cur->right;
        }
    }
    
    if (T->LastNode == T->LastNode->parent->right) {
        T->LastNode->parent->right = NULL;
    }
    else{
        T->LastNode->parent->left = NULL;
    }
    
    if (T->root == T->LastNode) {
        T->LastNode = cur;
    } else {
        // according to the lecture slides we need move the last to the root.
        HeapNode *ref = T->LastNode;
        Swap(T, T->root, ref);
        if (T->root != cur) {
            T->LastNode = cur;
        }
        // bubble down
        if (ref->parent && ref->key < ref->parent->key) {
            do {
                Heapfy(T, ref);
            } while (ref->parent && ref->key < ref->parent->key);
        }
        else {
            while (ref->left || ref->right) {
                
                if (ref->right &&  ref->left->key >= ref->right->key) {
                    if (ref->key > ref->right->key) {
                        Heapfy(T, ref->right);
                    } else {
                        break;
                    }
                }
                else{
                    if (ref->key > ref->left->key) {
                        Heapfy(T, ref->left);
                    } else {
                        break;
                    }
                }
                
            }
        }
    }
    return copy0;
}

int Min(Heap *T)
{
    return T->root->key;
}


// Time complexity analysis for MyTaskScheduler is O(nlog(n))
/*
 The reason for this time complexity is, the insertion and deletion are both log(n), and the total number of tasks is fixed, so the time complexity analysis for MyTaskScheduler is O(nlog(n)).
 
 The algorithm for this part is that we create two priority queues, the first one uses release time as the key, and the second remain empty until certain conditions are met. in this case, when release time is behind the core available time. Then we insert the min element of the first quene to the second one using deadline as the key. This will help ys find the earliest deadline, because we are using EDF.
 
 We could use array to store status of cores. OR we could also use a priority queue using cpu_time + Etime as the key.
 
 I tried both ways, but I did not have enough time to fix the bugs.
 
 */
int TaskScheduler(char *f1, char *f2, int m )
{
    int cpu_time = 0;
    Heap *C = newHeap();
    Heap *R = newHeap();
    Heap *D = newHeap();
    int count = 0;
    int Task, Etime, Rtime, Dline;
    FILE *file1 = NULL;
    file1 = fopen (f2, "a");
    FILE *file = NULL;
    file = fopen(f1, "r");
    if(file == 0) {
        perror("fopen");
        exit(1);
    }
    else{
        
        int current_available_core = 1;
        for (int i = 1; i <= m; i ++) {
            current_available_core ++;
            Insert(C, 0, current_available_core, 0, 0, 0);
        }
        /*this part is for checking if the input is valid.
         for example, if there are non intergers, or we need 4 integeres but only have 3.
         */
        while (fscanf(file, "%d %d %d %d", &Task, &Etime, &Rtime, &Dline) != EOF){
            if( isdigit(Rtime) == 1){
                printf("It is a wrong input\n");
                exit(1);
            }
            if( isdigit(Task) == 1){
                printf("It is a wrong input\n");
                exit(1);
            }
            if( isdigit(Etime) == 1){
                printf("It is a wrong input\n");
                exit(1);
            }
            if( isdigit(Dline) == 1){
                printf("It is a wrong input\n");
                exit(1);
            }
            
            Insert(R, Rtime, Task, Etime, Rtime, Dline);
            count ++;
            
        }
        while (R->root != NULL) {
            while (R->root != NULL && Min(R) <= cpu_time) {
                
                HeapNode *copy = RemoveMin(R);
                Insert(D, copy->Dline, copy->TaskName, copy->Etime, copy->Rtime, copy->Dline);
                RemoveMin(R);
            }
            while (D->root != NULL && Min(C) < cpu_time) {
                if ((Min(C) + D->root->Etime) > D->root->Dline) {
                    return 0;
                }
            }
            while (C->root != NULL) {
                RemoveMin(C);
                HeapNode *copy1 = RemoveMin(D);
                Insert(C, copy1->Etime, current_available_core, 0, 0, 0);
                fprintf(file1, "%d, %d, %d\n", copy1->TaskName, current_available_core, cpu_time);
            }

            cpu_time ++;
        }
        return 1;
    }
}


void printHeap(HeapNode *h)
{
    if(h!=NULL)
    {
        printf("h->key (%d), h->TaskName (%d), h->Etime (%d), h->Rtime (%d), h->Dtime (%d)\n",h->key, h->TaskName, h->Etime, h->Rtime, h->Dline);
        printHeap(h->left);
        printHeap(h->right);
    }
}


int main() //sample main for testing
{
    int i;
    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
    if (i==0) printf("No feasible schedule!\n");
//    Heap *h = newHeap();
//    Insert(h, 2, 1, 1, 1, 1);
//    printHeap(h->root);
//    Insert(h, 1, 2, 2, 2, 2);
//    Insert(h, 6, 3, 3, 3, 3);
//    Insert(h, 4, 4, 4, 4, 4);
//    Insert(h, 0, 5, 5, 5, 5);
//    Insert(h, 6, 6, 6, 6, 6);
//    Insert(h, 9, 7, 7, 7, 7);
//    Insert(h, 8, 8, 8, 8, 8);
//    Insert(h, 6, 9, 9, 9, 9);
//    Insert(h, 4, 10, 10, 10, 10);
//    // printf("%d", h->size);
//    for (int i = 0; i < 10; i++) {
//        RemoveMin(h);
//        printHeap(h->root);
//        printf("--------------------\n");
//    }
//    printHeap(h->root);
//    printf("%d", h->size);
//{ int i;
//    i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is a feasible schedule on 4 cores */
//    i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is no feasible schedule on 3 cores */
//    i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is a feasible schedule on 5 cores */
//    i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is no feasible schedule on 4 cores */
//    i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is no feasible schedule on 2 cores */
//    i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
//    if (i==0) printf("No feasible schedule!\n");
//    /* There is a feasible schedule on 2 cores */
    return 0;
}

