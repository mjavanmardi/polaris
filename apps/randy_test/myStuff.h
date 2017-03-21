#pragma once 

#include <memory>

class myStuff
{
public:
	myStuff();
	~myStuff();

	myStuff(const myStuff& ref);
	myStuff& operator=(const myStuff&);

	double calc(double val, int iteration);

private:
	class impl;
	std::unique_ptr<impl> pimpl;
};
