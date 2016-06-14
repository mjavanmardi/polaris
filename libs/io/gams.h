#ifndef gams
#define gams
#include "Shared.h"

namespace polaris{ namespace io {
//Forward declarations.
//
class V;
class I;
class E;
class O;
class time_e;
class time_w;



#pragma db object
class V
{
public:
    // Default Constructor
    V () {}        
	V (std::string v_)
	: v (v_)
	{
	}
	//Accessors
	const std::string& getV () const {return v;}
	void setV (const std::string& v_) {v = v_;}
	//Data Fields
private:
	friend class odb::access;
	std::string v;
};


#pragma db object
class I
{
public:
    // Default Constructor
    I () {}        
	I (std::string I_)
	: I (I_)
	{
	}
	//Accessors
	const std::string& getI () const {return I;}
	void setI (const std::string& I_) {I = I_;}
	//Data Fields
private:
	friend class odb::access;
	std::string I;
};


#pragma db object
class E
{
public:
    // Default Constructor
    E () {}        
	E (std::string v_, std::string I_, std::string I2_)
	: v (v_), I (I_), I2 (I2_)
	{
	}
	//Accessors
	const std::string& getV () const {return v;}
	void setV (const std::string& v_) {v = v_;}
	const std::string& getI () const {return I;}
	void setI (const std::string& I_) {I = I_;}
	const std::string& getI2 () const {return I2;}
	void setI2 (const std::string& I2_) {I2 = I2_;}
	//Data Fields
private:
	friend class odb::access;
	std::string v;
	std::string I;
	std::string I2;
};


#pragma db object
class O
{
public:
    // Default Constructor
    O () {}        
	O (std::string v_, double value_)
	: v (v_), value (value_)
	{
	}
	//Accessors
	const std::string& getV () const {return v;}
	void setV (const std::string& v_) {v = v_;}
	const double& getValue () const {return value;}
	void setValue (const double& value_) {value = value_;}
	//Data Fields
private:
	friend class odb::access;
	std::string v;
	double value;
};


#pragma db object
class time_e
{
public:
    // Default Constructor
    time_e () {}        
	time_e (std::string v_, std::string I_, std::string J_, double level_, double lo_, double up_, double marginal_)
	: v (v_), I (I_), J (J_), level (level_), lo (lo_), up (up_), marginal (marginal_)
	{
	}
	//Accessors
	const std::string& getV () const {return v;}
	void setV (const std::string& v_) {v = v_;}
	const std::string& getI () const {return I;}
	void setI (const std::string& I_) {I = I_;}
	const std::string& getJ () const {return J;}
	void setJ (const std::string& J_) {J = J_;}
	const double& getLevel () const {return level;}
	void setLevel (const double& level_) {level = level_;}
	const double& getLo () const {return lo;}
	void setLo (const double& lo_) {lo = lo_;}
	const double& getUp () const {return up;}
	void setUp (const double& up_) {up = up_;}
	const double& getMarginal () const {return marginal;}
	void setMarginal (const double& marginal_) {marginal = marginal_;}
	//Data Fields
private:
	friend class odb::access;
	std::string v;
	std::string I;
	std::string J;
	double level;
	double lo;
	double up;
	double marginal;
};


#pragma db object
class time_w
{
public:
    // Default Constructor
    time_w () {}        
	time_w (std::string v_, std::string I_, std::string J_, double level_, double lo_, double up_, double marginal_)
	: v (v_), I (I_), J (J_), level (level_), lo (lo_), up (up_), marginal (marginal_)
	{
	}
	//Accessors
	const std::string& getV () const {return v;}
	void setV (const std::string& v_) {v = v_;}
	const std::string& getI () const {return I;}
	void setI (const std::string& I_) {I = I_;}
	const std::string& getJ () const {return J;}
	void setJ (const std::string& J_) {J = J_;}
	const double& getLevel () const {return level;}
	void setLevel (const double& level_) {level = level_;}
	const double& getLo () const {return lo;}
	void setLo (const double& lo_) {lo = lo_;}
	const double& getUp () const {return up;}
	void setUp (const double& up_) {up = up_;}
	const double& getMarginal () const {return marginal;}
	void setMarginal (const double& marginal_) {marginal = marginal_;}
	//Data Fields
private:
	friend class odb::access;
	std::string v;
	std::string I;
	std::string J;
	double level;
	double lo;
	double up;
	double marginal;
};
}}
#endif