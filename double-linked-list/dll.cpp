#include <iostream>

class Node
{
public:
    int id; 
    int value;
    Node* prev;
    Node* next;

    Node(int nodeId, int val) : id(nodeId), value(val), prev(nullptr), next(nullptr) {}
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


    /**
     * @Time-Complexity: O(n)
    */
    Node* find(Node* current, int nodeId){
        if(current == nullptr) return nullptr;
        if(current -> id == nodeId) return current;
        return find(current -> next, nodeId);
    }

public:
    DLL() : head(nullptr), last(nullptr), size(0) {}

    /**
     * @Time-Complexity O(1)
    */
    Node* addFirst(int nodeId, int val)
    {
        Node* prevFirst = head;

        Node* newNode = new Node(nodeId, val);
        head = newNode;
        newNode->next = prevFirst;

        if (prevFirst != nullptr)
            prevFirst->prev = newNode;

        if (last == nullptr)
            last = newNode;

        incSize();

        return newNode;
    }

    /*
        @Time-Complexity: O(1)
    */
    Node* addLast(int nodeId, int val)
    {
        Node* prevLast = last;
        Node* newLast = new Node(nodeId, val);

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

    Node* findById(int nodeId){
        return find(head, nodeId);
    }
};

int main()
{
    DLL dll;

    dll.addFirst(1, 1);
    dll.addFirst(2, 2);
    dll.addLast(3, 3);

    Node* node = dll.findById(3);
    std::cout << node->value << std::endl;

    Iterator it(dll.getHead());

    while (it.hasNext())
    {   
        std::cout << it.current()->value << std::endl;
        it.next();
    }
}