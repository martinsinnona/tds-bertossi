#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>
#include "Grafo.h"

#define range pair<int, int>
#define edge pair<int, int>
#define matrix vector<vector<int>>
#define INF INT_MAX
#define NIL -1

using namespace std;

bool sortbyF(const pair<range, int>& a, const pair<range, int>& b){
	return (a.first.first < b.first.first);
}

Grafo::Grafo(vector<range> data) {

	V = data.size();

	vector<pair<range, int>> data_ord;

	// dummies

	int min_a = INT_MAX;
	int max_b = -INT_MAX;

	data_ord.push_back({ {-INT_MAX, 0}, -1 });	// left dummie

	for (int i = 0; i < data.size(); i++) {
		data_ord.push_back({ data[i], i });
		if (data[i].second > max_b) max_b = data[i].second;
	}

	min_a = data_ord[1].first.first;
	data_ord.push_back({ {INT_MAX, 0}, -1 });	// right dummie

	sort(data_ord.begin(), data_ord.end(), sortbyF);

	// se establecen los valores de los dummies

	data_ord[0].first = { min_a - 2, min_a - 1 };
	data_ord[V + 1].first = { max_b + 1, max_b + 2 };

	vector<int> res_aux;
	if (data_ord[1].first.second == max_b) {
		res_aux.push_back(1);
		res_aux.push_back(2);
		printRes(res_aux, data_ord);
		return;
	}


	// se ignoran los intervalos contenidos (pau preguntó y no es necesario)

	vector<bool> ignorado (V + 2, false);

	for (int i = 0; i < data_ord.size(); i++) {
		
		range r_i = data_ord[i].first;
		for (int j = 0; j < data_ord.size(); j++) {
			
			range r_j = data_ord[j].first;

			if (r_j.first < r_i.first && r_j.second > r_i.second) {
				ignorado[i] = true;
				break;
			}
		}
	}

	// se generan aristas de B y C

	vector<edge> B;
	vector<edge> C;

	for (int i = 0; i < data_ord.size(); i++) {

		range range_i = data_ord[i].first;

		int jmin = -1;
		int bmin = -1;

		for (int j = 0; j < data_ord.size(); j++) { // se puede mejorar <---------------

			range range_j = data_ord[j].first;

			bool b1 = range_i.first < range_j.first;
			bool b2 = range_i.second < range_j.second;
			bool b3 = range_j.first < range_i.second;

			if (b1 && b2 && b3) {
				if (!ignorado[i] && !ignorado[j]) B.push_back({ i, j });
			}
			else if (range_j.first > range_i.second && (jmin == -1 || bmin > range_j.second)) {
				jmin = j;
				bmin = range_j.second;
			}
		}
		for (int j = 0; j < data_ord.size(); j++)
		{
			range range_j = data_ord[j].first;

			if (range_j.first > range_i.second && range_j.first < bmin && !ignorado[j]) {
				C.push_back({ i, j });
			}
		}
	}

	// algoritmo principal

	res_aux = BERTOSSI(B, C);

	vector<int> res;

	printRes(res_aux, data_ord);
}

Grafo::Grafo(int V, vector<edge> E, vector<int> W) {

	this->V = V;
	this->E = E;
	this->W = W;
}

vector<int> Grafo::BERTOSSI(vector<edge>& B, vector<edge>& C) {

	int V = this->V + 2;
	vector<edge> E;
	vector<int> W;

	// arista entre i_in -> i_out, salvo para los dummies

	for (int i = 1; i < V - 1; i++) {
		E.push_back({ i, i + V });
		W.push_back(0);
	}

	// aristas de B, i_out -> j_in

	for (edge e : B) {
		E.push_back({e.first + V, e.second});
		W.push_back(1);
	}

	// aristas de C, i_in -> j_out

	for (edge e : C) {
		E.push_back({e.first, e.second + V});

		if (e.first != 0) { 
			W.push_back(1); 
		}else {
			W.push_back(0);
		}
	}

	// camino mínimo entre dummy y dummy

	Grafo G (V * 2, E, W);
	
	vector<int> d;
	vector<int> pi;

	G.DAG_SSSP(d, pi, 0);

	// se arma la solucion

	vector<int> camino;
	camino.push_back(G.V - 1);

	while (pi[camino.back()] != -1) {
		camino.push_back(pi[camino.back()]);
	}

	vector<int> res;

	// remapeo de los valores, corresponderán a indices de data_ord

	for (int i = 1; i < camino.size() - 1; i++) {

		int valor = camino[i] % V;

		if (res.size() > 0 && res.back() == valor) res.pop_back(); 
		res.push_back(valor);
	}

	return res;
}

bool Grafo::relax(vector<int>& d, vector<int>& pi, edge e) {

	int u = e.first;
	int v = e.second;

	if (d[u] != INF && d[v] > d[u] + MATW[u][v]) {

		d[v] = d[u] + MATW[u][v];
		pi[v] = u;

		return true;
	}

	return false;
}

void Grafo::generarMAT() {

	for (int i = 0; i < V; i++) {

		vector<int> v(V, INF);
		MATW.push_back(v);

		vector<int> v2;
		ADJ.push_back(v2);
	}

	int w = 0;
	for (edge e : E) {
		MATW[e.first][e.second] = W[w];
		ADJ[e.first].push_back(e.second);
		w++;
	}
}

vector<int> Grafo::TOPO_SORT(int s) {

	generarMAT();

	vector<int> vertices;
	vector<bool> ready(V, false);

	DFS_VISIT(vertices, ready, s);

	return vertices;
}

void Grafo::DFS_VISIT(vector<int>& vertices, vector<bool>& ready, int u) {
	
	ready[u] = true;

	for (int v : ADJ[u]) {
		if (!ready[v]) DFS_VISIT(vertices, ready, v);
	}

	vertices.push_back(u);
}

void Grafo::DAG_SSSP(vector<int>& d, vector<int>& pi, int s) {
	
	vector<int> vertices = TOPO_SORT(s);

	d = vector<int>(V, INF);
	pi = vector<int>(V, NIL);

	d[s] = 0;
	
	for (int i = vertices.size() - 1; i >= 0; i--) {

		int u = vertices[i];

		for (int v : ADJ[u]) {
			relax(d, pi, { u,v });
		}
	}
}

void Grafo::printRes(vector<int> res, vector<pair<range,int>> &data_ord) {

	cout << res.size() << endl;

	for (int i : res) {

		res.push_back(data_ord[i].second);
		cout << res.back() << " ";
	}
}