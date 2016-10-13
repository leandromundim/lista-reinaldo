/********************************************************           
    * Olá -- Este programa resolve o Exemplo 3-4 (pag. 116)  *   
    *   do livro "Operations research in production planning *   
    *   scheduling and inventory control. Lynwood A. Johnson,* 
    *   Douglas C. Montgomery."                              *
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

int main(int argc, char **argv){

	IloEnv env;
	IloCplex cplex(env);
	IloModel model(env);
	IloExpr expr(env);

	// Variable definitions
	IloNumVarArray2 X(env, 4);
	for (int i = 0 ; i < 4 ; i++){
		X[i] = IloNumVarArray (env, 4,0,INF,ILOINT);
	}

    // Objective function
	expr = 6*X[0][0]+6.2*X[0][1]+7.2*X[0][2]+7.4*X[0][3]+15*X[1][0]+15.4*X[1][1]
			+18*X[1][2]+18.4*X[1][3]+11*X[2][0]+11.2*X[2][1]+13.2*X[2][2]+13.4*X[2][3]
			+14*X[3][0]+14.3*X[3][1]+16.8*X[3][2]+17.1*X[3][3];

	model.add(IloMinimize(env,expr));
	expr.clear();

    // Constraints
    //(1) Constraints on regular time capacity
	for (int i = 0 ; i < 2 ; i++){
		expr += 0.03*X[0][i] + 0.15*X[1][i] + 0.05*X[2][i] + 0.1*X[3][i];
	}
	model.add (expr <= 400);
	expr.clear();

	for (int i = 0 ; i < 2 ; i++){
		expr += 0.06*X[0][i] + 0.12*X[1][i] + 0.1*X[3][i];
	}
	model.add (expr <= 400);
	expr.clear();

	for (int i = 0 ; i < 4 ; i++){
		expr += 0.05*X[0][i] + 0.1*X[1][i] + 0.05*X[2][i] + 0.12*X[3][i];
	}
	model.add (expr <= 500);
	expr.clear();

	expr = 0.04*(X[0][0]+X[0][2])+0.2*(X[1][0]+X[1][2]) + 0.03*(X[2][0]+X[2][2])+0.12*(X[3][0]+X[3][2]); 
	model.add (expr <= 450);
	expr.clear();


	for (int i = 0 ; i < 4 ; i++){
		expr += 0.02*X[0][i] + 0.06*X[1][i] + 0.02*X[2][i] + 0.05*X[3][i];
	}
	model.add (expr <= 400);
	expr.clear();

	//(2)
	expr = 0.04*(X[0][1]+X[0][3])+0.2*(X[1][1]+X[1][3]) + 0.03*(X[2][1]+X[2][3])+0.12*(X[3][1]+X[3][3]); 
	model.add (expr <= 100);
	expr.clear();

	//(3)
	expr = 2*(X[1][0]+X[1][1])+1.2*(X[3][0]+X[3][1]); 
	model.add (expr <= 2000);
	expr.clear();

	//(4)
	model.add (X[0][0] + X[0][1] + X[0][2] + X[0][3]  == 3000);
	model.add (X[1][0] + X[1][1] + X[1][2] + X[1][3]  == 500);
	model.add (X[2][0] + X[2][1] + X[2][2] + X[2][3]  == 1000);
	model.add (X[3][0] + X[3][1] + X[3][2] + X[3][3]  == 2000);

	cplex.extract(model);
	cplex.exportModel("saida.lp");

	IloNum upperBound = IloInfinity;
	cplex.solve();
    std::cout << "Solution = " << cplex.getObjValue() << std::endl;


	for (int i = 0 ; i < 4 ; i++){
		for (int j = 0 ; j < 4 ; j++){
			cout << " X[" << i+1 << "][" << j+1 << "] =  " <<  cplex.getValue(X[i][j]);
		}
		cout << endl;
	} 

	return 0;
}