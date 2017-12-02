#include "BoundDouble.h"
#include <iostream> //for operator<<

std::vector<bDouble*> bDouble::instances = {};

bDouble& bDouble::operator=(const bDouble& other) {
	min = other.min;
	max = other.max;
	target = other.target;
	value = other.value;
	speed = other.speed;
	return *this;
}
bDouble& bDouble::operator+=(const bDouble& other) {
	try { this->target = validate(this->target + other.value); } catch(...) { throw; }
	return *this;
}
bDouble& bDouble::operator-=(const bDouble& other) {
	try { this->target = validate(this->target - other.value); } catch(...) { throw; }
	return *this;
}
bDouble& bDouble::operator*=(const bDouble& other) {
	try { this->target = validate(this->target * other.value); } catch(...) { throw; }
	return *this;
}

bDouble& bDouble::operator/=(const bDouble& other) {
	try { this->target = validate(this->target / other.value); } catch(...) { throw; }
	return *this;
}

bDouble bDouble::operator+(const bDouble& other) {
	try { return bDouble(this->min, this->max, validate(this->target + other.value), this->value, this->speed); } catch(...) { throw; }
}

bDouble bDouble::operator-(const bDouble& other) {
	try { return bDouble(this->min, this->max, validate(this->target - other.value), this->value, this->speed); } catch(...) { throw; }
}

bDouble bDouble::operator*(const bDouble& other) {
	try { return bDouble(this->min, this->max, validate(this->target * other.value), this->value, this->speed); } catch(...) { throw; }
}

bDouble bDouble::operator/(const bDouble& other) {
	try { return bDouble(this->min, this->max, validate(this->target / other.value), this->value, this->speed); } catch(...) { throw; }
}

/*template <typename T> bDouble bDouble::operator+(T a, const bDouble& b) {
	return bDouble(b->min, b->max, b->target + static_cast<double>(a), b->value, b->speed);
}
template <typename T> bDouble bDouble::operator-(T a, const bDouble& b) {
	return bDouble(b->min, b->max, b->target - static_cast<double>(a), b->value, b->speed);
}
template <typename T> bDouble bDouble::operator*(T a, const bDouble& b) {
	return bDouble(b->min, b->max, b->target * static_cast<double>(a), b->value, b->speed);
}
template <typename T> bDouble bDouble::operator/(T a, const bDouble& b) {
	return bDouble(b->min, b->max, b->target / static_cast<double>(a), b->value, b->speed);
}*/

std::ostream& operator<<(std::ostream& os, const bDouble& val) {
	os << static_cast<double>(val);
	return os;
}

/*///Use as test, if you want
int main() {
	bDouble val(-180.0, 180.0, 0.0, 90.0);
	val = 180;
	std::cout << "0000 Value: " << val << std::endl;
	val.tick(1000);
	std::cout << "1000 Value: " << val << std::endl;
	val.tick(500);
	std::cout << "1500 Value: " << val << std::endl;
	val.tick(1000);
	std::cout << "2500 Value: " << val << std::endl;
	val -= 180;
	std::cout << "0000 Value: " << val << std::endl;
	val.tick(2000);
	std::cout << "2000 Value: " << val << std::endl;
	val.tick(1000);
	std::cout << "3000 Value: " << val << std::endl;
	val.tick(1000);
	std::cout << "4000 Value: " << val << std::endl;
	val.tick(1000);
	std::cout << "5000 Value: " << val << std::endl;

	bDouble a(-180.0, 180.0, 0.0, 90.0);
	bDouble b(a);
	std::cout << "a " << a << ", b " << b << std::endl;
	a = a + 10;
	bDouble::tick(1000);
	std::cout << "a " << a << ", b " << b << std::endl;
	a -= 9;
	bDouble::tick(1000);
	std::cout << "a " << a << ", b " << b << std::endl;
	b = a * 6;
	bDouble::tick(1000);
	std::cout << "a " << a << ", b " << b << std::endl;
	b /= 3;
	bDouble::tick(1000);
	std::cout << "a " << a << ", b " << b << std::endl;
	a = 2 * b;
	bDouble::tick(1000);
	std::cout << "a " << a << ", b " << b << std::endl;

	std::cout << "Error Handling" << std::endl;
	try {
		a+=200;
	} catch (std::range_error& e) {
		std::cerr << e.what() << std::endl;
	}
	try {
		a = a + 200;
	} catch (std::range_error& e) {
		std::cerr << e.what() << std::endl;
	}
}
*/
