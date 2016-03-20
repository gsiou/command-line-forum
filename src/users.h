#ifndef USERS_H
#define USERS_H

#include <string>
#include "forum.h"
#include "thread.h"
#include "system.h"

class System;

class User{
	//Base class, unregistered visitor
public:
	User(int id, const std::string& name, const std::string& password,int perm);

	int getID();
	std::string getUsername();
	std::string getPassword();

	/* Navigates user to forum */
	/* NULL as forum means user is at the top hierarchy (system) */
	void enterForum(Forum * f);

	/* Navigates user to thread */
	/* NULL as thread means user is not in a thread */
	void enterThread(Thread * t);

	/* Get current forum */
	/* NULL means user is at the top hierarchy (system) */
	Forum * getCurrentForum();

	/* Get current thread */
	/* NULL means not in thread */
	Thread * getCurrentThread();

	/* Returns access type of user */
	int getType();

	/* Changes user type to parameter */
	void setType(int i);

	/* Changes user name to parameter */
	void setUsername(const std::string& name);

	/* Changes user password to parameter */
	void setPassword(const std::string& name);

	/* Changes user's current forum to paremeter */
	void goToForum(Forum *f);

	/*********************
	* User forum actions *
	**********************/
	
	//Naming convention: all methods start with do as in doFooBar
	//All methods are virtual so different groups can or cannot execute them

	/* Creates a new forum and attaches it to current */
	//virtual void doCreateForum(System *s);

protected:
	Forum * currentForum;
	Thread * currentThread;
	int id;
	std::string name;
	std::string password;
	int perm; /* Permission code */
};


//class RegisteredUser : public User{
//public:
//	RegisteredUser(int id, const std::string& name, const std::string& password, int perm);
//};
//
//class GlobalModerator : public RegisteredUser{
//public:
//	GlobalModerator(int id, const std::string& name, const std::string& password, int perm);
//};
//
//class Adminstrator : public GlobalModerator{
//public:
//	Adminstrator(int id, const std::string& name, const std::string& password, int perm);
//	//void doCreateForum(System *s);
//};

#endif