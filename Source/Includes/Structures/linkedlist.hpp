/**
 *  Amethyst Operating System - Linked List code.
 *  Copyright 2024 Jack Scott <jack@jackscott.id.au>.
 *  Released under the terms of the ISC license.
*/

#ifndef INCLUDES_STRUCTURES_LINKEDLIST_HPP_
#define INCLUDES_STRUCTURES_LINKEDLIST_HPP_

#include <Types.h>

template <typename T>
class LinkedListItem {
    public:
        T data;
        LinkedListItem* next;
        LinkedListItem* prev;
};

template <typename T>
class LinkedList {
    public:
        LinkedList() {
            head = nullptr;
            current = nullptr;
        };

        /// @brief Moves to the next item, if it exists.
        /// @return True if there was an item to move to. False if there is no item here.
        bool Next() {
            if(current == nullptr) {
                return false;
            }

            current = current->next;

            return current != nullptr;
        };

        /// @brief Moves to the previous item, if it exists.
        /// @return True if there was an item to move to. False if there is no item here.
        bool Previous() {
            if(current == nullptr) {
                return false;
            }

            current = current->prev;

            return current != nullptr;
        };

        /// @brief Returns the data at the current location in the list.
        /// @return 
        T Current() {
            return current->data;
        }

        /// @brief Adds data to the end of the list.
        /// @param data 
        void Add(T data);

        void Remove(T data);

        /// @brief Resets the seek position to the head of the list.
        void Reset();
    
    private:
        LinkedListItem<T>* head;
        LinkedListItem<T>* current;
};

template <typename T>
void LinkedList<T>::Add(T data) {
    if(head == nullptr) {
        // special case first item in list.
        LinkedListItem<T>* item = new LinkedListItem<T>();
        item->data = data;
        item->next = head;
        item->prev = nullptr;

        head = item;

        current = head;
    } else {
        // add to end
        LinkedListItem<T>* ocurrent = head;

        while(ocurrent->next != nullptr) {
            ocurrent = ocurrent->next;
        }

        LinkedListItem<T>* item = new LinkedListItem<T>();
        item->data = data;
        item->next = nullptr;
        item->prev = ocurrent;
        ocurrent->next = item;
    }
}

template <typename T>
void LinkedList<T>::Remove(T data) {
    // step 1: find the right item.
    LinkedListItem<T>* item;

    item = head;

    while(item->data != data && item != nullptr) {
        item = item->next;
    }

    //here: either we have item or null.
    if(item == nullptr) {
        return;
    }

    // remove this item.
    LinkedListItem<T>* p = item->prev;
    LinkedListItem<T>* n = item->next;

    p->next = n;
    n->prev = p;

    delete item;
}

template <typename T>
void LinkedList<T>::Reset() {
    current = head;
}


#endif  // INCLUDES_STRUCTURES_LINKEDLIST_HPP_
