#include <iostream>
#include <Leaf/ECS/Manager/EntityManager.hpp>

int main()
{
	using namespace Leaf;

	using T = mp::TypeList<int, float>;
	using U = mp::TypeList<int, float>;
	ecs::EntityManager<T, u, 100> entityMan_;
}
