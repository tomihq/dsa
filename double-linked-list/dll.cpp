#include <iostream>

class Node
{
public:
    int value;
    Node* prev;
    Node* next;

    Node(int val) : value(val), prev(nullptr), next(nullptr) {}
};

/* 
    @description An Iterator is a pointer with state that can go forward query actual node and tell us when has ended.
    Note that we are not coupling the Iterator to a DLL. Instead, we're referring a node.
*/
class Iterator {
private:
    Node* currentNode;

public:
    Iterator(Node* first): currentNode(first){}

    Node* current(){
        return currentNode;
    }

    void next(){
        if(currentNode != nullptr){
            currentNode = currentNode->next;
        }
    }

    bool hasNext(){
        return currentNode != nullptr;
    }
};

class DLL
{
private:
    Node *head;
    Node *last;
    int size;

    int incSize()
    {
        size++;
        return size;
    }

public:
    DLL() : head(nullptr), last(nullptr), size(0) {}

    Node* addFirst(int val)
    {
        Node* prevFirst = head;

        Node* newNode = new Node(val);
        head = newNode;
        newNode->next = prevFirst;

        if (prevFirst != nullptr)
            prevFirst->prev = newNode;

        if (last == nullptr)
            last = newNode;

        incSize();

        return newNode;
    }

    Node* addLast(int val)
    {
        Node* prevLast = last;
        Node* newLast = new Node(val);

        if (prevLast != nullptr)
        {
            prevLast->next = newLast;
            newLast->prev = prevLast;
        }

        last = newLast;

        incSize();
        return newLast;
    }

    Node* getHead()
    {
        return head;
    }
};

int main()
{
    DLL dll;

    dll.addFirst(1);
    dll.addFirst(2);
    dll.addLast(3);
    
    Iterator it(dll.getHead());

    while (it.hasNext())
    {   
        std::cout << it.current()->value << std::endl;
        it.next();
    }
}