#include <iostream>
#include <vector>

using namespace std;


int main() {

	vector < int > valores = {1, 2 ,3 ,4 , 5, 6, 7, 8, 1};


	int best_i = 1;
	int best_j = 5;
	
	int valoresReferencia = valores[best_j];
	for(int i = 0; i < valores.size(); i++) {

		std::cout << valores[i] << " ";
	}
	std::cout << "\n";
	std::vector <int> valoresAdicionar = {valores[best_i], valores[best_i+ 1]};
	
	for(int i = 0; i < valoresAdicionar.size(); i++) {
		valores.erase(valores.begin() + best_i);
	}
	for(int i = 0; i < valoresAdicionar.size(); i++) {

		valores.insert(valores.begin() + best_j + i, valoresAdicionar[i]);
	}	
	for(int i = 0; i < valores.size(); i++) {

		std::cout << valores[i] << " ";
	}
	std::cout << "\n";
	return 0;
}
