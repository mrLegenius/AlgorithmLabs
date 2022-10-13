# Dynamic Array
Made for Algorimth analysis course in ITMO 2022 Gamedev.

## General
Dynamic array is a resizable template data structure.


## Interface

#### Array();
Creates an array with capacity 8

#### Array(int capacity); 
Creates an array with given capacity

#### ~Array(); 
Free all used memory

#### int insert(const T& value);
insert element into the end of array
if there is not enough space capacity will be doubled
 
#### int insert(int index, const T& value);
insert element into given position
if there is not enough space capacity will be doubled

#### void remove(int index); 
Remove element at given position

#### int size() const;
Returns current number of elements

#### Iterator iterator();
Returns iterator that pointing to the first element of array

#### ConstIterator iterator() const;
Returns constant iterator that pointing to the first element of array that can't be changed

#### Iterator reverseIterator();
Returns iterator that pointing to the last element of array

#### ConstIterator reverseIterator() const;
Returns constant iterator that pointing to the last element of array that can't be changed
