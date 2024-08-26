/**
 *  Amethyst Operating System - Linked List code.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STRUCTURES_LIST_HPP_
#define INCLUDES_STRUCTURES_LIST_HPP_

#include <Types.h>

template <typename T>
class LinkedListItem {
    public:
        T data;
        LinkedListItem* next;
};

template <typename T>
class LinkedList {
    public:
        LinkedList();

        bool Next() {
            if(current == nullptr) {
                return false;
            }

            current = current->next;

            return current != nullptr;
        };

        T Current() {
            return current->data;
        }

        void Add(T data);
        void Reset();
    
    private:
        LinkedListItem<T>* head;
        LinkedListItem<T>* current;
};


template <typename T>
LinkedList<T>::LinkedList() {
    head = nullptr;
    current = nullptr;
}

template <typename T>
void LinkedList<T>::Add(T data) {
    if(head == nullptr) {
        LinkedListItem<T>* item = new LinkedListItem<T>();
        item->data = data;
        item->next = head;

        head = item;

        current = head;
    } else {
        LinkedListItem<T>* ocurrent = head;

        while(ocurrent->next != nullptr) {
            ocurrent = ocurrent->next;
        }

        LinkedListItem<T>* item = new LinkedListItem<T>();
        item->data = data;
        item->next = nullptr;
        ocurrent->next = item;
    }
}

template <typename T>
void LinkedList<T>::Reset() {
    this.current = head;
}


#endif  // INCLUDES_STRUCTURES_LIST_HPP_
