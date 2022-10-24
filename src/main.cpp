#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>
#include <ctype.h> //Adiciona o isalnum
#include <limits>
#include <sstream>
#include <ios>
#include <fstream>

int **t;  






typedef struct Subsequence {

	double T, C; // Tempo e custo da subsequência
	int W; // Custo de atraso
	int first, last; // primeiro e último nós da subsequência

	inline static Subsequence Concatenate(Subsequence& sigma_1, Subsequence& sigma_2) {
	//	std::cout << "entrou" << std::endl;
		Subsequence sigma; // Subsequencia gerada pela concatenação
		/* Tempo do último nó da subsequência 1, até o primeiro nó da segunda	*/
	//	std::cout << "sigma1.last: " << sigma_1.last  << " sigma_2.first: " << sigma_2.first << std::endl;
		double temp = t[sigma_1.last][sigma_2.first];
	//	std::cout << "acessou" << std::endl;
		sigma.W = sigma_1.W + sigma_2.W; // Calculo do custo de atraso
		sigma.T = sigma_1.T + temp + sigma_2.T; // Calculo do tempo total
		sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;

		sigma.first = sigma_1.first;
		sigma.last = sigma_2.last;
	//		std::cout << "terminou a funcao" << std::endl;	
		return sigma;
	}
}Subsequence;


typedef struct {
	/* Toda solução possui uma sequencia inicial hamiltoniana, começa e inicia no primeiro nó. */
	std::vector < int > sequence = {1, 1};
	double cost;

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
	s->cost = subseq_matrix[0][n - 1].C;
	std::cout << "custo inicial: " << s->cost << std::endl;
}



double calculateSwapCost(Solution* s, int i, int j, std::vector < std::vector < Subsequence > >& subseq_matrix) {
	/* Dado uma solução 1 2 3 4 5 6 1
	 * Queremos dar swap em 2 e 3, que são os índices i = 1 e j = 2
	 * Logo, toda essa parte eh formada por uma concatenação da subsequencia que vai de 1->1 e 2->2,
	 * subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j - 1], subseq_matrix[j][j])
	 * Vamos supor que tenhamos a solução 1 2 3 4 5 6 7 8 9 e temos os indices i = 3 j = 6 para dar swap
	 * 1 2 3 7 5 6 4 8 9
	 * Temos sequencias que nao se alteram de 0 até i - 1, j + 1 até n - 1, e a central i + 1 ate j - 1
	 * 1 2 não se alteram
	 * 8 9 não se alteram
	 * 5 6 não se alteram
	 * Apenas de 3 -> 7 e 6 -> 4	*/

	int n =  s->sequence.size() - 1;

	/*
	 * Perceba que dividimos em uma subsequencia que vai do nó inicial até o nó anterior ao da troca concatenado com o nó da troca.
	 * A segunda subsequencia eh formada pela concatenação do posterior i da troca até o valor J da troca, com o valor trocado de i no lugar de J
	 * A terceira, é formada pelo sigma1 até o centro e por fim concatena tudo isso até o fim da subsequencia
	*/
	
	// Não vamos pegar a subsequencia invertida, de [j][i], pois se j = 2 e i = 5, pois representa a concatenação de todas as subsequencias de 2 até 5.
	/*  Vamos pegar a subsequencia de 0 até i - 1 e concatenar com a subsequencia de um unico nó, que foi aquela trocada	*/
	Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][j]);
	
	/* Mesma lógica que a anterior, concatena com a subsequencia de um unico nó	*/
	Subsequence sigmaCentral = Subsequence::Concatenate(subseq_matrix[i + 1][j - 1], subseq_matrix[i][i]);
	
	/* Concatena as duas anteriores	*/
	Subsequence sigma  = Subsequence::Concatenate(sigma1, sigmaCentral);
	
	/* Concatena toda a inicial com a final que se manteve constante	*/
	Subsequence sigma2 = Subsequence::Concatenate(sigma, subseq_matrix[j + 1][n - 1]);
	return sigma2.C; // retorna o custo
}
bool bestImprovementSwap(Solution* s, std::vector < std::vector < Subsequence > >& subseq_matrix) {
	double bestDelta = s->cost;
	int best_i, best_j;
	
	/* Pegamos a partir do primeiro indice e não pegamos o ultimo pois a sequencia é da seguinte forma 1 2 3 4 5 1	*/
	/* Ponto 1 do inicio e final sao fixos	*/
	/* 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1	*/
	/* Colocamos até j < s->sequencia.size() - 2, pois damos j + 1, ou seja se j = 13, se dermos +1 ocorre falha de memoria	*/
	for(int i = 1; i < s->sequence.size() - 1; i++) {
		
		for(int j = i + 1; j  < s->sequence.size() - 2; j++) {

			double delta = calculateSwapCost(s, i, j, subseq_matrix);
			if(delta < bestDelta) {
				std::cout << "achou troca melhor: " << delta;
				bestDelta = delta;
				best_i = i;
				best_j = j;
				getchar();
			}
		}
	}
	if(bestDelta != s->cost) {
		std::cout << "fez swap\n";	
		std::swap(s->sequence[best_i], s->sequence[best_j]);
		s->cost = bestDelta;
		updateAllSubseq(s, subseq_matrix);
		std::getchar();
		return true;
	}

	return false;
}

bool bestImprovement2opt(Solution* s, std::vector < std::vector < Subsequence >>& subseq_matrix) {
	
	int n = s->sequence.size() - 1;
	double bestDelta = s->cost;
	int best_i, best_j;

	for(int i = 1; i < s->sequence.size() - 1; i++) {

		for(int j =  i + 2; j < s->sequence.size() -2; j++) {
			
			/* O movimento 2opt, vamos pegar duas arestas não adjacentes e inserir o caminho entre elas de forma invertida	*/
			/* Dado o seguimento A B E D C F G H A	e os indices de troca 1 e 4	*/
			/* O movimento 2Opt, irá gerar a nova soluçao como A B (C D E) F G H A	*/
			/* Perceba que tudo de 0 até i, se manteve constante, como de j + 1 até n - 1, so mudou de i + 1 até J, que foi colocado de maneira invertida	*/

			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][i]); // subseq_matrix[j][i] já representa a subsequencia invertida de j até i
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][n - 1]);

			if(sigma2.C < bestDelta) {
				std::cout << "valor troca 2opt: " << sigma2.C << std::endl;
				bestDelta = sigma2.C;
				best_i = i;
				best_j = j;
				getchar();
				std::cout << "ocorreu melhor trocar 2opt" << std::endl;
			}
		}
	}

	if(bestDelta != s->cost) {
		
		int posicaoInicial = best_i + 1;
		int posicaoFinal = best_j;

		while(posicaoFinal > posicaoInicial) {
			
			std::swap(s->sequence[posicaoInicial], s->sequence[posicaoFinal]);
			posicaoFinal--;
			posicaoInicial++;
		}
		
		updateAllSubseq(s, subseq_matrix);
		return true;
	}

	return false;
}


bool bestImprovementOrOpt(Solution *s, std::vector < std::vector < Subsequence > >& subseq_matrix) {
	
	int n = s->sequence.size() - 1;
	double bestDelta = s->cost;
	int best_i, best_j;
	
	/* O movimento reinsertion é pegar um nó e movê-lo para outro ponto, como
	 * 1 2 3 (4) 5 6 7 8 9 10 1
	 * Vamos pegar o ponto 4 e transferir para o ponto 7
	 * Sendo assim i = 3 e j = 6 
	 * 1 2 3 5 6 7 (4) 8 9 10 1
	 * Perceba que tudo de 0 até i - 1 se manteve, assim como tudo de j + 1 até n - 1
	 * Vamos criar uma nova subsequencia com i - 1 até i + 1 e concatenar com i + 1 até j
	 * Assim como vamos criar uma de i até j + 1 e concatenar com o fim	*/
	
	for(int i = 1; i < s->sequence.size() - 1; i++) {

		for(int j = i + 1; j < s->sequence.size() - 2; j++) {
			
			/* Segue a mesma logica que o swap*/
			/* Utilizamos a concatenação com i + 1 até o valor de j, que se perceber foi o que manteve fixo	*/
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i + 1][j]);
			
			/* Concatena sigma1 com o valor da subsequencia de um único nó de i	*/
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i]);
			
			/* Concatena sigma 2 com a subsequencia final	*/
			Subsequence sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[j + 1][n - 1]);

			if(sigma3.C < bestDelta) {
				std::cout << "achou melhor delta por OrOpt: " << sigma3.C << std::endl;
				best_i = i;
				best_j = j;
				bestDelta = sigma3.C;
			}

		}

	}
	
	if(bestDelta != s->cost) {
		
		int posicaoInicial = best_i;
		int posicaoFinal = best_j;

		while(best_i < best_j) {

			std::swap(s->sequence[best_i], s->sequence[best_i + 1]);
			best_i++;
		}

		updateAllSubseq(s, subseq_matrix);

		return true;
	}





	return false;
}


bool bestImprovementOrOpt2(Solution* s, std::vector < std::vector < Subsequence > > &subseq_matrix) {

	int n = s->sequence.size() - 1;
	double bestDelta = s->cost;
	
	int best_i, best_j;

	/* O movimento OrOpt2, é o mesmo que reinsertion porém para dois nós, exemplo:
	 * Exemplo, vamos pegar i = 1 e j = 6 da sequencia
	 * 1 (2 3) 4 5 6 7 8 9 1
	 * Vamos pegar os nós i e i + 1, e transportar i para a posição j
	 * 1 4 5 6 7 8 (2 3) 9 1
	 * Logo, tudo de 0 até i - 1 se mantém fixo	*/
	
	// 1 2 3 4 5 6 7 8 9 1
	// i = 1 e j = 7
	// 1 4 5 6 7 8 9 2 3 1
	/*Analisar novamente*/
	for(int i = 1; i < s->sequence.size() - 1; i++){

		for(int j = i + 2; j < s->sequence.size() - 3; j++) {
			
			// Subsequencia nova, após a saída dos valores seguidos, no exemplo anterior seriam 1 4 5 6 7 8
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i + 2][j + 1]);
			
			// Subsequencia formada pelos nós que serão deslocados pelo OrOpt no caso anterior 2 e 3
			Subsequence sigma2 = Subsequence::Concatenate(subseq_matrix[i][i], subseq_matrix[i + 1][i + 1]);

			Subsequence sigma3 = Subsequence::Concatenate(sigma1, sigma2); // Concatena sigma1 e sigma2
			
			Subsequence sigma4 = Subsequence::Concatenate(sigma3, subseq_matrix[j + 2][n - 1]);

			if(sigma4.C < bestDelta) {
				std::cout << "achou melhor delta por OrOpt2: " << sigma4.C << std::endl;
				bestDelta = sigma4.C;
				best_i = i;
				best_j = j;
			}

		}
	}

	if(bestDelta != s->cost) {
		//Devemos levar o primeiro valor de i até o valor de j
		
		std::vector < int> valoresAdicionar = {s->sequence[best_i], s->sequence[best_i + 1]};

		for(int i = 0; i < s->sequence.size(); i++) {
			s->sequence.erase(s->sequence.begin() + best_i);
		}

		for(int i = 0; i < valoresAdicionar.size(); i++) {
			s->sequence.insert(s->sequence.begin() + best_j + i, valoresAdicionar[i]);
		}
		
		updateAllSubseq(s, subseq_matrix);
		std::cout << "fez oorOpt2" << std::endl;
		getchar();
	}
	
	

	return false;

}


void buscaLocal(Solution* s, std::vector < std::vector < Subsequence > > &subseq_matrix) {
	std::vector < int > NL = {1, 2, 3, 4, 5};
	bool improved = false;


	while(!NL.empty()) {
		int n = std::rand() % NL.size();

		switch(NL[n]) {
			
			case 1:
				improved = bestImprovementSwap(s, subseq_matrix);
				break;
			case 2:
				improved = bestImprovement2opt(s, subseq_matrix);
				break;
					
			case 3:
				improved = bestImprovementOrOpt(s, subseq_matrix);
				break;
				
			case 4:
				improved = bestImprovementOrOpt2(s, subseq_matrix);
				break;
			
		}

		if(improved) {
			NL = {1, 2, 3, 4, 5};
		}
		else {
			NL.erase(NL.begin() + n);
		}
	}
}


int main(int argc, char** argv) {
	

	/*	Organiza a entrada de dados para pegar a dimensao	*/
	std::string dimensaoDados;
	std::fstream arquivo;
	
	arquivo.open(argv[1], std::ios::in);
	
	std::getline(arquivo, dimensaoDados);
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
	
	/* Vector que irá conter todos os valores de distancia	*/
	std::vector < int > valoresLinha;

	/* Leitura do arquivo e separa os valores para armazenar no vector anterior	*/
	while(1) {

		if(arquivo.eof()) {
			break;
		}
		std::string linha;

		std::getline(arquivo, linha);

		std::stringstream linhaDados(linha);

		std::string palavra;

		while(std::getline(linhaDados, palavra, ' ')) {

			valoresLinha.push_back(std::stoi(palavra));
		}

	}

	int indice = 0;
	for(int i = 0; i < dimensao; i++) {

		for(int j = 0; j < dimensao; j++) {

			matriz[i][j] = valoresLinha[indice];
			indice++;
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
	
	buscaLocal(s, subseq_matrix);
	getchar();	
	for(int i = 0; i < s->sequence.size(); i++) {
		std::cout << s->sequence[i] << " ";
	}
	std::cout << std::endl;
	for(int i = 0; i < dimensao; i++) {


			delete matriz[i];
	}
	return 0;
}
