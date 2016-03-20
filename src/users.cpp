#include "users.h"
#include <iostream>


/* Constructors for User and all child classes */
User::User(int id, const std::string& name, const std::string& password, int perm) :
id(id), name(name),password(password), perm(perm),currentForum(NULL),currentThread(NULL){}

//RegisteredUser::RegisteredUser(int id, const std::string& name, const std::string& password, int perm) :
//User(id, name, password, perm){}
//
//GlobalModerator::GlobalModerator(int id, const std::string& name, const std::string& password, int perm) :
//RegisteredUser(id, name, password, perm){}
//
//Adminstrator::Adminstrator(int id, const std::string& name, const std::string& password, int perm) :
//GlobalModerator(id, name, password, perm){}

/*--------------*/
/* User methods */
/*--------------*/

int User::getID(){
	return this->id;
}

int User::getType(){
	return this->perm;
}

std::string User::getUsername(){
	return this->name;
}

std::string User::getPassword(){
	return this->password;
}

void User::enterForum(Forum * f){
	this->currentForum = f;
}

void User::enterThread(Thread * t){
	this->currentThread = t;
}

Forum * User::getCurrentForum(){
	return this->currentForum;
}

Thread * User::getCurrentThread(){
	return this->currentThread;
}

void User::setType(int i){
	this->perm = i;
}

void User::setUsername(const std::string& name){
	this->name = name;
}

void User::setPassword(const std::string& password){
	this->password = password;
}

void User::goToForum(Forum *f){
	this->currentForum = f;
}