#include <iostream>
#include "graph.h"
#include "queue.h"
using namespace std;



int main()
{

	/*graph* g = create_graph();
	graph_print(g);
	int* part = bipartite_dfs(g);

	if (part) {
		cout << "Vertices: ";
		for (int i = 0; i < g->count; ++i) {
			cout << i << " ";
		}

		cout << endl;

		cout << "Colours:  ";
		for (int i = 0; i < g->count; i++) {
			cout << part[i] << " ";
		}
	}
	else {
		cout << "Graph is not bipartite";
	}*/


	char filename[] = "graph.txt";
	graph* g = graph_read(filename);
	
	int* comp = graph_scc(g);
	for (int i = 0; i < g->count; ++i) {
		cout << i << " - " << comp[i] << endl;
	}

}
