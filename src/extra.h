#ifndef EXTRA_H
#define EXTRA_H
#include <iostream>
#include <string>
#include <vector>

class MyAVLTree {
private:
    struct Node{
        std::string name;
        int id, height;
        Node* left;
        Node* right;
        Node(std::string a, int x) : name(a), id(x), height(0), left(nullptr), right(nullptr) {}
    };
    Node* root = nullptr;

    // helper functions
    Node* helperInsert(Node* root, std::string name, int id);
    Node* helperRemove(Node* root, int id);
    void helperPreorder(Node* root, std::vector<std::string>& preorder);
    void helperPostorder(Node* root, std::vector<std::string>& postorder);
    void helperInorder(Node* root, std::vector<std::string>& inorder);

    // utility
    int maxHeight(int x, int y);
    int balanceFactor(int x, int y);
    void nodePreorder(Node* root, std::vector<Node*>& preorder);
    void nodeInorder(Node* root, std::vector<Node*>& inorder);
    int getHeight(Node* n);     // avoid segmentation fault

    // rotations
    Node* leftRotate(Node* &root);
    Node* rightRotate(Node* &root);
    Node* leftrightRotate(Node* &root);
    Node* rightleftRotate(Node* &root);
    
public:

    // commands
    void insert(std::string name, int id);
    void remove(int id);
    std::string search(int id);
    std::vector<int> search(std::string name);
    void removeInOrder(int node);

    // print
    std::vector<std::string> printPreorder();
    std::vector<std::string> printPostorder();
    std::vector<std::string> printInorder();
    int printLevelCount();
};


// helper functions
MyAVLTree::Node* MyAVLTree::helperInsert(Node* root, std::string name, int id) {
    if (root == nullptr) {      // create new node at leaf
        return new Node(name, id);
    }

    // insert and balance tree
    if (id < root->id) {
        root->left = helperInsert(root->left, name, id);
        
        // balance factor = 2
        if (balanceFactor(getHeight(root->left), getHeight(root->right)) == 2) {

            // left left & left right
            if (id < root->left->id) {
                root = rightRotate(root);
            } else {        // if (id > root->left->id) 
                root = leftrightRotate(root);
            }
        } 
    } else if (id > root->id) {
        root->right = helperInsert(root->right, name, id);

        // balance factor = -2
        if (balanceFactor(getHeight(root->left), getHeight(root->right)) == -2) {

            // right right & right left
            if (id > root->right->id) {
                root = leftRotate(root);
            } else {        // if (id < root->right->id) 
                root = rightleftRotate(root);
            }
        }
    }

    // update height
    root->height = maxHeight(getHeight(root->left), getHeight(root->right)) + 1;

    return root;
}
MyAVLTree::Node* MyAVLTree::helperRemove(Node* root, int id) {
    if (root == nullptr) {
        return nullptr;
    }

    // find id
    Node* temp;
    if (id < root->id) {
        root->left = helperRemove(root->left, id);
    } else if (id > root->id) {
        root->right = helperRemove(root->right, id);
    } else if (id == root->id) {        // id found

        // two children
        if (root->left != nullptr && root->right != nullptr) {

            // find inorder successor
            temp = root->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            // replace node with IS
            root->id = temp->id;
            root->name = temp->name;
            // delete IS
            root->right = helperRemove(root->right, temp->id);

            /*
            temp = getSuccessor(id);
            root->id = temp->id;
            root->name = temp->name;
            root->right = helperRemove(root->right, root->id);
            */
        } else if (root->left == nullptr || root->right == nullptr) {       // 1 or 0
            if (root->left == nullptr && root->right == nullptr) {      // 0 children
                temp = root;
                root = nullptr;
                delete temp;
                return root;
            }

            temp = root;
            if (root->left == nullptr) {        // 1 child
                root = root->right;
            } else {
                root = root->left;
            }
            delete temp;
        }

        // update height
        root->height = maxHeight(getHeight(root->left), getHeight(root->right)) + 1;
    }
    return root;
}
void MyAVLTree::helperInorder(Node* root, std::vector<std::string>& inorder) {
    if (root == nullptr) {
        return;
    }

    helperInorder(root->left, inorder);
    inorder.push_back(root->name);
    helperInorder(root->right, inorder);
}
void MyAVLTree::helperPreorder(Node* root, std::vector<std::string>& preorder) {
    if (root == nullptr) {
        return;
    }

    preorder.push_back(root->name);
    helperPreorder(root->left, preorder);
    helperPreorder(root->right, preorder);
    
}
void MyAVLTree::helperPostorder(Node* root, std::vector<std::string>& postorder) {
    if (root == nullptr) {
        return;
    }

    helperPostorder(root->left, postorder);
    helperPostorder(root->right, postorder);
    postorder.push_back(root->name);
}

// utility and traversal
int MyAVLTree::maxHeight(int x, int y) {
    return (x > y) ? x : y;
}
int MyAVLTree::balanceFactor(int x, int y) {
    return (x - y);
}
void MyAVLTree::nodePreorder(Node* root, std::vector<Node*>& preorder) {
    if (root == nullptr) {
        return;
    }

    preorder.push_back(root);
    nodePreorder(root->left, preorder);
    nodePreorder(root->right, preorder);
}
void MyAVLTree::nodeInorder(Node* root, std::vector<Node*>& inorder) {
    if (root == nullptr) {
        return;
    }

    nodeInorder(root->left, inorder);
    inorder.push_back(root);
    nodeInorder(root->right, inorder); 
}
int MyAVLTree::getHeight(Node* n) {
    if (n == nullptr) {     // height of null node = -1
        return -1;
    }
    return n->height;
}

// rotations
MyAVLTree::Node* MyAVLTree::leftRotate(Node* &root) {
    Node* newRoot = root->right;
    Node* temp = newRoot->left;

    // rotation
    newRoot->left = root;
    root->right = temp;

    // update height
    root->height = maxHeight(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = maxHeight(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}
MyAVLTree::Node* MyAVLTree::rightRotate(Node* &root) {
    Node* newRoot = root->left;
    Node* temp = newRoot->right;

    // rotation
    newRoot->right = root;
    root->left = temp;

    // update height
    root->height = maxHeight(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = maxHeight(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}
MyAVLTree::Node* MyAVLTree::leftrightRotate(Node* &node) {
    // left rotate
    node->left = leftRotate(node->left);
    // right rotate
    return rightRotate(node);
}
MyAVLTree::Node* MyAVLTree::rightleftRotate(Node* &node) {
    // right
    node->right = rightRotate(node->right);
    // left
    return leftRotate(node);
}

// public methods

void MyAVLTree::insert(std::string name, int id) {
    root = helperInsert(root, name, id);
}
void MyAVLTree::remove(int id) {
    root = helperRemove(root, id);
}
std::vector<std::string> MyAVLTree::printInorder() {
    std::vector<std::string> inorder;
    helperInorder(root, inorder);
    return inorder;
}
std::vector<std::string> MyAVLTree::printPreorder() {
    std::vector<std::string> preorder;
    helperPreorder(root, preorder);
    return preorder;
}
std::vector<std::string> MyAVLTree::printPostorder() {
    std::vector<std::string> postorder;
    helperPostorder(root, postorder);
    return postorder;
}
int MyAVLTree::printLevelCount() {
    if (root == nullptr) {
        return 0;
    }
    return root->height + 1;
}
void MyAVLTree::removeInOrder(int node) {
    std::vector<Node*> inorder;
    nodeInorder(root, inorder);
    int id = inorder[node]->id;
    remove(id);
}

// returns name of id   -   empty if DNE
std::string MyAVLTree::search(int id) {
    std::string name = "";
    std::vector<Node*> preorder;
    nodePreorder(root, preorder);

    for (int i = 0; i < preorder.size(); i++) {
        if (preorder[i]->id == id) {
            name = preorder[i]->name;
            break;
        }
    }

    return name;
}
// returns vector with all matching ids
std::vector<int> MyAVLTree::search(std::string name) {
    std::vector<int> ids;
    std::vector<Node*> preorder;
    nodePreorder(root, preorder);
    for (int i = 0; i < preorder.size(); i++) {
        if (preorder[i]->name == name) {
            ids.push_back(preorder[i]->id);
        }
    }
    return ids;
}

#endif