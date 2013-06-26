/*
 * main.cpp
 *
 *  Created on: 25.06.2013
 *      Author: smeat
 */


#include "GerberLoader.h"
#include "test/TestCords.h"

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

int main(){
	GerberLoader test;

	test.openFile("test.gbr");

	test.generateGeometry();/**/
}
