#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define MAX_STOP 10;


/* UWAGA: liczbę używanych f. bazowych można ustawić przez wartość
          zmiennej środowiskowej APPROX_BASE_SIZE
*/

/*
 * Funkcje bazowe: n - liczba funkcji a,b - granice przedzialu aproksymacji i
 * - numer funkcji x - wspolrzedna dla ktorej obliczana jest wartosc funkcji
 */

//n - stopień , x wartość:
double stopien_wart(int n, double x)
{
	double t[11];
	if(n==0)return 1;
	else
	if(n==1)return 1-x;
	else
	return ((2 * n  + 1 - x) * stopien_wart(n-1, x) - n*stopien_wart(n-2, x))/(n+1);
}



/* Pomocnicza f. do rysowania bazy */
double
xfi(double a, double b, int n, int i, FILE *out)
{
	double		h = (b - a) / (n - 1);
	double		h3 = h * h * h;
	int		hi         [5] = {i - 2, i - 1, i, i + 1, i + 2};
	double		hx      [5];
	int		j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	fprintf( out, "# nb=%d, i=%d: hi=[", n, i );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %d", hi[j] );
	fprintf( out, "] hx=[" );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %g", hx[j] );
	fprintf( out, "]\n" );
}

void
make_spl(points_t * pts, spline_t * spl)
{

	matrix_t       *eqs= NULL;
	double         *x = pts->x;
	double         *y = pts->y;
	double		a = x[0];
	double		b = x[pts->n - 1];
	int		i, j, k;
	int		nb = pts->n - 3 > 10 ? 10 : pts->n - 3;
	int max_baza = MAX_STOP;
  char *nbEnv= getenv( "APPROX_BASE_SIZE" );
	
	printf("%d\n", max_baza);

	if( nbEnv != NULL && atoi( nbEnv ) > 0 )
		nb = atoi( nbEnv );
	eqs = make_matrix(max_baza, max_baza + 1);
	
	for (j = 0; j < max_baza; j++) {
		for (i = 0; i < max_baza; i++)
			for (k = 0; k < pts->n; k++)
			{
				add_to_entry_matrix(eqs, j, i, stopien_wart(i, x[k]) * stopien_wart(j, x[k]));
				printf("%g\n", stopien_wart(i, x[k]) * stopien_wart(j, x[k]));
			}

		for (k = 0; k < pts->n; k++)
			add_to_entry_matrix(eqs, j, nb, y[k] * stopien_wart(j, x[k]));
	}


	if(piv_ge_solver(eqs))
	{
		spl->n = 0;
		return;
	}


	if (alloc_spl(spl, max_baza) == 0) {
		for (i = 0; i < spl->n; i++) {
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1);
			xx+= 10.0*DBL_EPSILON;  // zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double	ck = get_entry_matrix(eqs, k, nb);
				spl->f[i]  += ck * stopien_wart(k, xx);
				spl->f1[i] += ck * stopien_wart(k, xx);
				spl->f2[i] += ck * stopien_wart(k, xx);
				spl->f3[i] += ck * stopien_wart(k, xx);
			}
		}
	}
}
