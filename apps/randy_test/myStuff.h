#pragma once 

#include <memory>

class myStuff
{
public:
	myStuff();
	~myStuff();

	myStuff(const myStuff& ref);

	double calc(double val, int iteration);

private:
	class impl;
	std::unique_ptr<impl> pimpl;
};
