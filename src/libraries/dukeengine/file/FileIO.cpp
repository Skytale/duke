/*
 * FileIO.h
 *
 *  Created on: 19 avr. 2010
 *      Author: Guillaume Chatelet
 */

#include "FileIO.h"

using namespace mikrosimage::alloc;

FileIO::FileIO( Allocator* pAllocator ) : m_pAllocator( pAllocator ) {}

FileIO::~FileIO() {}

Allocator* FileIO::allocator() const
{
	return m_pAllocator;
}
