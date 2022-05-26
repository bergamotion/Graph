#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "graph.h"
#include "queue.h"
#include "stack.h"
#define max(a,b)   (((a) > (b)) ? (a) : (b))
#define min(a,b)   (((a) < (b)) ? (a) : (b))


graph* graph_init(int n)
{
	graph* result = (graph*)malloc(sizeof(graph));
	if (result) {
		result->count = n;
		result->head = (node*)malloc(sizeof(node));
		node* current_node = result->head;
		for (int i = 0; i < n - 1; i++) {
			current_node->value = i;
			current_node->neighbours = nullptr;
			current_node->next = (node*)malloc(sizeof(node));
			current_node = current_node->next;
		}

		current_node->value = n - 1;
		current_node->neighbours = nullptr;
		current_node->next = nullptr;
		return result;
	}
}

void add_arc(graph* g, int a, int b)
{
	node* current_node = g->head;
	while (current_node->value != a)
		current_node = current_node->next;
	list_append(&(current_node->neighbours), b);
}

void del_arc(graph* g, int a, int b)
{
	node* current_node = g->head;
	while (current_node->value != a)
		current_node = current_node->next;
	list_del(&(current_node->neighbours), b);
}


void add_edge(graph* g, int a, int b)
{
	add_arc(g, a, b);
	add_arc(g, b, a);
}


void del_edge(graph* g, int a, int b)
{
	del_arc(g, a, b);
	del_arc(g, b, a);
}

void graph_free(graph* g)
{
	node* current_node = g->head;
	while (current_node != nullptr) {
		list* list_elem = current_node->neighbours;
		while (list_elem != nullptr){
			list* temp = list_elem;
			list_elem = list_elem->next;
			free(temp);
		}
		node* temp = current_node;
		current_node = current_node->next;
		free(temp);
	}
	free(g);
}

void graph_print(graph* g)
{
	if (g) {
		node* current_node = g->head;
		while (current_node != nullptr) {
			std::cout << current_node->value << ": ";
			list* list_elem = current_node->neighbours;
			while (list_elem != nullptr) {
				if (list_elem->next == nullptr) std::cout << list_elem->val;
				else std::cout << list_elem->val << ", ";
				list_elem = list_elem->next;

			}
			current_node = current_node->next;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

}

void list_append(list** l, int a)
{
	if (*l == nullptr) {
		*l = (list*)malloc(sizeof(list));
		if (*l) {
			(*l)->next = nullptr;
			(*l)->val = a;
		}
	}
	else {
		list* current_node = *l;
		while (true) {
			if (current_node->val == a) return;
			if (current_node->next == nullptr) break;
			current_node = current_node->next;
		}
		current_node->next = (list*)malloc(sizeof(list));
		current_node = current_node->next;
		current_node->val = a;
		current_node->next = nullptr;
	}
}

void list_del(list** l, int a)
{
	if (*l) {
		if ((*l)->val == a) {
			list* temp = *l;
			*l = (*l)->next;
			free(temp);
			return;
		}
		list* current_node = (*l)->next;
		list* previous_node = *l;
		while (current_node) {
			if (current_node->val == a) {
				list* temp = current_node;
				previous_node->next = current_node->next;
				free(temp);
				return;
			}
			previous_node = current_node;
			current_node = current_node->next;
		}
	}
}

node* graph_get_vertex(graph* g, int vertex_number)
{
	node* vertex = g->head;
	if (vertex_number >= g->count || vertex_number < 0) return nullptr;
	for (int i = 0; i < vertex_number; i++) {
		vertex = vertex->next;
	}
	return vertex;
}

graph* create_graph()
{
	int n;
	std::cout << "Enter number of vertices: ";
	std::cin >> n;
	std::cout << std::endl;
	graph* g = graph_init(n);
	int a, b;
	std::cout << "Enter arcs, enter (-1, -1) to stop: " << std::endl;
	while (true) {
		std::cout << "> "; 
		std::cin >> a >> b;
		if (a == -1) break;
		add_arc(g, a, b);
	}
	std::cout << std::endl;
	return g;
}

int* bipartite_bfs(graph* g)
{
	node* current_vertex;
	list* neighbour;
	int* vertex_colour = (int*)calloc(g->count, sizeof(int));
	int* visited = (int*)calloc(g->count, sizeof(int));
	queue* vertices = queue_init();

	queue_push(vertices, 0);
	
	if (vertex_colour && visited) {

		visited[0] = 1;
		vertex_colour[0] = 1;
		while (*(vertices->front))
		{
			current_vertex = graph_get_vertex(g, queue_pop(vertices));
			neighbour = current_vertex->neighbours;
			while (neighbour) {
				if (!visited[neighbour->val]) {
					queue_push(vertices, neighbour->val);
					visited[neighbour->val] = 1;
				}

				if (!vertex_colour[neighbour->val]) {
					vertex_colour[neighbour->val] = vertex_colour[current_vertex->value] % 2 ? 2 : 1;
				}
				else if (vertex_colour[neighbour->val] == vertex_colour[current_vertex->value]) {
					free(vertex_colour);
					free(visited);
					return NULL;
				}
				neighbour = neighbour->next;
			}
		}
	}
	free(visited);
	return vertex_colour;
}

int* bipartite_dfs(graph* g)
{
	int* vertex_colour = (int*)calloc(g->count, sizeof(int));
	int* visited = (int*)calloc(g->count, sizeof(int));
	stack* vertices = stack_new_node(0);
	node* current_vertex;
	list* neighbour;

	if (vertex_colour && visited) {
		vertex_colour[0] = 1;

		while (vertices) {
			current_vertex = graph_get_vertex(g, stack_pop(&vertices));
			visited[current_vertex->value] = 1;
			neighbour = current_vertex->neighbours;
			
			while (neighbour) {
				if (!visited[neighbour->val]) {
					vertices = stack_push(vertices, neighbour->val);
				}  

				if (!vertex_colour[neighbour->val]) {
					vertex_colour[neighbour->val] = vertex_colour[current_vertex->value] % 2 ? 2 : 1;
				}
				else if (vertex_colour[neighbour->val] == vertex_colour[current_vertex->value]) {
					free(vertex_colour);
					free(visited);
					return NULL;
				}

				neighbour = neighbour->next;
			}
		}
	}
	free(visited);
	stack_free(vertices);
	return vertex_colour;
}




//int* topological_sort(graph* g)
//{
//	node* current_vertex;
//	list* neighbour;
//	int* vertex_colour = (int*)calloc(g->count, sizeof(int));
//	int* result = (int*)calloc(g->count, sizeof(int));
//	int k = 0;
//
//	for (int i = 0; i < g->count; ++i) {
//		if (vertex_colour[i] != 2) {
//			stack* result_stack = stack_init();
//			stack* temp_stack = stack_init();
//			temp_stack = stack_push(temp_stack, i);
//			result_stack = stack_push(result_stack, i);
//			vertex_colour[i] = 1;
//
//			while (temp_stack) {
//				current_vertex = graph_get_vertex(g, stack_pop(&temp_stack));
//			
//				neighbour = current_vertex->neighbours;
//
//				while (neighbour) {
//					if (vertex_colour[neighbour->val] == 1) {
//						free(vertex_colour);
//						free(result);
//						stack_free(result_stack);
//						stack_free(temp_stack);
//						return nullptr;
//					}
//					else if (vertex_colour[neighbour->val] != 2) {
//						temp_stack = stack_push(temp_stack, neighbour->val);
//						result_stack = stack_push(result_stack, neighbour->val);
//						vertex_colour[neighbour->val] = 1;
//					}
//
//					neighbour = neighbour->next;
//				}
//			}
//			while (result_stack) {
//				result[k] = stack_pop(&result_stack);
//				vertex_colour[result[k]] = 2;
//				++k;
//			}
//			stack_free(result_stack);
//			stack_free(temp_stack);
//		}
//	}
//	free(vertex_colour);
//
//	for (int i = 0; i < g->count / 2; i++)
//	{
//		int t = result[i];
//		result[i] = result[g->count - i - 1];
//		result[g->count - i - 1] = t;
//	}
//	return result;
//}

int* topological_sort(graph* g)
{
	node* current_vertex;
	list* neighbour;
	int* vertex_colour = (int*)calloc(g->count, sizeof(int));
	int* result = (int*)calloc(g->count, sizeof(int));
	int k = g->count - 1;

	for (int i = 0; i < g->count; ++i) {
		if (vertex_colour[i] != 2) {
			stack* result_stack = stack_init();
			stack* temp_stack = stack_init();
			temp_stack = stack_push(temp_stack, i);
			vertex_colour[i] = 1;

			while (temp_stack) {
				current_vertex = graph_get_vertex(g, stack_pop(&temp_stack));

				neighbour = current_vertex->neighbours;

				while (neighbour) {
					if (vertex_colour[neighbour->val] == 1) {
						free(vertex_colour);
						free(result);
						stack_free(result_stack);
						stack_free(temp_stack);
						return nullptr;
					}
					else if (vertex_colour[neighbour->val] != 2) {
						temp_stack = stack_push(temp_stack, neighbour->val);
						result_stack = stack_push(result_stack, neighbour->val);
						vertex_colour[neighbour->val] = 1;
					}
					neighbour = neighbour->next;
				}
			}
			while (result_stack) {
				result[k] = stack_pop(&result_stack);
				vertex_colour[result[k]] = 2;
				k--;
			}
			stack_free(result_stack);
			stack_free(temp_stack);
		}
	}
	free(vertex_colour);
	return result;
}

void graph_write(graph* g, char* filename)
{
	list* neighbour;
	std::ofstream fout(filename);
	fout << g->count << std::endl;
	for (int i = 0; i < g->count; i++) {
		neighbour = (graph_get_vertex(g, i))->neighbours;
		while (neighbour) {
			fout << i << "->" << neighbour->val << std::endl;
			neighbour = neighbour->next;
		}
	}
	fout.close();
}

graph* graph_read(char* filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open()) return nullptr;

	char* buff = (char*)malloc(sizeof(char) * 32);
	char* temp_to_free = buff;
	char* end;
	fin >> buff;
	int n = strtol(buff, &end, 10);
	graph* result = graph_init(n);
	int a, b;
	while (fin >> buff) {
		a = strtol(buff, &end, 10);
		if (buff == end) {
			free(buff);
			return nullptr;
		}
		buff = end;
		while (*buff != '>') ++buff;
		++buff;
		b = strtol(buff, &end, 10);
		if (buff == end) {
			free(temp_to_free);
			return nullptr;
		}
		add_arc(result, a, b);
	}
	free(temp_to_free);
	return result;
}

graph* transpose_graph(graph* g)
{
	graph* inverted_g = graph_init(g->count);
	node* current_vertex = g->head;
	list* neighbour;
	while (current_vertex) {
		neighbour = current_vertex->neighbours;
		while (neighbour) {
			add_arc(inverted_g, neighbour->val, current_vertex->value);
			neighbour = neighbour->next;
		}
		current_vertex = current_vertex->next;
	}
	return inverted_g;
}

int* graph_scc_dfs1(graph* g) {
	int* sorted_vertices = (int*)calloc(g->count, sizeof(int));
	int* visited = (int*)calloc(g->count, sizeof(int));
	int k = g->count - 1;
	node* current_vertex;
	list* neighbour;

	for (int i = 0; i < g->count; ++i) {
		if (!visited[i]) {
			stack* vertices = stack_new_node(i);
			stack* result_stack = stack_new_node(i);
			while (vertices) {
				current_vertex = graph_get_vertex(g, stack_pop(&vertices));
				visited[current_vertex->value] = 1;
				neighbour = current_vertex->neighbours;

				while (neighbour) {
					if (!visited[neighbour->val]) {
						vertices = stack_push(vertices, neighbour->val);
						result_stack = stack_push(result_stack, neighbour->val);
					}
					neighbour = neighbour->next;
				}
			}
			while (result_stack) {
				sorted_vertices[k] = stack_pop(&result_stack);
				k--;
			}
			stack_free(result_stack);
			stack_free(vertices);
		}
	}
	free(visited);
	return sorted_vertices;
}

int* graph_scc(graph* g)
{
	int* components = (int*)calloc(g->count, sizeof(int));
	int* sorted_vertices = graph_scc_dfs1(g);
	graph* transp_g = transpose_graph(g);
	node* current_vertex;
	list* neighbour;

	int* visited = (int*)calloc(g->count, sizeof(int));
	int component_index = 1;
	int vertex;

	for (int i = 0; i < g->count; ++i) {
		vertex = sorted_vertices[i];
		if (!visited[vertex]) {
			stack* vertices = stack_new_node(vertex);

			while (vertices) {
				current_vertex = graph_get_vertex(transp_g, stack_pop(&vertices));
				visited[current_vertex->value] = 1;
				components[current_vertex->value] = component_index;
				neighbour = current_vertex->neighbours;

				while (neighbour) {
					if (!visited[neighbour->val]) {
						vertices = stack_push(vertices, neighbour->val);
					}
					neighbour = neighbour->next;
				}
			}
			stack_free(vertices);
			component_index++;

		}
	}
	free(sorted_vertices);
	free(visited);
	graph_free(transp_g);

	return components;
}

void scc2_backtrack(graph* g, int* vertex_time, int* low, stack* vertices, int* is_mutable)
{
	while (vertices) {
		node* vertex = graph_get_vertex(g, stack_pop(&vertices));
		list* neighbour = vertex->neighbours;

		while (neighbour) {
			if (is_mutable(neighbour->val)) {
				low[vertex->value] = min(low[neighbour->val], low[neighbour->val]);

			}
		}
	}
	
}

int* graph_scc2(graph* g)
{
	stack* stck = stack_init();
	int* vertex_time = (int*)calloc(g->count, sizeof(int));
	int* low = (int*)calloc(g->count, sizeof(int));
	node* current_vertex;
	list* neighbour;
	int time = 1;

	for (int i = 0; i < g->count; i++) {

		if (!vertex_time[i]) {
			stack* vertices = stack_new_node(i);

			while (vertices) {
				current_vertex = graph_get_vertex(g, stack_pop(&vertices));
				if (!vertex_time[i]) {
					vertex_time[i] = time;
					low[i] = time;
					++time;

					neighbour = current_vertex->neighbours;
					while (neighbour) {
						if (!vertex_time[neighbour->val]) {
							vertices = stack_push(vertices, neighbour->val);
						}
						neighbour = neighbour->next;
					}
				}
			}
		}
	}

}