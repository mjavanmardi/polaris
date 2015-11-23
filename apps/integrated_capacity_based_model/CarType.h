#pragma once

enum CarModel {vissimCar};

class CarType
{
public:
	CarType(double _carLength, double _beta, double _delta, double _gamma, double _maxAcc, double _maxDec, 
		double _tau, double _dz) :
	  carLength(_carLength), beta(_beta), delta(_delta), gamma(_gamma), maxAcc(_maxAcc), maxDec(_maxDec),
	  tau(_tau), dz(_dz){}

	//### Getters ###
	double getCarLength() const;
	double getBeta() const;
	double getDelta() const;
	double getGamma() const;
	double getMaxAcc() const;
	double getMaxDec() const;
	double getTau() const;
	double getDz() const;

private:
	const double carLength;
	const double beta; //Free flow model parameter
	const double delta; //Free flow model parameter
	const double gamma; //Free flow model parameter
	const double maxAcc; //Maximum acceleration
	const double maxDec; //Maximum Deceleration
	const double tau; //Reaction time
	const double dz; //Security distance with a front car when speed=0
};