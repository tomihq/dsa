package main

import "fmt"

type Node struct {
	Key   int
	Value int
	Prev  *Node
	Next  *Node
}

type LRUCache struct {
	capacity int
	size     int
	items    map[int]*Node

	// Dummy nodes:
	// head.Next is the MRU node.
	// tail.Prev is the LRU node.
	head *Node
	tail *Node
}

func NewLRUCache(capacity int) *LRUCache {
	head := &Node{}
	tail := &Node{}

	head.Next = tail
	tail.Prev = head

	return &LRUCache{
		capacity: capacity,
		items:    make(map[int]*Node),
		head:     head,
		tail:     tail,
	}
}

func (cache *LRUCache) Get(key int) int {
	node, exists := cache.items[key]
	if !exists {
		return -1
	}

	cache.moveToFront(node)

	return node.Value
}

func (cache *LRUCache) Put(key int, value int) {
	// Existing key: update it and mark it as recently used.
	if node, exists := cache.items[key]; exists {
		node.Value = value
		cache.moveToFront(node)
		return
	}

	// New key.
	node := &Node{
		Key:   key,
		Value: value,
	}

	cache.items[key] = node
	cache.addToFront(node)
	cache.size++

	// Capacity exceeded: evict the LRU node.
	if cache.size > cache.capacity {
		lru := cache.removeLRU()

		delete(cache.items, lru.Key)
		cache.size--
	}
}

// addToFront inserts node immediately after the dummy head.
func (cache *LRUCache) addToFront(node *Node) {
	first := cache.head.Next

	node.Prev = cache.head
	node.Next = first

	cache.head.Next = node
	first.Prev = node
}

// remove disconnects an existing node from the linked list.
func (cache *LRUCache) remove(node *Node) {
	previous := node.Prev
	next := node.Next

	previous.Next = next
	next.Prev = previous

	// Optional cleanup.
	node.Prev = nil
	node.Next = nil
}

// moveToFront marks an existing node as the most recently used.
func (cache *LRUCache) moveToFront(node *Node) {
	cache.remove(node)
	cache.addToFront(node)
}

// removeLRU removes and returns the least recently used node.
func (cache *LRUCache) removeLRU() *Node {
	lru := cache.tail.Prev
	cache.remove(lru)

	return lru
}

func main() {
	cache := NewLRUCache(2)

	operations := []struct {
		name  string
		key   int
		value int
	}{
		{"put", 1, 1},
		{"put", 2, 2},
		{"get", 1, 0},
		{"put", 3, 3},
		{"get", 2, 0},
		{"put", 4, 4},
		{"get", 1, 0},
		{"get", 3, 0},
		{"get", 4, 0},
	}

	results := make([]any, 0, len(operations))

	for _, operation := range operations {
		switch operation.name {
		case "put":
			cache.Put(operation.key, operation.value)
			results = append(results, nil)

		case "get":
			value := cache.Get(operation.key)
			results = append(results, value)
		}
	}

	fmt.Println(results)
}
