#ifndef __BOUNDDOUBLE_H__
#define __BOUNDDOUBLE_H__

#include <vector>
#include <stdexcept>
//#include <iostream>

/** A Double value that has a upper and lower bound (min, max) and takes time to 
 * assume the value it was set to. The speed is given in 1/second, if the double 
 * for example is ment to represent an angle it would translate to 1rad/sec or 
 * 1deg/sec (depending on the system you use), A timer or loop has to keep track 
 * of time and call the public static bDouble::tick(int PassedTimeMS) in order to 
 * progress the double towards it's target value.
 */
class bDouble {
	static std::vector<bDouble*> instances;
	double min, max, target, value, speed;
	inline double validate(double test) {
		if (test<min||test>max) {
			throw std::range_error( "bDouble target value out of range!" );
		}
		return test;
	}
	inline void rangecheck(double low, double high) {
		if (low>high)
			throw std::range_error( "bDouble range is inverse, try swapping max/min!" );
	}
public:
	inline bDouble(double aMin, double aMax, double aTarget, double aValue, double aSpeed):min(aMin),max(aMax),target(aTarget),value(aValue),speed(aSpeed) {
		instances.push_back(this);
		//std::cout << "[bDouble] created (now " << instances.size() << ")" << std::endl;
		try {
			validate(aTarget);
			rangecheck(aMin, aMax);
		} catch (...) { throw; }
	}
	inline bDouble(double aMin, double aMax, double aInitial, double aSpeed):min(aMin),max(aMax),target(aInitial),value(aInitial),speed(aSpeed) {
		instances.push_back(this);
		//std::cout << "[bDouble] created (now " << instances.size() << ")" << std::endl;
		try {
		  validate(aInitial);
		  rangecheck(aMin, aMax);
		} catch (...) { throw; }
	}
	inline bDouble(bDouble const& original) {	
		min = original.min;
		max = original.max;
		target = original.target;
		value = original.value;
		speed = original.speed;
		instances.push_back(this);
		//std::cout << "[bDouble] created (now " << instances.size() << ")" << std::endl;
		try {
		  validate(target);
		  rangecheck(min, max);
		} catch (...) { throw; }
	}
	inline ~bDouble() {
		int i=-1;
		for (int j=0;j<instances.size();j++)
				if (instances[j] == this) {
					i=j;
				 	break;
			 	}
		if (i<0) throw std::runtime_error( "Lost bDouble instance from tracking list" );
		else instances.erase(instances.begin()+i);
		//std::cout << "[bDouble] deleted " << i << " (now " << instances.size() << ")" << std::endl;
	}

	inline operator double() const {
		return value;
	}
	/*inline operator float() const {
		return (float)value;
	}
	inline operator int() const {
		return (int)value;
	}*/
	bDouble& operator=(const bDouble&);
	template <typename T> bDouble& operator=(T);
	bDouble& operator+=(const bDouble&);
	template <typename T> bDouble& operator+=(T);
	bDouble& operator-=(const bDouble&);
	template <typename T> bDouble& operator-=(T);
	bDouble& operator*=(const bDouble&);
	template <typename T> bDouble& operator*=(T);
	bDouble& operator/=(const bDouble&);
	template <typename T> bDouble& operator/=(T);
	bDouble operator+(const bDouble&);
	template <typename T> bDouble operator+(T);
	bDouble operator-(const bDouble&);
	template <typename T> bDouble operator-(T);
	bDouble operator*(const bDouble&);
	template <typename T> bDouble operator*(T);
	bDouble operator/(const bDouble&);
	template <typename T> bDouble operator/(T);

	//template <typename T> bDouble operator+(T a, const bDouble& b);
	//template <typename T> bDouble operator-(T a, const bDouble& b);
	//template <typename T> bDouble operator*(T a, const bDouble& b);
	//template <typename T> bDouble operator/(T a, const bDouble& b);

	inline double getTarget() {
		return target;
	}
	template <typename T> inline bDouble& setSoft(T d) {
		double nt = target + static_cast<double>(d);
		if (nt<min) target = min;
		else if (nt>max) target = max;
		else target = nt;
		return *this;
	}
	inline bDouble& setMin(double aMin) {
		try { rangecheck(aMin, max); } catch (...) { throw; }
		min = aMin;
		if (target<min) target = min;
		return *this;
	}
	inline double getMin() {
		return min;
	}
	inline bDouble& setMax(double aMax) {
		try { rangecheck(min, aMax); } catch (...) { throw; }
		max = aMax;
		if (target>max) target = min;
		return *this;
	}
	inline double getMax() {
		return max;
	}
	inline bDouble& setSpeed(double aSpeed) {
		speed = aSpeed;
		return *this;
	}
	inline double getSpeed() {
		return speed;
	}
	inline bDouble& fixate() {
		target = value;
		return *this;
	}

	inline void t(int ms) {
		double dif = target-value;
		double dv = speed*(double)ms/1000.0;
		if (dif < 0) {
			if (-dif <= dv) value = target;
			else value -= dv;
		} else {
			if (dif <= dv) value = target;
			else value += dv;
		}
	}
	static void inline tick(int ms) {
		for (int i=0; i<instances.size(); i++)
				instances[i]->t(ms);
	}
	
};

#endif /*__BOUNDDOUBLE_H__*/
