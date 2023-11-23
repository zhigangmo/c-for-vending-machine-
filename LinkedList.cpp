#include "LinkedList.h"

LinkedList::LinkedList() {
    head = nullptr;
}

//iterate through the nodes and delete pointer memory.
LinkedList::~LinkedList() {
    Node *current = head;
    while (current) {
        Node *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

//take in Stock value and put it new node then add to linked list as head.
void LinkedList::add_node(Stock *data) {
    Node *new_node = new Node();
    new_node->data = data;
    new_node->next = head;
    head = new_node;
}

//to delete a node based on input reference id.
void LinkedList::delete_node(const std::string &id) {
    if (!head) {
        return; //if no head in linked list do nothing.
    }
    Node *current = head;
    Node *previous = nullptr;

    //iterate through nodes till find matching id value and delete it.
    while (current) {
        if (current->data->id == id) {
            if (previous) {
                //if find value set previous next pointer to current next to remove node from linked list.
                previous->next = current->next;
            } else {
                //if no previous node is available then set current node to head. meaning id found in head node. 
                head = current->next;
            }
            delete current; //delete the current node so removal complete.
            return; 
        }
        //move to next node so that can iterate through the linked list.
        previous = current;
        current = current->next;
    }
}


//Iterate through the linked list and check for id reference.
Node *LinkedList::search_node(const std::string &id) {
    Node *current = head;
    //check current node for if id matching and return node.
    while (current) {
        if (current->data->id == id) {
            return current;
        }
        current = current->next;
    }
    //no node found matcing id return null pointer.
    return nullptr;
}

Node *LinkedList::get_head() const {
    return head;
}

void LinkedList::sort() {
    if (!head || !head->next) return; //check the linked list is not only head or empty linked list.

    bool swapped = true;
    Node *current;
    Node *end = nullptr;

    while (swapped) {
        swapped = false;
        current = head;
        //go through linked list and check the id If the id > other id then swap values so put in ascending order.
        while (current->next != end) {
            if (current->data->id > current->next->data->id) {
                swap(current->data, current->next->data);
                swapped = true;
            }
            current = current->next;
        }
        end = current;
    }
}

//taking in pointer reference so can change original pointer.
void LinkedList::swap(Stock *&a, Stock *&b) {
    Stock *temp = a;
    a = b;
    b = temp;
}

//iterate through the head of the linked list and set to default value.
void LinkedList::setDefaultValue(Node* head, int defaultValue) {
    for (Node* current = head; current != nullptr; current = current->next) {
        current->data->on_hand = defaultValue;
    }
}