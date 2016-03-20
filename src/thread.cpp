#include "thread.h"

Thread::Thread(int id, int user_id, const std::string& name, bool isLocked, bool isPinned)
	: id(id), user_id(user_id), name(name),  locked(isLocked), pinned(isPinned) {}

Thread::~Thread(){
	std::vector<Post *>::iterator it;
	for (it = posts.begin(); it != posts.end(); it++){
		delete *it;
	}
}

int Thread::getID(){ return this->id; }

void Thread::createPost(int id, int user_id, const std::string& content){
	this->posts.push_back(new Post(id, user_id, content));
}

std::string Thread::getName(){
	return this->name;
}

bool Thread::isLocked(){
	return this->locked;
}

bool Thread::isPinned(){
	return this->pinned;
}

int Thread::getNumberOfPosts(){
	return this->posts.size();
}

Post * Thread::getPost(int id){
	std::vector<Post*>::iterator i;
	for (i = posts.begin(); i != posts.end(); i++){
		if ((*i)->getID() == id){
			return *i;
		}
	}
	return NULL;
}

Post * Thread::getPostByIndex(int index){
	std::vector<Post*>::iterator i;
	int ind = 0;
	for (i = posts.begin(); i != posts.end(); i++,ind++){
		if (ind == index){
			return *i;
		}
	}
	return NULL;
}

void Thread::setName(const std::string& newname){
	this->name = newname;
}

void Thread::lock(){
	this->locked = true;
}

void Thread::unlock(){
	this->locked = false;
}

void Thread::pin(){
	this->pinned = true;
}

void Thread::unpin(){
	this->pinned = false;
}

Post * Thread::getPostByID(int id){
	std::vector<Post *>::iterator it;
	for (it = posts.begin(); it != posts.end(); it++){
		if ((*it)->getID() == id){
			return *it;
		}
	}
	return NULL; //not found
}

void Thread::removePostByID(int id){
	std::vector<Post *>::iterator it;
	for (it = posts.begin(); it != posts.end(); it++){
		if ((*it)->getID() == id){
			posts.erase(it);
			return;
		}
	}
}

void Thread::addPost(Post *p){
	this->posts.push_back(p);
}

int Thread::getCreatorID(){
	return this->user_id;
}