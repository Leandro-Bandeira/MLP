#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include <ctype.h> //Adiciona o isalnum



int **t;  






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





Solution * Construction(int dimensao, int** matriz) {

	Solution* s = new Solution(); // Inicializa uma solução
	std::vector < int > CL;	

	
	/* Criação da lista de candidatos	*/
	for(int i = 2; i <= dimensao; i++) {

		CL.push_back(i);
	}
	
	
	int r = 1; // Origem
	int insercaoIndice = 1;
	/* A partir de CL, vamos criar uma lista chamada RCL, que são os melhores candidados da distancia deles até a origem*/
	/* Primeiramente temos que colocar CL em ordem crescente da sua distancia da origem*/
	while(!CL.empty()) {
		
		/*O método sort pode receber uma função como terceiro parametro, dito isso ele está recebendo uma função lambda
		 * Cada valor da função CL, será passado para a função em forma de A e B, por passagem de valor, desse modo 
		 * podemos comparar as matrizes de cada um e agrupá-los em forma crescente da menor distancia*/
		
		// Devemos retirar 1, porque CL contém os pontos de 1 ate o ponto de dimensao, mas matriz vai de 0 ate dimensao - 1
		std::sort(CL.begin(), CL.end(), [=](int A, int B) {
			return matriz[A - 1][r - 1] < matriz[B - 1][r - 1];
		});
		
	
		/*Gera valor aleatorio entre 1 e o tamanho de CL */
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<>alpha(1, CL.size());
		
		/* Vamos pegar a quantia de melhores candidados a partir do inicio de CL*/
		std::vector < int > RCL;
		int quantiaMelhoresCandidatos = alpha(gen);
		
		
		for(int i = 0; i < quantiaMelhoresCandidatos; i++) {
			RCL.push_back(CL[i]);
		}
		/* Vamos escolher aleatoriamente um valor de RCL	*/
		
		unsigned seed(time(0));
		std::srand(seed);

		int selecionado = std::rand() % RCL.size(); // Pega um indice de RCL aleatoriamente
		s->sequence.insert(s->sequence.begin() + insercaoIndice, RCL[selecionado]);
		
	
		/* Remoçao do elemento de CL */
		
		std::vector < int >::iterator it;

		it = std::find(CL.begin(), CL.end(), RCL[selecionado]);

		if(it != CL.end()) {

			CL.erase(it);
		}
		
		r = RCL[selecionado]; // Atualiza o ultimo nó adicionado
		insercaoIndice++;


		
		
	}  

	
	return s;
}

void updateAllSubseq(Solution *s, std::vector<std::vector< Subsequence >> &subseq_matrix) {
	
	int n =  s->sequence.size() - 1; // Numero de nós da instancia, retira 1, pois a solução é 1 2 3 4 5 1, porém ela só tem 5 nós não 6.
	/*Atualiza as subsequencias de um unico nó
	 * O custo de atraso = 0 se for o primeiro nó, caso não é 1
	 * O custo = 0
	 * O tempo = 0	*/
	
	// Depois vamos juntar em um unico for
	/*
	 * Imagina uma matriz, e as subsequencias de um unico nó formam a diagonal principal
	 * Lembrando que, subseq_matrix[i][j] contém as informações da subsequencia que vai de i ate j*/
	for(int i = 0; i < n; i++) {
		
		int v = s->sequence[i] - 1; // Vamos retirar 1, porque para acessar a matriz que vai de 0 até 13, a sequencia vai de 1 ate 14, dado n = 14
		subseq_matrix[i][i].W = (i > 0);
		subseq_matrix[i][i].C = 0;
		subseq_matrix[i][i].T = 0;
		subseq_matrix[i][i].first = v; 
		subseq_matrix[i][i].last = v;
	
		
	}
	
	/*
	 * Se temos a solução s = 1, 2, 5, 7, 8, 9, 1
	 * i = 0 e j = 1 ( subsequencia que vai de 0 ate 1 )
	 * Temos a subsequencia que vai de 1 ate 2 em s, ela é formada pela concatenação da subsequencia de 0 ate 0  + 1 até 1
	 * i = 0 e j = 2 ( subsequencia que vai de 0 ate 2 )
	 * Temos a subsequencia que vai de 1 ate 5 em s, ela é formada pela concatenação da subsequencia de 0 até 1 + subsequencia de 2 até 2
	 * i = 0 e j =3 ( subsequencia que vai de 0 ate 3 )
	 * Temos a subsequencia que vai de 1 até 7 em s, ela é formada pela concatenação da subsequencia de 0 até 2 + subsequencia de 3 até 3	*/
	for(int i = 0; i < n; i++) {

		for(int j = i + 1; j < n; j++) {
			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j]);
		}
	}
	/* Subsequencias invertidas
	 * Dado n = 5; logo o maior índice é 4
	 * i = 4 e j = 3 (subsequencia que vai de 4 até 3)
	 * Ela é formada pela concatenação da subsequencia de 4 até 4 + a subsequencia de 3 até 3	
	 * i = 4 e j = 2 (subsequencia que vai de 4 até 2)
	 * Ela é formada pela concatenação da subsequencia de 4 até 3 + a subsequencia de 2 ate 2*/
	/*	Perceba que as concatenações sempre são feitas a partir de uma matriz criada anteriormente  + elemento da diagonal principal	*/
	for(int i = n - 1; i >= 0; i--) {
		
		for(int j = i - 1; j >= 0; j--) {

			subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j + 1], subseq_matrix[j][j]);
		}

	}
	
	std::cout << "Atualizou as subsequencias\n";
}

int main() {

	/*	Organiza a entrada de dados para pegar a dimensao	*/
	std::string dimensaoDados;

	std::getline(std::cin, dimensaoDados);
	
	int tamanhoString = dimensaoDados.size();
	while(1) {
		
		if(isdigit(dimensaoDados[0])) {
			break;
		}

		dimensaoDados.erase(dimensaoDados.begin());
	}	 

	int dimensao = std::stoi(dimensaoDados);
	
	/* Criação da matriz de custos------------------	*/
	int** matriz = new int*[dimensao];
	
	for(int i = 0; i < dimensao; i++) {

		matriz[i] = new int[dimensao];
	}


	for(int i = 0; i < dimensao; i++) {

		for(int j = 0; j < dimensao; j++) {

			std::cin >> matriz[i][j];
		}
	}
	t = matriz; // atualiza o vetor de custos que será utilizado na concatenação
 
	/*-------------------------------------------------*/

	Solution* s = Construction(dimensao, matriz);
	
	
	std::cout << "sequencia apos construcao" << std::endl;
	for(int i = 0; i < s->sequence.size(); i++) {

		std::cout << s->sequence[i] << " ";
	}
	std::cout << std::endl;

	/* Matriz com todas as subsequencia
	 * a subseq_matrix[i][j] armazena informações da subsequencia que inicia em i e termina em j relativo a solução s	*/
	
	
	std::vector < std::vector < Subsequence > > subseq_matrix(dimensao, std::vector < Subsequence >(dimensao));
	
	
	updateAllSubseq(s, subseq_matrix);
	for(int i = 0; i < dimensao; i++) {


			delete matriz[i];
	}
	return 0;
}
