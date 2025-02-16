/*Deleting a derived class object using a pointer of base class type that has a non-virtual destructor results in undefined behavior. 
To correct this situation, the base class should be defined with a virtual destructor. */

// As a guideline, any time you have a virtual function in a class, you should immediately add a virtual destructor (even if it does nothing). 
// This way, you ensure against any surprises later. 


# include <iostream>
// # include <cstdlib>
# include <cmath>
# include <vector>

using namespace std;

// List of Classes

class PayOff
{
public:
	PayOff(){}
	virtual double operator()(double Spot) const = 0;
	virtual ~PayOff(){}
};


class PayOffCall : public PayOff
{
private:
	double Strike;
public:
	PayOffCall(double Strike_)
		: Strike(Strike_)
	{

	}
	virtual double operator()(double Spot) const
	{
		double val = Spot - Strike;
		return ((val>0.0)?val:0.0);
	}
	virtual ~PayOffCall(){}

};


class PayOffPut : public PayOff
{
private:
	double Strike;
public:
	PayOffPut(double Strike_)
		: Strike(Strike_)
	{

	}
	virtual double operator()(double Spot) const
	{
		double val = Strike - Spot;
		return ((val>0.0)?val:0.0);
	}
	virtual ~PayOffPut(){}

};


// List of Function descriptions

double GetOneGaussianBySummation();
double GetOneGaussianByBoxMuller();
double BoxMuller();
double SimpleMonteCarlo(const PayOff& thePayOff,
						double Expiry,
						double Spot,
						double Vol,
						double r,
						unsigned long NumberOfPaths);



class MonteCarlo
{
private:
		double variance;
		double rootVariance;
		double itoCorrection;
		double movedSpot;
		double thisSpot;
		double runningSum;
		double runningSquare;
		double standarderror;
		double mean;
public:
	MonteCarlo(const PayOff& thePayOff,
				double Expiry,
				double Spot,
				double Vol,
				double r,
				unsigned long NumberOfPaths)
	{
		variance = Vol*Vol*Expiry;
		rootVariance = sqrt(variance);
		itoCorrection = -0.5*variance;
		movedSpot = Spot*exp(r*Expiry +itoCorrection);
		runningSum = 0;
		runningSquare = 0;
		standarderror = 0;

		for (unsigned long i=0; i < NumberOfPaths; i++)
		{
			double thisGaussian = GetOneGaussianByBoxMuller();
			thisSpot = movedSpot*exp( rootVariance*thisGaussian);
			double thisPayOff = thePayOff(thisSpot);
			runningSum += thisPayOff;
			runningSquare += (thisPayOff*thisPayOff);
		}
		mean = runningSum / NumberOfPaths;
		standarderror = sqrt((runningSquare + NumberOfPaths*mean*mean - 2*mean*runningSum)/(NumberOfPaths*(NumberOfPaths - 1)));
		mean *= exp(-r*Expiry);

		cout<<"The price of the option is $"<<mean<<'\n';
		cout<<"The standard error of the simulation is $"<<standarderror<<'\n';
	}

	double get_price() const
	{
		return mean;
	}

	double get_standarderror() const
	{
		return standarderror;
	}
};



class MonteCarlo2
{
private:
		double variance;
		double rootVariance;
		double itoCorrection;
		double movedSpot;
		double thisSpot;
		double runningSum;
		double runningSquare;
		double standarderror;
		double mean;
public:
	MonteCarlo2(const PayOff& thePayOff,
				double Expiry,
				double Spot,
				double Vol,
				double r,
				unsigned long NumberOfPaths)
	{
		runningSum = 0;
		runningSquare = 0;
		standarderror = 0;

		for (unsigned long i=0; i < NumberOfPaths; i++)
		{
			double runningspot = 0;

			for(unsigned long j=1;j<=(12*Expiry);j++)
			{
				double t = ((double)j)/12;
				variance = Vol*Vol*t;
				rootVariance = sqrt(variance);
				itoCorrection = -0.5*variance;
				movedSpot = Spot*exp(r*t +itoCorrection);
				runningspot+= movedSpot*exp(rootVariance*BoxMuller());
			}
			thisSpot =  runningspot/(12*Expiry);
			double thisPayOff = thePayOff(thisSpot);
			runningSum += thisPayOff;
			runningSquare += (thisPayOff*thisPayOff);

		}
		mean = runningSum / NumberOfPaths;
		standarderror = sqrt((runningSquare + NumberOfPaths*mean*mean - 2*mean*runningSum)/(NumberOfPaths*(NumberOfPaths - 1)));
		mean *= exp(-r*Expiry);

		cout<<"The price of the option is $"<<mean<<'\n';
		cout<<"The standard error of the simulation is $"<<standarderror<<'\n';
	}

	double get_price() const
	{
		return mean;
	}

	double get_standarderror() const
	{
		return standarderror;
	}
};




// List of Function definitions


double GetOneGaussianBySummation()
{
	double result=0;
	for (unsigned long j=0; j < 12; j++){
		result += rand()/static_cast<double>(RAND_MAX);
	}
	result -= 6.0;
	return result;
}

double GetOneGaussianByBoxMuller()
{
	double result;
	double x;
	double y;
	double sizeSquared;
	do{
		x = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		y = 2.0*rand()/static_cast<double>(RAND_MAX)-1;
		sizeSquared = x*x + y*y;
	}while(sizeSquared >= 1.0);
	result = x*sqrt(-2*log(sizeSquared)/sizeSquared);
	return result;
}


double BoxMuller() 
{
    double u1 = rand()/static_cast<double>(RAND_MAX);
    double u2 = rand()/static_cast<double>(RAND_MAX);
    
    return sqrt(-2.0 * log(u1)) * cos(2*M_PI*u2);
   
}





 
 
int main()
{
	double Expiry = 1;
	double Strike = 105;
	double Spot = 100;
	double Vol = 0.2;
	double r = 0.05;
	unsigned long NumberOfPaths = 10000;
	unsigned long optionType = 0;


	// cout << "\nEnter expiry\n";
	// cin >> Expiry;

	// cout << "\nEnter strike\n";
	// cin >> Strike;

	// cout << "\nEnter spot\n";
	// cin >> Spot;

	// cout << "\nEnter vol\n";
	// cin >> Vol;

	// cout << "\nr\n";
	// cin >> r;

	// cout << "\nNumber of paths\n";
	// cin >> NumberOfPaths;

	// cout << "\nenter 0 for call, otherwise put \n";
	// cin >> optionType;

	PayOff* thePayOffPtr;

	if (optionType== 0){
		thePayOffPtr = new PayOffCall(Strike);
	}else{
		thePayOffPtr = new PayOffPut(Strike);
	}



	// double result = SimpleMonteCarlo(*thePayOffPtr,
	// 									Expiry,
	// 									Spot,
	// 									Vol,
	// 									r,
	// 									NumberOfPaths);

	MonteCarlo2 m1(*thePayOffPtr,Expiry,Spot,Vol,r,NumberOfPaths);



	
	delete thePayOffPtr;
	return 0;
	
}













 
