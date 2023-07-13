#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <string>

typedef std::string ItemType;

struct Node {
  ItemType value;
  Node    *next;
};

class LinkedList {
private:
  Node *head    = nullptr;
  int   m_count = 0; // number of elements in the list

public:
  // default constructor
  LinkedList();
  // copy constructor
  LinkedList(const LinkedList &rhs);
  // Destroys all the dynamically allocated memory
  // in the list.
  virtual ~LinkedList();
  // assignment operator
  const LinkedList &operator=(const LinkedList &rhs);
  // Inserts val at the front of the list
  void insertToFront(const ItemType &val);
  // Prints the LinkedList
  void printList() const;
  // Sets item to the value at position i in this
  // LinkedList and return true, returns false if
  // there is no element i
  bool get(int i, ItemType &item) const;
  // Reverses the LinkedList
  void reverseList();
  // Prints the LinkedList in reverse order
  void printReverse() const;
  // Appends the values of other onto the end of this
  // LinkedList.
  void append(const LinkedList &other);
  // Exchange the contents of this LinkedList with the other
  // one.
  void swap(LinkedList &other);
  // Returns the number of items in the Linked List.
  int size() const;

private:
  // disable move constructor and move assignment
  LinkedList(const LinkedList &&rhs)            = delete;
  LinkedList &operator=(const LinkedList &&rhs) = delete;
};

#endif