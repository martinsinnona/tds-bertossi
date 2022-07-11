#include <vector>

#define range pair<int,int>
#define edge pair<int,int>
#define matrix vector<vector<int>>

using namespace std;

class Grafo {

	public:
		Grafo(vector<range> data);
		Grafo(int V, vector<edge> E, vector<int> W);

	private:
		int V;
		vector<edge> E;
		vector<int> W;

		matrix MATW;
		matrix ADJ;
		
		bool relax(vector<int>& d, vector<int>& pi, edge e);
		void generarMAT();

		vector<int> BERTOSSI(vector<edge>& B, vector<edge>& C);
		void DAG_SSSP(vector<int>& d, vector<int>& pi, int s);
		vector<int> TOPO_SORT(int s);
		void DFS_VISIT(vector<int>& vertices, vector<bool>& ready, int u);

		void printRes(vector<int> res_aux, vector<pair<range,int>> &data_ord);
};