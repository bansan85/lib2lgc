# Tree

## Design

Implement a simple tree based on the composite pattern.

A tree is composed with Node.

Loop in not possible by design.

## Function

There only one function to modify a tree:
  - AddChild: add a child to the current node.

There two functions to browse the tree:
  - FindPath: find a path from a node to another one.
  - FindNode: find a node based on the id.

## Technical

A node is identified with a unique id. Insertion will failed if a node already exists with the same id.

Each node stores data in a unique_ptr.

Each children node is stored on a unique_ptr. That's mean that deleting a node and each children will be free. To allow link with parent, pointer is used.

