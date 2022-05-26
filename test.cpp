class Node
{
    public:
    int value;
    Node* next;
    Node();
    Node(int value);
};

class List{
    public:
    Node* root;
    int size;
    List();

    void insert(int value);
    ~List();
};
void createList()
{
    List t;
    t.insert(3);
    t.insert(4);
    std::cout<<t.root->value<<" "<<t.root->next->value;
}
