/**
 * @file Log.hpp
 * @author OcachoGames-David (ocachogames@gmail.com)
 * @brief This are the classes macros for Ocacho engine.
 * 
 * This allow us to implement certains behaviours inside our classes.
 * 
 * @version 0.1
 * @date 2021-12-07 
 *  
 * @copyright Copyright (c) 2021
 */

//================================================================
//NON COPY MACROS
//================================================================

/**
 * @brief Define (class) as a non copiable class 
 */
#define NON_COPIABLE_CLASS(class)\
	public:\
		class(const class&)             = delete;\
		class(const class&&)            = delete;\
		class& operator=(const class&)  = delete;\
		class& operator=(class&&)       = delete;
