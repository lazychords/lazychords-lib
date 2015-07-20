#include "Signature.hpp"
#include "Random.hpp"
Signature::Signature() : number(4), step(4)
{
	if (!check())
		throw std::runtime_error("Invalid signature definition.");
}

Signature::Signature(unsigned n, unsigned s) : number(n), step(s)
{
	if (!check())
		throw std::runtime_error("Invalid signature definition.");
}

bool Signature::check() const
{
    if (number > maxNumber) return false;
    if (step > maxStep) return false;
	unsigned test = step;
	while (test > 1)
	{
		if (test % 2 != 0)
			return false;
		test /= 2;
	}
	return (number>0 && step>0);
}

unsigned Signature::id() const
{
    unsigned logDenom = log_int(2,step);
    return logDenom * maxNumber + (number-1);
}

Signature Signature::fromId(unsigned hashValue)
{
    return Signature(hashValue%maxNumber+1,
                     pow_int(2,(hashValue/maxNumber)));

}

Signature Signature::randomInstance()
{
    return fromId(Random::uniform_int(0u,maxId()));
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

UFraction Signature::measureTime() const
{
	return UFraction(number, step);
}
