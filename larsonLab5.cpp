/******
Author: Mackenzie Larson
Date: 9/22/14
Class: CS 315 LAB05
Description: This program takes in a complex list from a user and will: 
1. First it asks for a user input as a string, builds the list and prints all the keys in the list with their depth.
2. Next it takes two integer values j and k from the user and changes the j-th item of the list to k. 
3. It then prints the updated list.

To Compile: g++ -g larsonLab5.cpp larsonL5out
            ./larsonL5out
One test case that tests multiple brackets and negative numbers the user can enter is: 
 [4, [12, 4, 3], [[5, 3, -1], 8], [12, 8]] 

The program was built based on the assumption that the user enters in the brackets/test case correctly. 
*****/
#include <iostream>
#include <cstdlib>
#include <ctype.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <sstream>

using namespace std;

//node class
class Node {

public:
  Node(int key1){
    key = key1;
    keyset = true;
    tag = 0;
    down = NULL;
    next = NULL;
  }

  Node(Node* dptr, Node* nptr) {
    tag = 1;
    down = dptr;
    next = nptr;
  }

  void setKey(int k) {
    key = k;
    keyset = true;
  }

  void setDown(Node* ptr) {
    down = ptr;
  }

  void setNext(Node* ptr) {
    next = ptr;
  }

  int getKey() {
    return key;
  }

  int getTag() {
    return tag;
  }

  bool getKeyset() {
    return keyset;
  }

  Node* getDown() {
    return down;
  }

  Node* getNext() {
    return next;
  }

  int size() {
    if (this == NULL)
      return 0;
    else if (this->tag == 0) return 1;
    else
      return down->size() + next->size();
  }


  void update(int j, int k) {
    // The update function reads in the list L and j and k. (j is the jth int in the list, and k is the value to be updated)
    // If the list is empty, print an error message

    if (isEmpty()){
      cout << "Not valid to update empty list" << endl;
      return;}
    else if (tag == 0){
      key = k;
      return;}
    else {
      int s = down->size();
      if (j <= s)
        down->update(j, k);
      else
        next->update(j - s, k);
    }
  } 

  bool isEmpty() {
    return this == NULL;
  }

  void print(int depth) {
    if (this == NULL) return;
    if (tag == 0) {
      cout << "(" << key << "," << depth << ")  " ;
    }
    else {
      down->print(depth+1);
      next->print(depth);
    } 
  } 

private:
  int tag;
  int key;
  bool keyset;
  Node* down;
  Node* next;
};

#define SIZE 100

class Stack {

private:
  int maxTop;//size - 1
  int top;// current top of stack
  Node * values[SIZE];// element array

public:

  bool IsEmpty() { return top == -1; }
  bool IsFull() { return top == maxTop; }

  Stack() {
    top=-1;
    maxTop= SIZE - 1;
  }

  void Push(Node* x) {
    if (IsFull())  //print error
      cout << "Error: the stack is full." << endl;
    else {
      values[++top]= x;
    }
  }

  Node*  Pop() {
    if (IsEmpty()) { 
      cout << "Error: the stack is empty." << endl;
      return NULL;
    }
    else {
      return values[top--];
    }
  }

  Node* Top() {
    if (IsEmpty()) {
      cout << "Error: the stack is empty." << endl;
      return NULL;
    }
    else
      return values[top];
  }
};

vector<string> tokenize (string T)
{
  vector<string> Tokens;

  for (int k = 0; k < T.length(); k++)
    {
      switch (T[k]) {
      case '[':
        Tokens.push_back("[");
        break;

      case ']':
        Tokens.push_back("]");
        break;

      case ' ':
        break;

      case ',':
        break;

	//made case for negative numbers
      case '-':

        Tokens.push_back("-");
        while (T[k+1] >= '0' && T[k+1] <= '9')
          {
            Tokens.back() += T[k+1];
            ++k;
          }

        break;

      default:
        string num;
        num += T[k];

        while (T[k+1] >= '0' && T[k+1] <= '9')
          {
            ++k;

            num += T[k];
          }
         Tokens.push_back(num);
        break;
      }
    }
  return Tokens;
}

Node * convert(string T)
{
  vector<string> Tokens = tokenize(T);
  Stack S;

  for (int k = 0; k < Tokens.size(); k++)
    {
      if (Tokens[k] == "[") // Is a [
        {
          Node* pointerNode = new Node(NULL, NULL);
          S.Push(pointerNode);
        }
      else if ((Tokens[k][0] >= '0' && Tokens[k][0] <= '9') || Tokens[k][0] == '-' ) // Is a digit & also for a negative case
        {
          int key;
          stringstream ss;
          ss << Tokens[k];
          ss >> key;
          Node* leafNode = new Node(key);
          S.Push(leafNode);
        }
      else //  Is "]"
        {
          Node * leafNode = S.Pop();
          Node * newPointerNode = new Node(leafNode, NULL);
          while (( S.Top()->getTag() == 0 || S.Top()->getDown() != NULL ))
            {
              leafNode = S.Pop();
              newPointerNode = new Node(leafNode, newPointerNode);
            }
          S.Pop();
          S.Push(newPointerNode);
        }
    }
  return S.Pop();
}

int main()
{
  string L;
  cout << "Enter a string of numbers enclosed in brackets: ";
  getline(cin, L);
  cout << "Building list represented by string. Printing info about keys and depth.(Key, Depth)" << endl;

  //tokenize
  vector<string> V = tokenize(L);

  //convert
  Node* Node1 = convert(L);
  cout << "Your list is printed below with the depth of each key:" << endl;
  Node1->print(0);
  cout << endl;

  //update
  int position;
  int value;
  cout << "Next the program will take two int values, j & k, entered by you the user to change the j-th term of the list to k." << endl;
  cout << "Enter the position of the key to be updated(j): ";
  cin >> position;
  cout << "Enter the value of the key to be updated to(k): ";
  cin >> value;
  Node1->update(position, value);
  cout << "Your updated list is: ";
  Node1->print(0);
  cout << endl;
}
