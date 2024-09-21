simulation: simulation.c list.c list.h gc.c gc.h
	gcc -L. simulation.c list.c gc.c -o simulation

.PHONY: clean
clean: 
	rm simulation