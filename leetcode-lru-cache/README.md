# LRU Cache
## Description
Implement an LRUCache that supports O(1) average-time get(key) returning the value or -1 and put(key, value) which inserts or updates a key and, if capacity is exceeded, evicts the least-recently-used key.

Input:
capacity = 2, operations = [
  ["put", 1, 1],
  ["put", 2, 2],
  ["get", 1],
  ["put", 3, 3],
  ["get", 2],
  ["put", 4, 4],
  ["get", 1],
  ["get", 3],
  ["get", 4]
]

Output:
[null, null, 1, null, -1, null, -1, 3, 4]

Explanation: put(1,1) and put(2,2) fill cache. get(1) returns 1 and marks it recent. put(3,3) evicts key 2 (LRU). get(2) returns -1 (evicted). put(4,4) evicts key 1 (LRU). get(1) returns -1. get(3) and get(4) return their values.

Constraints:

1 <= capacity <= 3000
0 <= key <= 10^4
0 <= value <= 10^5
At most 2 * 10^5 calls to get and put
Both get and put must run in O(1) average time complexity

Note that capacity, key and value are parameters. They could be any value. 

## Reasoning
get(key): O(1) then I need to have a Hash Map or something like that by key, -1 if it's not present.

    put(key, value) inserts or update a key.
    Insert:
        1. if CAPACITY == FULL, remove LRU key, then INSERT.
        2. if CAPACITY != FULL, then INSERT.

    Both get/put are O(1), that means I need to compute the LRU in O(1)

    Update: update the value of the the key stored.

    Challenge: to maintain the LRU track updated based in the Cache Storage status. 

    Solving: we cannot compare each data in the cache with a timestamp LRU field because that would be O(n). We need to have a value so in O(1) tell US which element is the LRU based in the cache.
    The problem here is: how could I notice in O(1) which is the LRU element? I need to insert in O(1) and get the "last element" in O(1) -> Double Linked List!

    Constraints:
    - Capacity is 1 <= capacity <= 3000, so theorically, if we do a lookup we're working with constant time O(1).
    - Key is 0 <= key <= 10^4, so, the maximum value is big.
    - Value is 0 <= value <= 10^5

    Examples:
    Capacity = 3, Operations = [
        ["put", 1, 1],
        ["put", 2, 2],
        ["get", 1],
        ["put", 3, 3],
        ["get", 1],
        ["get", 2],
        ["get", 3],
        ["get", 1],
        ["put", 4, 4]
    ]

    ["put", 4, 4] evicts ["put", 2, 2] as capacity is full and 2 is the LRU.

    But, is not exactly the same problem if we need to maintain and search a value in the Double Linked List? Imaginate you accessed a value, but is in the middle of the Double Linked List, you'll need to search it -> O(n).

    Yes, but the trick here (I think) is to do not allow the Hash Map to store the (key, value), instead, (key, NodeRefToDoubleLinkedList).

    So, for example: Capacity = 2
    Insert "1", 1
    Node1: key: "1", val: 1 
    HashMap: {"1", Node1 Ref}
    DoubleLinkedList: {Node1}

    Insert "2", 2
    Already exists? No. Operation: Insert, Capacity = Full? No. 
    Node2: key: "2", val: 2
    HashMap: {"2", Node2 Ref}
    DoubleLinkedList: {Node2, Node1}

    Insert "3", 3
    Already exists? No. Operation: Insert, Capacity = Full? Yes. Evict the LRU
    Find LRU: Node1. Remove it from the HashMap using the key, then remove it from the DoubleLinkedList.
    Then,
    Node3: key: "3", val: 3
    HashMap: {"3", Node3 Ref}
    DoubleLinkedList: {Node3, Node2}

    Get "2"
    Already exists? Yes. Operation: Retrieve.
    Search in HashMap by Key.
    Update the DoubleLinkedList, the new head of the DoubleLinkedList is Node2.

    Note that EACH PUT is consider an access and in the hashmap we save the POINTER to the element of the Double Linked List, so we can use it in operations such as this.head...



## Edge Cases
- What we're going to do if the key requested (get) doesn't exist?
- What if we put an existing key? Mark it as recently accessed and update its value.
- What if capacity is 1? 
