#include <iostream>
using namespace std;

template <typename T>
class Node
{
public:
    T key;
    Node *left;    //Points to the left child
    Node *sibling; //Points to the sibling
    Node() = default;
    Node(T k, Node *l, Node *sibl) : key(k), left(l), sibling(sibl){};
    void addChild(Node *node);
};

template <typename T>
void Node<T>::addChild(Node *node)
{
    if (left == nullptr)
        left = node;
    else
    {
        node->sibling = left;
        left = node;
    }
    return;
}

template <typename T>
class PairingHeap
{
private:
    Node<T> *root;
    bool empty(Node<T> *node) { return (node == nullptr); }
    Node<T> *Merge(Node<T> *a, Node<T> *b)
    {
        if (a == nullptr)
            return b;
        if (b == nullptr)
            return a;

        if (a->key < b->key)
        {
            a->addChild(b);
            return a;
        }
        else
        {
            b->addChild(a);
            return b;
        }

        return nullptr;
    }

    int top(Node<T> *node){ return node->key; }

    Node<T> *insert(Node<T> *node, int key){ return Merge(node, new Node<T>(key, nullptr, nullptr));}
    
    Node<T> *TwoPassMerge(Node<T> *node)
    {
        if (node == nullptr || node->sibling == nullptr)
            return node;
        else
        {
            Node<T> *a, *b, *newNode;
            a = node;
            b = node->sibling;
            newNode = node->sibling->sibling;

            a->sibling = nullptr;
            b->sibling = nullptr;

            return Merge(Merge(a, b), TwoPassMerge(newNode));
        }

        return nullptr;
    }

    Node<T> *del(Node<T> *node) { return TwoPassMerge(node->left);}

public:
    PairingHeap():root(nullptr){};
    bool empty() { return empty(root); }
    int top(){return top(root);}
    void insert(int key){root = insert(root,key);}
    void del(){root = del(root);}
    void join(PairingHeap other){root = Merge(root, other.root);}
};

int main()
{
    PairingHeap<int> heap1, heap2;
    // heap2.insert(5);
    // heap2.insert(2);
  
    // heap2.insert(6);
    // heap1.insert(1);
    // heap1.insert(3);
    // heap1.insert(4);
      
    // heap1.join(heap2);
    for(int i = 0; i < 20; i++){
        heap1.insert(i);
    }
    // cout << heap1.top() << endl;
    // heap1.del();
    while(!heap1.empty()){
        cout << heap1.top() << " ";
        heap1.del();
    }
    cout << "\n";
    // cout << heap1.top() << endl;
    cout<< (heap1.empty()?"True":"False")<<"\n";

    return 0;
}