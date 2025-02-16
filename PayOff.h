#ifndef PAYOFF_H
# define PAYOFF_H

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
	PayOffCall(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffCall(){}

};


class PayOffPut : public PayOff
{
private:
	double Strike;
public:
	PayOffPut(double Strike_);
	virtual double operator()(double Spot) const;
	virtual ~PayOffPut(){}

};




#endif                                                                                   