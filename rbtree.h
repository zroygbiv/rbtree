#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

class Node
{
public:
    explicit Node(int data);
    ~Node();
    int get_data() const { return data; }
    int get_color() const { return color; }
    Node * get_left() const;
    Node * get_right() const;
    Node * get_parent() const;
    void set_color(int c) { color = c; }
    void set_parent(Node * new_parent) { parent = new_parent; }
    void set_left(Node * new_left) { left = new_left; }
    void set_right(Node * new_right) { right = new_right; }
    bool is_left_child() const;
    bool is_right_child() const;
private:
    int color;  //red = 0, black = 1
    int data;
    Node * left;
    Node * right;
    Node * parent;
};

class RBTree
{
public:
    RBTree();
    RBTree(const RBTree & src);
    ~RBTree();
    RBTree & operator=(const RBTree & rhs);

    void insert_data(int new_data);
    bool search_data(int target);
    void inorder_traversal();

    Node * get_root();
    void destroy_tree();

private:
    Node * insert_data(Node * root_ptr, Node *& n_ptr);
    bool search_data(Node * root_ptr, int target);
    void inorder_traversal(Node * n_ptr);

    void balance_tree(Node *& n_ptr);
    void left_rotation(Node *& n_ptr);
    void right_rotation(Node *& n_ptr);
    static int get_node_color(Node *& n_ptr);
    static int get_parent_color(Node *& n_ptr);

    Node * copy_tree(Node * src_root);
    void destroy_tree(Node * root);

private:
    Node * root;
};

#endif //RBTREE_RBTREE_H