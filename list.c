#include <stdio.h>
#include <stdlib.h>
#include "list.h"




struct node {
    /*
    The node struct represents one index of the linked list,
    it contains one given datapoint and a pointer to the next 
    variable.
    */
    void* data;
    struct node* next;

};


struct list {
    /*
    this struct is used to make the initual linkerd list and send each 
    index into the node structure, so we have one datapoint and one 
    pointer. It also gives easy access to the size.
    */
    struct node* head;
    int size;
};


struct list_iterator {
    /*
    The list iterator keeps of one given node, and also the 
    first one.
    */
    struct node* current;
    struct node* head;
};



list_t *list_create(void) {
    /*
    This function creates a empty list with a head (first variable of a linked list)
    it also has a size paramters which is constructed for easy access for other functions.

    parameters: none
    return: empty list
    */

    list_t* empty_list = malloc(sizeof(struct list));
        //frees of memory

    empty_list -> head = NULL;
    empty_list -> size = 0;
        //variable definition

    return empty_list;
};



void list_destroy(list_t *list) {
    /*
    This is a function that frees up the storage of a linked list, in order to this:
    step 1) free up all the headers
    step 2) free up the whole list

    parameters: linked list
    returns: none
    */

    struct node *value = list -> head;
        //pointing to the current value
    struct node *next;
        //pointing to the next value

    while (value != NULL) {
        //when current == NULL we have reached the end
        next = value -> next;
            //this variable is the next head
        free(value);
        value = next;
    }

    free(list);
        //frees up the memory of the list
};



void list_addfirst(list_t *list, void *item) {
    /*
    This function takes in an item and converts it into a node so we can add it to the header,
    it the header is empty we increase the size +=1

    parameters: linked list, new item
    returns: none
    */
    
    struct node *new_node = malloc(sizeof(struct node));
        //frees up memory

    new_node -> data = item;
        //defines the new item as a node so we can pass it into the linked list


    list -> size += 1;
    new_node -> next = list -> head;
    list -> head = new_node;
        //every value gets pushed back one spot.
};




void list_addlast(list_t *list, void *item) {
    /*
    This function add a item to the end of the linked list, in order to do this,
    we need to define the new node and loop through the linked list to reach the
    end. This is done simuarly to the loop in the list destroy function. Where the current
    value gets updated to the next until we have reached the end.

    parameters: linked list, item
    returns: None
    */

    struct node *new_node = malloc(sizeof(struct node));
        //frees up memory

    struct node *value = list -> head;
        //pointing to the current value
    struct node *next;
        //pointing to the next value
    
    new_node -> data = item;
        //defines the new item as a node so we can pass it into the linked list
    new_node -> next = NULL;
        //since its the last element of the linked list

    if (list->head == NULL) {
        //checks if the list is empty
        list-> head = new_node;
        list -> size += 1;
        return;
    }

    while (value != NULL) {
        //loops through the whole list
        
        next = value -> next;

        if (next == NULL) {
            //picks up the next value just before the while loops end.
            value -> next = new_node;
            list -> size += 1;
                //update
            return;
        }

        value = next;
    }
};


void list_remove(list_t *list, void *item) {
    /*
    This function removes the first item in the linked list that matches the item passed into
    the function

    parameters: linked list, item
    returns: None.
    I got assisted with the logic from chatgpt here:
    https://chatgpt.com/share/66f2b5be-696c-800d-87da-d7fc38352f23
    */
    
    struct node *current = list -> head;
    struct node *previous = NULL;

    while (current != NULL) {
        //loops over the whole linked list
        if (item == current -> data) {

            if (previous == NULL) {

                list -> head = current -> next;
            } else {
                previous-> next = current -> next;
            }
            free(current);
            list -> size -= 1;
            return;
        }
        previous = current;
        current = current -> next;

}
};



int list_size(list_t *list) {
    /*
    simple function that returns the size of a list, which is already defined in the 
    data structure.

    parameters: linked list
    returns: size of linked list
    */

    return list -> size;
};

list_iterator_t *list_createiterator(list_t *list) {
    /*
    function that construct an iterator of a list, it
    also keep track of the head

    parameter(s): list
    return: list iterator
    */

    list_iterator_t *iterator = malloc(sizeof(struct list_iterator));
        //frees up enought memory

    iterator -> current = list -> head;
    //iterator -> head node
    iterator -> head = list -> head;
        //define variables


    return iterator;
};

void list_destroyiterator(list_iterator_t *iter) {
    /*
    This function frees up the storage of a given iteraton
    without affecting the rest of the list.
    */

    if (iter != NULL) {
        //makes sure current iteration is not at the end of the list
        free(iter);
    }

};
void *list_next(list_iterator_t *iter) {
    /*
    This function add one to the current iteration
    and returns the old iterator.
    */
    if (iter -> current == NULL) {
        return NULL;
        //if we are at the end of the list. 
    }

    void *data = iter -> current -> data;
        //take out the data from the node

    iter -> current = iter -> current -> next;
        //Define the new data

    return data;

};

void list_resetiterator(list_iterator_t *iter) {
    /*
    This function is quite simple since iter stores the head. 
    */

    iter -> current = iter -> head;


};
