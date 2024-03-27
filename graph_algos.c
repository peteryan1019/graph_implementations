/*
 * Graph algorithms.
 *
 * Author (of starter code): A. Tafliovich.
 */

#include <limits.h>
#include <stdlib.h>

#include "graph.h"
#include "minheap.h"

#define NOTHING -1
#define DEBUG 0

typedef struct records {
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/


Records* initRecords(Graph* graph, int startVertex){
/* Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
  Records * records = (Records *) malloc(sizeof(Records));
  records->numVertices = graph->numVertices;
  int n = graph->numVertices;
  bool * finished = (bool *) calloc(n, sizeof(bool));
  records->finished = finished;
  MinHeap * heap = newHeap(n);
  //not sure
  for(int i=0; i<graph->numVertices; i++){
    insert(heap, INT_MAX, i);
  }
  decreasePriority(heap, startVertex, 0);
  records->heap = heap;
  Edge * tree = (Edge *) calloc(n^2, sizeof(Edge));
  records->tree = tree;
  int * predecessors = (int *) calloc(n, sizeof(int));
  predecessors[startVertex] = startVertex;
  records->predecessors = predecessors;
  records->numTreeEdges = 0;

  return records;
}


/* Returns true iff 'heap' is NULL or is empty. */
bool isEmpty(MinHeap* heap){
  if(heap==NULL){
    return true;
  }
  return (heap->size == 0);
}

/* Prints the status of all current algorithm data: good for debugging. */

/* Add a new edge to records at index ind. */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight){
    Edge newEdge;
    newEdge.fromVertex = fromVertex;
    newEdge.toVertex = toVertex;
    newEdge.weight = weight;
    records->tree[ind] = newEdge;
    records->numTreeEdges ++;
  }

  EdgeList * append(EdgeList * path, EdgeList * newItem){
    if(newItem == NULL){
      return path;
    }
    if(path ==NULL){
      return newItem;
    }
    EdgeList * p = path;
    while(p->next!=NULL){
      p = p->next;
    }
    p->next = newItem;
    return path;
  }

Edge * findPreviousEdge(EdgeList* path){
    if(path==NULL){
      return NULL;
    }
    EdgeList * p = path;
    while(p->next !=NULL){
      p = p->next;
    }
    return p->edge;

}

/* Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex, int numVertices) {
    // Initialize an empty path
    EdgeList* path = NULL;

    // Initialize a current vertex to start tracing back from
    int currentVertex = vertex;

    // Continue until we reach the start vertex
    while (currentVertex != startVertex) {
        // Find the edge in distTree leading to the current vertex (i.e. the predecessor of vertex)
        Edge* edgeToCurrent = NULL;
        for (int i = 0; i < numVertices; i++) {
            if (distTree[i].toVertex == currentVertex) {
                edgeToCurrent = newEdge(distTree[i].fromVertex, distTree[i].toVertex, distTree[i].weight);
                break;
            }
        }

        // If no edge found, path construction fails
        if (edgeToCurrent == NULL) {
            printf("Error: No edge leading to vertex %d found.\n", currentVertex);
            deleteEdgeList(path); // Cleanup any allocated memory
            return NULL;
        }

        // Append this edge to the path
        EdgeList* newItem = newEdgeList(edgeToCurrent, NULL);
        if(path!=NULL){
        Edge * previous = findPreviousEdge(path);
        previous->weight = previous->weight - newItem->edge->weight;}
        path = append(path, newItem);
        // Move to the predecessor vertex to continue tracing the path
        currentVertex = edgeToCurrent->fromVertex;
    }
    return path;
}


/*************************************************************************
 ** Required functions
 *************************************************************************/

Edge* getMSTprim(Graph* graph, int startVertex){
  /* Runs Prim's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting MST: an array of Edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */
  int i=0;
  Records* records = initRecords(graph, startVertex);
  if(records->heap->indexMap[startVertex]==NOTHING){
    return NULL;
  }

  while(!isEmpty(records->heap)){
    HeapNode h = extractMin(records->heap);
    records->finished[h.id] = true;
    if(h.id != startVertex){
      addTreeEdge(records, i++, records->predecessors[h.id], h.id, h.priority);
    }
   
    EdgeList * elist;
    for(elist = graph->vertices[h.id]->adjList; elist!=NULL; elist = elist->next){
      if(records->finished[elist->edge->toVertex]==false && elist->edge->weight <getPriority(records->heap, elist->edge->toVertex)){
        decreasePriority(records->heap, elist->edge->toVertex, elist->edge->weight);
        records->predecessors[elist->edge->toVertex] = h.id;
      }
    }
  }
  return records->tree;
}

Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex){
  /* Runs Dijkstra's algorithm on Graph 'graph' starting from vertex with ID
 * 'startVertex', and return the resulting distance tree: an array of edges.
 * Returns NULL if 'startVertex' is not valid in 'graph'.
 * Precondition: 'graph' is connected.
 */

  int i =0;
  Records * records = initRecords(graph, startVertex);
  if(records->heap->indexMap[startVertex]==NOTHING){
    return NULL;
  }
  int n = graph->numVertices;
  int * distances = malloc(n * sizeof(int)); 
  //for any vertex v, distances[v.id] = distance from startVertex to v
  for(int j=0; j < n; j++){
    distances[j] = INT_MAX;
  }
  distances[startVertex] = 0;
  while(!isEmpty(records->heap)){
    HeapNode u = extractMin(records->heap);
    records->finished[u.id] = true;
    addTreeEdge(records, i++, records->predecessors[u.id], u.id, distances[u.id]);
    EdgeList * elist;
    for(elist = graph->vertices[u.id]->adjList; elist!=NULL; elist = elist->next){
      int v = elist->edge->toVertex;
      int distance = distances[u.id] + elist->edge->weight;
      if(distance < distances[v]){
        decreasePriority(records->heap, v, distance);
        distances[v] = distance;
        records->predecessors[v] = u.id;
      }
    }
  }
  return records->tree;
}

EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex){
  /* Creates and returns an array 'paths' of shortest paths from every vertex
 * in the graph to vertex 'startVertex', based on the information in the
 * distance tree 'distTree' produced by Dijkstra's algorithm on a graph with
 * 'numVertices' vertices and with the start vertex 'startVertex'.  paths[id]
 * is the list of edges of the form
 *   [(id -- id_1, w_0), (id_1 -- id_2, w_1), ..., (id_n -- start, w_n)]
 *   where w_0 + w_1 + ... + w_n = distance(id)
 * Returns NULL if 'startVertex' is not valid in 'distTree'.
 */
  
  EdgeList ** e = calloc(numVertices, sizeof(EdgeList*));
  for(int v = 0; v < numVertices; v++){
    EdgeList * path = makePath(distTree, v, startVertex, numVertices);
    e[v] = path;
  }
  return e;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records) {
  if (records == NULL) return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}

// main(){
//   Graph * graph = (Graph*) calloc(1, sizeof(Graph));
//   graph->numEdges = 3;
//   graph->numVertices = 3;
//   Vertex * v0 = calloc(1, sizeof(Vertex));
//   v0->id = 0;
//   v0->value = 0;
//   Edge * edge1 = calloc(1, sizeof(Edge));
//   edge1->fromVertex = 0;
//   edge1->toVertex = 1;
//   edge1->weight = 1;
//   Edge * edge2 = calloc(1, sizeof(Edge));
//   edge2->fromVertex = 0;
//   edge2->toVertex = 2;
//   edge2->weight = 5;
//   v0->adjList->edge = edge1;
//   v0->adjList->next->edge = edge2;
//   v0->adjList->next->next = NULL;
//   graph->vertices[0] = v0;

//   Vertex * v1 = calloc(1, sizeof(Vertex));
//   v1->id = 1;
//   v1->value = 1;
//   Edge * edge3 = malloc(sizeof(Edge));
//   edge3->fromVertex = 1;
//   edge3->toVertex = 2;
//   edge3->weight = 2;
//   v1->adjList->edge = edge3;
//   v1->adjList->next = NULL;

//   Vertex* v2 = calloc(1, sizeof(Vertex));
//   v2->id = 2;
//   v2->value = 2;
//   v2->adjList = NULL;

//   Edge * tree = getDistanceTreeDijkstra(graph, 0);
//   printTree(tree, graph->numEdges);
// }
