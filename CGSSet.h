#include <cassert>
#include <memory>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <iostream> 
#include <pthread.h>
#include <bits/stdc++.h>
#include "Mutex.h"

template <typename T>
class CGSSet
{
private:
  class Node
  {
  public:
     Node(T init_value){
      item = init_value;
      key = std::hash<T>()(init_value);
      next = nullptr;
    } 

    T item; 
    size_t key; 
    Node* next; 
  };

  Node* head; 
  Mutex mutex;

   
public:                
  CGSSet()
  {
    mutex.init();
    head = new Node(0);
    if (!head)
    	std::cout << "problem with head " << std::endl;
    head->next = new Node(INT_MAX);
    if (!head->next)
    	std::cout << "problem with head->next " << std::endl;
  }

  ~CGSSet()
  {
    Node *next = nullptr;
    for (Node *current = head; current != nullptr; current = next)
    {
      next = current->next;
      delete current;
    }
  }

  size_t generate_hash(const T& item)
  {
    return std::hash<T>()(item);
  }
  
  bool add(const T& item)
  {
    size_t key = generate_hash(item);
    mutex.lock();
    Node* prev = head;
    Node* curr = head->next;
    
    while (curr->key < key)
    {
      prev = curr;
      curr = curr->next;
      
    }
    if (curr->key == key)
    {
      mutex.unlock();
      return false;
    }
    Node* newNode = new Node(item);
    if (!newNode)
    	std::cout << "problem with insert "  << std::endl;
    newNode->next = curr;
    prev->next = newNode;
    mutex.unlock();
    return true;
  }

  bool remove(const T& item)
  {
    size_t key = generate_hash(item);
    mutex.lock();
    Node* prev = head;
    Node* curr = head->next;
    while (curr->key < key)
    {
      prev = curr;
      curr = curr->next;
    }

    if (curr->key == key)
    {
      prev->next = curr->next;
      delete curr;
      mutex.unlock();
      return true;
    }
    mutex.unlock();
    return false;
  }

  bool contains(const T& item)
  {
    size_t key = generate_hash(item);
    mutex.lock();
    Node* curr = head;
    while (curr->key < key)
    {
      curr = curr->next;
    }

   mutex.unlock();
    return curr->key == key;
  }
};

