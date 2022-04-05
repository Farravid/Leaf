/**
 * @file ogpch.hpp
 * @author David (davidmg103@gmail.com)
 * @brief This is the precompiled header used by the core engine.
 * 
 * This includes all the libraries from std needed.
 * 
 * @version 0.1
 * @date 2021-12-08
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//================================================================
//C++ libraries
//================================================================
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <algorithm>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <sstream> 

#include <cstdint>
#include <cstring>
#include <string>
#include <string.h>
#include <string_view>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <typeindex>
#include <vector>
#include <list>
#include <array>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>

#include <cmath>
#include <cfenv>
#include <limits>
#include <tuple>
#include <variant>
#include <type_traits>
#include <stdexcept>


#ifdef OG_WINDOWS
#else
	#include <signal.h>
#endif