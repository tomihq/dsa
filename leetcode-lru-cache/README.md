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

## LRU Cache — Reasoning

## 1. Fast lookup by key

`get(key)` must run in `O(1)` average time and return either the stored value or `-1` if the key does not exist.

Therefore, I need a hash map indexed by key:

```text
key -> stored data
```

A hash map gives:

```text
Lookup: O(1) average
Insert: O(1) average
Delete: O(1) average
```

At this point, the hash map solves the lookup requirement, but it does not tell me which key is the least recently used.

---

## 2. What `put` needs to do

`put(key, value)` has two possible cases.

### Existing key

* Update its value.
* Mark it as the most recently used key.

### New key

* Insert the new key.
* Mark it as the most recently used key.
* If the cache exceeds its capacity, evict the least recently used key.

Therefore, both `get` and `put` modify the usage order.

A successful `get` counts as an access, and every `put` also counts as an access.

---

## 3. The main challenge

The problem is not only finding a key in `O(1)`.

I also need to maintain enough information to answer these questions in `O(1)`:

```text
Which key is the least recently used?
How do I remove it?
How do I mark an arbitrary key as recently used?
```

Using a timestamp in every entry would not be enough by itself.

Although updating a timestamp is `O(1)`, finding the minimum timestamp would require checking every element:

```text
O(capacity)
```

Therefore, I need a structure that explicitly maintains the access order.

---

## 4. First attempt: an array

My first idea was to use an array ordered from most recently used to least recently used:

```text
[MRU, ..., LRU]
```

For example:

```text
[3, 1, 2]
```

The first element would be the most recently used key, and the last element would be the least recently used key.

This makes accessing and removing the LRU element easy:

```text
Last element: O(1)
Remove last element: O(1)
```

I initially considered the array operations to be `O(1)` because the problem states:

```text
capacity <= 3000
```

However, `capacity` is still an input parameter. The running time grows as the capacity grows, even if the input has a maximum allowed value.

Therefore, operations that move or scan up to `capacity` elements are:

```text
O(capacity)
```

not `O(1)`.

The main problem appears when an element in the middle is accessed.

For example:

```text
Before:
[3, 1, 2]

get(1)

After:
[1, 3, 2]
```

To do this with an array, I would need to:

1. Find `1`: `O(capacity)`.
2. Remove it from the middle: `O(capacity)`.
3. Insert it at the beginning: `O(capacity)`.

Even if I stored the array index in another hash map, inserting or removing elements would change the indices of many other entries.

Therefore, an array does not satisfy the required complexity.

---

## 5. Replacing the array with a doubly linked list

I still need a structure that represents this order:

```text
Most recently used -> ... -> Least recently used
```

A doubly linked list provides:

```text
Insert at the head: O(1)
Remove the tail: O(1)
Remove a known node: O(1)
```

The list can be organized as:

```text
HEAD <-> MRU <-> ... <-> LRU <-> TAIL
```

Then:

```text
head.next = most recently used node
tail.prev = least recently used node
```

This solves two important operations:

* Adding a recently used element at the front.
* Finding and removing the least recently used element from the back.

However, there is still another problem.

---

## 6. The doubly linked list alone is not enough

Suppose the list is:

```text
HEAD <-> Node3 <-> Node1 <-> Node2 <-> TAIL
```

and I execute:

```text
get(1)
```

`Node1` is in the middle of the list.

If I only had the linked list, I would need to search for it by traversing the list:

```text
O(capacity)
```

Therefore, the doubly linked list solves fast reordering, but it does not solve fast lookup.

At this point:

* The hash map can find a key in `O(1)`.
* The doubly linked list can move or remove a known node in `O(1)`.

The solution is to connect both structures.

---

## 7. The key idea: the hash map stores node references

Instead of storing:

```text
key -> value
```

the hash map stores:

```text
key -> reference to the corresponding DLL node
```

Each node contains:

```text
Node {
    key
    value
    prev
    next
}
```

Therefore:

```text
HashMap:
key -> Node reference

Doubly linked list:
Node <-> Node <-> Node
```

The hash map acts as an index into the linked list.

It does not store a separate copy of the value. It points directly to the node containing the key and value.

This means that an arbitrary node can be found in `O(1)` average time:

```text
node = map[key]
```

Once I have the node reference, I can remove it from its current position in `O(1)`:

```text
node.prev.next = node.next
node.next.prev = node.prev
```

Then I can insert the same node at the head in `O(1)`.

There is no need to search the linked list.

---

## 8. Example

Assume:

```text
capacity = 2
```

### `put(1, 1)`

Create:

```text
Node1 {
    key: 1
    value: 1
}
```

State:

```text
HashMap:
1 -> Node1

DLL:
HEAD <-> Node1 <-> TAIL
```

`Node1` is both the MRU and the LRU.

---

### `put(2, 2)`

The key does not exist, so create `Node2` and insert it at the head.

```text
HashMap:
1 -> Node1
2 -> Node2

DLL:
HEAD <-> Node2 <-> Node1 <-> TAIL
          MRU       LRU
```

The cache is now full.

---

### `put(3, 3)`

The key does not exist, and the cache is full.

The LRU node is immediately before the tail:

```text
lru = tail.prev
```

Therefore:

```text
lru = Node1
```

Remove it from the hash map using the key stored inside the node:

```text
delete map[Node1.key]
```

Then remove `Node1` from the linked list.

After eviction, create `Node3`, store its reference in the hash map, and insert it at the head.

```text
HashMap:
2 -> Node2
3 -> Node3

DLL:
HEAD <-> Node3 <-> Node2 <-> TAIL
          MRU       LRU
```

---

### `get(2)`

Search the hash map:

```text
node = map[2]
```

The result is a direct reference to `Node2`.

There is no linked-list traversal.

Remove `Node2` from its current position and insert it at the head.

Before:

```text
HEAD <-> Node3 <-> Node2 <-> TAIL
```

After:

```text
HEAD <-> Node2 <-> Node3 <-> TAIL
          MRU       LRU
```

Return:

```text
2
```

---

## 9. Operations

### `get(key)`

```text
1. Search the key in the hash map.
2. If it does not exist, return -1.
3. Obtain the node reference.
4. Move the node to the head of the DLL.
5. Return its value.
```

Complexity:

```text
O(1) average
```

---

### `put(key, value)`

If the key exists:

```text
1. Obtain the node reference from the hash map.
2. Update its value.
3. Move it to the head.
```

If the key does not exist:

```text
1. Create a new node.
2. Store key -> node reference in the hash map.
3. Insert the node at the head.
4. If capacity is exceeded:
   a. Obtain the LRU node using tail.prev.
   b. Remove its key from the hash map.
   c. Remove the node from the DLL.
```

Complexity:

```text
O(1) average
```

---

## 10. Invariants

The implementation must preserve these invariants:

1. Every key in the hash map points to exactly one node in the doubly linked list.
2. Every real node in the doubly linked list has exactly one corresponding hash-map entry.
3. `head.next` is always the most recently used node.
4. `tail.prev` is always the least recently used node.
5. A successful `get` moves the accessed node to the head.
6. Every `put`, whether insertion or update, moves the affected node to the head.
7. The number of real nodes never exceeds the cache capacity.

---

## 11. Final complexity

| Operation               |     Complexity |
| ----------------------- | -------------: |
| Hash-map lookup         | `O(1)` average |
| Insert at DLL head      |         `O(1)` |
| Remove a known DLL node |         `O(1)` |
| Find the LRU node       |         `O(1)` |
| Evict the LRU node      | `O(1)` average |
| `get`                   | `O(1)` average |
| `put`                   | `O(1)` average |
| Space                   |  `O(capacity)` |

The important insight is that the hash map and the doubly linked list are not independent copies of the cache.

The linked list stores the actual nodes and maintains their usage order, while the hash map is an index that gives direct access to those same nodes.

## Edge Cases
- What we're going to do if the key requested (get) doesn't exist?
- What if we put an existing key? Mark it as recently accessed and update its value.
- What if capacity is 1? 
