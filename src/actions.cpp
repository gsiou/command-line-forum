#include "system.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

/* This file contains every action related to user input */


//local function to convert int to string 
std::string getString(int i);

void System::doCreateForum(){
	std::string forumname;
	std::cout << "Name of new forum: ";
	std::getline(std::cin, forumname);
	currentUser->getCurrentForum()->createForum(this->lastForumID + 1, forumname);
	this->lastForumID++;
}

void System::doListUsers(){
	std::cout << "<id>|<name>|<group>" << std::endl;
	for (int i = 0; i < users.size(); i++){
		std::cout << users[i]->getID() << " | " << users[i]->getUsername() <<
			" | " << users[i]->getType() << std::endl;
	}
}

void System::doChangeType(){
	std::cout << "Give user ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	User * u = this->getUser(id);
	if (u != NULL){
		std::cout << "Give new group type:" << std::endl;
		std::getline(std::cin, input);
		int user_g = atoi(input.c_str());
		if (user_g >= 1 && user_g <= 3){
			u->setType(user_g);
			std::cout << "User group changed." << std::endl;
		}
		else{
			std::cout << "Can only be 1, 2 or 3" << std::endl;
		}
	}
	else{
		std::cout << "User does not exist." << std::endl;
	}
}

void System::doDeleteUser(){
	std::cout << "Give user ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	if (getUser(id) == NULL){
		std::cout << "User does not exist." << std::endl;
	}
	else{
		std::string uname = getUser(id)->getUsername();
		this->deleteUser(id);
		std::cout << "User: " << uname << " deleted." << std::endl;
	}
}

void System::doChangeUsername(){
	std::cout << "Give user ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	User *u = getUser(id);
	if (u == NULL){
		std::cout << "User does not exist." << std::endl;
	}
	else{
		std::cout << "New Username:" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		u->setUsername(input);
		std::cout << "Done." << std::endl;
	}
}

void System::doChangePassword(){
	std::cout << "Give user ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	User *u = getUser(id);
	if (u == NULL){
		std::cout << "User does not exist." << std::endl;
	}
	else{
		std::cout << "New Password:" << std::endl;
		std::string input;
		std::getline(std::cin, input);
		u->setPassword(input);
		std::cout << "Done." << std::endl;
	}
}

void System::doNavigateUser(int forum_id){
	if (currentUser->getCurrentForum()->getForum(forum_id) != NULL){
		currentUser->goToForum(currentUser->getCurrentForum()->getForum(forum_id));
	}
	else{
		std::cout << "Forum does not exist!" << std::endl;
	}
}

void System::doNavigateUser(){
	std::cout << "Forum ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	this->doNavigateUser(id);
}

void System::doVisitThread(){
	std::cout << "Thread ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	ExtendedForum *cur;
	cur = (ExtendedForum *)currentUser->getCurrentForum();
	Thread *t;
	t = cur->getThread(id);
	if (t == NULL){
		std::cout << "Thread does not exist." << std::endl;
	}
	else{
		currentUser->enterThread(t);
	}
}

void System::doVisitHome(){
	currentUser->enterForum(this->forum); //parent forum
	currentUser->enterThread(NULL);
}

void System::doGoUserUp(){
	if (currentUser->getCurrentThread() != NULL){
		currentUser->enterThread(NULL); //exit thread
	}
	else{
		currentUser->enterForum(currentUser->getCurrentForum()->getParent());
	}
}

void System::doMakeThread(){
	std::cout << "Thread title:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	ExtendedForum *ef = (ExtendedForum *)currentUser->getCurrentForum();
	ef->createThread(this->lastThreadID + 1, false, false, currentUser->getID(), input);
	std::cout << "Thread created." << std::endl;
	//enter the newly created thread
	Thread *t = ef->getThread(this->lastThreadID + 1);
	this->lastThreadID++;
	currentUser->enterThread(t);
	std::cout << "Enter first message of thread." << std::endl;
	this->doReply();
}

/*
void System::doMakePost(){
	std::cout << "Post text (one line only):" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	currentUser->getCurrentThread()->createPost(this->lastPostID + 1, currentUser->getID(), input);
	this->lastPostID++;
	std::cout << "Post created." << std::endl;
}
*/

void System::doChangeForumTitle(){
	std::cout << "New title:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	ExtendedForum *f;
	f = (ExtendedForum *)currentUser->getCurrentForum();
	f->setName(input);
	std::cout << "Title changed." << std::endl;
}

void System::doMoveForum(){
	std::cout << "New path:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	
	//saving
	Forum *f = currentUser->getCurrentForum();
	Forum *parent = f->getParent();

	//getting user to parent forum
	currentUser->enterForum(parent);

	//navigating and append
	if (input == "S"){
		this->forum->addForum(f);
	}
	else{
		//parse ids
		int depth = 0;
		int i = 0;
		while (input[i++] != '\0') if (input[i] == '.') depth++;
		ExtendedForum *curr = (ExtendedForum *)this->forum;
		int start = 0;
		while (depth >= 0){
			int id = atoi(input.substr(start, input.find('.', start)).c_str());
			start += input.find('.', start) + 1;
			depth--;
			curr = (ExtendedForum *)curr->getForum(id);
			if (curr == NULL){
				std::cout << "Wrong path." << std::endl;
				return;
			}
		}
		if (curr == f){
			std::cout << "You cant a move a forum inside itself!" << std::endl;
		}
		else{
			curr->addForum(f);
			std::cout << "Forum moved successfully." << std::endl;
		}
	}

	//remove forum from old location
	parent->removeForumById(f->getID());
}

void System::doDeleteForum(){
	ExtendedForum *f = (ExtendedForum *)currentUser->getCurrentForum();
	Forum *parent = f->getParent();
	currentUser->enterForum(parent);
	//remove the reference from parent
	parent->removeForumById(f->getID());
	//free memory and delete all subforums,
	//threads and posts using destructors
	delete f;

	std::cout << "Forum deleted." << std::endl;
}

void System::doReply(){
	if (currentUser->getCurrentThread() == NULL) return;
	if (currentUser->getCurrentThread()->isLocked()){
		std::cout << "You cant post on a locked thread." << std::endl;
		return;
	}
	std::cout << "Message text: " << std::endl;
	std::string input;
	std::getline(std::cin, input);
	Thread *t = currentUser->getCurrentThread();
	t->createPost(this->lastPostID + 1, currentUser->getID(), input);
	this->lastPostID++;
}

void System::doChangeThreadTitle(){
	std::cout << "New title:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	Thread *t;
	t = currentUser->getCurrentThread();
	t->setName(input);
	std::cout << "Title changed." << std::endl;
}

void System::doTogglePinned(){
	Thread *t = currentUser->getCurrentThread();
	if (t->isPinned()){
		t->unpin();
		std::cout << "Un-Stickied." << std::endl;
	}
	else{
		t->pin();
		std::cout << "Stickied." << std::endl;
	}
}

void System::doToggleLocked(){
	Thread *t = currentUser->getCurrentThread();
	if (t->isLocked()){
		t->unlock();
		std::cout << "Unlocked." << std::endl;
	}
	else{
		t->lock();
		std::cout << "Locked." << std::endl;
	}
}

void System::doDeleteThread(){
	Thread *t = currentUser->getCurrentThread();
	ExtendedForum *parent = (ExtendedForum *)currentUser->getCurrentForum();
	currentUser->enterThread(NULL); //exit thread
	//remove the reference from parent
	parent->removeThreadByID(t->getID());
	//free memory and delete all subforums,
	//threads and posts using destructors
	delete t;

	std::cout << "Thread deleted." << std::endl;
}

void System::doMoveThread(){
	std::cout << "New path:" << std::endl;
	std::string input;
	std::getline(std::cin, input);

	//saving
	Thread *t = currentUser->getCurrentThread();
	ExtendedForum *parent = (ExtendedForum *)currentUser->getCurrentForum();

	//getting user to parent forum
	currentUser->enterThread(NULL);

	//parse ids
	int depth = 0;
	int i = 0;
	while (input[i++] != '\0') if (input[i] == '.') depth++;
	ExtendedForum *curr = (ExtendedForum *)this->forum;
	int start = 0;
	while (depth >= 0){
		int id = atoi(input.substr(start, input.find('.', start)).c_str());
		start += input.find('.', start) + 1;
		depth--;
		curr = (ExtendedForum *)curr->getForum(id);
		if (curr == NULL){
			std::cout << "Wrong path." << std::endl;
			return;
		}
	}
	//curr->addForum(f);
	curr->addThread(t);
	std::cout << "Thread moved successfully." << std::endl;

	//remove forum from old location
	parent->removeThreadByID(t->getID());
}

void System::doDeletePost(){
	std::cout << "Post ID:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	Thread *t = currentUser->getCurrentThread();
	int id = atoi(input.c_str());
	Post *p;
	p = t->getPostByID(id);
	if (p == NULL){
		std::cout << "Post does not exist." << std::endl;
	}
	else{
		t->removePostByID(p->getID());
		delete p;
	}
}

void System::doMovePost(){
	std::cout << "Post id:" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	int id = atoi(input.c_str());
	
	Thread *t = currentUser->getCurrentThread();
	Post *p = t->getPostByID(id);
	if (p == NULL){
		std::cout << "Post does not exist!" << std::endl;
		return;
	}

	std::cout << "New path:" << std::endl;
	std::getline(std::cin, input);

	//parse ids
	int depth = 0;
	int i = 0;
	while (input[i++] != '\0') if (input[i] == '.') depth++;
	ExtendedForum *curr = (ExtendedForum *)this->forum;
	int start = 0;
	while (depth > 0){
		id = atoi(input.substr(start, input.find('.', start)).c_str());
		start += input.find('.', start) + 1;
		depth--;
		curr = (ExtendedForum *)curr->getForum(id);
		if (curr == NULL){
			std::cout << "Wrong path." << std::endl;
			return;
		}
	}
	id = atoi(input.substr(start, input.find('.', start)).c_str());
	Thread *newThread = curr->getThread(id);
	if (newThread == NULL){
		std::cout << "Thread does not exist" << std::endl;
		return;
	}
	newThread->addPost(p);
	std::cout << "Post moved successfully." << std::endl;

	//remove forum from old location
	t->removePostByID(p->getID());
}

void System::saveState(){
	std::ofstream forumfile;
	std::ofstream userfile;
	std::ofstream threadfile;
	std::ofstream postfile;

	//Erase content first
	forumfile.open(f_forum.c_str());
	userfile.open(f_user.c_str());
	threadfile.open(f_thread.c_str());
	postfile.open(f_post.c_str());
	forumfile.close();
	userfile.close();
	threadfile.close();
	postfile.close();

	//Reopening for append
	forumfile.open(f_forum.c_str(),std::fstream::app);
	userfile.open(f_user.c_str(), std::fstream::app);
	threadfile.open(f_thread.c_str(), std::fstream::app);
	postfile.open(f_post.c_str(), std::fstream::app);
	
	//First the users
	std::vector<User *>::iterator it;
	for (it = users.begin(); it != users.end(); it++){
		userfile << (*it)->getID() << " "
			<< (*it)->getUsername() << " "
			<< (*it)->getPassword() << " "
			<< (*it)->getType() << " "
			<< std::endl;
	}

	int n = this->forum->getNumberOfForums();
	for (int i = 0; i < n; i++){
		visitAllForums(this->forum->getForumByIndex(i), "", forumfile);
	}
	for (int i = 0; i < n; i++){
		visitAllThreads(this->forum->getForumByIndex(i), "", threadfile);
	}
	for (int i = 0; i < n; i++){
		visitAllPosts(this->forum->getForumByIndex(i), "", postfile);
	}
}

void System::visitAllForums(ExtendedForum *f, std::string prefix, std::ofstream& fs){
	fs << prefix << f->getID() << " " << f->getName() << std::endl;
	int n = f->getNumberOfForums();
	if (n != 0){
		ExtendedForum * temp;
		std::string sid = getString(f->getID());
		for (int i = 0; i < n; i++){
			temp = f->getForumByIndex(i);
			visitAllForums(temp, prefix + sid + ".", fs);
		}
	}
}

void System::visitAllThreads(ExtendedForum *f, std::string prefix, std::ofstream& fs){
	int threadn = f->getNumberOfThreads();
	Thread *tmp;
	for (int i = 0; i < threadn; i++){
		tmp = f->getThreadByIndex(i);
			fs << prefix << f->getID() << "." << tmp->getID() << " " 
				<< (tmp->isPinned() ? "S" : "N") << " "
				<< (tmp->isLocked() ? "L" : "N") << " "
				<< tmp->getCreatorID() << " "
				<< tmp->getName() << std::endl;
	}
	int n = f->getNumberOfForums();
	if (n != 0){
		ExtendedForum * temp;
		std::string sid = getString(f->getID());
		for (int i = 0; i < n; i++){
			temp = f->getForumByIndex(i);
			visitAllThreads(temp, prefix + sid + ".", fs);
		}
	}
}

void System::visitAllPosts(ExtendedForum *f, std::string prefix, std::ofstream& fs){
	int threadn = f->getNumberOfThreads();
	Thread *tmp;
	for (int i = 0; i < threadn; i++){
		tmp = f->getThreadByIndex(i);
		int postn = tmp->getNumberOfPosts();
		Post *p;
		for (int j = 0; j < postn; j++){
			p = tmp->getPostByIndex(j);
			fs << prefix << f->getID() << "." << tmp->getID()
				<< "." << p->getID()
				<< " " << p->getUserID()
				<< " " << p->getContent()
				<< std::endl;
		}
	}
	int n = f->getNumberOfForums();
	if (n != 0){
		ExtendedForum * temp;
		std::string sid = getString(f->getID());
		for (int i = 0; i < n; i++){
			temp = f->getForumByIndex(i);
			visitAllPosts(temp, prefix + sid + ".", fs);
		}
	}
}

//local function

std::string getString(int i){
	/*
	std::string res;
	char buffer[500];
	_itoa(i, buffer, 10);
	res = buffer;
	*/
	std::stringstream ss;
	ss << i;
	std::string res = ss.str();
	return res;
}