#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<algorithm>
#include<ctime>
#include<random>
using namespace std;

double nextDouble (double lower_bound, double upper_bound)
{
	static uniform_int_distribution<int> rand(lower_bound*10000, upper_bound*10000);
	static default_random_engine engine(static_cast<unsigned int>(time(0)));
	return ((double)rand(engine))/10000;
}

int nextInt(int lower_bound, int upper_bound)
{
	static uniform_int_distribution<int> rand(lower_bound, upper_bound - 1);
	static default_random_engine engine(static_cast<unsigned int>(time(0)));
	return rand(engine);
}

double function(vector<double>n)
{
	double powNum = 0;
	double mutipleNum = 0;
	double x = 0;
	for (auto i : n)
		x += i*i;
	return x;
}

vector<vector<double>>initial(int np,int D,double low_bound,double upper_bound)
{
	vector<vector<double>>initial_vector;
	vector<double>list(D);
    double i1 = (double)low_bound / D;
	double i2 = (double)upper_bound / D;
	//cout << i1 << i2 << endl;

	for (int i = 0; i < np; i++)
	{
		for (int j = 0; j < D; j++)
		{
			//list[j] = nextDouble(i1*(j+1), i2*(j+1));
			list[j] = nextDouble(low_bound, upper_bound);
		}
		initial_vector.push_back(list);
	}
	return initial_vector;
}

vector<double>find_best(vector<vector<double>>population)
{
	double min = function(population[0]);
	int index = 0;
	for (int i = 1; i < population.size(); i++)
	{
		if (min > function(population[i]))
		{
			min = function(population[i]);
			index = i;
		}
	}
	return population[index];
}

vector<double>better_mutation(double lambda,double F, vector<vector<double>>population,double upper,double lower)
{
	int D = population[0].size();
	int NP = population.size();
	int r1 = nextInt(0, NP);

	int r2 = nextInt(0, NP);
	while (r2 == r1)
		r2 = nextInt(0, NP);

	int r3 = nextInt(0, NP);
	while (r2 == r3 || r3 == r1)
		r3 = nextInt(0, NP);

	vector<double>u_vector(D);
	vector<double>best = find_best(population);
	for (int i = 0; i < D; i++)
	{
		double temp = u_vector[i];
		u_vector[i] = population[r1][i] + F*(population[r2][i] - population[r3][i])
			+ lambda*(best[i] - population[r1][i]);
		if (u_vector[i] > upper || u_vector[i] < lower)
			u_vector[i] = temp;
		/*if (function(temp) < function(u_vector))
		{
			u_vector[i] = population[r1][i] + F*(population[r3][i] - population[r2][i])
				+ lambda*(best[i] - population[r1][i]);
		}*/
	}
	return u_vector;
}

vector<double>mutation(double F, vector<vector<double>> population,double upper,double lower)
{
	int D = population[0].size();
	int NP = population.size();
	int r1 = nextInt(0, NP);

	int r2 = nextInt(0, NP);
	while (r2 == r1)
		r2 = nextInt(0, NP);

	int r3 = nextInt(0, NP);
	while (r2 == r3 || r3 == r1)
		r3 = nextInt(0, NP);

	vector<double>u_vector(D);
	for (int i = 0; i < D; i++)
	{
		double temp = u_vector[i];
		u_vector[i] = population[r1][i] + F*(population[r2][i] - population[r3][i]);
		if (u_vector[i] > upper || u_vector[i] < lower)
			u_vector[i] = temp;
	}
	return u_vector;
}

vector<double>Crossover(double CR,vector<double>t,vector<double>u_v)
{
	int D = t.size();
	for (int i = 0; i < D; i++)
	{
		double _cr = nextDouble(0, 1);
		if (_cr < CR)
		{
			u_v[i] = t[i];
		}
	}
	if (function(t) > function(u_v))
		return u_v;
	else
		return t;
}

void DE_algrithm(int MAX_GENERATION)
{
	int NP = 100;
	int D = 30;
	int upper = 100;
	int lower = -100;
	double F = 0.6;
	double CR = 0.8;
	double lambda = 0.9;
	vector<vector<double>>NUM = initial(NP, D, lower, upper);
	for(int i=0;i<MAX_GENERATION;i++)
	{
		//vector<vector<double>>newNUM = NUM;
		for (int j = 0; j < NP; j++)
		{
			vector<double>temp = better_mutation(lambda, F, NUM,upper,lower);
			//vector<double>temp = mutation( F, NUM);
			NUM[j] = Crossover(CR, NUM[j], temp);
			//newNUM[j] = Crossover(CR, NUM[j], temp);
		}
		//NUM = newNUM;
		cout << "Generation " << i+1 << endl;
		/*for (auto a : NUM)
		{
			cout << function(a) << endl;
		}*/
		cout <<fixed<<setprecision(60)<< function(find_best(NUM)) << endl;
		/*for (auto a : find_best(NUM))
		{
			cout << a <<endl;
		}*/
		cout << endl;
	}
	
	//cout << function(find_best(NUM)) << endl;
}

int main()
{
	DE_algrithm(500);
}