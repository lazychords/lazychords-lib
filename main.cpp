#include <ostream>
#include <iostream>
#include <type_traits>

#define BUG
class Test
{

};

struct delay
{
    template<typename T>
    delay(const T&);
};

std::false_type operator<<(std::ostream&, delay);

template<typename T>
struct Has
{
    static constexpr bool value = !std::is_same<decltype(std::declval<std::ostream&>() << std::declval<const T&>()), std::false_type>::value;
};

#ifdef BUG
bool deb = Has<Test>::value;
#endif

std::ostream& operator<<(std::ostream&, const Test&);

bool end = Has<Test>::value;

int main()
{
    #ifdef BUG
    std::cout<<"deb = "<<deb<<"\n";
    #endif
    std::cout<<"end = "<<end<<"\n";
}
