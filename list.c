#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 500, 1500, 2500, 3500, 4500, 5500, 6500, 7500, 8500, 9500, 10500, 11500};

// each list node contains an integer key value and pointer to next list node
typedef struct node {
    int key;
    struct node *next;
} list_node;

// list's beginning is called the head of list
struct node *head = NULL;


struct node* list_insert(int value) {
    if(head == NULL) { //jesli head jest pusty, to tworzymy pierwszy element
        head = (list_node*) malloc(sizeof(list_node));
        head->key = value;
        head->next = NULL;
    }
    else { // w innym wypadku dodajemy element do jego poprzednika
        list_node *iter = head;
        while(iter->next != NULL) {
            iter = iter->next;
        }
        iter->next = (list_node *) malloc(sizeof(list_node));
        iter->next->key  = value;
        iter->next->next = NULL;
    }
}

//def list_search(value)
//    iter ← head
//    while iter ≠ null and iter.key ≠ value
//        iter ← iter.next
//    return iter

struct node* list_search(int value) {
    list_node * iter = head;
    while(iter->next != NULL && iter->key != value){
        iter = iter->next;
    }
    return iter;
}

void list_delete(int value) {
    list_node * current = head;
    list_node * temporary = head;
    list_node * previous;
    //sprawdzamy czy poszukiwany element jest pierwszy,
    //jesli tak zmieniamy wskaznik head na nastepny element
    if(temporary != NULL) {
        if(temporary->key == value){
            head = temporary->next;
            free(temporary);
            return;
        }
    }

    while(temporary != NULL && temporary->key != value) {
        previous = temporary; //sledzimy poprzedni element listy
        temporary = temporary->next;
    }
    previous->next = temporary->next;
    free(temporary);
}
//def list_size()
//    size ← 0
//    iter ← head
//    while iter ≠ null
//        size ← size + 1
//        iter ← iter.next
//    return size

unsigned int list_size() {
    int i = 0;
    list_node * iter = head;
    while(iter != NULL) {
        iter = iter->next;
        i++;
    }
    return i;
}

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int main() {
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++) {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // always create an array of size `n` and fill it with increasing values
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // if true, reorder array elements randomly
            if (enable_shuffle) {
                shuffle(t, n);
            }

            // insert elements in the order present in array `t`
            clock_t insertion_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_insert(t[k]);
                assert(iter != NULL);       // inserted element cannot be NULL
                assert(iter->key == t[k]);  // inserted element must contain the expected value
            }
            insertion_time = clock() - insertion_time;

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_search(t[k]);
                assert(iter != NULL);       // found element cannot be NULL
                assert(iter->key == t[k]);  // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int k = 0, l = n; k < n; k++, l--) {
                assert(list_size() == l);   // list size must be equal to the expected value
                list_delete(t[k]);
            }
            assert(list_size() == 0);       // after all deletions, the list size is zero
            assert(head == NULL);           // after all deletions, the list's head is NULL

            free(t);

            printf("%d\t%s\t%f\t%f\n", n, enable_shuffle ? "true" : "false",
                   (double)insertion_time / CLOCKS_PER_SEC,
                   (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
