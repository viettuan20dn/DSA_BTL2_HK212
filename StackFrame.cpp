#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

/*
 * Khai bao 1 phan tu stack hay 1 toán hang
 */
Element::Element(float value, int typeCode)
{
    this->value = value;
    this->typeCode = typeCode;
}
void Element::setTCode(const int &TCode)
{
    this->typeCode = TCode;
}
void Element::setValue(const float &value)
{
    this->value = value;
}
int Element::getTCode()
{
    return this->typeCode;
}
float Element::getValue()
{
    return this->value;
}
bool Element::operator==(const Element &element)
{
    return (this->value == element.value);
}
bool Element::operator<(const Element &element)
{
    return (this->value < element.value && this->typeCode == element.typeCode);
}
bool Element::operator>(const Element &element)
{
    return (this->value > element.value && this->typeCode == element.typeCode);
}
Element operator*(const Element &e1, const Element &e2)
{
    return Element(e1.value * e2.value, e1.typeCode);
}
Element operator+(const Element &e1, const Element &e2)
{
    return Element(e1.value + e2.value, e1.typeCode);
}
Element operator-(const Element &e1, const Element &e2)
{
    return Element(e1.value - e2.value, e1.typeCode);
}
Element operator/(const Element &e1, const Element &e2)
{
    if (e1.typeCode == 0)
        return Element((int)(e1.value / e2.value), 0);
    else
    {
        return Element(e1.value / e2.value, 1);
    }
}
Element operator&(const Element &e1, const Element &e2)
{
    return Element((int)e1.value & (int)e2.value, 0);
}
Element operator|(const Element &e1, const Element &e2)
{
    return Element((int)e1.value | (int)e2.value, 0);
}

Node::Node()
{
    left = right = NULL;
}
Node::Node(const string &key, const Element &data)
{
    this->key = key;
    this->data = data;
    this->balance = EH;
    this->left = this->right = NULL;
}

//* - operation AVL
Node *AVL_Tree::rotateLeft(Node *&node)
{
    Node *tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    return tmp;
}
Node *AVL_Tree::rotateRight(Node *&node)
{
    Node *tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    return tmp;
}
Node *AVL_Tree::leftBalance(Node *&node, bool &taller)
{
    Node *leftTree = node->left;
    if (leftTree->balance == LH)
    {
        node->balance = EH;
        leftTree->balance = EH;
        node = rotateRight(node);
        taller = false;
    }
    else
    { // leftTree - RH
        Node *rightOfLeftTree = leftTree->right;
        if (rightOfLeftTree->balance == LH)
        {
            leftTree->balance = EH;
            node->balance = RH;
        }
        else if (rightOfLeftTree->balance == EH)
        {
            leftTree->balance = EH;
            node->balance = EH;
        }
        else
        {
            leftTree->balance = LH;
            node->balance = EH;
        }
        rightOfLeftTree->balance = EH;
        taller = false;
        node->left = rotateLeft(leftTree);
        node = rotateRight(node);
    }
    return node;
}
Node *AVL_Tree::rightBalance(Node *&node, bool &taller)
{
    Node *rightTree = node->right;
    if (rightTree->balance == RH)
    {
        node->balance = EH;
        rightTree->balance = EH;
        node = rotateLeft(node);
        taller = false;
    }
    else // rightTree LH
    {
        Node *leftOfRightTree = rightTree->left;
        if (leftOfRightTree->balance == LH)
        {
            rightTree->balance = RH;
            node->balance = EH;
        }
        else if (leftOfRightTree->balance == EH)
        {
            rightTree->balance = EH;
            node->balance = EH;
        }
        else
        {
            rightTree->balance = EH;
            node->balance = LH;
        }
        leftOfRightTree->balance = EH;
        taller = false;
        node->right = rotateRight(rightTree);
        node = rotateLeft(node);
    }
    return node;
}
Node *AVL_Tree::insertRec(Node *&node, Node *&newNode, bool &taller)
{
    if (node == NULL)
    {
        node = newNode;
        taller = true;
        this->size++;
        return node;
    }
    if (node->key > newNode->key)
    {
        node->left = insertRec(node->left, newNode, taller);
        if (taller)
        {
            if (node->balance == RH)
            {
                node->balance = EH;
                taller = false;
            }
            else if (node->balance == EH)
            {
                node->balance = LH;
            }
            else // LH + taller
            {
                node = leftBalance(node, taller);
            }
        }
    }
    else
    {
        node->right = insertRec(node->right, newNode, taller);
        if (taller)
        {
            if (node->balance == LH)
            {
                node->balance = EH;
                taller = false;
            }
            else if (node->balance == EH)
            {
                node->balance = RH;
            }
            else
            {
                node = rightBalance(node, taller);
            }
        }
    }
    return node;
}
Node *AVL_Tree::removeRightBalance(Node *&node, bool &shorter)
{
    /*
     * case 1: no rebalance
     */
    if (node->balance == LH)
    {
        node->balance = EH;
        return node;
    }
    if (node->balance == EH)
    {
        node->balance = RH;
        shorter = false;
        return node;
    }

    // * node->balance==RH;
    /*
     * case 2: rebalance
     *        + rotate once
     *		 + rotate twice
     */

    // * rotate once
    Node *rightNode = node->right;
    if (rightNode->balance != LH)
    {
        if (rightNode->balance == EH)
        {
            rightNode->balance = LH;
            node->balance = RH;
            shorter = false;
        }
        else
        {
            // rightNode->balance = RH
            rightNode->balance = EH;
            node->balance = EH;
        }
        node = rotateLeft(node);
    }
    else
    {
        // *rightNode->balance==LH
        // *rotate twice
        Node *leftOfRightNode = rightNode->left;
        if (leftOfRightNode->balance == LH)
        {
            rightNode->balance = RH;
            node->balance = EH;
        }
        else if (leftOfRightNode->balance == EH)
        {
            node->balance = EH;
            rightNode->balance = EH;
        }
        else
        {
            //* leftOfRightNode->balance==RH
            rightNode->balance = EH;
            node->balance = LH;
        }

        leftOfRightNode->balance = EH;
        node->right = rotateRight(rightNode);
        node = rotateLeft(node);
    }

    return node;
}
Node *AVL_Tree::removeLeftBalance(Node *&node, bool &shorter)
{
    //* case1: no reblance
    if (node->balance == RH)
    {
        node->balance = EH;
        return node;
    }
    if (node->balance == EH)
    {
        node->balance = LH;
        shorter = false;
        return node;
    }

    //* case2: reblance once
    // node->balance==LH;
    Node *leftNode = node->left;
    if (leftNode->balance != RH)
    {
        if (leftNode->balance == EH)
        {
            leftNode->balance = RH;
            node->balance = LH;
            shorter = false;
        }
        else
        {
            //* leftNode=LH
            leftNode->balance = EH;
            node->balance = EH;
        }
        node = rotateRight(node);
    }
    else
    {
        //* case 3: reblance twice
        //* leftNode->balance= RH
        Node *rightOfLeftNode = leftNode->right;
        if (rightOfLeftNode->balance == RH)
        {
            leftNode->balance = LH;
            node->balance = EH;
        }
        else if (rightOfLeftNode->balance == EH)
        {
            node->balance = EH;
            leftNode->balance = EH;
        }
        else
        {
            //* rightOfLeftNode->balance=LH
            leftNode->balance = EH;
            node->balance = RH;
        }
        rightOfLeftNode->balance = EH;
        node->left = rotateLeft(leftNode);
        node = rotateRight(node);
    }
    return node;
}
Node *AVL_Tree::removeRec(Node *&node, const string &delKey, bool &shorter, bool &success)
{
    if (node == NULL)
        return NULL;

    if (node->key > delKey)
    {
        node->left = removeRec(node->left, delKey, shorter, success);
        if (shorter)
        {
            node = removeRightBalance(node, shorter);
        }
    }
    else if (node->key < delKey)
    {
        node->right = removeRec(node->right, delKey, shorter, success);
        if (shorter)
        {
            node = removeLeftBalance(node, shorter);
        }
    }
    else
    {
        Node *tmp = node;
        if (node->right == NULL)
        {
            node = node->left;
            delete tmp;
            shorter = true;
            success = true;
        }
        else if (node->left == NULL)
        {
            node = node->right;
            delete tmp;
            shorter = true;
            success = true;
        }
        else
        {
            tmp = node->left;
            while (tmp->right != NULL)
            {
                tmp = tmp->right;
            }
            node->data = tmp->data;
            node->key = tmp->key;
            node->left = removeRec(node->left, tmp->key, shorter, success);
            if (shorter)
            {
                node = removeRightBalance(node, shorter);
            }
        }
        this->size--;
    }
    return node;
}
AVL_Tree::AVL_Tree()
{
    this->root = NULL;
    this->size = 0;
}
void AVL_Tree::insert(Node* &newNode)
{
    bool flag = false;
    this->root = insertRec(root, newNode, flag);
}
void AVL_Tree::remove(const string &delKey)
{
    bool shorter = false;
    bool success = false;
    removeRec(root, delKey, shorter, success);
}
void AVL_Tree::clear(Node *&head)
{
    if (head != NULL)
    {
        Node *left = head->left;
        Node *right = head->right;
        clear(left);
        clear(right);
        delete head;
        head = NULL;
    }
}
int AVL_Tree::getSize()
{
    return this->size;
}
Node *AVL_Tree::getParent(const string &var)
{
    Node *parent = NULL;
    Node *tmp = this->root;
    while (tmp)
    {
        if (tmp->key > var)
        {
            parent = tmp;
            tmp = tmp->left;
        }
        else if (tmp->key < var)
        {
            parent = tmp;
            tmp = tmp->right;
        }
        else
        {
            return parent;
        }
    }
    return NULL;
}
Node *AVL_Tree::getVar(const string &var)
{
    Node *tmp = this->root;
    while (tmp)
    {
        if (tmp->key > var)
        {
            tmp = tmp->left;
        }
        else if (tmp->key < var)
        {
            tmp = tmp->right;
        }
        else
        {
            return tmp;
        }
    }
    return NULL;
}
bool AVL_Tree::isHead(const string &var)
{
    if (this->root == NULL)
        return false;
    return this->root->key == var;
}

AVL_Tree::~AVL_Tree()
{
    clear(this->root);
}
//* end AVL TREE

int NumberOfCommand(string s)
{
    int No = 0;
    if (s.find("add") != string::npos)
        No = 1;
    else if (s.find("sub") != string::npos)
        No = 3;
    else if (s.find("mul") != string::npos)
        No = 5;
    else if (s.find("div") != string::npos)
        No = 7;
    else if (s == "irem")
        No = 9;
    else if (s.find("neg") != string::npos)
        No = 10;
    else if (s == "iand")
        No = 12;
    else if (s == "ior")
        No = 13;
    else if (s.find("neq") != string::npos)
        No = 16;
    else if (s.find("eq") != string::npos)
        No = 14;
    else if (s.find("lt") != string::npos)
        No = 18;
    else if (s.find("gt") != string::npos)
        No = 20;
    else if (s == "ibnot")
        No = 22;
    else if (s.find("const") != string::npos)
        No = 23;
    else if (s.find("load") != string::npos)
        No = 25;
    else if (s.find("store") != string::npos)
        No = 27;
    else if (s == "i2f")
        No = 29;
    else if (s == "f2i")
        No = 30;
    else if (s == "top")
        No = 31;
    else if (s.find("val") != string::npos)
        No = 32;
    else if (s.find("par") != string::npos)
        No = 33;
    return No;
}

//* StackFrame
StackFrame::StackFrame()
{
    this->tree = new AVL_Tree();
    this->line = 0;
}

void StackFrame::run(string filename)
{
    ifstream ifs(filename);
    if (!ifs)
    {
        throw out_of_range("Error: file not opened.");
    }
    stack<Element> st; // operand stack
    //* gia thiet: cac lenh hoan toan dung ngu phap
    string commandLine;

    while (getline(ifs, commandLine))
    {
        this->line++;
        int No = NumberOfCommand(commandLine);
        switch (No)
        {
        case 1: //* iadd and fadd- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();

            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // iadd
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // iadd->fadd
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // thuc hien phep cong
            st.push(e1 + e2);

            break;
        }
        case 3: //* isub and fsub- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // isub
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // isub->fsub
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // Thuc hien phep tru
            // boi vi phan tu tren cung la toan tu thu 2
            st.push(e2 - e1);

            break;
        }
        case 5: //* imul and fmul- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();

            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // imul
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // imul->fmul
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // thuc hien phep nhan
            st.push(e1 * e2);

            break;
        }
        case 7: //* idiv and fdiv- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // idiv
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // idiv->fdiv
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            if (e1.getValue() == 0)
                throw DivideByZero(line);
            st.push(e2 / e1);

            break;
        }
        case 9: //* irem- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            // Kiem tra kieu du lieu cua toan hang
            if (e1.getTCode() || e2.getTCode())
            {
                throw TypeMisMatch(line);
            }
            else
            {
                if (e1.getValue() == 0)
                    throw DivideByZero(line);
                st.push(e2 - ((e2 / e1) * e1));
            }
            break;
        }
        case 10: //* ineg and fneg- Done
        {
            // kiem tra stack
            if (st.empty())
                throw StackEmpty(line);

            // Lay phan tu dau ra
            Element e = st.top();
            st.pop();

            // kiem tra kieu du lieu
            if (commandLine[0] == 'i' && e.getTCode() == 1)
                throw TypeMisMatch(line);
            if (commandLine[0] == 'f' && e.getTCode() == 0)
            {
                e.setTCode(1);
                e.setValue((float)e.getValue());
            }

            // thuc hien dao dau
            e.setValue((-1) * e.getValue());
            st.push(e);
            break;
        }
        case 12: //* iand- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            // Kiem tra kieu du lieu cua toan hang
            if (e1.getTCode() || e2.getTCode())
            {
                throw TypeMisMatch(line);
            }
            else
                st.push(e1 & e2);

            break;
        }
        case 13: //* ior- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            // Kiem tra kieu du lieu cua toan hang
            if (e1.getTCode() || e2.getTCode())
            {
                throw TypeMisMatch(line);
            }
            else
                st.push(e1 | e2);

            break;
        }
        case 14: //* ieq and feq- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // ieq
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // ieq->feq
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // Thuc hien so sanh va day ket qua vao ngan xep
            Element e;
            e.setTCode(0);
            if (e2 == e1)
                e.setValue(1);
            else
                e.setValue(0);
            st.push(e);

            break;
        }
        case 16: //* ineq and fneq- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            if (commandLine[0] == 'i') // ineq
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // ineq->fneq
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // Thuc hien so sanh va day ket qua vao ngan xep
            Element e;
            e.setTCode(0);
            if (e2 == e1)
                e.setValue(0);
            else
                e.setValue(1);
            st.push(e);

            break;
        }
        case 18: //* ilt and flt- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            // Kiem tra kieu du lieu cua toan hang
            if (commandLine[0] == 'i') // ilt
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // ilt->flt
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // Thuc hien so sanh va day ket qua vao ngan xep
            Element e;
            e.setTCode(0);
            if (e2 < e1)
                e.setValue(1);
            else
                e.setValue(0);
            st.push(e);

            break;
        }
        case 20: //* igt and fgt- Done
        {
            // Lay lan luot 2 phan tu dau cua ngan xep
            if (st.empty())
                throw StackEmpty(line);
            Element e1 = st.top();
            st.pop();
            if (st.empty())
                throw StackEmpty(line);
            Element e2 = st.top();
            st.pop();

            // Kiem tra kieu du lieu cua toan hang
            if (commandLine[0] == 'i') // ilt
            {
                // Kiem tra kieu du lieu cua toan hang
                if (e1.getTCode() || e2.getTCode())
                {
                    throw TypeMisMatch(line);
                }
            }
            else // ilt->flt
            {
                // Thuc hien ep kieu int->float(neu co)
                if (e1.getTCode() == 0)
                {
                    e1.setTCode(1);
                    e1.setValue((float)e1.getValue());
                }
                if (e2.getTCode() == 0)
                {
                    e2.setTCode(1);
                    e2.setValue((float)e2.getValue());
                }
            }

            // Thuc hien so sanh va day ket qua vao ngan xep
            Element e;
            e.setTCode(0);
            if (e2 > e1)
                e.setValue(1);
            else
                e.setValue(0);
            st.push(e);

            break;
        }
        case 22: //* ibnot- Done
        {
            // kiem tra stack
            if (st.empty())
                throw StackEmpty(line);

            // Lay phan tu dau ra
            Element e = st.top();
            st.pop();

            // kiem tra kieu du lieu
            if (e.getTCode() == 1)
                throw TypeMisMatch(line);

            // thuc hien so sanh
            if (e.getValue() == 0)
                st.push(Element(1, 0));
            else
                st.push(Element(0, 0));
            break;
        }
        case 23: //* iconst and fconst- Done
        {
            //* check stack full ?
            if (st.size() == OPERAND_STACK_MAX_SIZE)
                throw StackFull(line);

            //* get argument
            unsigned int iSpace = commandLine.find(" ");
            Element e;
            if (commandLine[0] == 'i')
            {
                int thamSo = stoi(commandLine.substr(iSpace + 1));
                e.setValue(thamSo);
                e.setTCode(0);
            }
            else
            {
                //* commandLine[0]=='f'
                float thamSo = stof(commandLine.substr(iSpace + 1));
                e.setValue(thamSo);
                e.setTCode(1);
            }
            //* assumption: argument always is true

            st.push(e);
            break;
        }
        case 25: //* iload and fload- Done
        {
            // get key
            unsigned int iSpace = commandLine.find(" ");
            string var = commandLine.substr(iSpace + 1);

            // get key
            Node *key = tree->getVar(var);

            if (!key)
                throw UndefinedVariable(line);

            // check data type
            if ((commandLine[0] == 'i' && key->data.getTCode() == 1) ||
                (commandLine[0] == 'f' && key->data.getTCode() == 0))
                throw TypeMisMatch(line);

            // check stack full
            if (st.size() == OPERAND_STACK_MAX_SIZE)
                throw StackFull(line);

            // copy->delete in memory->add into stack
            Element newE = key->data;
            tree->remove(var);
            st.push(newE);

            break;
        }
        case 27: //* istore and fstore- Done
        {
            //* check empty stack
            if (st.empty())
                throw StackEmpty(line);

            //* get top stack
            Element e = st.top();
            st.pop();

            //* check type of operand
            if ((commandLine[0] == 'i' && e.getTCode() == 1) ||
                (commandLine[0] == 'f' && e.getTCode() == 0))
                throw TypeMisMatch(line);

            //* get key
            unsigned int iSpace = commandLine.find(" ");
            string var = commandLine.substr(iSpace + 1);

            //* check su ton tai cua no
            Node *node = tree->getVar(var);
            if (node != NULL)
            {
                node->data = e;
            }
            else
            {
                //* check size khong gian luu tru cuc bo
                if (tree->getSize() == LOCAL_VARIABLE_SPACE_SIZE)
                    throw LocalSpaceFull(line);

                //* store
                Node *newNode = new Node(var, e);
                tree->insert(newNode);
            }
            break;
        }
        case 29: //* i2f- Done
        {
            // Kiem tra stack
            if (st.empty())
                throw StackEmpty(line);

            // lay ra phan tu dau tien
            Element e = st.top();
            st.pop();

            // kiem tra kieu du lieu
            if (e.getTCode() == 1)
                throw TypeMisMatch(line);

            // Thuc hien convert int -> float
            e.setTCode(1);
            e.setValue((float)e.getValue());
            st.push(e);

            break;
        }
        case 30: //* f2i- Done
        {
            // Kiem tra stack
            if (st.empty())
                throw StackEmpty(line);

            // lay ra phan tu dau tien
            Element e = st.top();
            st.pop();

            // kiem tra kieu du lieu
            if (e.getTCode() == 0)
                throw TypeMisMatch(line);

            // Thuc hien convert float -> int
            e.setTCode(0);
            e.setValue((int)e.getValue());
            st.push(e);

            break;
        }
        case 31: //* top- Done
        {
            if (st.empty())
            {
                throw StackEmpty(line);
            }
            if (st.top().getTCode() == 0)
            {
                cout << (int)st.top().getValue() << endl;
            }
            else
            {
                cout << st.top().getValue() << endl;
            }
            break;
        }
        case 32: //* val- Done
        {
            //* Memory empty?
            if (tree->getSize() == 0)
                throw UndefinedVariable(line);

            //* get key
            unsigned int iSpace = commandLine.find(" ");
            string var = commandLine.substr(iSpace + 1);

            //* get Node
            Node *node = tree->getVar(var);
            if (node == NULL)
                throw UndefinedVariable(line);

            //* print
            if (node->data.getTCode() == 0)
            {
                cout << (int)node->data.getValue();
            }
            else
            {
                cout << node->data.getValue();
            }
            cout << endl;
            break;
        }
        case 33: //* par- Done
        {
            //* tree empty
            if (tree->getSize() == 0)
                throw UndefinedVariable(line);

            //* get var
            unsigned int iSpace = commandLine.find(" ");
            string var = commandLine.substr(iSpace + 1);

            Node *parent = tree->getParent(var);
            if (parent == NULL)
            {
                if (tree->isHead(var))
                {
                    cout << "null";
                }
                else
                {
                    throw UndefinedVariable(line);
                }
            }
            else
                cout << parent->key;
            cout << endl;
            break;
        }
        default:
            break;
        }
    }

    ifs.close();
}
StackFrame::~StackFrame()
{
    delete tree;
}