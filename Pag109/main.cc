/********************************************************           
    * Olá -- Este programa resolve o exemplo da página 109,  *   
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
	double lixo;
	int  i, j;

	int      n;//numero de produtos
	int      k;//numero de mix de produtos

	ifstream in(argv[1]);
	in >> n;
	_prod *produtos;
   	produtos = (_prod*)calloc(n, sizeof(_prod));

	for(j= 0; j < n; j++){
		in >> produtos[j].ID;
		in >> produtos[j].p;
		in >> produtos[j].c;
		in >> produtos[j].l;
		in >> produtos[j].u;		
	}

	in >> k;
    double **a;
    a = (double**)calloc(k, sizeof(double*)); 
    for(i = 0;i<k;i++) {
        a[i] = (double*)calloc(n, sizeof(double));
    }
	int *b;
   	b = (int*)calloc(k, sizeof(int));

    for(i = 0;i<k;i++) {
		for(j= 0; j < n; j++){
			in >> a[i][j];
		}
		in >> b[i];
	}

	IloEnv env;
	IloCplex cplex(env);
	IloModel model(env);
	IloExpr expr(env);

	// Variable definitions
	IloNumVarArray x(env,n,0,INF,ILOFLOAT);//padrao
	IloRangeArray Rest = IloAdd(model, IloRangeArray(env));    // parte da Coluna


    // Objective function
	for (int i = 0 ; i < n; i++){
		expr += (produtos[i].p-produtos[i].c)*x[i];
	}
	model.add(IloMaximize(env,expr));
	expr.clear();

    // Constraints
	for(j= 0; j < k; j++){
		for (i = 0 ; i < n ; i++){
			expr += a[j][i]*x[i];
		}
		Rest.add (expr <= b[j]);
		model.add (Rest);
		expr.clear();
	}
	for (i = 0 ; i < n ; i++){
		Rest.add (x[i] >= produtos[i].l);
		model.add (Rest);
		Rest.add (x[i] <= produtos[i].u);
		model.add (Rest);
	}

	cplex.extract(model);
	cplex.exportModel("saida.lp");

	IloNum upperBound = IloInfinity;
	cplex.solve();
    std::cout << "Solution = " << cplex.getObjValue() << std::endl;
   	for(j= 0; j < k+n+1; j++){ 
	    std::cout << "Valor dual da restrição c" << j+1 << " = " << cplex.getDual(Rest[j]) << std::endl;
	}
	return 0;
}
