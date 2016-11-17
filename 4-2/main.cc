/********************************************************           
    * Olá -- Este programa resolve o exemplo da página 195,  *   
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
typedef IloArray <IloNumVarArray2> IloNumVarArray3;
typedef IloArray <IloNumVarArray3> IloNumVarArray4;
typedef IloArray <IloNumVarArray4> IloNumVarArray5;

typedef struct{
	int ID;
	int p;   			//preco de venda
	int c;  			//custo variavel
	int l; 				//numero minimo
	int u;				//numero maximo
} _prod;


/// MAIN PROGRAM ///

int
main(int argc, char **argv)
{

	int      I =  3;
	int      J = 13;
	int      K = 13;

	ifstream in("dados.dat");

  int ***p;
  p = (int***)calloc(I, sizeof(int**)); 
	for (int i = 0 ; i < I; i++){
		p[i] = (int**)calloc(J, sizeof(int*)); 
		for (int j = 0 ; j < J; j++){
	 		p[i][j] = (int*)calloc(K, sizeof(int));
		}
	}


	for (int k = 0 ; k < K; k++){
		for (int i = 0 ; i < I; i++){
			for (int j = 0 ; j < J; j++){
		  	in >> p[i][j][k];
				//cout << " " << p[i][j][k];
			}
		}
	}

  int *d;
	d = (int*)calloc(K, sizeof(int));


	for (int k = 0 ; k < K; k++){
  	in >> d[k];
		//cout << " " << d[k];
	}


  int **pi;
  pi = (int**)calloc(I, sizeof(int*)); 
	for (int i = 0 ; i < I; i++){
		pi[i] = (int*)calloc(J, sizeof(int)); 
	}


	for (int i = 0 ; i < I; i++){
		for (int j = 0 ; j < J; j++){
	  	in >> pi[i][j];
			cout << " " << pi[i][j];
		}
		cout << "\n";
	}


  int **c;
  c = (int**)calloc(I, sizeof(int*)); 
	for (int i = 0 ; i < I; i++){
		c[i] = (int*)calloc(J, sizeof(int)); 
	}


	for (int i = 0 ; i < I; i++){
		for (int j = 0 ; j < J; j++){
	  	in >> c[i][j];
			cout << " " << c[i][j];
		}
		cout << "\n";
	}

  int **h;
  h = (int**)calloc(I, sizeof(int*)); 
	for (int i = 0 ; i < I; i++){
		h[i] = (int*)calloc(J, sizeof(int)); 
	}


	for (int i = 0 ; i < I; i++){
		for (int j = 0 ; j < J; j++){
	  	in >> h[i][j];
			cout << " " << h[i][j];
		}
		cout << "\n";
	}


  int ***cus;
  cus = (int***)calloc(I, sizeof(int**)); 
	for (int i = 0 ; i < I; i++){
		cus[i] = (int**)calloc(J, sizeof(int*)); 
		for (int j = 0 ; j < J; j++){
	 		cus[i][j] = (int*)calloc(K, sizeof(int));
		}
	}


	for (int k = 0 ; k < K; k++){
		for (int i = 0 ; i < I; i++){
			for (int j = 0 ; j < J; j++){
		  	in >> cus[i][j][k];
				cout << " " << cus[i][j][k];
			}
			cout << "\n";
		}
	}

	in.close();



	IloEnv env;
	IloCplex cplex(env);
	IloModel model(env);
	IloExpr expr(env);

	// Variable definitions
	IloNumVarArray3 Y(env,I);
	for (int i = 0 ; i < I; i++){
		Y[i] = IloNumVarArray2 (env, J);
		for (int j = 0 ; j < J; j++){
			Y[i][j] = IloNumVarArray (env, K, 0,INF,ILOFLOAT);
		}
	}

	IloNumVar z(env,0, INF ,ILOINT);


  // Objective function
	expr.clear();
	for (int i = 0 ; i < I; i++){
		for (int j = 0 ; j < J; j++){
			for (int k = 0 ; k < K; k++){
				expr += cus[i][j][k]*Y[i][j][k];
			}
		}
	}

	model.add(IloMinimize(env,expr));
	expr.clear();

	// Constraints

	for (int i = 0 ; i < I; i++){
		for (int j = 0 ; j < J; j++){
			for (int k = 0 ; k < K; k++){
				expr += Y[i][j][k];
			}
			model.add (expr <= pi[i][j]);
			expr.clear();
		}
	}



	for (int k = 0 ; k < K; k++){
		for (int i = 0 ; i < I; i++){
			for (int j = 0 ; j < J; j++){
				expr += Y[i][j][k];
			}
		}
		model.add (expr == d[k]);
		expr.clear();
	}



	cplex.extract(model);
	cplex.exportModel("saida.lp");

	IloNum upperBound = IloInfinity;
	cplex.solve();
  std::cout << "Solution = " << cplex.getObjValue() << std::endl;

	return 0;
}
