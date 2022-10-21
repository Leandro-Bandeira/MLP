#include <iostream>
#include <vector>

using namespace std;


int main() {

	vector < int > valores = {1, 2 ,3 ,4 , 5, 6, 7, 8, 1};


	int posicaoInicial = 1;

	int posicaoFinal = 4;
	
	vector < int > valoresAdicionar;
	for(int i = 0; i < 2; i++) {
		
		valoresAdicionar.push_back(valores[posicaoInicial + i]);

	}
	
	valores.insert(valores.begin() + posicaoFinal, valoresAdicionar.begin(), valoresAdicionar.end());
	
	for(int i = 0; i < 2; i++) {

		valores.erase(valores.begin() + posicaoInicial + i);
	}
	for(int i = 0; i < valores.size(); i++) {

		std::cout << valores[i] << " ";
	}
	std::cout << "\n";
	return 0;
}
