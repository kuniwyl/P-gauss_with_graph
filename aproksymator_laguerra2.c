#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>



#define baza 10 

/* UWAGA: liczbę używanych f. bazowych można ustawić przez wartość
          zmiennej środowiskowej APPROX_BASE_SIZE
*/

/*
 * Funkcje bazowe: n - liczba funkcji a,b - granice przedzialu aproksymacji i
 * - numer funkcji x - wspolrzedna dla ktorej obliczana jest wartosc funkcji
 */

double stop(int n, int y , double x)
{
		if(n==0)return 1;	
		if(n==1)return 1 + (double)y - x;
	        return ((((2.0 * n - 1 + y - x) * stop(n-1, y, x) -((n-1) + y) * stop(n-2, y , x) ) ) / n);
}


double di(int n, double x, int k, int y)
{
	int a = k % 2 == 0 ? 1 : -1;
	if(k<=n)
		return a * stop(n - k, y + k , x);
	return 0;
}

void
make_spl(points_t * pts, spline_t * spl, int level)
{

	matrix_t       *eqs= NULL;
	double         *x = pts->x;
	double         *y = pts->y;
	double		a = x[0];
	double		b = x[pts->n - 1];
	int		i, j, k;
	int 		nb = level;
	
	eqs = make_matrix(nb, nb + 1);

	for (j = 0; j < nb; j++) {
		for (i = 0; i < nb; i++)
			for (k = 0; k < pts->n; k++)
			{
				add_to_entry_matrix(eqs, j, i, stop(i, 0, x[k]) * stop(j, 0, x[k]));	
			}
		for (k = 0; k < pts->n; k++)
			add_to_entry_matrix(eqs, j, nb, y[k] * stop(j, 0, x[k]));
	}

	if (piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}

	if (alloc_spl(spl, nb) == 0) {
		for (i = 0; i < spl->n; i++) {
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1);
			xx+= 10.0*DBL_EPSILON;  // zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double		ck = get_entry_matrix(eqs, k, nb);
				spl->f[i]  += ck * stop(k, 0, xx);
				spl->f1[i] += ck * di(k, xx, 1, 0);
				spl->f2[i] += ck * di(k, xx, 2, 0);
				spl->f3[i] += ck * di(k, xx, 3, 0);
			}
		}
	}
}
