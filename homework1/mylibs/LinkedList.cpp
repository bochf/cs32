#include "LinkedList.h"

#include <iostream>
#include <string>

using namespace std;

LinkedList::LinkedList() = default;

LinkedList::LinkedList(const LinkedList &rhs) {
  if (nullptr == rhs.head) {
    head    = nullptr;
    m_count = 0;
    return;
  }

  head    = new Node{rhs.head->value, rhs.head->next};
  m_count = rhs.m_count;

  Node *current = head;
  while (nullptr != current->next) {
    current->next = new Node{current->next->value, current->next->next};
    current       = current->next;
  }
}

LinkedList::~LinkedList() {
  Node *tmp;
  while (nullptr != head) {
    tmp = head->next;
    delete head;
    head = tmp;
  }
  m_count = 0;
}

const LinkedList &LinkedList::operator=(const LinkedList &rhs) {
  // copy values from the rhs to myself
  Node  root{"", head}; // a fake node in front of head
  Node *tail = &root;
  Node *pos  = rhs.head;
  while (pos != nullptr) {
    // go through the rhs list, replace value or create new node in the lhs list
    if (tail->next) {
      tail->next->value = pos->value;
    } else {
      tail->next = new Node{pos->value, nullptr};
    }
    tail = tail->next;
    pos  = pos->next;
  }

  // delete rest of elements if I have more
  // create a transient LinkedList object point to the current node,
  // all the extra elements will be deleted in the destructor
  if (tail->next) {
    LinkedList tmp;
    tmp.head = tail->next;
  }

  tail->next = nullptr;
  head       = root.next;
  m_count    = rhs.m_count;
  return *this;
}

void LinkedList::insertToFront(const ItemType &val) {
  head = new Node{val, head};
  ++m_count;
}

void LinkedList::printList() const {
  for (Node *node = head; nullptr != node;) {
    cout << node->value;
    node = node->next;
    if (node) {
      cout << " ";
    }
  }
  cout << endl;
}

bool LinkedList::get(int i, ItemType &item) const {
  if (i < 0 || i >= m_count) {
    return false;
  }

  Node *node = head;
  for (int pos = 0; pos < i; ++pos) {
    if (nullptr == node) {
      return false;
    }
    node = node->next;
  }
  item = node->value;
  return true;
}

void LinkedList::reverseList() {
  if (m_count <= 1)
    return;

  Node *prev(nullptr);
  Node *next;
  while (head != nullptr) {
    next       = head->next;
    head->next = prev;
    prev       = head;
    head       = next;
  }
  head = prev;
}

void LinkedList::printReverse() const {
  if (head) {
    string output(head->value);
    for (Node *node = head->next; nullptr != node; node = node->next) {
      output = node->value + " " + output;
    }
    cout << output;
  }
  cout << endl;
}

void LinkedList::append(const LinkedList &other) {
  Node  root{"", head}; // a fake node in front of head
  Node *tail = &root;
  while (tail->next) {
    tail = tail->next;
  }

  tail->next = other.head;
  while (tail->next) {
    tail->next = new Node{tail->next->value, tail->next->next};
    tail       = tail->next;
  }

  head = root.next;
  m_count += other.size();
}

void LinkedList::swap(LinkedList &other) {
  Node *tmp  = head;
  head       = other.head;
  other.head = tmp;

  m_count ^= other.m_count;
  other.m_count ^= m_count;
  m_count ^= other.m_count;
}

int LinkedList::size() const { return m_count; }