#pragma once

namespace ml {
/*
A hybrid data structure of a linked list and a vector.
It primarily acts as a vector but new allocations are made in a linked list fashion 
instead of copying the data to a new location.

Vector elements: XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
List elements:   X->X->X->X->X->X
Hybrid elements: X->XX->XXXX->XXXXXXXX->XXXXXXXXXXXXXXXX

The initial capacity can be set by explicitly reserving the memory.
The growth factor is 2x.
*/
template <typename T, typename Allocator = std::allocator<T>>
class linked_vector {};
}
