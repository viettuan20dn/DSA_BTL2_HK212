#include <iostream>
#include "test.cpp"
using namespace std;
Node::Node()
{
    next=NULL;
}
Node::Node(int value)
{
    this->value=value;
    this->next=NULL;
}
List::List()
{
    size=0;
    root=nullptr;
}
List::~List()
{
    root=NULL;
}
void List::insert(int value)
{
    Node* newNode=new Node(value);
    if (this->size==0) root=newNode;
    else
    {
        newNode->next=root;
        root=newNode;
    }
    size++;
}

int main()
{
    createList();
}