#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
//
//  MyDLList.c
//  Comp9024 Assignment1
//
//  Created by Jack on 2018/3/24.
//


// data structures representing DLList
// data type for nodes
typedef struct DLListNode {
    int  value;  // value (int) of this list item
    struct DLListNode *prev;
    // pointer previous node in list
    struct DLListNode *next;
    // pointer to next node in list
} DLListNode;

// data type for doubly linked lists
typedef struct DLList{
    int  size;      // count of items in list
    DLListNode *first; // first node in list
    DLListNode *last;  // last node in list
} DLList;

// create a new DLListNode
DLListNode *newDLListNode(int it){
    DLListNode *new;
    new = malloc(sizeof(DLListNode));
    assert(new != NULL);
    new->value = it;
    new->prev = new->next = NULL;
    return new;
}

// create a new empty DLList
DLList *newDLList(){
    struct DLList *L;
    L = malloc(sizeof (struct DLList));
    assert (L != NULL);
    L->size = 0;
    L->first = NULL;
    L->last = NULL;
    return L;
}

// Create a function for inserting a node from the tail of the DLList.
// The reason I decide to insert nodes from the tail is that the DLList will keep a same order with the original files.
void insert_at_Tail(struct DLList *L, int it) {
    // L_Node is the new node that will be added to the list.
    DLListNode *L_Node =newDLListNode(it);
    // p is the last node of the DLList L.
    DLListNode *p = L->last;
    // If the last node of the L is NULL, it means the L is a empty DLList, then the new node will be the first node.
    if(p == NULL) {
        L->first = L_Node;
        L->last = L_Node;
    }
    // If The DLList has ready had nodes, then link the last node of L to the new node, then set the newly node as the new tail.
    else{
        L_Node->prev = p;
        p->next = L_Node;
        L->last = L->last->next;
    }
}

// Create a function for merging DLLists.
// This function will be used to help us with the setUnion and setIntersection.
void merge_two_DLL (struct DLList *u, struct DLList *v){
    // link the last node of the first DLList to the first node of the second DLList. I use two temporary pointers for the operation.
    DLListNode *p = u->last;
    DLListNode *p2 = v->first;
    // using two temporary pointers for merging.
    p->next = p2;
    p2->prev = p;
    u->size = u->size + v->size;
    // the mereged linked list is u, so we can free v.
}

// create a DLList from a text file
// time complexity is O(n)
DLList *CreateDLListFromFileDlist(const char *filename){
    // For the case if filename is “stdin”.
    // I use fgets to detect the empty line at the end based on Dr Wu's hint.
    // then use fget() to read the input and store it in a buffer (string), and then use sscanf() to get an integer from the buffer.
    // We need to use "strcmp" to compare two strings. <string.h> need to be included.
    if (!strcmp(filename, "stdin")){
        struct DLList *L = newDLList();
        int n;
        //the numbers of inut should be less than 100k, but should be enough for this assignment
        int values[100000];
        char buffer[100000];
        //numbers of the inputs
        int num = 0;
        while (num < 100000){
            fgets(buffer, sizeof(buffer), stdin);
            //detecting if the input is a integer
            if (sscanf(buffer, "%d", &values[num]) == 1) {
                num++;
            }
            //if a space is detected, quit the loop.
            else {
                break;
            }
        }
        //inserting the elements to the DLList
        for (n = 0; n < num; n++) {
            insert_at_Tail(L, values[n]);
            //increasing the size by 1 after adding an element
            L->size ++;
        }
        return L;
    }
    //if the user wants to create a DLList from a given file.
    else{
        FILE* file;
        file = fopen (filename, "r");
        int num;
        // create a DLList first then insert node from the tail
        struct DLList *L = newDLList();
        while (fscanf (file, "%d", &num) != EOF)
        {
            insert_at_Tail(L, num);
            L->size ++;
        }
        fclose (file);
        return L;
    }
}

// display items of a DLList
// time complexity for this function is O(n)
void printDLList(struct DLList *u)
{
    if (u->size > 0) {
        DLListNode* p = u->first;
        while(p != NULL){
            printf("%d\n", p->value);
            p = p->next;
        }
    }
    else{
        printf("The DLList is empty.\n");
    }
}

// clone a DLList
// time complexity is O(n)
// firstly, create a new DLList.
// secondly, create a pointer which points to the first node.
// finally, insert the value of the current node to the new DLList and do it repeatly until the pointer reaches NULL.
DLList *cloneList(struct DLList *u)
{
    struct DLList *L = newDLList();
    DLListNode *p = u->first;
    while(p != NULL){
        insert_at_Tail(L, p->value);
        L->size ++;
        p = p->next;
    }
    return L;
}

// compute the union of two DLLists u and v
// time complexity for this function is O(n^2)
// the idea of this function is combining two DLList then removing duplicates of it
// according to Dr Wu, there is a special case that needed to be considered.
// which is when there is no Union for two DLList.
// in this case, "no linked list node is created (only the header node containing size and teo pointers is created)"
// this is why I clone the DLList u and then compare the size with merged DLList with the sum of the cloned list and DLList v
DLList *setUnion(struct DLList *u, struct DLList *v){
    // creating two cloned DLList before merging
    // the reason for this step is to not make any changes to the original DLList, so Union and Intersection can be done at the same time
    struct DLList *D = cloneList(u);
    struct DLList *W = cloneList(v);
    merge_two_DLL(D, W);
    // the p and q pointers are for identifying elements and checking their values
    // del is for deleting element which is duplicated and then free it
    struct DLListNode *p, *q, *del;
    p = D->first;
    // the outter loop is to pick one element, the inner loop is to compare the value between the picked and the rest elements
    while (p != NULL && p->next != NULL) {
        q = p;
        while (q->next != NULL) {
            if (p->value == q->next->value) {
                del = q->next;
                q->next = q->next->next;
                free(del);
                D->size --;
            }
            else{
                q = q->next;
            }
        }
        p = p->next;
    }
    return D;
}

// compute the insection of two DLLists u and v
// time complexity for this function is O(n^2)
// the idea of implementing this function is very similar to the setUnion function
// merage two DLList, when find a node which has the same value with the other one,
// instead of deleting it, I insert it to a new DLList then return this new DLList
DLList *setIntersection(struct DLList *u, struct DLList *v)
{
    // for the same reason, we need to create two cloned DLList
    struct DLList *D = cloneList(u);
    struct DLList *W = cloneList(v);
    merge_two_DLL(D, W);
    struct DLList *L = newDLList();
    struct DLListNode *p, *q;
    p = D->first;
    while (p != NULL && p->next != NULL) {
        q = p;
        while (q->next != NULL) {
            if (p->value == q->next->value) {
                insert_at_Tail(L, p->value);
                L->size ++;
                q->next = q->next->next;
            }
            else{
                q = q->next;
            }
        }
        p = p->next;
    }
    if (L->size == 0) {
        printf("There is no intersection between these two DLList.\n");
        return L;
    }
    else{
        return L;
    }
}

// free up all space associated with list
// time complexity for this function is O(n)
void freeDLList(struct DLList *L)
{
    DLListNode *p = L->first;
    DLListNode *q;
    while (p != NULL) {
        q = p->next;
        free(p);
        L->size --;
        p = q;
    }
}

int main()
{
    DLList *list1, *list2, *list3, *list4;
    
    list1=CreateDLListFromFileDlist("File1.txt");
    printDLList(list1);
    printf("----------------------\n");
    //list2=CreateDLListFromFileDlist("stdin");
    //printDLList(list2);
//    printf("----------------------\n");
//    list3=setUnion(list1, list2);
//    printDLList(list3);
//    printf("----------------------\n");
//    list4=setIntersection(list1, list2);
//    printDLList(list4);
//    printf("----------------------\n");
//    freeDLList(list1);
//    freeDLList(list2);
//    freeDLList(list3);
//    freeDLList(list4);
//
//    printf("please type all the integers of list1\n");
//    list1=CreateDLListFromFileDlist("stdin");
//
//    printf("please type all the integers of list2\n");
//    list2=CreateDLListFromFileDlist("stdin");
//
//    list3=cloneList(list1);
//    printDLList(list3);
//    printf("----------------------\n");
//    list4=cloneList(list2);
//    printDLList(list4);
//    freeDLList(list1);
//    freeDLList(list2);
//    freeDLList(list3);
//    freeDLList(list4);
//    printDLList(list4);
    return 0;
}

