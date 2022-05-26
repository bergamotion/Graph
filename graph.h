#pragma once

struct list {
	int val;
	list* next;
};

struct node {
	int value;
	list* neighbours;
	node* next;
};

struct graph {
	int count;
	node* head; // node **list;
};

graph* graph_init(int n);
void add_arc(graph* g, int a, int b);
void del_arc(graph* g, int a, int b);
void add_edge(graph* g, int a, int b);
void del_edge(graph* g, int a, int b);
void list_append(list** l, int a);
void list_del(list** l, int a);
void graph_free(graph* g);
void graph_print(graph* g);
graph* create_graph();
node* graph_get_vertex(graph* g, int vertex_number);
int* bipartite_bfs(graph* g);
int* bipartite_dfs(graph* g);
//int* topological_sort(graph* g);
int* topological_sort(graph* g);
void graph_write(graph* g, char* filename);
graph* graph_read(char* filename);
graph* transpose_graph(graph* g);
int* graph_scc(graph* g);



