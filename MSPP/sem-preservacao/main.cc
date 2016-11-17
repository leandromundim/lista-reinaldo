/********************************************************           
    * Olá - Este programa resolve o modelo de dimensionamento*
		*		de lotes (múltiplos períodos, múltiplos itens, capa- *
    *   citados e sem preservacao de preparacao.						 *
		*		Do livro "Operations research in production planning *   
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
main(int argc, char **argv){

	int I = 3;
	int T = 4;

	ifstream in("dados.dat");
	//demanda d[i][t]
  int **d;
  d = (int**)calloc(I, sizeof(int*)); 
	for (int i = 0 ; i < I; i++){
		d[i] = (int*)calloc(T, sizeof(int)); 
	}
	for (int i = 0 ; i < I; i++){
		for (int t = 0 ; t < T; t++){
			in >> d[i][t]; 
		}
	}
  int **dtal;
  dtal = (int**)calloc(I, sizeof(int*)); 
	for (int i = 0 ; i < I; i++){
		dtal[i] = (int*)calloc(T, sizeof(int)); 
	}
	for (int i = 0 ; i < I; i++){
		for (int t = 0 ; t < T; t++){
			in >> dtal[i][t]; 
		}
	}


	//b[i] tempo para produzir uma unidade do item i
  int *b;
	b = (int*)calloc(I, sizeof(int));
	b[0] = 20;
	b[1] = 10;
	b[2] = 20;


	//C[t] capacidade de producao de uma maquina no periodo t
  int *C;
	C = (int*)calloc(T, sizeof(int));
	C[0] = 280;
	C[1] = 320;
	C[2] = 280;
	C[3] = 400;

	//sp[i] 
  int *sp;
	sp = (int*)calloc(I, sizeof(int));
	for (int i = 0 ; i < I; i++){
  	sp[i] = 40;
	}

	//s[i] te
  int *s;
	s = (int*)calloc(I, sizeof(int));
	s[0] = 350;
	s[1] = 100;
	s[2] = 90;

	//h[i] te
  int *h;
	h = (int*)calloc(I, sizeof(int));
 	h[0] = 150;
 	h[1] = 100;
 	h[2] = 70;

	//I0[i] te
  int *I0;
	I0 = (int*)calloc(I, sizeof(int));
	for (int i = 0 ; i < I; i++){
  	I0[i] = 0;
	}


	IloEnv env;
	IloCplex cplex(env);
	IloModel model(env);
	IloExpr expr(env);

	// Variable definitions
	char str[40];
	IloNumVarArray2 y(env,I);
	for (int i = 0 ; i < I; i++){
		y[i] = IloNumVarArray (env, T, 0,INF,ILOBOOL);
		for (int t = 0 ; t < T ; t++){
			sprintf (str,"y[%d][%d]",i,t);
			y[i][t].setName(str);
		}
	}


	IloNumVarArray2 x(env,I);
	for (int i = 0 ; i < I; i++){
		x[i] = IloNumVarArray (env, T, 0,INF,ILOFLOAT);
		for (int t = 0 ; t < T ; t++){
			sprintf (str,"x[%d][%d]",i,t);
			x[i][t].setName(str);
		}
	}

	IloNumVarArray2 estoque(env,I);
	for (int i = 0 ; i < I; i++){
		estoque[i] = IloNumVarArray (env, T, 0,INF,ILOFLOAT);
		for (int t = 0 ; t < T ; t++){
			sprintf (str,"est[%d][%d]",i,t);
			estoque[i][t].setName(str);
		}
	}

  // Objective function
	expr.clear();
	for (int i = 0 ; i < I; i++){
		for (int t = 0 ; t < T; t++){
			expr += s[i]*y[i][t]+h[i]*estoque[i][t];
		}
	}

	model.add(IloMinimize(env,expr));
	expr.clear();

	// Constraints

	for (int t = 0 ; t < T; t++){
		for (int i = 0 ; i < I; i++){
			int sumdit = 0;
			for (int tal = t ; tal < T; tal++) sumdit += dtal[i][tal];
			if(((C[t]-sp[i])/b[i]) < sumdit) model.add (x[i][t] <= ((C[t]-sp[i])/b[i])*y[i][t]);	
			else model.add (x[i][t] <= (sumdit)*y[i][t]);	
		}
	}


	expr.clear();
	for (int t = 0 ; t < T; t++){
		for (int i = 0 ; i < I; i++){
				expr += sp[i]*y[i][t] + b[i]*x[i][t];

		}
		model.add (expr <= C[t]);	
		expr.clear();
	}


	for (int t = 0 ; t < T; t++){
		for (int i = 0 ; i < I; i++){
			if(t==0) model.add (estoque[i][t] ==I0[i] +x[i][t] -d[i][t]);	
			else model.add (estoque[i][t] == estoque[i][t-1] +x[i][t] -d[i][t]);	
		}
	}


	cplex.extract(model);
	cplex.exportModel("saida.lp");

	IloNum upperBound = IloInfinity;
	cplex.solve();
  std::cout << "Solution = " << cplex.getObjValue() << std::endl;
	
	cout << " y[I][T]" << endl;
	for (int i = 0 ; i < I; i++){
		for (int t = 0 ; t < T ; t++){
			cout << "\t" <<  cplex.getValue(y[i][t]);
		}
	  cout << endl;
	}


	cout << " x[I][T]" << endl;
	for (int i = 0 ; i < I; i++){
		for (int t = 0 ; t < T ; t++){
			cout << "\t" <<  cplex.getValue(x[i][t]);
		}
	  cout << endl;
	}
	return 0;
}
