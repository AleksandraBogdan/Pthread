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
class LSSet
{
private:

  class NodeLazySync
  {
  public:
    NodeLazySync(T init_value){
      item = init_value;
      key = std::hash<T>()(init_value);
      next = nullptr;
      mutex.init();
      marked = false;
    } 

    T item; 
    size_t key; 
    NodeLazySync* next; 
    Mutex mutex;
    bool marked;
  };

  NodeLazySync* head; 
   
public:                
  LSSet()
  {
    head = new NodeLazySync(0);
    if (!head)
      std::cout << "problem with head " << std::endl;
    head->next = new NodeLazySync(INT_MAX);
    if (!head->next)
    	std::cout << "problem with head->next " << std::endl;
  }

  ~LSSet()
  {
    NodeLazySync *next = nullptr;
    for (NodeLazySync *current = head ; current != nullptr; current = next)
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
    while (true)
    {
      NodeLazySync* prev = head;
      NodeLazySync* curr = head->next;
      while (curr->key < key)
      {
        prev = curr;
        curr = curr->next;
      }
      prev->mutex.lock();
      curr->mutex.lock();
      if (validate(prev, curr))
      {
        if (curr->key == key)
        {
          prev->mutex.unlock();
          curr->mutex.unlock();
          return false;
        }
        else
        {
          NodeLazySync* newNode = new NodeLazySync(item);
          if (!newNode)
            std::cout << "problem with insert " << std::endl;
          newNode->next = curr;
          prev->next = newNode;
          prev->mutex.unlock();
          curr->mutex.unlock();
          return true;
        }
      }
      prev->mutex.unlock();
      curr->mutex.unlock();
    }
  }
  

 bool remove(const T& item)
  {
    size_t key = generate_hash(item);
    while (true)
    {
      NodeLazySync* prev = head;
      NodeLazySync* curr = head->next;
      while (curr->key < key)
      {
        prev = curr;
        curr = curr->next;
      }

      prev->mutex.lock();
      curr->mutex.lock();
      if (validate(prev, curr))
      {
        if (curr->key == key)
        {
          curr->marked = true;
         
          prev->mutex.unlock();
          curr->mutex.unlock();
           prev->next = curr->next;
          delete curr;
          return true;
        }
        else
        {
          prev->mutex.unlock();
          curr->mutex.unlock();
          return false;
        }
      }
      prev->mutex.unlock();
      curr->mutex.unlock();
    }
  }

   bool contains(const T& item)
  {
    size_t key = generate_hash(item);
    NodeLazySync* curr = head;
    while(curr->key < key)
      curr = curr->next;
    return curr->key == key && !curr->marked;
  }

   bool validate(NodeLazySync* prev, NodeLazySync* curr)
  {
    return !prev->marked && !curr->marked && prev->next == curr;
  }

};
