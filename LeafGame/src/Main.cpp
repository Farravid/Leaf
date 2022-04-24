#include <Leaf/ECS/Manager/EntityManager.hpp>
#include <Leaf/Utility/Debug/Debug.hpp>
#include <Leaf/Core/Concepts/Concepts.hpp>
#include <Leaf/Core/Concepts/TypeTraits.hpp>

GEN_TRAIT_CLASS_HAS_MEMBER(distance)

template<typename T>
requires has_member_distance_v<T, float>
struct A
{
};

struct B
{
	float distance { };
};

float test()
{
	LF_ENGINE_ASSERT(1 == 0, "Not valid bro");
	return 5.0f;
}


void seefunction(auto) { std::cout << __PRETTY_FUNCTION__ "\n"; }

int main()
{
	using namespace Leaf;

	Leaf::Log::init();

;	A<B> obj{ };
	
	//using T = decltype(has_member_distance_v<B, float>);
	//seefunction(T{});

	//static_assert(has_member_distance_v<B, float>);

	//test();

	//using T = mp::TypeList<int, float>;
	//using U = mp::TypeList<int, float>;
}
