#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

double fun( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/5; // +-10%
                return (1+r)*(10*x*x*x*x+5*x*x*x+x*x);
}
double sin_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/5; // +-10%
                return (1+r)*sin(x);
}
double tg_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/5; // +-10%
                return (1+r)*tan(x);
}
double log_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/2.5; // +-20%
                return (1+r)*log(x);
}
double exp_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/2.5; // +-20%
                return (1+r)*exp(x);
}
double fun5_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/10; // +-5%
                return (1+r)*(1-2*x+4*x*x+2*x*x*x-5*x*x*x*x-3*x*x*x+x*x);
}
double fun_r( double x ) {
                double r= ((double)rand() - RAND_MAX/2.0)/RAND_MAX/5; // +-10%
                return (1+r)*(1/x+x/2);
}
typedef struct {
    char *name;
    double (*funkcja)(double);
} fun_t;

int main( int argc, char **argv ) {
                int n= argc > 1 ? atoi( argv[1] ) : 100;
                double a= argc > 2 ? atof( argv[2] ) : -3;
                double b= argc > 3 ? atof( argv[3] ) : 2;
                FILE *out= argc > 4 ? fopen( argv[4], "w" ) : stdout;
                fun_t t[] = { { "tg_r", tg_r }, { "sin_r", sin_r}, { "log_r", log_r }, { "log", log }, { "exp_r" , exp_r }, { "fun5_r" , fun5_r }, { "fun_r" , fun_r }, { "fun" , fun } };
                double (*funkcja)(double) = NULL;
                char *name = argc > 5 ? argv[5] : "fun";
                for( int i= 0; i < sizeof t / sizeof t[0]; i++ )
                if( strcmp( name, t[i].name ) == 0 )
                funkcja = t[i].funkcja;
                if( funkcja == NULL )
                {
                    fprintf( stderr, "%s: nie wiem, co to %s!\n", argv[5], name );
                    return 1;
                }
                srand( time(NULL) );

                int i;
                double dx = (b-a)/(n-1);

                for( i =0; i < n; i++ ) {
                                fprintf( out, "%g %g\n", a+i*dx, funkcja(a+i*dx) );
                }
                fclose( out );

                return 0;
}
