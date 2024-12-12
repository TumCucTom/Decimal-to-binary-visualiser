// Implementation of the list module.
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each node in a doubly linked list is stored in this structure. The user of the
// module does not have any knowledge of nodes.
struct node { 
  struct node *back; 
  item x; 
  struct node *next; 
};
typedef struct node node;

// A list is to be represented as a circular list. It has to contain a doubly linked list
// of nodes that hold an item each and one special sentinel node that links to the two ends of the list.
// The none field holds this sentinel node. It has a default item, must always be present for
// a list and always sits before the first item node and after the last item node. For a list
// that holds no nodes the back and next fields of the sentinel node point to the sentinel
// node itself. The current node refers to the currently selected item node in the list. If
// the current node is the none node then no item is selected.
struct list { 
  node *none, 
  *current; };
typedef struct list list;

//------------------------- Functions Start -------------------------------

// FUNCTION #1: Create a new empty list and make e the default item that is
// returned by functions in case no item is selected. No item is selected in an empty list.

list *newList(item e){
  //create a new node
  node* nullNode = NULL;
  nullNode = (node*)malloc(sizeof(node));
  nullNode -> x = e;
  nullNode -> next = nullNode; //set both pointers to itself (only item in list)
  nullNode -> back = nullNode;

  //create the list
  list* new = NULL;
  new = (list*)malloc(sizeof(list));
  new -> none = nullNode; //set both the none and current to the deafualt node
  new -> current = nullNode;
  return new;
}

// FUNCTION #2: Free up the list and all the data in it. Does not have to run in O(1).

void freeList(list *xs){
  node* current = xs -> current;
  node* next = current -> next;

  //whilst the next node to be deleted isn't itself (until there is only one item)
  while(current -> next != next){
    next = current -> next; // get the next node before freeing
    free(current);
    current = next;
  }
  free(current); // free the last item
  free(xs);

}

// FUNCTIONS #3/#4: Set the current item to the first item or to the last
// item of the list, respectively. If the list has no items the functions do nothing
// and no item is selected.

void first(list *xs){
  // if the next item is not itself (one item in the list - the defualt):
  // set current to the first item (node after the default)
  if(xs -> current != xs -> current -> next) xs -> current = xs -> none -> next;

}

void last(list *xs){
  // if the next item is not itself (one item in the list - the defualt):
  // set current to the first item (node after the default)
  if(xs -> current != xs -> current -> next) xs -> current = xs -> none -> back;
}

// FUNCTION #5: Returns true if no item is selected, i.e. the 'none' position.
// Otherwise it returns false.

bool none(list *xs){
  // if the current item is the default item
  if(xs -> current == xs -> none) return true;
  return false;
}

// FUNCTION #6: Make the item following the currently selected item the current item
// and return true. If after is called while the last item is the current item, then no item is
// selected and true is returned. If the function is called while no item
// is selected then the function does nothing and returns false.

bool after(list *xs){
  // return false is theere are no items
  if(xs -> current == xs -> none) return false;
  //select next item
  xs -> current = xs -> current -> next;
  return true;
}

// FUNCTION #7: Make the item before the currently selected item the current item and return true.
// If before is called while the first item is the current item, then no item is
// selected and true is returned. If the function is called while no item
// is selected then the function does nothing and returns false.

bool before(list *xs){
  // return false if there are no items 
  if(xs -> current == xs -> none) return false;
  //select previous item
  xs -> current = xs -> current -> back;
  return true;
}

// FUNCTION #8: Get the current item. If get is called and no item is selected
// then the default item is returned.

item get(list *xs){
  // returning default item works same as returning any other
  return xs -> current -> x;
}

// FUNCTION #9: Set the current item and return true. If set is called while no
// item is selected then the function does nothing and returns false.

bool set(list *xs, item x){
    // if there are no items then return false
    if(xs -> current == xs-> none) return false;
    // set the current item's data (x) to x
    xs -> current -> x = x;
    return true;
}

// FUNCTION #10: Inserts an item after the current item and makes it the current item.
// If insertAfter is called while no item is selected then
// the function inserts the item at the beginning of the list
// before the first item.

void insertAfter(list *xs, item x){
  // create a new node and set data (x) to x
  node* newNode = NULL;
  newNode = (node*)malloc(sizeof(node));
  newNode -> x = x;

  // if there is an item selected
  if(xs ->  current != xs -> none){
    node* xsCurrent = xs -> current;
    node* followingNode = xsCurrent -> next;

    //set the new node's pointers to be inbetween the selected node and the following node
    newNode -> next = followingNode;
    newNode -> back = xsCurrent;
    // set the pointers of the selected node and following node to the new node respectively
    xsCurrent -> next = newNode;
    followingNode -> back = newNode;
  }

  // if there is no item selected
  else{
    node* xsNone = xs -> none;
    node* followingNode = xsNone -> next;

    // set the new node's pointers to be inbetween the null/defualt node and the first node
    newNode -> next = followingNode;
    newNode -> back = xsNone;

    // set the pointers of the null and old first node to the new node respectively
    xsNone-> next = newNode;
    followingNode -> back = newNode;
  }

  // the inserted node is the new current
  xs -> current = newNode;
}

// FUNCTION #11: Inserts an item before the current item and makes it the current item.
// If insertAfter is called while no item is selected then the function inserts
// the item at the end of the list after the last item.
void insertBefore(list *xs, item x){
  // create a new node and set data (x) to x
  node* newNode = NULL;
  newNode = (node*)malloc(sizeof(node));
  newNode -> x = x;

  //if there is a selected item
  if(xs ->  current != xs -> none){
    node* xsCurrent = xs -> current;
    node* prevNode = xsCurrent -> back;

    // set the new node's pointers to be inbetween the selected node and the previous one
    newNode -> next = xsCurrent;
    newNode -> back = prevNode;

    // set the pointers of the selected node and the previous node to the new node respectively
    xsCurrent -> back = newNode;
    prevNode -> next = newNode;
  }

  //if there is not a selected item
  else{
    node* xsNone = xs -> none;
    node* prevNode = xsNone -> back;

    // set the new node's pointers to be behdind the last item and before the null (circular)
    newNode -> next = xsNone;
    newNode -> back = prevNode;

    // set the pointers of the null and the last item to the new node accordingly
    xsNone-> back = newNode;
    prevNode -> next = newNode;
  }

  //the inserted node is the new current
  xs -> current = newNode;
}

// FUNCTION #12: Delete the current item and make its successor the current item, then
// return true. If deleteToAfter is called while the last item is the current item then
// the last item is deleted, no item is selected, and true is returned.
// If deleteToAfter is called while no item is selected then the
// function does nothing and returns false.

bool deleteToAfter(list *xs){
  // if no item is selected
  if(xs -> current == xs->none) return false;

  node* current = xs -> current;
  node* prevNode = current -> back;
  node* nextNode = current -> next;

  // set the pointers of the nodes around the current item to exclude it
  prevNode -> next = nextNode;
  nextNode -> back = prevNode;

  //free the node and set the current node to be after the old deleted node
  free(current);
  xs -> current = nextNode;
  return true;
}

// FUNCTION #13: Delete the current item and make its predecessor the current item, then
// return true. If deleteToBefore is called while the first item is the current item then
// the first item is deleted, no item is selected, and true is returned.
// If deleteToBefore is called while no item is selected then the
// function does nothing and returns false.

bool deleteToBefore(list *xs){
  //if no item is selected
  if(xs -> current == xs->none) return false;

  node* current = xs -> current;
  node* prevNode = current -> back;
  node* nextNode = current -> next;

  //set the pointers of the nodes around the current item to exclude it
  prevNode -> next = nextNode;
  nextNode -> back = prevNode;

  //free the node and set the new current to be the node before the old deleted node
  free(current);
  xs -> current = prevNode;
  return true;
}

// FUNCTION #14: Reform the list by making the list end with the current item whilst
// preserving the sequence of all list items. The current item and items originally before 
// the current item will then be at the end of the reformed list. The reformed list will start
// with the item originally succeeding the current item. The current item stays the same.
// A list with no item selected will not be changed by this function.

void reform(list *xs){
  node* current = xs -> current;
    node* none = xs -> none;
    node* first = none -> next;
    node* last = none -> back;
    node* nextNode = current -> next;

  // if an item is selected and there is not only one item (no change needed)
  if(current != none && nextNode != none){

    //starting from the none node:
    none -> next = nextNode;
    nextNode -> back = none;

    //all nodes are now in order until the origional last node
    last -> next = first;
    first -> back = last;

    //node are then in order until the current (new last) node:
    current -> next = none;
    none -> back = current;
  }
}

//------------------------- Testing Start -------------------------------

// Test the list module, using int as the item type. Strings are used as
// 'pictograms' to describe lists. Single digits represent items and the '|' symbol
// in front of a digit indicates that this is the current item. If the '|' symbol
// is at the end of the string then 'none' of the items is selected. The strings
// "|37", "3|7", "37|" represent a list of two items, with the current position
// at the first item, the last item, and a situation where 'none' of the items
// is selected.
#ifdef test_list

// Build a list from a pictogram, with -1 as the default item.
// Note: You do not need to understand this function to solve the coursework.
list *build(char *s) {
  list *xs = malloc(sizeof(list));
    int n = strlen(s);
    node *nodes[n];
    for (int i = 0; i < n; i++) nodes[i] = malloc(sizeof(node));
    for (int i = 0; i < n; i++) nodes[i]->next = nodes[(i + 1) % n];
    for (int i = 1; i < n + 1; i++) nodes[i % n]->back = nodes[i - 1];
    xs->none = nodes[0];
    xs->none->x = -1;
    node *p = xs->none->next;
    for (int i = 0; i < strlen(s); i++) {
      if (s[i] == '|') xs->current = p;
      else {
        p->x = s[i] - '0';
        p = p->next;
      }
    }
  return xs;
}

// Destroy a list which was created with the build function and which matches a pictogram.
// Note: You do not need to understand this function to solve the coursework.
void destroy(list *xs, char *s) {
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++) nodes[i] = nodes[i-1]->next;
  for (int i = 0; i < n; i++) free(nodes[i]);
  free(xs);
}

// Check that a list matches a pictogram.
// Note: You do not need to understand this function to solve the coursework.
bool match(list *xs, char *s) {
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++) nodes[i] = nodes[i - 1]->next;
  if (nodes[n - 1]->next != xs->none) return false;
    for (int i = 1; i < n; i++) {
      if (nodes[i]->back != nodes[i - 1]) return false;
    }
  node *p = xs->none->next;
  for (int i = 0; i < strlen(s); i++) {
    if (s[i] == '|') {
      if (p != xs->current) return false;
    }
    else {
      if (p->x != s[i] - '0') return false;
      p = p->next;
    }
  }
  return true;
}

// The tests use an enumeration to say which function to call.
enum { First, Last, None, After, Before, Get, Set, InsertAfter, InsertBefore, DeleteToAfter, DeleteToBefore, Reform};
typedef int function;

// A replacement for the library assert function.
void assert(int line, bool b) {
  if (b) return;
  printf("The test on line %d fails.\n", line);
  exit(1);
}

// Call a given function with a possible integer argument, returning a possible
// integer or boolean result (or -1).
// Note: You do not need to understand this function to solve the coursework.
int call(function f, list *xs, int arg) {
  int result = -1;
  switch (f) {
    case None: result = none(xs); break;
    case First: first(xs); break;
    case Last: last(xs); break;
    case After: result = after(xs); break;
    case Before: result = before(xs); break;
    case Get: result = get(xs); break;
    case Set: result = set(xs, arg); break;
    case InsertAfter: insertAfter(xs, arg); break;
    case InsertBefore: insertBefore(xs, arg); break;
    case DeleteToBefore: result = deleteToBefore(xs); break;
    case DeleteToAfter: result = deleteToAfter(xs); break;
    case Reform: reform(xs); break;
    default: assert(__LINE__, false);
  }
  return result;
}

// Check that a given function does the right thing. The 'in' value is passed to
// the function or is -1. The 'out' value is the expected result, or -1.
bool check(function f, int in, char *before, char *after, int out) {
  list *xs = build(before);
  int result = call(f, xs, in);
  //check your list has indeed a circular list structure
  assert(__LINE__, (xs->none == xs->none->next->back));
  assert(__LINE__, (xs->none == xs->none->back->next));
  //check that your function works correctly as the tests demand
  bool ok = (match(xs, after) && (result == out));
  destroy(xs, after);
  return ok;
}

// Test newList, and call freeList. The test for freeList is that the memory
// leak detector in the -fsanitize=address or -fsanitize=leak compiler option
// reports no problems.
void testNewList() {
    list *xs = newList(-1);
    //check circular list structure
    assert(__LINE__, (xs->none == xs->none->next));
    assert(__LINE__, (xs->none == xs->none->back));
    //check that an empty list is produced with a sentinel correctly
    assert(__LINE__, match(xs, "|"));
    freeList(xs);
}

// Test the various 14 functions.
void testFirst() {
    assert(__LINE__, check(First, -1, "|", "|", -1));
    assert(__LINE__, check(First, -1, "|37", "|37", -1));
    assert(__LINE__, check(First, -1, "3|7", "|37", -1));
    assert(__LINE__, check(First, -1, "37|", "|37", -1));
}

void testLast() {
    assert(__LINE__, check(Last, -1, "|", "|", -1));
    assert(__LINE__, check(Last, -1, "|37", "3|7", -1));
    assert(__LINE__, check(Last, -1, "3|7", "3|7", -1));
    assert(__LINE__, check(Last, -1, "37|", "3|7", -1));
}

void testNone() {
    assert(__LINE__, check(None, -1, "|", "|", true));
    assert(__LINE__, check(None, -1, "|37", "|37", false));
    assert(__LINE__, check(None, -1, "3|7", "3|7", false));
    assert(__LINE__, check(None, -1, "37|", "37|", true));
}

void testAfter() {
    assert(__LINE__, check(After, -1, "|", "|", false));
    assert(__LINE__, check(After, -1, "|37", "3|7", true));
    assert(__LINE__, check(After, -1, "3|7", "37|", true));
    assert(__LINE__, check(After, -1, "37|", "37|", false));
}

void testBefore() {
    assert(__LINE__, check(Before, -1, "|", "|", false));
    assert(__LINE__, check(Before, -1, "|37", "37|", true));
    assert(__LINE__, check(Before, -1, "3|7", "|37", true));
    assert(__LINE__, check(Before, -1, "37|", "37|", false));
}

void testGet() {
    assert(__LINE__, check(Get, -1, "|", "|", -1));
    assert(__LINE__, check(Get, -1, "|37", "|37", 3));
    assert(__LINE__, check(Get, -1, "3|7", "3|7", 7));
    assert(__LINE__, check(Get, -1, "37|", "37|", -1));
}

void testSet() {
    assert(__LINE__, check(Set, 5, "|", "|", false));
    assert(__LINE__, check(Set, 5, "|37", "|57", true));
    assert(__LINE__, check(Set, 5, "3|7", "3|5", true));
    assert(__LINE__, check(Set, 5, "37|", "37|", false));
}

void testInsertAfter() {
    assert(__LINE__, check(InsertAfter, 5, "|", "|5", -1));
    assert(__LINE__, check(InsertAfter, 5, "|37", "3|57", -1));
    assert(__LINE__, check(InsertAfter, 5, "3|7", "37|5", -1));
    assert(__LINE__, check(InsertAfter, 5, "37|", "|537", -1));
}

void testInsertBefore() {
    assert(__LINE__, check(InsertBefore, 5, "|", "|5", -1));
    assert(__LINE__, check(InsertBefore, 5, "|37", "|537", -1));
    assert(__LINE__, check(InsertBefore, 5, "3|7", "3|57", -1));
    assert(__LINE__, check(InsertBefore, 5, "37|", "37|5", -1));
}

void testDeleteToAfter() {
    assert(__LINE__, check(DeleteToAfter, -1, "|", "|", false));
    assert(__LINE__, check(DeleteToAfter, -1, "|37", "|7", true));
    assert(__LINE__, check(DeleteToAfter, -1, "3|7", "3|", true));
    assert(__LINE__, check(DeleteToAfter, -1, "37|", "37|", false));
    assert(__LINE__, check(DeleteToAfter, -1, "|5", "|", true));
}

void testDeleteToBefore() {
    assert(__LINE__, check(DeleteToBefore, -1, "|", "|", false));
    assert(__LINE__, check(DeleteToBefore, -1, "|37", "7|", true));
    assert(__LINE__, check(DeleteToBefore, -1, "3|7", "|3", true));
    assert(__LINE__, check(DeleteToBefore, -1, "37|", "37|", false));
    assert(__LINE__, check(DeleteToBefore, -1, "|5", "|", true));
}

void testReform() {
    assert(__LINE__, check(Reform, -1, "|", "|", -1));
    assert(__LINE__, check(Reform, -1, "|37", "7|3", -1));
    assert(__LINE__, check(Reform, -1, "3|7", "3|7", -1));
    assert(__LINE__, check(Reform, -1, "37|", "37|", -1));
}

int main() {
    testNewList();
    testFirst();
    testLast();
    testNone();
    testAfter();
    testBefore();
    testGet();
    testSet();
    testInsertAfter();
    testInsertBefore();
    testDeleteToAfter();
    testDeleteToBefore();
    testReform();
    printf("List module tests run OK.\n");
    return 0;
}
#endif
