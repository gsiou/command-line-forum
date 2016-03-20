#include "forum.h"
#include <iostream>

Forum::Forum(int id, Forum * parent) : 
id(id),parent(parent){}

Forum::~Forum(){
	std::vector<ExtendedForum *>::iterator it;
	for (it = subforums.begin(); it != subforums.end(); it++){
		delete *it;
	}
}

void Forum::createForum(int id, const std::string& name){
	subforums.push_back(new ExtendedForum(id, name,this));
}

int Forum::getID(){ return this->id; }


ExtendedForum * Forum::getForum(int id){
	for (int i = 0; i < subforums.size(); i++){
		if (subforums[i]->getID() == id){
			return (subforums[i]);
		}
	}
	return NULL;
}

Forum * Forum::getParent(){
	return this->parent;
}

int Forum::getNumberOfForums() { return subforums.size(); }

void Forum::addForum(Forum * f){
	//TODO
	//avoid casting
	//TODO
	//sort this by id for save file
	this->subforums.push_back((ExtendedForum *)f);
}

void Forum::removeForumById(int id){
	std::vector<ExtendedForum*>::iterator it;
	for (it = subforums.begin(); it != subforums.end(); it++){
		if ((*it)->getID() == id){
			subforums.erase(it);
			return;
		}
	}
}

void Forum::printContent(){
	std::cout << "Available Forums:" << std::endl;
	
	/*
	for (int i = 0; i < subforums.size(); i++){
	std::cout << " " << this->subforums[i]->getName() << " (ID:" << this->subforums[i]->getID()
		<< ")" << std::endl;
	}
	*/

	std::vector<ExtendedForum*>::iterator it;
	for(it = subforums.begin();it != subforums.end();it++){
		std::cout << " " << (*it)->getName() << " (ID:" << (*it)->getID() << ")" << std::endl;
	}
}

ExtendedForum * Forum::getForumByIndex(int index){
	for (int i = 0; i < subforums.size(); i++){
		if (i == index){
			return (subforums[i]);
		}
	}
	return NULL; //not found
}

ExtendedForum::ExtendedForum(int id, const std::string& name, Forum * parent) :
Forum(id,parent),name(name){}

ExtendedForum::~ExtendedForum(){
	std::vector<Thread*>::iterator it;
	for (it = threads.begin(); it != threads.end(); it++){
		delete *it;
	}
}

std::string ExtendedForum::getName(){ return this->name; }

void ExtendedForum::createThread(int id, bool pinned, bool locked,
	int user_id, const std::string& name){
	threads.push_back(new Thread(id, user_id, name, locked, pinned));
}

Thread * ExtendedForum::getThread(int id){
	for (int i = 0; i < this->threads.size(); i++){
		if (threads[i]->getID() == id){
			return threads[i];
		}
	}
	return NULL; /* If nothing found */
}

int ExtendedForum::getNumberOfThreads() { return threads.size(); }

Thread * ExtendedForum::getThreadByIndex(int index){
	for (int i = 0; i < threads.size(); i++){
		if (i == index){
			return (threads[i]);
		}
	}
	return NULL; //not found 
}

void ExtendedForum::setName(const std::string& newname){
	this->name = newname;
}

void ExtendedForum::printContent(){
	Forum::printContent(); //print forums

	Thread *temp;
	std::cout << "Available Threads: " << std::endl;
	for (int i = 0; i < this->getNumberOfThreads(); i++){
		temp = this->getThreadByIndex(i);
		if (temp->isPinned()){
			std::cout << "Sticky: ";
		}
		std::cout << " " << temp->getName() << " (ID:" << temp->getID()
			<< ")";
		if (temp->isLocked()){
			std::cout << " (Locked)";
		}
		std::cout << std::endl;
	}
}

void ExtendedForum::addThread(Thread *t){
	this->threads.push_back(t);
}

void ExtendedForum::removeThreadByID(int id){
	std::vector<Thread*>::iterator it;
	for (it = threads.begin(); it != threads.end(); it++){
		if ((*it)->getID() == id){
			threads.erase(it);
			return;
		}
	}
}