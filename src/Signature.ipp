#ifndef SIGNATURE_IPP_H_INCLUDED
#define SIGNATURE_IPP_H_INCLUDED
constexpr unsigned Signature::maxId()
{
    return (binary_log(maxStep)+1)*maxNumber;
}


#endif
