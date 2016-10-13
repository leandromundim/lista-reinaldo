
# ---------------------------------------------------------------------
# Variрveis do sistema (instalaусo na lotcplex) 
# ---------------------------------------------------------------------


SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio126/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio126/concert

#CC  = valgrind --leak-check=yes g++
CC = g++


# ---------------------------------------------------------------------
# Opушes do compilador 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD

# ---------------------------------------------------------------------
# Diretзrios e Flags
# ---------------------------------------------------------------------

#CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -m64 -lm -lpthread
CLNFLAGS  = -L$(CPLEXLIBDIR) -lcplex -m64 -lm -lpthread

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 


# ---------------------------------------------------------------------
# MODIFIQUE A PARTIR DAQUI!
# NESTE EXEMPLO: 
# peqEspCG1.0 ж o arquivo cpp principal
# ---------------------------------------------------------------------

ex : main.o
	${CC} ${CCFLAGS} main.o $(CCLNFLAGS) -o ex 

main.o : main.cc
	${CC} ${CCFLAGS} -c main.cc

clean :
	rm *.o main *~



