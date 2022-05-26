#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
#include <stack>

/*
* Khai bao 1 phan tu stack hay 1 toán hang
*/
class Element
{
private:
    float value;
    int typeCode;

public:
    Element() {}
    Element(float value, int typeCode);
    
    void setTCode(const int&);
    void setValue(const float&);
    float getValue();   
    int getTCode();

    bool operator==(const Element &e);
    bool operator<(const Element &e);
    bool operator>(const Element &e);
    friend Element operator*(const Element&e1,const Element&e2);
    friend Element operator+(const Element&e1,const Element&e2);
    friend Element operator-(const Element&e1,const Element&e2);
    friend Element operator/(const Element&e1,const Element&e2);
    friend Element operator&(const Element&e1,const Element&e2);
    friend Element operator|(const Element&e1,const Element&e2);
};

enum BalanceValue
{
    LH = -1,
    EH = 0,
    RH = 1
};
class AVL_Tree;

class Node
{
public:
    std::string key;
    Element data;
	BalanceValue balance;
	Node* left;
	Node* right;
public:
    Node();
	Node(const std::string& key,const Element& data);
};

//* begin AVL Tree
class AVL_Tree{
private:
	Node* root;
    int size;
protected:
	Node* rotateLeft(Node* &node);
	Node* rotateRight(Node* &node);
	Node* leftBalance(Node* &node,bool &taller);
    Node* rightBalance(Node* &node,bool &taller);
    Node* insertRec(Node *&node, Node* &newNode, bool &taller);
    Node* removeRightBalance(Node* &node,bool& shorter);
    Node* removeLeftBalance(Node* &node,bool& shorter);
    Node* removeRec(Node*& node,const std::string &delKey,bool& shorter,bool& success);
    
    public:
	AVL_Tree();
	void insert(Node* &newNode);
	void remove(const std::string &delKey);
	void clear(Node* &head);
    int getSize();
    Node* getParent(const std::string &var);
    Node* getVar(const std::string &var);
    bool isHead(const std::string &var);
	~AVL_Tree();
	
};


//* end AVL Tree
class StackFrame
{
    AVL_Tree* tree;      // khong gian luu tru du lieu cuc bo
    int line;   // cho biet dong lenh dang chay

public:
    StackFrame();
    void run(std::string filename);
    ~StackFrame();
};

#endif // !__STACK_FRAME_H__