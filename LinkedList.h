#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"

class LinkedList
{
public:
    LinkedList();
    ~LinkedList();
    void add_node(Stock *data);
    void delete_node(const std::string &id); 
    Node *search_node(const std::string &id); 
    Node *get_head() const; 
    void sort();
    void swap(Stock *&a, Stock *&b);
    void setDefaultValue(Node* head, int defaultValue);
  

private:
    // the beginning of the list
    Node* head;
  
    // how many nodes are there in the list?
    //unsigned count;
};

#endif  // LINKEDLIST_H































// #ifndef LINKEDLIST_H
// #define LINKEDLIST_H
// #include "Node.h"


// class LinkedList
// {
// public:
//     LinkedList();

//     ~LinkedList();
//     //added methods
//     void add_node(Stock *data);
//     void LinkedList::delete_node(const std::string &id);
//     Node *LinkedList::search_node(const std::string &id);
//     Node *LinkedList::get_head() const ;

//     // more functions to be added perhaps...

// private:
//     // the beginning of the list
//     Node* head;
  
//     // how many nodes are there in the list?
//     //unsigned count;
// };

// #endif  // LINKEDLIST_H


