#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "winbgi2.h"
#include "rk4.h"

void scan(double* x);
void rhs_fun(double t, double* X, double* F);
void veuler(double t, double* X, double h, int n, void (*fun)(double, double*, double*), double* X1);
double energia(double w, double a);

double l = 1;		//dlugosc wahadla
double g = 9.80665;		//przyspieszenie ziemskie
double m = 1;		//masa kulki

void main()
{
	double t, h, e, a0, w0, t0, tm;
	double* X, * X1;
	int n = 2;
	t0 = 0;		//czas poczatkowy 
	tm = 10;		//czas koncowy

	do
	{
		printf("Podaj mase kulki: [m] \n");
		scan(&m);
	} while (m <= 0);

	do
	{
		printf("Podaj dlugosc wahadla: [l] \n");
		scan(&l);
	}while (l<=0);

	printf("Podaj warunki poczatkowe: [a0 (w radianach), w0(w radianach na sekunde)] \n");
	scan(&a0);
	scan(&w0);

	printf("Podaj krok calkowania: [h] \n");
	scan(&h);
	

	X = (double*)malloc( n * sizeof(double));
	X1 = (double*)malloc( n * sizeof(double));
	
	if (X == NULL && X1 == NULL)
	{

		printf("Wystapil blad alokacji :( \n");
		exit(1);

	}
	

	graphics(600, 600);			//oblsuga grafiki
	scale(-a0-w0-2, -w0-3, a0+w0+2, w0+3);
	title("EULER - CZERWONY \t RK4 - ZIELONY","","");
	
	printf("\nMETODA EULERA\n");		//liczenie metoda Eulera
	X[0] = w0;
	X[1] = a0;
	t = t0;

	setcolor(1); //czerwony
	point(X[1], X[0]);
	for(t; t<=tm; t+=h )
	{

		veuler(t, X, h, n, rhs_fun, X1);
		X[0] = X1[0];
		X[1] = X1[1];
		e = energia(X[0], X[1]);
		printf("EULER\talfa = %lf\tomega = %lf\tenergia = %lf \n", X[1], X[0], e);
		point(X[1], X[0]);
		
	}
	
	printf("\n\n\n\nMETODA RK4\n");			//liczenie metoda RK4
	X[0] = w0;
	X[1] = a0;
	t = t0;

	setcolor(0.5);		//zielony
	point(X[1], X[0]);
	for (t; t <= tm; t += h)
	{
		
		vrk4(t, X, h, n, rhs_fun, X1);
		X[0] = X1[0];
		X[1] = X1[1];
		e = energia(X[1], X[0]);
		printf("RK4\talfa = %lf\tomega = %lf\tenergia = %lf \n", X[1], X[0], e);
		point(X[1], X[0]);

	}

	

	free(X);
	free(X1);
	wait();
}

void scan(double* x)		//funkcja do wczytywania i sprawdzania zgodnosci inputu
{
	double temp;
	while (scanf("%lf", &temp) != 1)
	{
		printf("Nieprawid³owy format danych! \n");
		int n;
		while ((n = getchar()) != EOF && n != '\n');
	}
	*x = temp;
}

void rhs_fun(double t, double* X, double* F)		//funkcja obliczajaca prawe strony rownania
{
	
	F[1] = -(g / l) * sin(X[0]);
	F[0] = X[1];
	
}

void veuler(double t, double* G, double h, int n, void (*fun)(double, double*, double*), double* G1)		//funkcja liczaca metoda Eulera
{

	fun(t, G, G1);
	int i = 0;
	for(i; i<n; i++)
	{
		G1[i] = G[i] + h * G1[i] ;
	}
	
}
double energia(double w, double a)			//funkcja liczaca energie calkowita
{	

	return ((m * l * l) / 2 ) * (w * w) + m * g * l * (1 - cos(a));

}


