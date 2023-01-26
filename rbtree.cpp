#include "rbtree.h"

#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

/***  Node Class  ***/
Node::Node(int new_data)
{
    color = 0;
    data = new_data;
    parent = left = right = nullptr;
}

Node::~Node()
{
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

Node * Node::get_left() const
{
    if (left) return left;
    else return nullptr;
}

Node * Node::get_right() const
{
    if (right) return right;
    else return nullptr;
}

Node * Node::get_parent() const
{
    if (!parent) return nullptr;
    return parent;
}

bool Node::is_left_child() const
{
    if (this == this->get_parent()->get_left()) return true;
    else return false;
}

bool Node::is_right_child() const
{
    if (this == this->get_parent()->get_right()) return true;
    else return false;
}


/***  Red Black Tree Class  ***/
RBTree::RBTree(): root(nullptr)
{}

RBTree::RBTree(const RBTree & src)
{
    root = copy_tree(src.root);
}

RBTree & RBTree::operator=(const RBTree & rhs)
{
    if (this == &rhs) return *this;

    destroy_tree(this->root);
    this->root = copy_tree(rhs.root);

    return *this;
}

RBTree::~RBTree()
{
    destroy_tree(root);
}

//Copy tree
Node * RBTree::copy_tree(Node * src_root)
{
    if (!src_root) return nullptr;

    int src_data = src_root->get_data();
    auto root_cpy = new Node(src_data);

    root_cpy->set_left(copy_tree(src_root->get_left()));
    root_cpy->set_right(copy_tree(src_root->get_right()));

    return root_cpy;
}

//Destroy tree wrapper function
void RBTree::destroy_tree()
{
    destroy_tree(root);
    root = nullptr;
}

//Destroy tree helper function
void RBTree::destroy_tree(Node * root)
{
    if (!root) return;

    destroy_tree(root->get_left());
    destroy_tree(root->get_right());
    delete root;
}

//Get root
Node * RBTree::get_root()
{
    if (root) return root;
    else return nullptr;
}

//Insert new data wrapper function
void RBTree::insert_data(int new_data)
{
    Node * n = new Node(new_data);
    root = insert_data(root, n);
    balance_tree(n);
}

//Insert new data helper function
Node * RBTree::insert_data(Node * root_ptr, Node *& n_ptr)
{
    //If tree is empty
    if (!root_ptr) return n_ptr;

    //Logical comparison: Concept data < Concept data
    if (n_ptr->get_data() < root_ptr->get_data())
    {
        root_ptr->set_left(insert_data(root_ptr->get_left(), n_ptr));
        root_ptr->get_left()->set_parent(root_ptr);
    }
        //Logical comparison: Concept data >= Concept data
    else if (n_ptr->get_data() >= root_ptr->get_data())
    {
        root_ptr->set_right(insert_data(root_ptr->get_right(), n_ptr));
        root_ptr->get_right()->set_parent(root_ptr);
    }

    return root_ptr;
}

//Search by name wrapper function
bool RBTree::search_data(int target)
{
    bool match = search_data(root, target);
    return match;
}

//Search by name helper function
bool RBTree::search_data(Node * root_ptr, int target)
{
    if (!root_ptr) return false;

    search_data(root_ptr->get_left(), target);
    if (root_ptr->get_data() == target)
    {
        return true;
    }
    search_data(root_ptr->get_right(), target);

    return false;
}

/*
Attempt rebalancing of tree after new insertion, determine necessary rotations
If parent of new node (n_ptr) is red, apply rebalancing. If the parent's sibling
is black, perform a rotation. The type of rotation depends on the insertion path.
If the insertion path from grandparent to parent to new node is straight
(both L or both R), do a single rotation. If the insertion path is angled
(L->R or R->L), two rotations are required.
 */
void RBTree::balance_tree(Node *& n_ptr)
{
    //Temporary parent & grandparent node pointers
    Node * temp_parent = nullptr;
    Node * temp_gparent = nullptr;

    //New node pointer != tree root; is red; parent is also red
    while (n_ptr != root && get_node_color(n_ptr) == 0
           && get_parent_color(n_ptr) == 0)
    {
        temp_parent = n_ptr->get_parent();
        temp_gparent = temp_parent->get_parent();

        //Node parent is left child of grandparent
        if (temp_parent == temp_gparent->get_left())
        {
            //Initialize parent sibling node pointer
            Node * parent_sibling = temp_gparent->get_right();

            //If parent sibling is red; set to black
            //Set parent to black, set grandparent to red
            if (get_node_color(parent_sibling) == 0)
            {
                parent_sibling->set_color(1);
                temp_parent->set_color(1);
                temp_gparent->set_color(0);
                n_ptr = temp_gparent;
            }
            else
            {
                //New node pointer is right child of parent
                if (n_ptr == temp_parent->get_right())
                {
                    left_rotation(temp_parent);
                    n_ptr = temp_parent;
                    temp_parent = n_ptr->get_parent();
                }

                //Perform right rotation
                right_rotation(temp_gparent);

                //Swap parent and grandparent colors
                int temp_color = get_node_color(temp_gparent);
                temp_gparent->set_color(get_node_color(temp_parent));
                temp_parent->set_color(temp_color);

                //Set new node pointer up to parent
                n_ptr = temp_parent;
            }
        }
            //Node parent is right child of grandparent
        else if (temp_parent == temp_gparent->get_right())
        {
            //Initialize parent sibling node pointer
            Node * parent_sibling = temp_gparent->get_left();

            //If parent sibling is red; set to black
            //Set parent to black, set grandparent to red
            if (get_node_color(parent_sibling) == 0)
            {
                parent_sibling->set_color(1);
                temp_parent->set_color(1);
                temp_gparent->set_color(0);
                n_ptr = temp_gparent;
            }
            else
            {
                //Else new node is left child of parent
                if (n_ptr == temp_parent->get_left())
                {
                    //Perform right rotation; reassign pointers
                    right_rotation(temp_parent);
                    n_ptr = temp_parent;
                    temp_parent = n_ptr->get_parent();
                }

                //Perform left rotation
                left_rotation(temp_gparent);

                //Swap parent and grandparent colors
                int temp_color = temp_gparent->get_color();
                temp_gparent->set_color(temp_parent->get_color());
                temp_parent->set_color(temp_color);

                //Set new node pointer up to parent
                n_ptr = temp_parent;
            }
        }
    }

    //Always set root node to black
    root->set_color(1);
}

//Perform left rotation
void RBTree::left_rotation(Node *& n_ptr)
{
    Node * right_child_ptr = n_ptr->get_right();
    n_ptr->set_right(right_child_ptr->get_left());

    if (n_ptr->get_right())
    {
        n_ptr->get_right()->set_parent(n_ptr);
    }

    right_child_ptr->set_parent(n_ptr->get_parent());

    if (!n_ptr->get_parent())
    {
        root = right_child_ptr;
    }
    else if (n_ptr->is_left_child())
    {
        n_ptr->get_parent()->set_left(right_child_ptr);
    }
    else
    {
        n_ptr->get_parent()->set_right(right_child_ptr);
    }

    right_child_ptr->set_left(n_ptr);
    n_ptr->set_parent(right_child_ptr);
}

//Perform right rotation
void RBTree::right_rotation(Node *& n_ptr)
{
    Node * left_child_ptr = n_ptr->get_left();
    n_ptr->set_left(left_child_ptr->get_right());

    if (n_ptr->get_left())
    {
        n_ptr->get_left()->set_parent(n_ptr);
    }

    left_child_ptr->set_parent(n_ptr->get_parent());

    if (!n_ptr->get_parent())
    {
        root = left_child_ptr;
    }
    else if (n_ptr->is_left_child())
    {
        n_ptr->get_parent()->set_left(left_child_ptr);
    }
    else if (n_ptr->is_right_child())
    {
        n_ptr->get_parent()->set_right(left_child_ptr);
    }

    left_child_ptr->set_right(n_ptr);
    n_ptr->set_parent(left_child_ptr);
}

//Inorder print wrapper function
void RBTree::inorder_traversal()
{
    if (!root)
    {
        cout << "Database is empty" << endl;
        return;
    }

    inorder_traversal(root);
    cout << endl;
}

//Inorder print helper function
void RBTree::inorder_traversal(Node * n_ptr)
{
    if (!n_ptr) return;

    inorder_traversal(n_ptr->get_left());
    cout << n_ptr->get_data() << " ";
    inorder_traversal(n_ptr->get_right());
}

//Get node color
int RBTree::get_node_color(Node *& n_ptr)
{
    if (!n_ptr) return 1;
    else return n_ptr->get_color();
}

//Get node color of parent
int RBTree::get_parent_color(Node *& n_ptr)
{
    if (!n_ptr || !n_ptr->get_parent()) return 1;
    else return n_ptr->get_parent()->get_color();
}