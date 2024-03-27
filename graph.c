/*
 * Our graph implementation.
 *
 * Author: A. Tafliovich.
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge* edge) {
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList* head) {
  while (head != NULL) {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex* vertex) {
  if (vertex == NULL) {
    printf("NULL");
  } else {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph* graph) {
  if (graph == NULL) {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++) {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/
Edge* newEdge(int fromVertex, int toVertex, int weight){
  /* Returns a newly created Edge from vertex with ID 'fromVertex' to vertex
 * with ID 'toVertex', with weight 'weight'.
 */
  Edge * edge = (Edge*) calloc(1, sizeof(Edge));
  edge->fromVertex = fromVertex;
  edge->toVertex = toVertex;
  edge->weight = weight;
  return edge;
}

EdgeList* newEdgeList(Edge* edge, EdgeList* next){
  /* Returns a newly created EdgeList containing 'edge' and pointing to the next
 * EdgeList node 'next'.
 */
 EdgeList* edgeList = (EdgeList *) malloc(sizeof(EdgeList));
 if(edgeList ==NULL){
  fprintf(stderr, "memory not allocated for edgeList");
 }
 edgeList->edge = edge;
 edgeList->next = next;
 return edgeList;
}


Vertex* newVertex(int id, void* value, EdgeList* adjList){
  /* Returns a newly created Vertex with ID 'id', value 'value', and adjacency
 * list 'adjList'.
 * Precondition: 'id' is valid for this vertex
 */
  Vertex * vertex = (Vertex *) malloc(sizeof(Vertex));
  vertex->id = id;
  vertex->adjList = adjList;
  vertex->value = value;
  return vertex;
}


Graph* newGraph(int numVertices){
  /* Returns a newly created Graph with space for 'numVertices' vertices.
 * Precondition: numVertices >= 0
 */
  Graph* graph = (Graph *) malloc(sizeof(Graph));
  graph->numVertices = numVertices;
  graph->numEdges = 0;
  graph->vertices = (Vertex **) calloc(numVertices, sizeof(Vertex *));
  for(int i =0; i < numVertices; i++){
    graph->vertices[i] = NULL;
  }

  return graph;
}


void deleteEdgeList(EdgeList* head){
  /* Frees memory allocated for EdgeList starting at 'head'.
 */
  EdgeList * p= head;
  EdgeList * q = NULL;
  while(p!=NULL){
    q = p->next;
    free(p->edge);
    free(p);
    p=q;
  }
}


void deleteVertex(Vertex* vertex){
  if (vertex == NULL) return;  // Safe-guard against NULL input

  EdgeList *current = vertex->adjList;
  while (current != NULL) {
    EdgeList *temp = current;
    current = current->next;

    // If Edges are dynamically allocated, free them here
    free(temp->edge);  // Assuming each edge is dynamically allocated

    free(temp);  // Free the EdgeList node itself
  }

  // Finally, free the vertex
  free(vertex);
}


void deleteGraph(Graph* graph){
  /* Frees memory allocated for 'graph'.
 */
  for(int i=0; i<graph->numVertices; i++){
    deleteVertex(graph->vertices[i]);
  }
  free(graph->vertices);
  free(graph);
}