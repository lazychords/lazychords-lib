#include "Signature.hpp"

Signature::Signature()
{
	number = 4;
	step = 4;
	if (!check())
		throw std::runtime_error("Invalid signature definition.");
}

Signature::Signature(unsigned number, unsigned step)
{
	this->number = number;
	this->step = step;
	if (!check())
		throw std::runtime_error("Invalid signature definition.");
}

bool Signature::check() const
{
	unsigned test = step;
	while (test > 1)
	{
		if (test % 2 != 0)
			return false;
		test /= 2;
	}
	return (number>0 && step>0);
}

bool Signature::operator==(const Signature& other) const
{
	return other.number == number && other.step == step;
}

bool Signature::operator!=(const Signature& other) const
{
	return !(*this==other);
}

unsigned Signature::getNumber() const
{
	return number;
}

unsigned Signature::getStep() const
{
	return step;
}

Fraction Signature::measureTime() const
{	
	return (Fraction)number/(Fraction)step;
}
