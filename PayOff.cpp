#include "PayOff.h"

PayOffCall::PayOffCall(double Strike_)
 : Strike(Strike_)
{

}
double PayOffCall::operator() (double Spot) const
{
	double val = Spot - Strike;
	return ((val>0.0)?val:0.0);
}


PayOffPut::PayOffPut(double Strike_)
 : Strike(Strike_)
{

}
double PayOffPut::operator() (double Spot) const
{
	double val = Strike - Spot;
	return ((val>0.0)?val:0.0);
}