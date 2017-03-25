// main.cpp
// Matt Grant
// Compile with std=c++14
#include"Perceptron.h"
#include<vector>
using std::vector;
#include<iostream>
using std::cout;
using std::cin;
using std::endl;

void nodeTest();
void pTest();
void netTest();

int main(){
    //nodeTest();
    //pTest();
    netTest();
    return 0;
}

void pTest(){
    Perceptron p;
    int b0 = p.createBNode(1);
    int b1 = p.createBNode(1);
    int b2 = p.createBNode(3);
    int b3 = p.createSNode();
    int b4 = p.createNode([](vector<float> x) -> float{
            float z = 0;
            for(auto i : x){ z += i; }
            return z;
        });
    
    p.linkNode(b0, b1, 1);
    p.linkNode(b0, b2, 1);
    p.linkNode(b2, b3, 1);
    p.linkNode(b3, b4, 1);

    p.printLinks();

}

void netTest(){
    Perceptron p;
    int b1 = p.createBNode(0);
    int b2 = p.createBNode(0);
    int b3 = p.createBNode(2);
    int b4 = p.createSNode();

    p.linkNode(b1, b3, 1);
    p.linkNode(b2, b3, 2);
    p.linkNode(b1, b4, 1);
    p.linkNode(b2, b4, 1);

    p.linkInput(b1);
    p.linkInput(b2);

    p.printLinks();  

    vector<float> i;
    i.push_back(1);
    i.push_back(2);

    map<int, float> o = p.run(i);

    for(auto i : o){
        cout << "Node " << i.first << " outputs " << i.second << endl;
    }  
    
}