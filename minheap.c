/*
 * Our min-heap implementation.
 *
 * Author (starter code): A. Tafliovich.
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- to help designing your code
 *************************************************************************/

int priorityAt(MinHeap* heap, int nodeIndex){
/* Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
       return heap->arr[nodeIndex].priority;
       
}

int min_index(MinHeap * heap, int index1, int index2){
       if(heap->arr[index1].priority < heap->arr[index2].priority){
              return index1;
       }
       else{
              return index2;
       }
}
bool invalid_index(MinHeap* heap, int index){
       return (index > heap->size || index < ROOT_INDEX);
}

int leftIdx(MinHeap* heap, int nodeIndex){
       int left_index = nodeIndex * 2;
       if(left_index > heap->size){
              return NOTHING;
       }
       return left_index;
}


int rightIdx(MinHeap* heap, int nodeIndex){
       /* Returns the index of the right child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such right
 * child.
 */
       int right_index = (nodeIndex * 2 + 1);
       if(right_index > heap->size){
              return NOTHING;
       }
       else{
              return right_index;
       }

}
bool isValidIndex(MinHeap * heap, int maybeIdx) {
  if (maybeIdx > 0 && maybeIdx <= heap -> size)
    return true;
  return false;
}
int parentIdx(MinHeap* heap, int nodeIndex){
/* Returns the index of the parent of a node at index 'nodeIndex' in minheap
 * 'heap', if such exists.  Returns NOTHING if there is no such parent.
 */
       if(nodeIndex == 1){
              return NOTHING;
       }

       return (nodeIndex / 2);
}

void swap(MinHeap* heap, int index1, int index2){
       /* Swaps contents of heap->arr[index1] and heap->arr[index2] if both 'index1'
 * and 'index2' are valid indices for minheap 'heap'. Has no effect
 * otherwise.
 */
       if(invalid_index(heap, index1) || invalid_index(heap, index2)){
              printf("invalid index, no effect, please debug");
              return;
       }
       int id1 = heap->arr[index1].id;
       int id2 = heap->arr[index2].id;
       HeapNode node1 = heap->arr[index1];
       HeapNode node2 = heap->arr[index2];
       heap->arr[index1] = node2;
       heap->arr[index2] = node1;
       heap->indexMap[id1] = index2;
       heap->indexMap[id2] = index1;
}


void bubbleUp(MinHeap* heap, int nodeIndex){
/* Bubbles up the element newly inserted into minheap 'heap' at index
 * 'nodeIndex', if 'nodeIndex' is a valid index for heap. Has no effect
 * otherwise.
 */
       int parent_index = parentIdx(heap, nodeIndex);
       while(nodeIndex > 1 && heap->arr[parent_index].priority > heap->arr[nodeIndex].priority){
             if(!invalid_index(heap, parent_index) && !invalid_index(heap, nodeIndex)){
              swap(heap, nodeIndex, parent_index);
              nodeIndex = parent_index;
              parent_index = parentIdx(heap, nodeIndex);
             }
             else{
              break;
             }     
       }
}

int find_least_priority_index(MinHeap * heap, int self_index, int leftIndex, int rightIndex){

       int smaller_priority_index;
       int least_priority_index;


       HeapNode leftNode = heap->arr[leftIndex];
       HeapNode rightNode = heap->arr[rightIndex];
       HeapNode selfNode= heap->arr[self_index];
       if(leftNode.priority < rightNode.priority){
              smaller_priority_index = leftIndex;
       }
       else{
              smaller_priority_index = rightIndex;
       }

       if(heap->arr[smaller_priority_index].priority < selfNode.priority){
              least_priority_index = smaller_priority_index;
       }
       else{
              least_priority_index = self_index;
       }

       return least_priority_index;
}


void bubbleDown(MinHeap* heap){
/* Bubbles down the element newly inserted into minheap 'heap' at the current index,
 * if it exists. Has no effect otherwise.
 */
       
       int self_index = ROOT_INDEX;
       int left_index = leftIdx(heap, self_index);
       int right_index = rightIdx(heap, self_index);
       int least_priority_index = find_least_priority_index(heap, self_index, left_index, right_index);
       if(least_priority_index == NOTHING){
           if(invalid_index(heap, left_index)){
              return;
           }
           if(invalid_index(heap, right_index)){
              least_priority_index = min_index(heap, left_index, self_index);
           }   
       }
       while(least_priority_index != self_index && !invalid_index(heap, self_index)){
              swap(heap, self_index, least_priority_index);
              self_index = least_priority_index;
              left_index = leftIdx(heap, self_index);
              right_index = rightIdx(heap, self_index);
              least_priority_index = find_least_priority_index(heap, self_index, left_index, right_index);
                     if(least_priority_index == NOTHING){
           if(invalid_index(heap, left_index)){
              return;
           }
           if(invalid_index(heap, right_index)){
              least_priority_index = min_index(heap, left_index, self_index);
           }   
       }
       }
}

/* Returns the index of the left child of a node at index 'nodeIndex' in
 * minheap 'heap', if such exists.  Returns NOTHING if there is no such left
 * child.
 */




/* Returns True if 'maybeIdx' is a valid index in minheap 'heap', and 'heap'
 * stores an element at that index. Returns False otherwise.
 */
// bool isValidIndex(MinHeap* heap, int maybeIdx);

/* Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex){
       return heap->arr[nodeIndex];
}





int idAt(MinHeap* heap, int nodeIndex){
/* Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
       return heap->arr[nodeIndex].id;
}


int indexOf(MinHeap* heap, int id){
       /* Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
       return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/

HeapNode getMin(MinHeap* heap){
       /* Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
       return heap->arr[ROOT_INDEX];
}


HeapNode extractMin(MinHeap* heap){
       /* Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
       HeapNode min_node = getMin(heap);

       //start replacing the root's content with that of the last node in the heap
       int root_id = heap->arr[ROOT_INDEX].id;
       swap(heap, ROOT_INDEX, heap->size);
       heap->indexMap[root_id] = NOTHING;
       heap->arr[heap->size].priority = NOTHING;
       heap->arr[heap->size].id = NOTHING;
       heap->size = heap->size - 1;
       bubbleDown(heap);
       return min_node;
}


MinHeap* newHeap(int capacity){

       MinHeap * heap = (MinHeap*) malloc(sizeof(MinHeap));
       heap->capacity = capacity;
       heap->size = 0;
       heap->indexMap = (int *) malloc( (capacity) * sizeof(int));
       for(int i=0; i < capacity; i++){
              heap->indexMap[i] = NOTHING;
       }
       heap->arr = (HeapNode *) malloc( (capacity + 1) * sizeof(HeapNode));
       HeapNode empty_node;
       empty_node.priority = NOTHING;
       empty_node.id = NOTHING;
       for(int i=0; i < (capacity + 1); i++){
              heap->arr[i] = empty_node;
       }
       return heap;
}

bool invalid_id(MinHeap * heap, int id){
       return (id < ROOT_INDEX || id >= heap->capacity);
}

bool decreasePriority(MinHeap* heap, int id, int newPriority){
       /* Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 * Note: this function bubbles up the node until the heap property is restored.
 */
       if(invalid_id(heap, id)){
              return false;
       }
       int self_index = heap->indexMap[id];
       int self_priority = heap->arr[self_index].priority;
       if (newPriority >= self_priority){
              return false;
       }

       heap->arr[self_index].priority = newPriority;
       bubbleUp(heap, self_index);
       return true;
}


int getPriority(MinHeap* heap, int id){
/* Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
       int index = heap->indexMap[id];
       return priorityAt(heap, index);
}

void insert(MinHeap* heap, int priority, int id){

       /* Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Precondition: 'id' is unique within this minheap
 *               0 <= 'id' < heap->capacity
 *               heap->size < heap->capacity
 */

       if(heap->size >= heap->capacity){
              printf("capacity reached, did not insert\n");
              return;
       }

       HeapNode node;
       node.id = id;
       node.priority = priority;
       heap->size++; 
       heap->arr[heap->size] = node;
       // update the indexmapping
       heap->indexMap[id] = heap->size;
       // now bubble up the newly inserted node
       int self_index = heap->size;
       
       // bubble up
       bubbleUp(heap, self_index);
}


void deleteHeap(MinHeap* heap){
/* Frees all memory allocated for minheap 'heap'.
 */
     free(heap->arr);
     free(heap->indexMap);
     free(heap);
}
/*********************************************************************
 ** Helper function provided
 *********************************************************************/
void printHeap(MinHeap* heap) {
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, priorityAt(heap, i), idAt(heap, i), i,
           indexOf(heap, i));
  printf("%d: %d [%d]\t\t\n", heap->capacity, priorityAt(heap, heap->capacity),
         idAt(heap, heap->capacity));
  printf("\n\n");
}

