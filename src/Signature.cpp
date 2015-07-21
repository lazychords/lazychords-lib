#include "Signature.hpp"
#include "Random.hpp"
Signature::Signature() : number(4), step(4)
{
	check();
}

Signature::Signature(unsigned n, unsigned s) : number(n), step(s)
{
	check();
}

void Signature::check() const
{
    ASSERT (number <= maxNumber) ;
    ASSERT (step <= maxStep);
	unsigned test = step;
	while (test > 1)
	{
		ASSERT(test % 2 == 0)
		test /= 2;
	}
	ASSERT(number>0 && step>0);
}

unsigned Signature::id() const
{
    unsigned logDenom = logUnsigned(2,step);
    return logDenom * maxNumber + (number-1);
}

Signature Signature::fromId(unsigned hashValue)
{
    return Signature(hashValue%maxNumber+1,
                     powUnsigned(2,(hashValue/maxNumber)));

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

unsigned Signature::maxId()
{
    return (logUnsigned(2,maxStep)+1)*maxNumber;
}
