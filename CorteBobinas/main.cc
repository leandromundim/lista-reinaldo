/********************************************************           
    * Olá -- Este programa resolve o exemplo do problema de  *
    *   corte do slide 17 do capitulo 3.                     *   
    * Authr:  Leandro, Mundim                                *   
    *                                                        *   
    * Uso:  Disciplina Pesquisa Operacional Aplicada à Gerên-*   
    *   cia da Produção. Professor Reinaldo Morabito.        *    
    *                                                        *   
    ********************************************************/ 

#include <ilcplex/ilocplex.h>
#include <stdlib.h>


ILOSTLBEGIN

#define RC_EPS 1.0e-6
#define INF 1.0e6


typedef IloArray <IloNumVarArray> IloNumVarArray2;
typedef IloArray <IloNumVarArray2> IloNumVarArray3;
typedef IloArray <IloNumVarArray3> IloNumVarArray4;
typedef IloArray <IloNumVarArray4> IloNumVarArray5;


/// MAIN PROGRAM ///

int
main(int argc, char **argv)
{
	double lixo;
	int  i, j;

	int      nP;//numero de padroes
	int       n;//numero de itens

	ifstream in(argv[1]);
	in >> nP;
	in >> n;
	int *demand;
   	demand = (int*)calloc(n, sizeof(int));

	for(j= 0; j < n; j++)
		in >> demand[j];
	in >> lixo;	in >> lixo;
	in >> n;

    int **padroes;
    padroes = (int**)calloc(nP, sizeof(int*)); 
    for(i = 0;i<nP;i++) {
        padroes[i] = (int*)calloc(n, sizeof(int));
    }

	for(i= 0; i < nP; i++){
		for(j= 0; j < n; j++)
			in >> padroes[i][j];
		in >> lixo;	in >> lixo; in >> lixo;
	}

	IloEnv env;
	IloCplex cplex(env);
	IloModel model(env);
	IloExpr expr(env);

	// Variable definitions
	IloNumVarArray x(env,nP,0,INF,ILOFLOAT);//padrao
	//IloNumVarArray y(env,nP,0,INF,ILOINT);//se vai usar um padrao

    // Objective function
	for (int i = 0 ; i < nP ; i++){
		expr +=x[i];
	}
	model.add(IloMinimize(env,expr));
	expr.clear();

    // Constraints
	for(j= 0; j < n; j++){
		for (i = 0 ; i < nP ; i++)
			expr += padroes[i][j]*x[i];
		model.add (expr >= demand[j]);
		expr.clear();
	}

	cplex.extract(model);
	cplex.exportModel("saida.lp");

	IloNum upperBound = IloInfinity;
	cplex.solve();
    std::cout << "Solution = " << cplex.getObjValue() << std::endl;
	for (int j = 0 ; j < nP ; j++){
		cout << " X[" << j+1 << "] =  " <<  cplex.getValue(x[j]) << endl;
	}

	return 0;
}
