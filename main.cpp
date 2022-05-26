#include <iostream>
#include "StackFrame.cpp"
#include "errors.h"
using namespace std;

void test(string filename) {
    StackFrame *sf = new StackFrame();
    try {
        sf->run(filename);
    }
    catch (exception& e) {
        cout << e.what();
    }
    delete sf;
}

int main() {
    //* Node active ->done
    //* AVL_tree -> done
    //* setup bo nho ->not yet
    //* setup cau lenh dau tien
    test("text.txt");
    return 0;
}