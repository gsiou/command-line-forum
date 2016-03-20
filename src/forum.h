#ifndef FORUM_H
#define FORUM_H

#include <string>
#include <vector>
#include "thread.h"

class ExtendedForum;

class Forum{
public:
	Forum(int id,Forum * parent);

	~Forum();

	int getID();

	//Returns pointer to forum with id or null if not found
	ExtendedForum * getForum(int id);

	//Creates forum and attaches it to this one
	void createForum(int id, const std::string& name);

	//Returns parent forum of NULL if system
	Forum * getParent();

	//Returns number of subforums
	int getNumberOfForums();

	//Returns forum in index given or NULL if out of bounds
	ExtendedForum * getForumByIndex(int i);

	//Adds a forum as subforum of this one
	void addForum(Forum * f);

	//Remove forum by id
	//Only removes the reference
	//Does not delete/free memory
	void removeForumById(int id);

	//Prints forums
	virtual void printContent();
protected:
	int id;
	//std::string name;
	std::vector<ExtendedForum*> subforums;
	//std::vector<Thread*> threads;
	Forum * parent;
};

class ExtendedForum : public Forum{
	//contains threads
	//has a name
public:
	ExtendedForum(int id, const std::string& name, Forum* parent);

	~ExtendedForum();

	//Returns forum's name
	std::string getName();

	//Sets the forum's name 
	void setName(const std::string& newname);

	//Returns number of thread
	int getNumberOfThreads();

	//Returns forum in index given or NULL if out of bounds
	Thread * getThreadByIndex(int i);

	//Creates a thread and attaches it to this one
	void createThread(int id, bool pinned, bool locked, int user_id, const std::string& name);

	//Returns thread with ID or NULL
	Thread * getThread(int id);

	//Prints forums and threads
	void printContent();

	//Adds a thread as child of this one
	void addThread(Thread *t);

	//removes thread with id
	//Only removes the reference
	//Does not delete/free memory
	void removeThreadByID(int id);
private:
	std::string name;
	std::vector<Thread*> threads;
};

#endif