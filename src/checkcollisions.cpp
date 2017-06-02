#include "checkcollisions.h"
#include "entity.h"
#include "bulletmanager.h"
#include "meshmanager.h"
#include "game.h"

CheckCollisions* CheckCollisions::checker = 0;

CheckCollisions::CheckCollisions() {
	if (checker == 0) {
		checker = this;
	}
}

CheckCollisions::~CheckCollisions() {}


void CheckCollisions::checkAll() {
	staticToDynamic();
	dynamicToDynamic();
	bulletToStatic();
	bulletToDynamic();
}

void CheckCollisions::staticToDynamic(){
	BulletManager* manager = BulletManager::getInstance();
	Mesh mesh;

	for (int i = 0; i < EntityCollider::staticColliders.size(); i++) {
		EntityCollider* firstEntity = EntityCollider::staticColliders[i];
		CollisionModel3D* first_collision_model = MeshManager::getMesh((firstEntity->meshName).c_str())->collisionModel;

		first_collision_model->setTransform(firstEntity->getGlobalMatrix().m);

		for (int j = 0; j < EntityCollider::dynamicColliders.size(); j++) {

			EntityCollider* secondEntity = EntityCollider::dynamicColliders[j];
			CollisionModel3D* second_collision_model = MeshManager::getMesh((secondEntity->meshName).c_str())->collisionBox;

			first_collision_model->setTransform(firstEntity->getGlobalMatrix().m);

			if (first_collision_model->collision(second_collision_model, -1, 0, secondEntity->getGlobalMatrix().m) == false)
				continue;

			//std::cout << "Collision staticToDynamic" << std::endl;

			firstEntity->onCollision();
			secondEntity->onCollision();

		}
	}
}

void CheckCollisions::dynamicToDynamic() {
	BulletManager* manager = BulletManager::getInstance();
	Mesh mesh;

	for (int i = 0; i < EntityCollider::dynamicColliders.size(); i++) {
		EntityCollider* firstEntity = EntityCollider::dynamicColliders[i];
		CollisionModel3D* first_collision_model = MeshManager::getMesh((firstEntity->meshName).c_str())->collisionBox;

		first_collision_model->setTransform(firstEntity->getGlobalMatrix().m);
		//std::cout << actualEntity->meshName << std::endl;

		for (int j = 0; j < EntityCollider::dynamicColliders.size(); j++) {
			if (i == j) continue; //No puede ser la misma entidad

			EntityCollider* secondEntity = EntityCollider::dynamicColliders[j];
			CollisionModel3D* second_collision_model = MeshManager::getMesh((secondEntity->meshName).c_str())->collisionBox;

			first_collision_model->setTransform(firstEntity->getGlobalMatrix().m);

			if (first_collision_model->collision(second_collision_model, -1, 0, secondEntity->getGlobalMatrix().m) == false)
				continue;

			std::cout << "Collision dynamicToDynamic" << std::endl;

			firstEntity->onDynamicCollision(secondEntity);
			secondEntity->onDynamicCollision(firstEntity);

		}
	}
}

void CheckCollisions::bulletToStatic() {
	BulletManager* manager = BulletManager::getInstance();
	Mesh mesh;

	for (int i = 0; i < EntityCollider::staticColliders.size(); i++) {
		EntityCollider* actualEntity = EntityCollider::staticColliders[i];
		CollisionModel3D* collision_model = MeshManager::getMesh((actualEntity->meshName).c_str())->collisionModel;

		collision_model->setTransform(actualEntity->getGlobalMatrix().m);
		//std::cout << actualEntity->meshName << std::endl;

		for (int j = 0; j < NUMBULLETS; j++) {
			Bullet& bullet = manager->bullets[j];
			if (bullet.ttl <= 0) continue;

			Vector3 start = bullet.lastPosition;
			Vector3 front = bullet.vel;
			front.normalize();

			if (collision_model->rayCollision(start.v, front.v, true, 0.0, 10.0) == false)
				continue;

			//std::cout << "Collision bulletToStatic" << std::endl;

			bullet.ttl = 0.0001;
			actualEntity->onCollision();
			//std::cout << "Colision" << std::endl;
			
			/* //Collision test OK it works
			Vector3 collision;
			collision_model->getCollisionPoint(collision.v, false);
			
			std::string playerString = "p38";
			EntityMesh* player = new EntityMesh();
			player->name = playerString;
			player->meshName = playerString;
			player->textureName = playerString;

			Matrix44 modelPlayer;
			modelPlayer.traslate(collision.x, collision.y, collision.z);
			player->model = modelPlayer;

			actualEntity->addChild(player);
			*/
						
		}
	}
}

void CheckCollisions::bulletToDynamic() {
	BulletManager* manager = BulletManager::getInstance();
	Mesh mesh;

	for (int i = 0; i < EntityCollider::dynamicColliders.size(); i++) {
		EntityCollider* actualEntity = EntityCollider::dynamicColliders[i];
		CollisionModel3D* collision_model = MeshManager::getMesh((actualEntity->meshName).c_str())->collisionBox;

		collision_model->setTransform(actualEntity->getGlobalMatrix().m);
		//std::cout << actualEntity->meshName << std::endl;

		for (int j = 0; j < NUMBULLETS; j++) {
			Bullet& bullet = manager->bullets[j];
			if (bullet.ttl <= 0) continue;
			if (bullet.authorId == actualEntity->id) continue; //Linea diferente

			Vector3 start = bullet.lastPosition;
			Vector3 front = bullet.vel;
			front.normalize();

			if (collision_model->rayCollision(start.v, front.v, true, 0.0, 10.0) == false)
				continue;

			//std::cout << "Collision bulletToDynamic" << std::endl;

			bullet.ttl = 0.0001;
			actualEntity->onCollision();
		
		}
	}
}

CheckCollisions* CheckCollisions::getInstance() {
	if (checker == 0)
		new CheckCollisions();

	return checker;
}