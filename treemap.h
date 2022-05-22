#include <string>
//your AVL tree implementation here
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <sstream>

//Tree class to store the node objects created when student is inserted
class Tree {

public:
    struct Node {
        std::string name;
        //store ID as string just in case of leading zeros and to easily count if the lenght is 8
        std::string ID;
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 1;
        Node(std::string name, std::string ID);
    };

    Node* head = nullptr;
    Node* traverse = head;
    bool success = false;

    //Function declarations
    Tree();
    Node* getHead();
    bool getSuccess();
    void setSuccess(bool found);
    bool insert(std::string name, std::string ID, Tree* avl);
    Node* insertHidden(Tree::Node* root, std::string name, std::string ID, bool& s);
    Node* remove(std::string ID, Tree::Node* node, Tree* avl);
    std::string search(std::string find, Tree::Node* node, std::vector<Tree::Node*>* names);
    std::string searchID(std::string ID, Tree::Node* node);
    void searchName(std::string name, Tree::Node* node, std::vector<Tree::Node*>* names);
    void printInorder(Tree::Node* node, std::vector<Tree::Node*>* names);
    void printPreorder(Tree::Node* node, std::vector<Tree::Node*>* names);
    void printPostorder(Tree::Node* node, std::vector<Tree::Node*>* names);
    void printLevelCount(Tree::Node* node);
    void removeInorder(int gatorN, std::vector<Tree::Node*>* nodes, Tree* avl);
    Node* rotateRightLeft(Tree::Node* node);
    Node* rotateLeftRight(Tree::Node* node);
    Node* rotateLeft(Tree::Node* node);
    Node* rotateRight(Tree::Node* node);

    Node* inorderSuccessor(Tree::Node* node);

};

int balance(Tree::Node* node);
int max(int left, int right);
int nodeHeight(Tree::Node* node);

//Returns success variable to know if to print successful or not
bool Tree::getSuccess() {
    return success;
}
//sets success value
void Tree::setSuccess(bool found) {
    success = found;
}
//insert funciton that calls on another hidden function so that pointers are not visible in main function. 
bool Tree::insert(std::string name, std::string ID, Tree* avl)
{
    bool s = false;
    Tree::Node* temp = Tree::insertHidden(avl->getHead(), name, ID, s);
    avl->head = temp;
    return s;
}
//function that inserts new student/node in the tree
Tree::Node* Tree::insertHidden(Tree::Node* root, std::string name, std::string ID, bool& s)
{
    //checks if ID is the appropriate 8 digits long
    if (ID.length() != 8) {
        return nullptr;
    }
    //if we reach a place in the tree that is null, then that is a spot that the new node can reside in
    if (root == nullptr) {
        s = true;
        return new Tree::Node(name, ID);
    }
    //checks if the new ID is bigger than the current node's ID, and if so then call insert function again through recursion and the result goes into the right side of the current node
    if (stol(ID) > stol(root->ID)) {
        root->right = Tree::insertHidden(root->right, name, ID, s);
    }
    //checks if the new ID is smaller than the current node's ID, and if so then call insert function again through recursion and the result goes into the left side of the current node
    else if (stol(ID) < stol(root->ID)) {
        root->left = Tree::insertHidden(root->left, name, ID, s);
    }
    else {
        //If ID already exists then print unsuccessful
        return root;
    }
    //calculate the height of the root by taking the bigger value of the left and right node's height and add 1
    //From Kapoor's lecture slides
    root->height = 1 + std::max(nodeHeight(root->left), nodeHeight(root->right));

    //go to balance function to calculate the balance of root
    int balanceVal = balance(root);
    //if node is right heavy, do right left rotation or left rotation
    if (balanceVal < -1) {
        if (balance(root->right) > 1) {
            return Tree::rotateRightLeft(root);
        }
        else {
            return Tree::rotateLeft(root);
        }
    }
    //If node is left heavy, do either left right rotation or right rotation
    else if (balanceVal > 1) {
        if (balance(root->left) < -1) {
            return Tree::rotateLeftRight(root);
        }
        else {
            return Tree::rotateRight(root);
        }
    }
    
    //return the new node
    return root;
}
//remove specified person through an ID
Tree::Node* Tree::remove(std::string ID, Tree::Node* node, Tree* avl)
{
    //base case  of recursion
    if (node == nullptr) {
        return nullptr;
    }
    //go to right if ID is bigger recursively
    if (stol(ID) > stol(node->ID)) {
        node->right = remove(ID, node->right, avl);
    }
    //if ID is less than current node's ID, go to the left of the node recursively
    else if (stol(ID) < stol(node->ID)) {
        node->left = remove(ID, node->left, avl);
    }
    else if (stol(ID) == stol(node->ID)) {
        //Node has 0 children, so just delete it
        avl->setSuccess(true);
        if (node->left == nullptr && node->right == nullptr) {
            return nullptr;
        }
        //node has child on the left so replace node with child
        else if (node->right == nullptr) {
            Tree::Node* replacement = node->left;
            delete node;
            return replacement;
        }
        //node has child on the right so replace node with child
        else if (node->left == nullptr) {
            Tree::Node* replacement = node->right;
            delete node;
            return replacement;
        }
        //Node has two children, so find inorder successor
        else {
            Tree::Node* successor = Tree::inorderSuccessor(node->right);
            node->ID = successor->ID;
            node->name = successor->name;
            node->right = Tree::remove(successor->ID, node->right, avl);
        }

    }
    return node;
}
//Finds the inorder successor of the node about to be deleted
Tree::Node* Tree::inorderSuccessor(Tree::Node* node) {
    Tree::Node* successor = node;
    //loops until the left most node is found
    while (successor->left != nullptr) {
        successor = successor->left;
    }
    return successor;
}
//Directs which search function to go to based on string
std::string Tree::search(std::string find, Tree::Node* node, std::vector<Tree::Node*>* names) {
    std::string result = "";
    //if string has any numbers, go into searchID, else go to searhName
    if (std::any_of(find.begin(), find.end(), ::isdigit)) {
        result = Tree::searchID(find, node);
    }
    else {
        Tree::searchName(find, node, names);
    }
    return result;
}
//searches for the ID recursively. When found, print the name
std::string Tree::searchID(std::string ID, Tree::Node* node)
{
    if (node == nullptr) {
        return "";
    }
    //if any of the numbers contains a letter, print unsuccessful
    else if (std::any_of(ID.begin(), ID.end(), ::isalpha)) {
        return "";
    }
    else if (ID == node->ID) {
        std::string temp = node->name;
        //removes the quotes from the name to be printed
        temp.erase(std::remove(temp.begin(), temp.end(), '\"'), temp.end());
        return temp;
    }
    else if (node->ID < ID) {
        return searchID(ID, node->right);
    }
    else {
        return searchID(ID, node->left);
    }
}

void Tree::searchName(std::string name, Tree::Node* node, std::vector<Tree::Node*>* names) {
    //populates name array with the nodes in a preorder order
    Tree::printPreorder(node, names);
    bool found = false;
    //searches array for name, if found then prints it
    for (unsigned int i = 0; i < names->size(); i++) {
        if (names->at(i)->name == name) {
            std::cout << names->at(i)->ID << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "unsuccessful" << std::endl;
    }
    names->clear();
}
//prints the tree in an inorder fashion by pushing to a vector
void Tree::printInorder(Tree::Node* node, std::vector<Tree::Node*>* names)
{
    if (node != nullptr) {
        printInorder(node->left, names);
        names->push_back(node);
        printInorder(node->right, names);
    }

}
//prints the tree in an preorder fashion by pushing to a vector
void Tree::printPreorder(Tree::Node* node, std::vector<Tree::Node*>* names)
{
    if (node != nullptr) {
        names->push_back(node);
        printPreorder(node->left, names);
        printPreorder(node->right, names);
    }
}
//prints the tree in an postorder fashion by pushing to a vector
void Tree::printPostorder(Tree::Node* node, std::vector<Tree::Node*>* names)
{
    if (node != nullptr) {
        printPostorder(node->left, names);
        printPostorder(node->right, names);
        names->push_back(node);
    }
}
//prints level count of tree by using a queue 
void Tree::printLevelCount(Tree::Node* node)
{
    std::queue<Tree::Node*> q;
    q.push(node);
    int levelCount = 0;
    
    //loops until queue is empty and every time the inner for loop ends then the level is finished
    while (!q.empty()) {
        int qSize = q.size();

        for (int i = 0; i < qSize; i++) {            

            if (q.front()->right != nullptr) {
                q.push(q.front()->right);
            }
            if (q.front()->left != nullptr) {
                q.push(q.front()->left);
            }

            q.pop();
        }
        //adds one to level count once inner for loop ends. All nodes in the level are visited when inner loop ends
        levelCount++;
    }
    std::cout << levelCount << std::endl;

}
//uses remove function and printInorder function to organize the nodes in a vector and then remove the nth one in the vector by using remove function
void Tree::removeInorder(int gatorN, std::vector<Tree::Node*>* nodes, Tree* avl)
{
    //checks if number is valid against the number of nodes
    if ((unsigned)gatorN >= nodes->size()) {
        std::cout << "unsucessful" << std::endl;
    }
    else {
        avl->head = Tree::remove(nodes->at(gatorN)->ID, avl->head, avl);
    }
}
//initializes node object
Tree::Node::Node(std::string name, std::string ID)
{
    this->name = name;
    this->ID = ID;
    this->left = nullptr;
    this->right = nullptr;
    this->height = 1;

}
//initializes tree object
Tree::Tree() {
    head = nullptr;
}
//returns head of the tree
Tree::Node* Tree::getHead()
{
    return head;
}
//calculates balance of the node by looking at its left and right nodes 
int balance(Tree::Node* node) {
    if (node == nullptr) {
        return 0;
    }
    else {
        int left;
        int right;
        //makes sure i am not acessing a null pointer
        if (node->left == nullptr) {
            left = 0;
        }
        else {
            left = node->left->height;
        }
        if (node->right == nullptr) {
            right = 0;
        }
        else {
            right = node->right->height;
        }

        return left - right;
    }
}
//returns max value of the heights
int max(int left, int right) {
    if (left > right) {
        return left;
    }
    else {
        return right;
    }

}
//retrieves the height of the node
int nodeHeight(Tree::Node* node) {
    if (node == nullptr) {
        return 0;
    }
    else {
        return node->height;
    }
}
//rotates the tree right and then left to fix an imbalance
Tree::Node* Tree::rotateRightLeft(Tree::Node* node)
{
    node->right = Tree::rotateRight(node->right);
    Tree::Node* leftR = Tree::rotateLeft(node);
    return leftR;
}
//rotates the tree left and then right to fix imbalance by using left and right rotation functions
Tree::Node* Tree::rotateLeftRight(Tree::Node* node) {
    node->left = Tree::rotateLeft(node->left);
    Tree::Node* rightR = Tree::rotateRight(node);
    return rightR;
}
//rotates nodes left if there is a right heavy parent
Tree::Node* Tree::rotateLeft(Tree::Node* node) {

    Tree::Node* grandchild;
    Tree::Node* newRoot = node->right;

    grandchild = node->right->left;

    newRoot->left = node;

    node->right = grandchild;

    return newRoot;
}
//rotates node right if the node is left heavy
Tree::Node* Tree::rotateRight(Tree::Node* node) {

    Tree::Node* grandchild;
    Tree::Node* newRoot;

    grandchild = node->left->right;

    newRoot = node->left;
    newRoot->right = node;

    node->left = grandchild;

    return newRoot;
}
class OrderedMap 
{
    private:
        Tree map;

    public:
        OrderedMap();
        ~OrderedMap();
        bool insert(const std::string ID, const std::string NAME);
        std::string search(const std::string ID);
        std::string traverse();
        bool remove(const std::string ID);
};


OrderedMap::OrderedMap()
{
    map = Tree();
}

OrderedMap::~OrderedMap()
{
}

bool OrderedMap::insert(const std::string ID, const std::string NAME)
{
    return map.insert(NAME, ID, &map);
}

std::string OrderedMap::search(const std::string ID)
{
    return map.searchID(ID,map.getHead());
}

std::string OrderedMap::traverse()
{
    std::string names = "";
    std::vector<Tree::Node*> results;
    map.printPreorder(map.getHead(), &results);
    for (int i = 0; i < results.size(); i++) {
        names += results[i]->name + ", ";
    }
    names = names.substr(0, names.length() - 2);
    return names;
}

bool OrderedMap::remove(const std::string ID)
{
    map.remove(ID, map.getHead(), &map);
    return map.getSuccess();
}
/**
//Do not change main() 
int main()
{
    OrderedMap myMap;
    int lines = 0;
    std::string command = "", ufid = "", name = "";
    std::cin >> lines;
    while(lines--)
    {
        std::cin >> command;
        if(command == "insert") 
        {
            std::cin >> ufid >> name;
            std::cout << myMap.insert(ufid, name) << "\n";
        }
        else if(command == "search")
        {
            std::cin >> ufid;
            std::cout << myMap.search(ufid) << "\n";
        }
        else if(command == "traverse")
        {
            std::cout << myMap.traverse() << "\n";
        }
        else if(command == "remove")
        {
            std::cin >> ufid;
            std::cout << myMap.remove(ufid) << "\n";
        }  
    }

    return 0;
}
**/