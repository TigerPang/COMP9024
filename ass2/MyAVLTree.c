/*
 MyAVLTree.c
 COMP9024 Assignment 2
 
 Created by Jack on 2018/4/21.
 Copyright © 2018. All rights reserved.
 The complexity will be given for the required functions.
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like

// data type for avl tree nodes
typedef struct AVLTreeNode {
    int key; //key of this item
    int  value;  //value (int) of this item
    int height; //height of the subtree rooted at this node
    struct AVLTreeNode *parent; //pointer to parent
    struct AVLTreeNode *left; //pointer to left child
    struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
    int  size;      // count of items in avl tree
    AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
    AVLTreeNode *new;
    new = malloc(sizeof(AVLTreeNode));
    assert(new != NULL);
    new->key = k;
    new->value = v;
    new->height = 0; // height of this new node is set to 0
    new->left = NULL; // this node has no child
    new->right = NULL;
    new->parent = NULL; // no parent
    return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
    AVLTree *T;
    T = malloc(sizeof (AVLTree));
    assert (T != NULL);
    T->size = 0;
    T->root = NULL;
    return T;
}

/*
 Comments:
 1. This is the help function that is used to remove parentheses and comma.
 2. string is the input from buffer and split is delimeters which is how we split the strings. In this case: "%*[(]%*[,]%*[)]%*c " is the format, it means we want onlt the integers are obtained. "*c" means we read '\n', but we don't stored it.
 3. I also set up a counter and return it, I need it for storing the integers into array. This will help me with the insertion.
 4. I want to preserve the originals, so I copied string to dest. sest is used as a container.
 */
int split(char dest[][80], char *string, const char *split)
{
    int count = 0;
    char *result = NULL;
    result = strtok(string, split);
    while( result != NULL )
    {
        strcpy(dest[count++], result);
        result = strtok(NULL, split);
    }
    return count;
}

/*
 This help function is to find hight.
 It is better to do it this way, because we don't give a integer for hight in AVLTreeNode.
 */
int height(struct AVLTreeNode *node)
{
    if (node == NULL)
    {
        return 0;
    }
    else{
        int h = node->height;
        return h;
    }
}

/*
 This help function is to do the left rotate.
 We need to switch pointers and update hights.
 */
AVLTreeNode *leftR(struct AVLTreeNode *node){
    
    struct AVLTreeNode *node1 = node->right;
    struct AVLTreeNode *node2 = node1->left;
    node1->left = node;
    node->right = node2;
    
    if(height(node->left) > height(node->right)){
        node->height = height(node->left) + 1;
    }
    else{
        node->height = height(node->right) + 1;
    }
    if(height(node1->left) > height(node1->right)){
        node1->height = height(node1->left) + 1;
    }
    else{
        node1->height = height(node1->right) + 1;
    }
    
    return node1;
}

/*
 This help function is to do the right rotate.
 We need to switch pointers and update hights.
 */
AVLTreeNode *rightR(struct AVLTreeNode *node){
    
    struct AVLTreeNode *node1 = node->left;
    struct AVLTreeNode *node2 = node1->right;
    node1->right = node;
    node->left = node2;
    if(height(node1->left) > height(node1->right)){
        node1->height = height(node1->left) + 1;
    }
    else{
        node->height = height(node->right) + 1;
    }
    if(height(node->left) > height(node->right)){
        node->height = height(node->left) + 1;
    }
    else{
        node->height = height(node->right) + 1;
    }
    
    return node1;
}

/*
 Comments:
 1. This is the help function for inserting nodes. The reason I did this help function is that it would be much easier for performing required functions such as CreateAVLTree, InsertNode and MergeTwoAVLTrees
 2. If the initial input is empty, for example, the fiest input from keyboard is a space or the file is empty, we create a new node.
 3. Balancing is a vital part for insertion. For a AVL tree, the hight difference between left hand side and the right hand side is less or equal to 1. Plus the basic rule for BST is keysValue(left) < keyValue(root) < keysValue(right)
 4. I used recurtion for insertion, the ancestors of recent added node will be visited from bottomo to top.
 5. The steps are do a normal insertion, update hight and rotate if imbalanced.
 
 */

AVLTreeNode *insertNodes(struct AVLTreeNode *node, int key, int value)
{
    if (node == NULL) {
        node = newAVLTreeNode(key, value);
    }
    if (key > node->key) {
        node->right = insertNodes(node->right, key, value);
    }
    else if (key < node->key){
        node->left = insertNodes(node->left, key, value);
    }
    else{
        return node;
    }
    
    if (height(node->left) > height(node->right)) {
        node->height = height(node->left) + 1;
    }
    else{
        node->height = height(node->right) + 1;
    }
    
    int height_difference = height(node->left) - height(node->right);
    
    if (height_difference > 1 && key < node->left->key) {
        node = rightR(node);
        return node;
    }
    
    if (height_difference > 1 && key > node->left->key) {
        node->left = leftR(node->left);
        node = rightR(node);
        return node;
    }
    
    if (height_difference < -1 && key > node->right->key) {
        node = leftR(node);
        return node;
    }
    
    if (height_difference < -1 && key < node->right->key) {
        node->right = rightR(node->right);
        node = leftR(node);
        return node;
    }
    
    return node;
}

/*
 We are ask for printing all the items stored in the avl tree T sorted in increasing order of keys. Therefore, I just need to perform an inorder traversals. This is a help function.
 */
void in_order(struct AVLTreeNode *node){
    if(node != NULL)
    {
        in_order(node->left);
        printf("(%d, %d)\n", node->key, node->value);
        in_order(node->right);
    }
}

/*
 Comments:
 This help function is for helping CloneAVLTree(AVLTree *T). Creating a clone tree is pretty stright forward, create a new node and assign the key and value to it.
 */
AVLTreeNode* copy(struct AVLTreeNode *node){
    if(node == NULL){
        return NULL;
    }
    struct AVLTreeNode *clone = newAVLTreeNode(node->key, node->value);
    clone->left = copy(node->left);
    clone->right = copy(node->right);
    return clone;
}

/*
 Comments:
 This help function is for helping MergeTwoAVLTrees().
 The idea is to search for tree2's nodes and insert them into tree1.
 */
AVLTreeNode *combine(struct AVLTreeNode *node1, struct AVLTreeNode *node2){
    
    if (node2 != NULL) {
        combine(node1, node2->left);
        insertNodes(node1, node2->key, node2->value);
        combine(node1, node2->right);
    }
    return node1;
}

/*
 This is the helper function for DeleteNode().
 There are several cases, but for this assignment we dont need to worry about duplicate keys, so only three cases.
 The main steps are:
 (1) Find whether the target node is in the left subtree or the right subtree
 (2) We call this function recursively untill we find this target node.
 (3) There are three different situationss, one is the node has no children, the other one is the node has one child and the node has two children.
 (5) Maintain a good height difference between the left subtree and the right subtree.
 */
AVLTreeNode *deleteNodes(struct AVLTreeNode *node, int key){
    if (node == NULL) {
        return NULL;
    }
    
    if (key < node->key) {
        node->left = deleteNodes(node->left, key);
    }
    else if(key > node->key){
        node->right = deleteNodes(node->right, key);
    }
    else{
        if (node->left == NULL && node->right == NULL) {
            free(node);
            node = NULL;
        }
        else if (node->left == NULL) {
            struct AVLTreeNode *temp = node;
            node = node->right;
            free(temp);
        }
        else  {
            struct AVLTreeNode *temp = node;
            node = node->left;
            free(temp);
        }
        
    }
    if (node == NULL){
        return node;
    }
    else{
        int height_difference1 = height(node->left) - height(node->right);
        if (height_difference1 > 1 && height(node->left->left) - height(node->left->right) < 0) {
            node->left = leftR(node->left);
            node = rightR(node);
            return node;
        }
        if (height_difference1 > 1 && height(node->left->left) - height(node->left->right) >= 0) {
            node = rightR(node);
            return node;
        }
        
        if (height_difference1 < -1 && height(node->right->left) - height(node->right->right) <= 0) {
            node = leftR(node);
            return node;
        }
        
        if (height_difference1 < -1 && height(node->right->left) - height(node->right->right) > 0) {
            node->right = rightR(node->right);
            node = leftR(node);
            return node;
        }
    }
    
    return node;
}

void print_delete(struct AVLTreeNode *node){
    if(node != NULL)
    {
        print_delete(node->left);
        printf("The node: %d has been deleted.\n", node->key);
        print_delete(node->right);
    }
}






/*
 I think there a problem for FreeAVLTree. After the avl tree is freed, I tried to print it, it should say the tree is empty, because the root should points to NULL. But it only makes the local node pointer to NULL. After consult with Dr.Hu I realised that the function below does not make the pointer to NULL. Therefore, we need to access not only the address of the node, but also the address of the pointer. So we need a double pointer rto access the address of the pointer.
 */
void free_tree(struct AVLTreeNode **node){
    if (node == NULL || *node == NULL) return ;
    free_tree(&(*node)->left);
    free_tree(&(*node)->right);
    free(*node);
    *node = NULL;
}

/*
 Time complexity for CreateAVLTree() is O(Logn), where n is the number of keys. Because I include insertion function in this function, and the time complexity for insertion is O(Logn), so overall, it is O(nLogn).
 For creating a AVL tree, It is a must to mantain the hight difference between the left sub tree and the right sub tree less or equal to 1.
 This is done in the insertion function.
 Comments:
 1. I thought Create AVL tree ftom the standard input is the same as the first Assignment, but I am wrong. It requires much more extra work, because from "Specifications", it requires us to implement "In case of an error in the input, this function will print the error and your program terminates.", which is not required in the first assignment.
 2. For creating AVL tree from the user's input, I used strtok function to remove Parentheses and comma. The purpose is obtaining integers.
 3. For creating AVL tree from the File, I used sscanf function to remove Parentheses and comma.
 4. For checking if a space is detected, we only need to find out if the first char of the buffer is a space.
 5. For checking if the input is correct, I firstly use the sscanf to check the initial input, then check if the splited string can be converted to a integer.
 */
struct AVLTree *CreateAVLTree(const char *filename)
{
    if (!strcmp(filename, "stdin")){
        printf("Please enter the nodes to create a AVL tree.\n");
        struct AVLTree *tree = newAVLTree();
        char buffer[1000] = "";
        char dst[100][80];
        int check_input1;
        int check_input2;
        for(;;){
            if (!fgets (buffer, sizeof buffer, stdin))
                break;
            if (*buffer == ' ')
                break;
            if (sscanf(buffer, "%*[(]%d%*[,]%d%*[)]%*c", &check_input1, &check_input2) == 0) {
                printf("It is a wrong input\n");
                exit(1);
            }
            int n[100];
            int m[100];
            int count = 0;
            int count1 = 0;
            int count2 = split(dst, buffer, "%*[(]%*[,]%*[)]%*c ");
            for (int i = 1; i < count2; i+=2){
                count ++;
                int j = atoi(dst[i]);
                if(atoi(dst[i]) == 0){
                    printf("It is a wrong input\n");
                    exit(1);
                }
                n[count] = j;
            }
            for (int q = 0; q < count2-1; q+=2){
                count1 ++;
                int p = atoi(dst[q]);
                if(atoi(dst[q]) == 0){
                    printf("It is a wrong input\n");
                    exit(1);
                }
                m[count1] = p;
            }
            for (int o = 1; o < count+1; o++){
                tree->root = insertNodes(tree->root, m[o], n[o]);
                tree->size ++;
            }
        }
        return tree;
    }
    else{
        struct AVLTree *tree = newAVLTree();
        int key;
        int value;
        FILE *file = NULL;
        file = fopen(filename, "r");
        if(file == 0) {
            perror("fopen");
            exit(1);
        }
        else{
            while (fscanf(file, " %*[(]%d%*[,]%d%*[)]%*c ", &key, &value) != EOF){
                tree->root = insertNodes(tree->root, key, value);
                tree->size ++;
            }
        }
        fclose(file);
        return tree;
    }
}

/*
 Time complexity for CloneAVLTree() is O(logn)
 Create a new tree first, then assign node from the help function to it.
 */
AVLTree *CloneAVLTree(AVLTree *T)
{
    struct AVLTree *tree = newAVLTree();
    tree->root = copy(T->root);
    return tree;
}

/*
 Time complexity for Search is O(logn)
 Like the doubly linked list, we just need to visit every nodes in the tree. If we find a mached key, we just return it, else, return a NULL as request by the assignment specifications.
 I do not want to modify the original, so I used a cloned tree instead.
 */
AVLTreeNode *Search(AVLTree *T, int k)
{
    struct AVLTree *tree = newAVLTree();
    struct AVLTreeNode* node;
    tree = CloneAVLTree(T);
    node = tree->root;
    while (node != NULL && node->key != k) {
        if (k < node->key) {
            node = node->left;
        }
        else{
            node = node->right;
        }
    }
    if (node == NULL){
        return NULL;
    }
    else{
        return node;
    }
}

/*
 Time complexity for InsertNode() is O(logn)
 From the requirement"If the key k exists in the tree, this function simply returns 0 without adding the new item (k, v) to the tree. Otherwise, it inserts the new item (k, v) into the avl tree T, increases the tree size by one and returns 1."
 It means we need to implement search function first, and use it in this insert function.
 */
int InsertNode(AVLTree *T, int k, int v)
{
    if (Search(T, k) == NULL) {
        T->root = insertNodes(T->root, k, v);
        return 1;
    }
    else{
        return 0;
    }
}

/*
 Time complexity for DeleteNode() is O(Logn)
 */
int DeleteNode(struct AVLTree *T, int k)
{
    if (Search(T, k) != NULL) {
        T->root = deleteNodes(T->root, k);
        return 1;
    }
    else{
        return 0;
    }
}

/*
 Time complexity for FreeAVLTree() is O(Logn)
 */
void FreeAVLTree(AVLTree *T)
{
    print_delete(T->root);
    free_tree(&T->root);
}

/*
 Time complexity for PrintAVLTree() is O(n)
 */
void PrintAVLTree(AVLTree *T)
{
    if (T->root == NULL){
        printf("The tree is empty.\n");
    }
    else{
        in_order(T->root);
    }
}
/*
 Time complexity for MergeTwoAVLTrees() is O(logn)
 Create a new tree first, then assign node from the help function to it. We are asked to distory  both T1 and T2, so I create a new cloned tree. Note that because I merged T2 to T1 first, so T1 contains all nodes from both T1 and T2.
 */
AVLTree *MergeTwoAVLTrees(AVLTree *T1, AVLTree *T2)
{
    struct AVLTree *merged_tree = newAVLTree();
    struct AVLTree *cloned_tree = newAVLTree();
    combine(T1->root, T2->root);
    merged_tree->root = T1->root;
    cloned_tree = CloneAVLTree(merged_tree);
    FreeAVLTree(T1);
    FreeAVLTree(T2);
    return cloned_tree;
}

int main()
{
    int i,j;
    AVLTree *tree1, *tree2, *tree3, *tree4;
    AVLTreeNode *node1;
    
    tree1=CreateAVLTree("stdin");
    PrintAVLTree(tree1);
    FreeAVLTree(tree1);
    //you need to create the text file file1.txt
    // to store a set of items with distinct keys
    tree2=CreateAVLTree("file1.txt");
    PrintAVLTree(tree2);
    tree3=MergeTwoAVLTrees(tree1, tree2);
    PrintAVLTree(tree3);
    FreeAVLTree(tree2);
    FreeAVLTree(tree3);

    tree4=newAVLTree();
    j=InsertNode(tree4, 10, 10);
    for (i=0; i<15; i++)
    {
        j=InsertNode(tree4, i, i);
        if (j==0) printf("(%d, %d) already exists\n", i, i);
    }
    PrintAVLTree(tree4);
    node1=Search(tree4,20);
    if (node1!=NULL)
        printf("key= %d value= %d\n",node1->key,node1->value);
    else
        printf("Key 20 does not exist\n");

    for (i=17; i>0; i--)
    {
        j=DeleteNode(tree4, i);
        if (j==0)
            printf("Key %d does not exist\n",i);
        PrintAVLTree(tree4);
    }
    FreeAVLTree(tree4);
    return 0;
}
