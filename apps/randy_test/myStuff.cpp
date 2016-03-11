#include "myStuff.h"
#include <cmath>
//#include <iostream>

class myStuff::impl
{
public:
	double calc(double input, int iteration)
	{
		double out = input;
		for (int i=0; i<1000; ++i)
		{
			out = pow(out, 2);
			out = pow(out, .5);
		}
		out += .01 * iteration;
		return out;
	}
};

myStuff::myStuff() : pimpl(new impl())
{
//	std::cout << "allocating" << std::endl;
};

myStuff::myStuff(const myStuff& ref) : pimpl(new impl())
{
	//	std::cout << "allocating" << std::endl;
};

myStuff::~myStuff()
{
//	std::cout << "freeing" << std::endl;
};

double myStuff::calc(double input, int iteration)
{
	return pimpl->calc(input, iteration);
}