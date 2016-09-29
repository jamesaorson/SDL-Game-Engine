#include "Entity.h"
#include "Vector2D.h"

//Constructors
Entity::Entity() {
	//Initialize to origin
	position.x = DEFAULT_X;
	position.y = DEFAULT_Y;
	velocity.x = DEFAULT_X;
	velocity.y = DEFAULT_Y;
 
	ID = UNKNOWN_VALUE;
}

//Destructor
Entity::~Entity() {}

//Accessors
Vector2D Entity::getPosVector() {
	return position;
}

int Entity::getXPos() {
	return position.x;
}

int Entity::getYPos() {
	return position.y;
}

Vector2D Entity::getVelVector() {
	return velocity;
}

int Entity::getXVel() {
	return velocity.x;
}

int Entity::getYVel() {
	return velocity.y;
}

int Entity::getID() {
	return ID;
}

//Mutators
void Entity::setPosVector(int x, int y) {
	position.x = x;
	position.y = y;
}

void Entity::setXPos(int x) {
	position.x = x;
}

void Entity::setYPos(int y) {
	position.y = y;
}

//Member functions
void Entity::move() {
	position.x += velocity.x;
	position.y += velocity.y;
}