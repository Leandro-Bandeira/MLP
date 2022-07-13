#ifndef MLP_H
#define MLP_H

#include <vector>


typedef struct {
	
	/*
	 *T-> Duração de uma subsequência
	 *C-> Custo acumulado de uma subsequência
	 *W-> Custo de atraso de uma subsequência
	 */
	double T, C;

	int W;

	int first, last; // Primeiro e ultimos nós da subsequencia

	inline static void Concatenate(Subsequence &sigma , 
			Subsequence &sigma_1, 
			Subsequence &sigma_2) 
	{
		// Temo de custo do último valor do primeiro até o primeiro
		// do segundo
		double temp = t[sigma_1.last][sigma_2.first];

		sigma.W = sigma_1.W + sigma_2.W;

		sigma.T = sigma_1.T + temp + sigma_2.T;
		sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
		sigma.first = sigma_1.first;
		sigma.last = sigma_2.last;

		


	}
}Subsequence;


typedef struct {
	std::vector < int < sequencia = {1 , 1};
	
	double valorObj = 0;

}Solucao;




class Mlp {
	
	public:
		Mlp();

	private:
};

















#endif
