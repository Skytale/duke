/*
 * Memory.h
 *
 *  Created on: 16 avr. 2010
 *      Author: Guillaume Chatelet
 */

#ifndef MEMORYUTILS_H_
#define MEMORYUTILS_H_

template<typename T>
T roundDown( T value, const T step )
{
	return ( value / step ) * step;
}

template<typename T, T STEP>
T roundDown( T value )
{
	return roundDown( value, STEP );
}

template<typename T>
T roundUp( T value, const T step )
{
	return roundDown<T>( value + step - 1, step );
}

template<typename T, T STEP>
T roundUp( T value )
{
	return roundUp( value, STEP );
}

#endif /* MEMORYUTILS_H_ */