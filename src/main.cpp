#include <iostream>
#include <vector>
#include <random>
#include <algorithm>




#define TAMANHO 100;

int t[100][100];

typedef struct Subsequence {

	double T, C; // Tempo e custo da subsequência
	int W; // Custo de atraso
	int first, last; // primeiro e último nós da subsequência

	inline static Subsequence Concatenate(Subsequence& sigma_1, Subsequence& sigma_2) {
		Subsequence sigma; // Subsequencia gerada pela concatenação
		
		/* Tempo do último nó da subsequência 1, até o primeiro nó da segunda	*/
		double temp = t[sigma_1.last][sigma_2.first];
		sigma.W = sigma_1.W + sigma_2.W; // Calculo do custo de atraso
		sigma.T = sigma_1.T + temp + sigma_2.T; // Calculo do tempo total
		sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;

		sigma.first = sigma_1.first;
		sigma.last = sigma_2.last;
		
		return sigma;
	}
}Subsequence;


typedef struct {
	/* Toda solução possui uma sequencia inicial hamiltoniana, começa e inicia no primeiro nó. */
	std::vector < int > sequence = {1, 1};

}Solution;



void escolher3Aleatorios(Solution* s, int dimensao) {
		
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<>dist(1, dimensao); // Gera numeros aleatorios de 1 até a dimensão dada
	
	std::vector < int > sequencia = s->sequence;
	
	std::sort(sequencia.begin(), sequencia.end()); // Organiza os valores para aplicar a busca binária
	for(int i = 0; i < 3; i++) {

		int numeroEscolhido = dist(gen);


		/* Aplicando busca binaria*/		
		int limiteSuperior = sequencia.size() - 1;
		int meio = 0;
		int limiteInferior = 0;
		bool numeroExiste = false;

		while(limiteInferior <= limiteSuperior) {
			
			meio = (limiteInferior + limiteSuperior) / 2;

			int valorCentral = sequencia[meio];

			if(valorCentral == numeroEscolhido) {

				numeroExiste = true;
				break;
			}
			if (valorCentral > numeroEscolhido){
				
				limiteInferior = meio + 1;

			}
			else {
				limiteSuperior = meio - 1;
			}
			
		}
		
		//Caso o valor não exista, colocamos ele na posição de indice i + 1, pois o primeiro valor deve ser 1 
		if(!numeroExiste) {
			s->sequence.insert(s->sequence.begin() + i + 1, numeroEscolhido);
		}
		else {
			i--;
		}


	}

}

Solution * Construction(int dimensao) {

	Solution* s = new Solution(); // Inicializa uma solução

	escolher3Aleatorios(s, 10);

	for(int i = 0; i < s->sequence.size(); i++) {

		std::cout << s->sequence[i] << " ";
	}
	std::cout << '\n';
	
	return s;
}

int main() {
	
	Solution* s = Construction(10);	
	return 0;
}
