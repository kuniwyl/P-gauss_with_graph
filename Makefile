aprox: main.o splines.o points.o aproksymator_na_bazie.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_na_bazie.o -L gaus -l ge

aprox1: main.o splines.o points.o aproksymator_laguerra.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_laguerra.o -L gaus -l ge -ggdb

aprox2: main.o splines.o points.o aproksymator_laguerra2.o gaus/libge.a
	$(CC) -o aprox  main.o splines.o points.o aproksymator_laguerra2.o -L gaus -l ge -ggdb

intrp: main.o splines.o points.o interpolator.o gaus/libge.a
	$(CC) -o intrp  main.o splines.o points.o interpolator.o -L gaus -l ge

prosta: main.o splines.o points.o prosta.o
	$(CC) -o prosta  main.o splines.o points.o prosta.o	

aproksymator_na_bazie.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_na_bazie.c

aproksymator_laguerra.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_laguerra.c

aproksymator_laguerra2.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c aproksymator_laguerra2.c

interpolator.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c interpolator.c

.PHONY: clean

clean:
	-rm *.o aprox intrp prosta
