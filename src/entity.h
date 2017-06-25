/*
	Clase para la gestion de entidades del juego,
	Establecer relaciones entre ellas y pintarlas en pantalla.

	Marc Aranda y D�dac Serrano TJE 2017
*/


#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "framework.h"
#include "mesh.h"
#include "texture.h"
/*
	Clase para la gesti�n de todo elemento del juego que
	est� sujeto al sistema de coordenadas, ya deba ser pintada o no.

	D�dac Serrano TJE 2017
*/


#include "shader.h"
#include "camera.h"
#include "extra/coldet/coldet.h"

class Entity
{
public:
	Entity(); //constructor 
	~Entity(); //destructor

	//some attributes 
	std::string name;
	Matrix44 model;
	static unsigned idCounter;
	unsigned id;

	//local vectors
	Vector3 front;
	
	static Shader* default_shader;
	Shader* shader;

	Entity* parent; //pointer to my parent entity 
 
    //pointers to my children
    std::vector<Entity*> children;
 
    //methods
    void addChild(Entity* ent); 
    void removeChild(Entity* ent);
 
	//methods overwriten by derived classes
	virtual void render(Camera* c);
	virtual void update(float elapsed_time);

	//some useful methods...
	Matrix44 getGlobalMatrix(); //returns transform in world coordinates
	Vector3 getPosition();
	Vector3 getGlobalPosition();
	Vector3 getFront();
	Vector3 getTop();
};


class EntityMesh : public Entity
{
public:
	//Attributes of this class 
	Vector3 color;
	std::string meshName;
	std::vector<std::string> textureNames;
	bool clip;

	EntityMesh();
	~EntityMesh();

	//methods overwriten 
	void render(Camera* camera);
};


class EntityCollider : public EntityMesh
{
public:
	static std::vector<EntityCollider*> dynamicColliders;
	static std::vector<EntityCollider*> staticColliders;
	float stunned;

	EntityCollider();
	~EntityCollider();

	virtual void onBulletCollision();
	virtual void onStaticCollision();
	virtual void onDynamicCollision(EntityCollider* colliderEntity);
	void setDynamic();
	void setStatic();
};

class EntityShooter : public EntityCollider
{
public:
	float bulletTime;
	float bulletCooldown;
	int healthPoints;
	int detectionRange;
	//Se utilitza a IA pero es un atribut independent de cada entitat
	Vector3 targetPosition;

	EntityShooter();
	~EntityShooter();
	//virtual void shoot() = 0;

};

class Drone : public EntityShooter //Shooter
{
public: 
	Vector3 lastPos;
	int lastPosCd;

	Drone();
	Drone(float seconds);
	~Drone();
	
	void update(float dt);
	void shoot();
	void onBulletCollision();
	void onStaticCollision();
	void onDynamicCollision(EntityCollider* colliderEntity); 
	//Hereda de EntityCollider, perd healthPoints o el que sigui
};

class Turret : public EntityShooter //Shooter
{
public:
	Turret();
	Turret(float seconds);
	~Turret();

	void update(float dt);
	void shoot();
	void onCollision();
	void onDynamicCollision(EntityCollider* colliderEntity);
	//Hereda de EntityCollider, perd healthPoints o el que sigui
};

/*
class Detector : public EntityCollider //Collider (no dispara)
{
public:
	Detector();
	~Detector();
	void update(float dt);
	void onCollision(); //Hereda de EntityCollider, perd healthPoints o el que sigui
};
*/
#endif // !ENTITY_H
