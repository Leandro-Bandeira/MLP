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

	std::uniform_int_distribution<>dist(2, dimensao); // Gera numeros aleatorios de 2 até a dimensão dada
	
	std::vector < int > sequencia = s->sequence; // inicializa um vetor temporario com os dados do antigo
	

	int adicionados = 0;
	while(adicionados < 3) {

		int numeroEscolhido = dist(gen);
		
		std::sort(sequencia.begin(), sequencia.end()); // Organiza os valores para aplicar a busca binária
		/* Aplicando busca binaria	*/		
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
				
				limiteSuperior = meio - 1;

			}
			else {
				limiteInferior = meio + 1;
			}
			
		}
		
		
		// Adiciona a partir do índice 1, pois o 0 é o valor 1 2 4 5 3 1, temos que ter essa sequência
		// Caso ele não acha o velor, adiciona referente ao ponto adicionado
		/*				
		if(!std::binary_search(sequencia.begin(), sequencia.end(), numeroEscolhido)) {
			//Atualiza os dois vetores
			sequencia.insert(sequencia.begin() + adicionados + 1, numeroEscolhido);
			s->sequence.insert(s->sequence.begin() + (adicionados + 1), numeroEscolhido);
			++adicionados;
		}
		
		//Caso o valor não exista, colocamos ele na posição de indice i + 1, pois o primeiro valor deve ser 1 
		*/
		if(!numeroExiste) {
			s->sequence.insert(s->sequence.begin() + (adicionados + 1), numeroEscolhido);
			sequencia.insert(sequencia.begin() + (adicionados + 1), numeroEscolhido);
			++adicionados;
		}
		


	}


}

Solution * Construction(int dimensao) {

	Solution* s = new Solution(); // Inicializa uma solução
	
	/* Escolhe inicialmente 3 valores aleatórios	*/
	escolher3Aleatorios(s, dimensao);
	

	/* Gera CL - Candidate list	*/
	std::vector < int > sequencia(s->sequence);
	std::sort(sequencia.begin(), sequencia.end()); // Organiza  para poder utilizar o binary_search
	std::vector < int > CL; // Candidate list
	/* Percorre todos os valores de 2 até o valor da dimensão, verifica se ele está dentro da sequencia da solução, caso não o adiciona */
	for(int i = 2; i <= dimensao; i++) {
		
		/* Caso o valor de i não esteja na sequencia, adiciona na lista de candidado	*/ 
		if(!std::binary_search(sequencia.begin(), sequencia.end(), i)) {
			CL.push_back(i);
		}

	}
	
	int origem = 0;
	
	/* A partir de CL, vamos criar uma lista chamada RCL, que são os melhores candidados da distancia deles até a origem*/
	/* Primeiramente temos que colocar CL em ordem crescente da sua distancia da origem*/
	while(CL.empty()) {



	}

	
	return s;
}

int main() {
	
	int dimensao = 0;
	std::cin >> dimensao;

	int** matriz = new int*[dimensao];

	for(int i = 0; i < dimensao; i++) {

		matriz[i] = new int(dimensao);

		for(int j = 0; j < dimensao; j++) {

			std::cin >> matriz[i][j];
		}
	}
	
	Solution* s = Construction(10);	
	
	for(int i = 0; i < dimensao; i++) {

		for(int j = 0; j < dimensao; j++) {

			std::cout << matriz[i][j] << " ";
		}
		std::cout << std::endl;
	}
	for(int i = 0; i < dimensao; i++) {


			delete matriz[i];
	}
	return 0;
}
