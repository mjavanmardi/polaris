#pragma once

enum CarModel {vissimCar};

class CarType
{
public:
	CarType(double _carLength, double _beta, double _delta, double _gamma, double _maxAcc, double _maxDec) :
	  carLength(_carLength), beta(_beta), delta(_delta), gamma(_gamma), maxAcc(_maxAcc), maxDec(_maxDec){}

	//### Getters ###
	double getCarLength() const;
	double getBeta() const;
	double getDelta() const;
	double getGamma() const;
	double getMaxAcc() const;
	double getMaxDec() const;
private:
	const double carLength;
	const double beta; //free flow model parameter
	const double delta;//free flow model parameter
	const double gamma;//free flow model parameter
	const double maxAcc;
	const double maxDec;
};