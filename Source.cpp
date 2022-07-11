#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include "Grafo.h"

#define range pair<int, int>

using namespace std;

void leerEntrada(vector<range>& data) {

	vector<int> rawdata;

	string s;
	cin >> s;

	while (s != "") {
		rawdata.push_back(stoi(s));
		s = "";
		cin >> s;
	}

	for (int i = 1; i < rawdata.size() - 1; i += 2) {
		data.push_back({ rawdata[i], rawdata[i + 1] });
	}
}

void leerTest(string url, vector<range>& data) {
	
	ifstream file;
	file.open(url);

	vector<int> rawdata;

	string s;
	file >> s;

	while (s != "") {
		rawdata.push_back(stoi(s));
		s = "";
		file >> s;
	}

	for (int i = 1; i < rawdata.size() - 1; i += 2) {
		data.push_back({ rawdata[i], rawdata[i + 1] });
	}

	file.close();
}

int main() {
	
	vector<range> data;
	leerEntrada(data);

	Grafo G (data);

	return 0;
}