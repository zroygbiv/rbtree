#include <iostream>
#include "rbtree.h"

using namespace std;

int main() {

    srand(time(NULL));

    RBTree new_tree;
    int data = 0;

    for (auto i = 0; i < 10; i++) {
        data = rand() % 100;
        new_tree.insert_data(data);
    }
    cout << "Inorder traversal: " << endl;
    new_tree.inorder_traversal();

    return 0;
}