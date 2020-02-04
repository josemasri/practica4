MPICC=mpicc
MRUN=mpirun

#Aqui esta la lista de parametros
PAR_I= -Wall -lm
PAR_LIB= -c -Wall -shared

#Archivos de cabecera
INC=.

#Archivos
PROG_01=ejercicio1

#Parametros
NODES=10

#Procesos
all: uno

uno:
	@echo "**************** COMPILADO CODIGO: $(PROG_01) ****************"
	$(MPICC) $(PAR_I) $(PROG_01).c -o $(PROG_01) -I$(INC)

rununo:
	@echo "**************** COMPILADO CODIGO: $(PROG_01) ****************"
	$(MPICC) $(PAR_I) $(PROG_01).c -o $(PROG_01) -I$(INC)
	$(MRUN) -np $(NODES) -hostfile maq $(PROG_01)

clean:
	rm $(PROG_01)