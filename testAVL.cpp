//* xem la cay AVL co hoat dong khong ?
    Element e(12,0);
    string key="xyz";
    Node* node0= new Node(key,e);

    Element e1(11.09,1);
    string key1="a";
    Node* node1= new Node(key1,e1);

    Element e2(10,0);
    string key2="xz";
    Node* node2= new Node(key2,e2);

    Element e3(11.11,1);
    string key3="z";
    Node* node3= new Node(key3,e3);

    Element e4(100,0);
    string key4="u";
    Node* node4= new Node(key4,e4);

    Element e5(12.07,1);
    string key5="b";
    Node* node5= new Node(key5,e5);

    Element e6(12.3,1);
    string key6="t";
    Node* node6= new Node(key6,e6);

    AVL_Tree tree;
    tree.insert(node0);
    tree.insert(node2);
    tree.insert(node3);
    tree.insert(node1);
    tree.insert(node4);
    tree.insert(node5);
    tree.insert(node6);
    tree.remove("u");
    tree.printAVLTree();
    return;