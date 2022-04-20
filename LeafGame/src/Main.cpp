#include <Leaf/ECS/Manager/EntityManager.hpp>
#include <Leaf/Utility/Debug/Debug.hpp>


float test()
{
	LF_ENGINE_ASSERT(1 == 0, "Not valid bro");
	return 5.0f;
}


int main()
{
	using namespace Leaf;

	Leaf::Log::init();

	test();

	using T = mp::TypeList<int, float>;
	using U = mp::TypeList<int, float>;
}
