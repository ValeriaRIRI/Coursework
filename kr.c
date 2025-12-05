#define _CRT_SECURE_NO_DEPRECATE
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <locale.h>
#include <math.h>

double f(double x);
double factorial(int n);
double search_root(double a, double b, double epsilon, double y);
double derivative(double x);

int main() {
	setlocale(LC_ALL, "RUS");
	int c;



	do {

		printf("\nВыберите операцию (1 - вычисление f(x), 2 - таблица значений, 3 - поиск корней на интервале, 4 - производная в точке, 0 - выход из программы): ");
		scanf("%d", &c);
		switch (c) {
			case 1: {
				double x;
				printf("\nВведите x для вычисления функции f(x): ");
				scanf("%lf", &x);
				double res = f(x);

				if (isnan(res)) printf("f(x) не определена\n");
				else printf("f(%.5lf)=%.5lf\n", x, res);
				
				break;

			}
			case 2: {
				double x, y, s;
				printf("Введите начало интервала: ");
				scanf("%lf", &x);
				printf("Введите шаг: ");
				scanf("%lf", &s);
				printf("Введите конец интервала: ");
				scanf("%lf", &y);
				printf("|	x	|	f(x)   |\n");
				if (s == 0 || ((x < y) && s < 0) || ((x > y) && s > 0)) {
					log_error("Шаг выбран неправильно\n");
				}
				else {
					for (double i = x; (s > 0 ? i <= y : i >= y); i += s)
						printf("| %10.3lf | %10.3lf |\n", i, f(i));
				}
				break;
			}
			case 3: {
				double a, b;
				double y;
				double epsilon;
				printf("Введите начало и конец интервала: ");
				scanf("%lf %lf", &a, &b);
				printf("Введите y: ");
				scanf("%lf", &y);
				printf("Введите точность поиска значения: ");
				scanf("%lf", &epsilon);
				double root = search_root(a, b, epsilon, y);
				if (isnan(root)){
					log_error("Корень не найден или неподходящие параметры.");
				}else{
					printf("При y равном %lf, значение x будет примерно равно %lf, при точности %lf ", y, root, epsilon);
				}
				break;	
			}
			case 4: {
					double x;
					printf("Введите x для вычисления производной: ");
					scanf("%lf", &x);
					double df = derivative(x);
					if (isnan(df)) {
						log_error("Ошибка вычисления производной.");
						return -1;
					}
					else {
						printf("f'(%.6f) = %.10f\n", x, df);
					}
				
				break;
			}
			default: {
				printf("Такой операции не существует");
				break;
			}

		}
	}while(c != 0);
	return 0;
}


//функция для нахождения факториала
double factorial(int n) { 
	double result = 1;
	if (n < 0) {
		log_error("Ошибка: факториал от отрицательного числа не определен\n");
		return -1;
	}
	for (int i = 1; i <= n; i++) {
		result *= i;
	}
	return result;
}

// f(x)
double f(double x) {
	double y;
	if (x < -2) {
		y = cbrt(x); //cbrt - функция для нахождения кубического корня из файла math.h
	}
	else if (x >= -2 && x < 0)  {
		y = (sin(x)) / pow(x, 2);
	}
	else if (x >= 0) {
		double sum = 0; //переменная для хранения суммы
		for (int n = 0; n <= 12 /*добавить define*/ ; n++) { // n - предел функции
			double num = pow(-1, n) * pow(x, 2 * n + 1); //числитель
			double denom1 = (2 * n + 1); // часть знаменателя без факториала
			double denom2 = (2 * n + 1); // часть знаменателя с факториалом
			double denom = denom1 * factorial(denom2); // знаменатель
			sum += num / denom; //арифметическая прогрессия
		}
		y = sum;
	}
	return y;
}


//поиск корней функции
double search_root(double a, double b, double epsilon, double y) {
		// решаем f(x) - y = 0 методом деления отрезка пополам
		double fa = f(a) - y;
		double fb = f(b) - y;
		
		if (epsilon <= 0) {
			log_error("Ошибка: точность должна быть положительным числом.\n");
			return -1;
		}
		if (a == b) {
			log_error("Ошибка: начало и конец интервала совпадают.\n");
			return -1;
		}
		// Проверка смены знака на концах интервала (условие существования корня)
		if (fa * fb > 0) {
			log_error("Ошибка: на заданном интервале нет корня.\n");
			return -1;
		}

		double c;
		while (fabs(b - a) > epsilon) {
			c = (a + b) / 2;
			double fc = f(c) - y;
			if (fa * fc <= 0) {
				b = c;
				fb = fc;
			}
			else {
				a = c;
				fa = fc;
			}
		}
		double x = (a + b) / 2;
		return x;
}

// производная
double derivative(double x) {
	const double h = 1e-5;
	double f1 = f(x + h);
	double f2 = f(x - h);

	if (isnan(f1) || isnan(f2)) return -1;  

	return (f1 - f2) / (2.0 * h);
}

log_error(const char* message) {
	FILE* log_file = fopen("errors.log", "a");
	if (log_file) {
		fprintf(log_file, "ERROR: %s\n", message);
		fclose(log_file);
	}
}
