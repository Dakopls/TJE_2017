/*
	Clase para la comprovaci�n de colisiones entre entidades
	del juego.

	D�dac Serrano 2017
*/

#ifndef CHECKCOLLISIONS_H
#define CHECKCOLLISIONS_H

#include "mesh.h"

class CheckCollisions {
public:
	static CheckCollisions* checker;

	CheckCollisions();
	~CheckCollisions();

	void checkAll();
	void staticToDynamic();
	void dynamicToDynamic();
	void bulletToStatic();
	void bulletToDynamic();

	static CheckCollisions* getInstance();
};

#endif // !CHECKCOLLISIONS_H
