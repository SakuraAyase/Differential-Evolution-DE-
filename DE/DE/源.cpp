#include<cmath>
#include<vector>
#include<ctime>
#include<random>
#include<iostream>
#define pi 3.1415926535
#define E  2.71828182845904523536
using namespace std;

double randDouble(double min, double max)
{
	static default_random_engine engine(time(nullptr));
	static uniform_real_distribution<double> dis(min, max);
	return dis(engine);
}

bool better(double a, double b)
{
	if (a < b)
		return true;
	else
		return false;
}

double fitnessFunction(vector<double> pos)
{
	double result = 0.0;
	for (int i = 0; i < pos.size(); i++)
	{
		result += pow(pos[i], 2);
	}

	return result;
}

class P
{
public:

	vector<double> position;

};
class DE
{
public:

	DE(int dim, int m, int Tmax, double max, double min, double F,
		double CR, double percent)
	{

		this->dim = dim;
		this->m = m;
		this->Tmax = Tmax;
		this->max = max;
		this->min = min;
		this->F = F;
		this->CR = CR;
		this->dt = dt;
		this->percent = percent;
		particles.resize(m);
		gbest = 0;
		T = 0;
	}

	void initialParticles(int i)
	{
		particles[i].position.resize(dim);
		for (int j = 0; j < dim; j++)
			particles[i].position[j] = randDouble(min, max);
		if (better(fitnessFunction(particles[i].position), fitnessFunction(particles[gbest].position)))
			gbest = i;
	}

	void initialAllParticles()
	{
		for (int i = 0; i < m; i++)
		{
			initialParticles(i);
		}
	}

	void updateParticle(int i)
	{


		//update position, in DE processing
		int r1;
		int r2;
		int r3;
		static default_random_engine engine1(time(nullptr));
		static uniform_int_distribution<int> dis1(0, m - 1);

		//mutation
		do
		{
			r1 = dis1(engine1);
			r2 = dis1(engine1);
			r3 = dis1(engine1);

		} while (r1 == r2 || r2 == r3 || r1 == r3 || r1 == i || r2 == i || r3 == i);
		vector<double>V = particles[r1].position;
		for (int j = 0; j < dim; j++)
		{
			V[j] = V[j] + F * (particles[r2].position[j] - particles[r3].position[j]);
			if (V[j] > max)
				V[j] = max;

			else if (V[j] < min)
				V[j] = min;
		}
		//crossover
		vector<double>U = V;
		static default_random_engine engine2(time(nullptr));
		static uniform_int_distribution<int> dis2(0, dim - 1);
		int rn = dis2(engine2);
		for (int j = 0; j < dim; j++)
		{
			double rd = randDouble(0, 1);
			if (rd > CR && j != rn)
			{
				U[j] = particles[i].position[j];
			}
		}

		//seletion
		if (better(fitnessFunction(U), fitnessFunction(particles[i].position)))
		{
			particles[i].position = U;
		}
		if (better(fitnessFunction(particles[i].position), fitnessFunction(particles[gbest].position)))
			gbest = i;
	}

	void updateAllParticles()
	{
		for (int i = 0; i < m; i++)
		{
			updateParticle(i);
		}

		T++;
	}

	int dim;
	int m;
	int Tmax;
	double max;
	double min;
	double dt;
	double w;

	double F;

	double CR;

	double percent;

	vector<P> particles;
	int T;
	int gbest;

};

//1.65894e-73
//1.2972e-67
//1.33272e-67
void run()
{
	int dim = 30;
	int m = 100;
	int Tmax = 5000;
	double max = 100;
	double min = -100;

	double F = 0.5;
	double CR = 0.5;

	double percent = 0.2;


	DE plade = DE(dim, m, Tmax, max, min, F,
		CR, percent);
	plade.initialAllParticles();
	vector<double>fitness;
	fitness.push_back(fitnessFunction(plade.particles[plade.gbest].position));
	for (int i = 0; i < Tmax; i++)
	{

		plade.updateAllParticles();
		//plade.caculatePSD();
		//fitness.push_back(pso.getFitness());

		fitness.push_back(fitnessFunction(plade.particles[plade.gbest].position));
		cout << "第" << i << "次迭代结果：";
		cout << ", fitness = " << fitnessFunction(plade.particles[plade.gbest].position) << endl;



	}
}

int main()
{
	run();
	system("pause");
}
