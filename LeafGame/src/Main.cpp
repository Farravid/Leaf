#include <iostream>
#include <Leaf/ECS/Manager/EntityManager.hpp>
#include <Leaf/Utility/Log/Log.hpp>

int main()
{
	
	using namespace Leaf;

	Leaf::Log::init();

	LF_ENGINE_LOG_WARN("Test de como va esto {0} ", 5);

	using T = mp::TypeList<int, float>;
	using U = mp::TypeList<int, float>;
}
