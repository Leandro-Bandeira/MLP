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
#include <iomanip>

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



void printaSolucao(int best_i, int best_j, Solution *s, std::string nomeMovimento) {

		std::cout << "Cost: " << s->cost << std::endl;
		
		std::cout << "best_i: " << best_i << " e best_j: " << best_j << '\n';
		std::string saidaString = "Solution before " + nomeMovimento;
		std::cout << saidaString;
		for(int i = 0; i < s->sequence.size(); i++) {
			std::cout << s->sequence[i] << " ";
		}
		std::cout << '\n';

}

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
	
	int n =  s->sequence.size(); // Numero de nós da instancia, retira 1, pois a solução é 1 2 3 4 5 1, porém ela só tem 5 nós não 6.
	/*Atualiza as subsequencias de um unico nó
	 * O custo de atraso = 0 se for o primeiro nó, caso não é 1
	 * O custo = 0
	 * O tempo = 0	*/
	
	// Depois vamos juntar em um unico for
	/*
	 * Imagina uma matriz, e as subsequencias de um unico nó formam a diagonal principal
	 * Lembrando que, subseq_matrix[i][j] contém as informações da subsequencia que vai de i ate j*/
	// Se a minha instancia tem dimensão 14, temos 15 pontos contando com 1 no final, logo a matriz é 15 x 15, seus indices vão de 0 a 14.
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
		
	std::cout << std::setfill ('-') << std::setw(169) << '\n';
	std::cout << "Atualizou as subsequencias\n";
	s->cost = subseq_matrix[0][n - 1].C;
	std::cout << "Custo inicial: " << s->cost << std::endl;
	std::cout << std::setfill ('-') << std::setw(169) << '\n';
			
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
	
	/* 1 2 3 4 5 6 7 8 9 10 1, i = 3 e j = 7
	 * 1 2 3 (8) 5 6 7 (4) 9 10 1 */
	int n =  s->sequence.size();

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
	Subsequence sigma2  = Subsequence::Concatenate(sigma1, sigmaCentral);
	
	/* Concatena toda a inicial com a final que se manteve constante	*/
	/* Verificamos se a posição de troca é o ultimo vértice, para não enviar j + 1	*/
	Subsequence sigma3;
	sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[j + 1][n - 1]);
	
	return sigma3.C; // retorna o custo
}
bool bestImprovementSwap(Solution* s, std::vector < std::vector < Subsequence > >& subseq_matrix) {
	double bestDelta = s->cost;
	int best_i, best_j;

	/* Se temos, 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1, s->sequence.size() retorna 15, logo no for irá iterar de 0 a 14, quebrando, logo devemos iterar de 
	 * 0 a 13, por isso temos n = s->sequence.size() - 1 = 14, pois nossas matrizes e vertices são do tipo dimensao - 1 x dimensao -1	*/

	int n = s->sequence.size() - 1; // Como o tamanho da sequencia é sempre mais 1, por conta do 1 no final damos -1 para fazer o for com o tamanho real
	// -> devemos lembrar que s->sequence sempre vale mais 1, por conta do 1 final	
	/* Pegamos a partir do primeiro indice e não pegamos o ultimo pois a sequencia é da seguinte forma 1 2 3 4 5 1	*/
	/* Ponto 1 do inicio e final sao fixos	*/
	/* 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1	*/
	/* Colocamos até j < s->sequencia.size() - 2, pois damos j + 1, ou seja se j = 13, se dermos +1 ocorre falha de memoria	*/
	/* A troca com o ultimo nó é diferente, 1 (14) 3 4 5 6 7 8 9 10 11 12 13 (2) 1	*/
	for(int i = 1; i < n - 1; i++) {
			
		for(int j = i + 1; j  < n; j++) {

			double delta = calculateSwapCost(s, i, j, subseq_matrix);
			if(delta < bestDelta) {
				bestDelta = delta;
				best_i = i;
				best_j = j;
			}
		}
	}
	if(bestDelta != s->cost) {
		
		s->cost = bestDelta;
		printaSolucao(best_i, best_j, s, "Swap: ");
		std::swap(s->sequence[best_i], s->sequence[best_j]);
		updateAllSubseq(s, subseq_matrix);

		std::cout << "Solution after swap: ";

		for(int i = 0; i < s->sequence.size(); i++) {
			
			std::cout << s->sequence[i] << " ";
		}
		std::cout << '\n';
			
		std::cout << std::setfill ('-') << std::setw(169) << '\n';
		return true;
	}

	return false;
}

bool bestImprovement2opt(Solution* s, std::vector < std::vector < Subsequence >>& subseq_matrix) {
	
	int n = s->sequence.size();
	double bestDelta = s->cost;
	int best_i, best_j;

	for(int i = 1; i < s->sequence.size() - 1; i++) {
		
		for(int j =  i + 2; j < s->sequence.size() - 1; j++) {
			
			/* O movimento 2opt, vamos pegar duas arestas não adjacentes e inserir o caminho entre elas de forma invertida	*/
			/* Dado o seguimento A B E D C F G H A	e os indices de troca 1 e 4	*/
			/* O movimento 2Opt, irá gerar a nova soluçao como A B (C D E) F G H A	*/
			/* Perceba que tudo de 0 até i, se manteve constante, como de j + 1 até n - 1, so mudou de i + 1 até J, que foi colocado de maneira invertida	*/

			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[j][i]); // subseq_matrix[j][i] já representa a subsequencia invertida de j até i
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j + 1][n - 1]);

			if(sigma2.C < bestDelta) {
				bestDelta = sigma2.C;
				best_i = i;
				best_j = j;
			}
		}
	}

	if(bestDelta != s->cost) {
		
		s->cost = bestDelta; // Atualiza o custo
		
		printaSolucao(best_i, best_j, s, "2 Opt: ");	
		int posicaoInicial = best_i;
		int posicaoFinal = best_j;

		while(posicaoFinal > posicaoInicial) {
			
			std::swap(s->sequence[posicaoInicial], s->sequence[posicaoFinal]);
			posicaoFinal--;
			posicaoInicial++;
		}
		
		updateAllSubseq(s, subseq_matrix);

		std::cout << "Solution after 2-Opt: ";

		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << '\n';

		std::cout << std::setfill ('-') << std::setw(169) << '\n';
		return true;
	}

	return false;
}


bool bestImprovementOrOpt(Solution *s, std::vector < std::vector < Subsequence > >& subseq_matrix) {
	
	int n = s->sequence.size();
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
	
	/* i = 3 e j = 10
	 * 1 2 3 5 6 7 8 9 10 (4) 1	*/
	for(int i = 1; i < n - 2; i++) {

		for(int j = i + 1; j < n - 1; j++) {
			
			/* Segue a mesma logica que o swap*/
			/* Utilizamos a concatenação com i + 1 até o valor de j, que se perceber foi o que manteve fixo	*/
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i + 1][j]);
			
			/* Concatena sigma1 com o valor da subsequencia de um único nó de i	*/
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i]);
			
			Subsequence sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[j + 1][n - 1]);
			

			if(sigma3.C < bestDelta) {
				best_i = i;
				best_j = j;
				bestDelta = sigma3.C;
			}

		}

	}
	
	bool caso2 = false;
	/* Vamos executar o movimento Or-opt de trás pra frente
	 * 1 2 3 4 5 6 7 8 9 10 1
	 * i = 13 e j = 1
	 * 1 10 2 3 4 5 6 7 8 9 1	
	 * i = 13 e j = 2
	 * 1 2 10 3 4 5 6 7 8 9 1
	 * 1 2 3 4 5 6 7 8 9 10 (11) 12 13 14 1
	 * i = 10 e j = 6
	 * 1 2 3 4 5 6 11 7 8 9 10 12 13 14 1	*/
	for(int i =  n - 2; i > 1; i-- ){

		for(int j = 1; j < i; j++) {
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][j - 1], subseq_matrix[i][i]);
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][i - 1]);
			Subsequence sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[i + 1][n - 1]);

			if(sigma3.C < bestDelta) {
				
				bestDelta = sigma3.C;
				best_i = i;
				best_j = j;
				caso2 = true;

			}

		}
	}
	
	if(bestDelta != s->cost and !caso2) {
		
		s->cost = bestDelta; // Atualiza o custo

		printaSolucao(best_i, best_j, s, "Or-Opt: ");
		int posicaoInicial = best_i;
		int posicaoFinal = best_j;

		while(best_i < best_j) {

			std::swap(s->sequence[best_i], s->sequence[best_i + 1]);
			best_i++;
		}
		
		std::cout << "Solution after Or-Opt: ";
		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		
		std::cout << '\n';
		updateAllSubseq(s, subseq_matrix);
		
		std::cout << std::setfill ('-') << std::setw(169) << '\n';
			
		return true;
	}
	
	if(caso2 == true){

		std::cout << "entrou no caso 2" << std::endl;

		s->cost = bestDelta;
		printaSolucao(best_i, best_j, s, "Or-Opt: ");

		int valorReferencia = s->sequence[best_j];
		int valorAdicionar = s->sequence[best_i];

		s->sequence.erase(s->sequence.begin() + best_i);

		for(int i = 0; i < s->sequence.size(); i++) {

			if(s->sequence[i] == valorReferencia) {
				s->sequence.insert(s->sequence.begin() + i, valorAdicionar);
				break;
			}
		}


		updateAllSubseq(s, subseq_matrix);
		
		getchar();
		std::cout << "Solution after OrOpt: ";

		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
		getchar();
		return true;
	}




	return false;
}


bool bestImprovementOrOpt2(Solution* s, std::vector < std::vector < Subsequence > > &subseq_matrix) {

	int n = s->sequence.size();
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
	/* Instancia de dimensão 14: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1
	 * a matriz de custos vai de 0 a 13 e a matriz de subsequencia de 0 a 14 para pegar todos os nós, para a logica não quebrar
	 * devemos pegar até o nó 12, de índice 11, logo s->sequence.size() - 3 = 15 - 3 = 12, vamos de 0 a 11
	 * Se i = 11, j = 13.
	 * Então temos 1 2 3 4 5 6 7 8 9 10 11 (12 13) 14 1
	 * 1 2 3 4 5 6 7 8 9 10 11 14 (12 13) 1
	 * Conclui-se que j deve ir até s->sequence.size() - 1, 15 - 1 = 14, de 0 a 13
	 *
	 * Testar para i = 10, j = 12
	 * 1 2 3 4 5 6 7 8 9 10 (11 12) 13 14 1
	 * 1 2 3 4 (5 6) 7 8 9 (10) 13 14 (11 12) 1	
	 * A Lógica anterior funciona até j = 12, mas j  = 13 deve ser mudado.
	 * Pegando o exemplo anterior, 1 2 3 4 (5 6) 7 8  9 (10) 11 12 13 14 1
	 * i = 4 j = 9
	 *  1 2 3 4 7 8 9 10 (5 6) 11 12 13 14 1
	 *  i = 4 e j = 13
	 *  1 2 3 4 7 8 9 10 11 12 13 14 (5 6) 1*/
	for(int i = 1; i < s->sequence.size() - 3; i++){

		for(int j = i + 2; j < s->sequence.size() - 1; j++) {
			
			// Subsequencia nova, após a saída dos valores seguidos, no exemplo anterior seriam 1 4 5 6 7 8
			
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i + 2][j]);
			

			// Subsequencia formada pelos nós que serão deslocados pelo OrOpt no caso anterior 2 e 3
			Subsequence sigma2 = Subsequence::Concatenate(subseq_matrix[i][i], subseq_matrix[i + 1][i + 1]);
		
			Subsequence sigma3 = Subsequence::Concatenate(sigma1, sigma2); // Concatena sigma1 e sigma2
			Subsequence sigma4;
			//Concatena com o ultimo nó que é o que falta
			sigma4 = Subsequence::Concatenate(sigma3, subseq_matrix[j + 1][n - 1]);


			if(sigma4.C < bestDelta) {
				bestDelta = sigma4.C;
				best_i = i;
				best_j = j;
			}

		}
	}
	
	bool caso2 = false;

	/* 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1
	 * i = 13,e j = 4
	 * logo, temos 
	 * 1 2 3 4 (5) 6 7 8 9 10 11 12 (13 14) 1
	 * 1 2 3 4 (13 14) (5) 6 7 8 9 10 11 12 1,
	 * então temos que 0 até j - 1 se mantém, concatena com o conjunto e a posição de j, finaliza com j + 1 até  i - 2 e finaliza com n -1	
	 * Começa com i = s->sequence.size() - 3, pois pegamos a posição de i mais o valor do seu lado direito
	 * i = 12 e j = 11
	 * 1 2 3 4 5 6 7 8 9 10 11 12 (13 14) 1
	 * 1 2 3 4 5 6 7 8 9 10 11 (13 14) 12 1
	 */
	for(int i = s->sequence.size() - 3; i > 1; i--) {
		
		for(int j = 1; j < i; j++) {
			
			Subsequence sigmaCentral = Subsequence::Concatenate(subseq_matrix[i][i], subseq_matrix[i + 1][i + 1]);

			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][j - 1], sigmaCentral);

			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[j][i - 1]);

			Subsequence sigma3 = Subsequence:: Concatenate(sigma2, subseq_matrix[i + 2][n - 1]);

			if(sigma3.C < bestDelta) {

				bestDelta = sigma3.C;
				best_i = i;
				best_j = j;
				caso2 = true;
			}
		}

	}


	if(bestDelta != s->cost and !caso2) {
		//Devemos levar o primeiro valor de i até o valor de j
		
		s->cost = bestDelta;
		printaSolucao(best_i, best_j, s, "Or-Opt2: ");
		std::vector < int> valoresAdicionar = {s->sequence[best_i], s->sequence[best_i + 1]};

		int valorReferencia = s->sequence[best_j];
		for(int i = 0; i < valoresAdicionar.size(); i++) {
			s->sequence.erase(s->sequence.begin() + best_i);
		}
		

		for(int i = 0; i < s->sequence.size(); i++) {

			if(s->sequence[i] == valorReferencia) { 
				
				for(int j = 0; j < valoresAdicionar.size(); j++) {

					s->sequence.insert(s->sequence.begin() + i + j + 1, valoresAdicionar[j]);
				}
				break;
			}
		}
		
		std::cout << "Solution after Or-Opt2: ";
		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
		updateAllSubseq(s, subseq_matrix);

	
		std::cout << std::setfill ('-') << std::setw(169) << '\n';
			
		return true;
	}
	
	if(caso2 ) {
		std::cout << "caso 2 OrOpt2" << std::endl;
		s->cost = bestDelta;
		printaSolucao(best_i, best_j, s, "Or-Opt2: ");
		std::vector < int> valoresAdicionar = {s->sequence[best_i], s->sequence[best_i + 1]};

		int valorReferencia = s->sequence[best_j];
		for(int i = 0; i < valoresAdicionar.size(); i++) {
			s->sequence.erase(s->sequence.begin() + best_i);
		}
		

		for(int i = 0; i < s->sequence.size(); i++) {

			if(s->sequence[i] == valorReferencia) { 
				
				for(int j = 0; j < valoresAdicionar.size(); j++) {

					s->sequence.insert(s->sequence.begin() + i + j, valoresAdicionar[j]);
				}
				break;
			}
		}
		
		std::cout << "Solution after Or-Opt2: ";
		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
		getchar();
		updateAllSubseq(s, subseq_matrix);

	
		std::cout << std::setfill ('-') << std::setw(169) << '\n';
		getchar();	
		return true;
	}

	return false;

}


bool bestImprovementOrOpt3(Solution* s, std::vector < std::vector < Subsequence > > &subseq_matrix) {
	int n = s->sequence.size();
	
	double bestDelta = s->cost;
	int best_i, best_j;
	
	/* Dado a sequencia 1 2 3 4 5 6 7 8 9 10 11 12 13 14 1	*/
	/* Vamos pegar três vértices consecutivos e mudar a posição	*/
	/* i = 1 e j = 8
	 * 1 (2 3 4) 5 6 7 8 9 10 11 12 13 14 1
	 * 1 5 6 7 8 9 10 11 (2 3 4) 12 13 14 1 
	 * Logo, tudo de 0 até i - 1 se manteve constante, e de i + 3 até j + 2 também, pega o que se manteve concatenato e termina com o fim
	 * Que é j + 3 até n - 1.	*/
	
	/* Analisando para último caso	
	 * Dado a sequencia anterior, j = 13 e i = 1
	 * 1 5 6 7 8 9 10 11 12 13 14 (2 3 4) 1
	 * i = 10 logo, j = 13
	 * 1 2 3 4 5 6 7 8 9 10 (11 12 13) 14 1
	 * 1 2 3 4 5 6 7 8 9 10 14 (11 12 13) 1
	 *
	 * i = 9, logo j = 12
	 * 1 2 3 4 5 6 7 8 9 (10 11 12) 13 14 1
	 * 1 2 3 4 5 6 7 8 9 13 (10 11 12)14 1
	 *
	 * i = 1 e j = 8
	 * 1 (2 3 4) 5 6 7 8 9 10 11 12 13 14 1
	 * 1 5 6 7 8 9 (2 3 4) 10 11 12 13 14 1
	 * Vamos posicionar anterior ao número posicionado em j
	 * então 0 até i - 1 se mantém fixo, i + 2 até j - 1
	 * No ultimo nó, temos i = 1 e j = 13
	 * 1 5 6 7 8 9 10 11 12 13 14 (2 3 4) 1
	 * O ultimo caso, é i = 10 e j = 13
	 * 1 2 3 4 5 6 7 8 9 10 (11 12 13) 14 1
	 * 1 2 3 4 5 6 7 8 9 10 14 (11 12 13) 1*/
	
	//Or opt, fo modificada, verificar depois se ta certo
	for(int i = 1; i < s->sequence.size() - 4; i++) {

		for(int j = i + 3; j < s->sequence.size() - 1; j++) {
			
			Subsequence sigma1 = Subsequence::Concatenate(subseq_matrix[0][i - 1], subseq_matrix[i + 3][j]);
			Subsequence sigma2 = Subsequence::Concatenate(sigma1, subseq_matrix[i][i + 2]); // subseq_matrix[i][i + 2] é a sequencia retirada
			
			Subsequence sigma3 = Subsequence::Concatenate(sigma2, subseq_matrix[j + 1][n - 1]);

			if(sigma3.C < bestDelta) {
				best_i = i;
				best_j = j;

				bestDelta = sigma3.C;

			}
		}
	}


	if(bestDelta != s->cost) {
		
		s->cost = bestDelta;

		printaSolucao(best_i, best_j, s, "Or-Opt3: ");
		std::vector < int > valoresAdicionar = {s->sequence[best_i], s->sequence[best_i + 1], s->sequence[best_i + 2]};
		int valorReferencia = s->sequence[best_j];
		for(int i = 0; i < valoresAdicionar.size(); i++) {

			s->sequence.erase(s->sequence.begin() + best_i);
		}
		

		for(int i = 0; i < s->sequence.size(); i++) {
	
			if(s->sequence[i] == valorReferencia) {
				
				for(int j = 0; j < valoresAdicionar.size(); j++) {
					s->sequence.insert(s->sequence.begin() + i + j + 1, valoresAdicionar[j]);
				}
				break;
			}
		}

		updateAllSubseq(s, subseq_matrix);

		std::cout << "Solution after Or-opt3: ";

		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << '\n';
		
		std::cout << std::setfill ('-') << std::setw(169) << '\n';
	
		return true;
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
			
			case 5:
				improved = bestImprovementOrOpt3(s, subseq_matrix);
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


void perturbacao(Solution *s) {
	// Gera numero aleatorio entre 2 e a dimensão do grafo 	
	std::random_device rd;
	
	int limite;
	if(s->sequence.size() > 20) {
	
		limite = s->sequence.size() / 10;
	}
	else {
		limite = 2;
	}

	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(2, limite);
	
	/* Pega inicialmente os tamanhos aleatórios e a posicaoInicial do primeiro */	
	int tamanho1 =  dist(gen);
	int tamanho2 = dist(gen);
		
	std::uniform_int_distribution <> dist1(1, s->sequence.size() - 2 - tamanho1); // Podemos pegar qualquer posição
	std::uniform_int_distribution <> dist2(1, s->sequence.size() - 2 - tamanho2); // Garantimos que vamos pegar uma posição inicial que não estoure o programa

	int posicaoInicial1 = dist1(gen);

	/* Devemos então, garantir que a posicao inicial do 2, comece após a sequencia do primeiro	*/
	int posicaoInicial2;
	
	while(1){
	
		posicaoInicial2 = dist2(gen);

		if(posicaoInicial2 != posicaoInicial1 && (posicaoInicial2 > posicaoInicial1 + tamanho1 || posicaoInicial2 < posicaoInicial1 - tamanho2)) {
			break;
		}
	}
	
	
	std::vector < int > sequencia1;
	std::vector < int > sequencia2;
	int valor1, valor2;
	
	valor1 = s->sequence[posicaoInicial1 - 1];
	valor2 = s->sequence[posicaoInicial2 - 1];
	int contador1 = tamanho1;
	while(contador1--)
	{

		sequencia1.push_back(s->sequence[posicaoInicial1]);
		s->sequence.erase(s->sequence.begin() + posicaoInicial1);
	}
	
	int k = 0;
	int contador = tamanho2;
	for(int i = 0; i < s->sequence.size() - 1; i++) 
	{
		if(valor2 == s->sequence[i]) {
			k = i + 1;
			while(contador--) {
				sequencia2.push_back(s->sequence[k]);
				s->sequence.erase(s->sequence.begin() + k);
			}

			break;
		} 
	}


	int j = 0;
	for(int i = 0; i < s->sequence.size() - 1; i++) {

		if(valor1 == s->sequence[i]) {
			k = i + 1;
			while(tamanho2--) {

				s->sequence.insert(s->sequence.begin() + k, sequencia2[j]);
				k++;
				j++;
			}
			j = 0;
		}

		if(valor2 == s->sequence[i]) {
			k = i + 1;

			while(tamanho1--) {
				s->sequence.insert(s->sequence.begin() + k, sequencia1[j]);
				k++;
				j++;
			}
			j = 0;
		}
	}

	
	std::cout << "Solution after Perturbation: ";
	for(int i = 0; i < s->sequence.size(); i++) {

		std::cout << s->sequence[i] << " ";
	}
	std::cout << std::endl;
	
}

int main(int argc, char** argv) {
	
	int custoEntrada = std::atoi(argv[2]);
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
	double sum = 0;
	//for(int i = 0; i < 10; i++) {

	
		/*-------------------------------------------------*/
		Solution* s = Construction(dimensao, matriz);
	
		std::cout << std::setfill('-') << std::setw(169) << '\n';
	
		std::cout << "sequencia apos construcao" << std::endl;
		for(int i = 0; i < s->sequence.size(); i++) {

			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
		std::cout << std::setfill('-') << std::setw(169) << '\n';

		/* Matriz com todas as subsequencia
		* a subseq_matrix[i][j] armazena informações da subsequencia que inicia em i e termina em j relativo a solução s	*/
	
		// Somamos mais 1 para incluir o 1 final
		// Dado uma instancia de dimensão 14, temos 14 pontos, mas em teoria temos 15 pontos por conta do ciclo hamiltoniano 1 --- 1
		std::vector < std::vector < Subsequence > > subseq_matrix(dimensao + 1, std::vector < Subsequence >(dimensao + 1));	
		updateAllSubseq(s, subseq_matrix);
	
		buscaLocal(s, subseq_matrix);
		for(int i = 0; i < s->sequence.size(); i++) {
			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
	
	
		perturbacao(s);
		updateAllSubseq(s, subseq_matrix);
		buscaLocal(s, subseq_matrix);
		for(int i = 0; i < s->sequence.size(); i++) {
			std::cout << s->sequence[i] << " ";
		}
		std::cout << std::endl;
		int n = s->sequence.size();
		s->cost = subseq_matrix[0][n - 1].C;
	//	std::cout << "Iteracao " << i << '\n';
		std::cout << "Cost: " << s->cost << std::endl;
		std::cout << "gap: " << (s->cost - custoEntrada) / custoEntrada << std::endl;
		sum += (s->cost - custoEntrada) / custoEntrada;

		int repetidos = 0;
		for(int i = 1; i < s->sequence.size() - 1; i++) {

			int valor = s->sequence[i];

			for(int  j = i + 1; j < s->sequence.size(); j++) {

				if(valor == s->sequence[j]) {
					repetidos++;
				}
			}
		}
		std::cout << "repetidos: " << repetidos << std::endl;
		std::cout << "tamanho instancia: " << s->sequence.size() - 1 << std::endl;
	
	//}

	//std::cout << "gap: " << sum / 10 << std::endl;
	for(int i = 0; i < dimensao; i++) {
		delete matriz[i];
	}
	return 0;
}
