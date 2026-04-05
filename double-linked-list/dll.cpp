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
class Iterator
{
private:
    Node* currentNode;

public:
    Iterator(Node* first) : currentNode(first) {}

    Node* current()
    {
        return currentNode;
    }

    void next()
    {
        if (currentNode != nullptr)
        {
            currentNode = currentNode->next;
        }
    }

    bool hasNext()
    {
        return currentNode != nullptr;
    }
};

class DLL
{
private:
    Node* head;
    Node* last;
    int size;

    Node* unshift(int nodeId, int val)
    {
        Node *prevFirst = head;

        Node *newNode = new Node(nodeId, val);
        head = newNode;
        newNode->next = prevFirst;

        if (prevFirst != nullptr)
            prevFirst->prev = newNode;

        if (last == nullptr)
            last = newNode;

        size++;
        return newNode;
    }

    Node* push(int nodeId, int val)
    {
        Node *prevLast = last;
        Node *newLast = new Node(nodeId, val);

        if (prevLast != nullptr)
        {
            prevLast->next = newLast;
            newLast->prev = prevLast;
        }

        last = newLast;

        size++;
        return newLast;
    }

    /**
     * @Time-Complexity: O(n)
     */
    Node* find(Node *current, int nodeId)
    {
        if (current == nullptr)
            return nullptr;
        if (current->id == nodeId)
            return current;
        return find(current->next, nodeId);
    }

    Node* update(int nodeId, int value)
    {
        Node *foundNode = find(head, nodeId);
        if (foundNode == nullptr)
            return nullptr;
        foundNode->value = value;

        return foundNode;
    }

    /**
     * @Time-Complexity: O(n)
     */
    Node* removeById(int nodeId)
    {
        if (size == 0)
            return nullptr;

        Node* foundNode = find(head, nodeId);
        if (foundNode == nullptr)
            return nullptr;

        return remove(foundNode);
    }

    /**
     * @Time-Complexity: O(1)
    */
    Node* remove(Node* node){
        if (node->prev != nullptr) node->prev->next = node->next;
        else head = node->next;
        
        if (node->next != nullptr) node->next->prev = node->prev;
        else last = node->prev;
        
        node->prev = nullptr;
        node->next = nullptr;

        size--;

        return node;
    }
public:
    DLL() : head(nullptr), last(nullptr), size(0) {}

    /**
     * @Time-Complexity O(1)
     */
    Node* addFirst(int nodeId, int val)
    {
        return unshift(nodeId, val);
    }

    /*
        @Time-Complexity: O(1)
    */
    Node* addLast(int nodeId, int val)
    {
        return push(nodeId, val);
    }

    Node* getHead()
    {
        return head;
    }

    /**
     * @Time-Complexity: O(n)
     */
    Node* findById(int nodeId)
    {
        return find(head, nodeId);
    }

    /**
     * @Time-Complexity: O(n)
     */
    Node* updateById(int nodeId, int val)
    {
        return update(nodeId, val);
    }
    
    /**
     * @Time-Complexity: O(n) 
    */
    Node* deleteById(int nodeId)
    {
        return removeById(nodeId);
    }

    /**
     * @Time-Complexity: O(1)
     */
    Node* deleteNode(Node* nodeId){
        return remove(nodeId);
    }
};

int main()
{
    DLL dll;

    dll.addFirst(1, 1);
    dll.addFirst(2, 2);
    Node* node = dll.addLast(3, 3);
    dll.updateById(3, 10);
    dll.deleteById(2);
    dll.deleteNode(node);

    Iterator it(dll.getHead());

    while (it.hasNext())
    {
        std::cout << it.current()->value << std::endl;
        it.next();
    }
}