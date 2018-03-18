 #Single Linked List
 A Single linked list is a simple data structure that consists of the following structure
 ```C++
 template<typename T>
 struct Node
 {
 	T value;
 	Node<T>* next;
 };
 ```
#Struct `Single_List`
```C++
template<typename T>
struct Single_List
```
- **brief:**      A Single linked list container
- **tparam:**     T     Type of the values in the list

##Typedef `Data_Type`
```C++
using Data_Type = T;
```
Data type of the single linked list

##Typedef `Node_Type`
```C++
using Node_Type = internal::Single_List_Node<Data_Type>;
```
Node Type of the single linked list
