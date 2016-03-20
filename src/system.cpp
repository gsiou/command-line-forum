#include "system.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>

System::System(const std::string& name,const std::string& f_forum, const std::string& f_user,
	const std::string& f_thread, const std::string& f_post) :
	name(name),f_forum(f_forum), f_user(f_user), f_thread(f_thread), f_post(f_post)
{
	lastForumID = lastPostID = lastThreadID = lastUserID = 0;
	this->forum = new Forum(0, NULL);
}

System::~System(){
	//delete forum
	delete this->forum;

	//delete users
	std::vector<User *>::iterator it;
	for (it = users.begin(); it != users.end(); it++){
		delete *it;
	}
}

void System::load(){
	std::ifstream forumfile(this->f_forum.c_str());
	std::ifstream usersfile(this->f_user.c_str());
	std::ifstream threadsfile(this->f_thread.c_str());
	std::ifstream postsfile(this->f_post.c_str());
	std::string line;

	/* Parsing could get a bit messy */

	/* Reading forumhierarchy */
	while (std::getline(forumfile, line)){
		int depth = 0;
		int i = 0;
		int id;
		while (line[i++] != ' ') if (line[i] == '.') depth++;
		if (depth == 0){
			//Attached to system
			id = atoi(line.substr(0, line.find(' ')).c_str());
			std::string myname = line.substr(line.find(' ') + 1, line.find('\n'));
			this->forum->createForum(id, myname);
		}
		else{
			ExtendedForum *curr = (ExtendedForum *)this->forum;
			int start = 0;
			while (depth > 0){
				id = atoi(line.substr(start, line.find('.',start)).c_str());
				start += line.find('.',start) + 1;
				depth--;
				curr = (ExtendedForum *) curr->getForum(id);
			}
			id = atoi(line.substr(line.find_last_of('.') + 1, line.find(' ')).c_str());
			std::string myname = line.substr(line.find(' ') + 1, line.find('\n'));
			curr->createForum(id, myname);
		}
		if (id > lastForumID) lastForumID = id;
	}

	/* Reading users */
	while (std::getline(usersfile, line)){
		int id;
		int perms = 0;
		char myname [150]= "";
		char password [150] = "";
		sscanf(line.c_str(), "%d %s %s %d", &id,myname,password,&perms);
		this->users.push_back(new User(id, myname, password, perms));
		lastUserID = id;
	}

	/* Reading threads */
	while (std::getline(threadsfile,line)){
		std::string ids;
		char pinned, locked;
		int user_id;
		int ind = 0;
		ids = line.substr(ind, line.find(' '));
		ind += line.find(' ') + 1;
		pinned = line[ind];
		ind += 2;
		locked = line[ind];
		ind += 2;
		user_id = atoi(line.substr(ind, line.find(' ',ind)).c_str());
		ind = line.find(' ', ind) + 1;
		std::string myname = line.substr(ind, line.size());
		int depth = 0;
		int i = 0;
		while (ids[i++] != '\0') if (ids[i] == '.') depth++;
		ExtendedForum *curr = (ExtendedForum *)this->forum;
		int start = 0;
		while (depth > 0){
			int id = atoi(ids.substr(start, ids.find('.', start)).c_str());
			start += ids.find('.', start) + 1;
			depth--;
			curr = (ExtendedForum *)curr->getForum(id);
		}
		int id = atoi(ids.substr(ids.find_last_of('.') + 1, ids.find(' ')).c_str());
		bool isPinned = (pinned == 'S') ? true : false;
		bool isLocked = (locked == 'L') ? true : false;
		curr->createThread(id, isPinned, isLocked, user_id, myname);
		if(id > lastThreadID) this->lastThreadID = id;
	}

	/* Reading posts */
	while (std::getline(postsfile, line)){
		std::string ids;
		int user_id;
		std::string content;
		int ind = 0;
		ids = line.substr(ind, line.find(' '));
		ind = line.find(' ') + 1;
		user_id = atoi(line.substr(ind, line.find(' ')).c_str());
		ind = line.find(' ', ind) + 1;
		content = line.substr(ind, line.size());
		int depth = 0;
		int i = 0;
		while (ids[i++] != '\0') if (ids[i] == '.') depth++;
		ExtendedForum *curr = (ExtendedForum*) this->forum;
		int start = 0;
		while (depth > 1){
			int id = atoi(ids.substr(start, ids.find('.', start)).c_str());
			start += ids.find('.', start) + 1;
			depth--;
			curr = (ExtendedForum *)curr->getForum(id);
		}
		int thid = atoi(ids.substr(start, ids.find('.', start)).c_str());
		Thread *t = curr->getThread(thid);
		int id = atoi(ids.substr(ids.find_last_of('.') + 1, ids.find(' ')).c_str());
		t->createPost(id, user_id, content);
		if(id > lastPostID) this->lastPostID = id;
	}
}

bool isValidNumber(const std::string& s);

void System::operate(){
	if (currentUser == NULL) { return; }
	std::cout << "Welcome to " << this->name << std::endl;
	std::cout << "Logged in as :" << currentUser->getUsername() << std::endl;
	//Navigating
	currentUser->enterForum(this->forum); /* system */
	bool done = false;
	std::string option;
	int perms = currentUser->getType();
	while (!done){
		option = this->_showUserOptions();
		if (currentUser->getCurrentForum() == this->forum){
			//User inside system
			if (isValidNumber(option)){
				int goid = atoi(option.c_str());
				this->doNavigateUser(goid);
			}
			else if (option == "L"){
				//save state
				this->saveState();
				//exit
				done = true;
			}
			else if (option == "X"){
				done = true;
			}
			else if (option == "N" && perms == 3){
				this->doCreateForum();
			}
			else if (option == "U" && perms == 3){
				option = this->_showUManagementOptions();
				if (option == "C"){
					//list users
					this->doListUsers();
				}
				else if (option == "M"){
					//change type
					this->doChangeType();
				}
				else if (option == "D"){
					//delete user
					this->doDeleteUser();
				}
				else if (option == "U"){
					//change username
					this->doChangeUsername();
				}
				else if (option == "P"){
					//change password
					this->doChangePassword();
				}
				else if (option == "L"){
					this->saveState();
				}
				else if (option == "X"){
					return;
				}
				else{
					std::cout << "Wrong option." << std::endl;
				}
			}
			else{
				std::cout << "Wrong option." << std::endl;
			}
		}
		else if (currentUser->getCurrentThread() == NULL){
			//In forum but not in thread
			if (option == "F"){
				//goto forum
				this->doNavigateUser();
			}
			else if (option == "T"){
				//goto thread
				this->doVisitThread();
			}
			else if (option == "B"){
				//go up
				this->doGoUserUp();
			}
			else if (option == "H"){
				//returns current user to front page
				this->doVisitHome();
			}
			else if (option == "L"){
				//save
				this->saveState();
				//exit
			}
			else if (option == "X"){
				//exit
				return;
			}
			else if (currentUser->getType() >= 1 && option == "C"){
				//new thread
				this->doMakeThread();
			}
			else if (currentUser->getType() == 3){
				if (option == "N"){
					//create forum
					this->doCreateForum();
				}
				else if (option == "E"){
					//change title
					this->doChangeForumTitle();
				}
				else if (option == "D"){
					//delete
					this->doDeleteForum();
				}
				else if (option == "M"){
					//move forum
					this->doMoveForum();
				}
				else{
					std::cout << "Wrong option." << std::endl;
				}
			}
			else{
				std::cout << "Wrong option." << std::endl;
			}
		}
		else{
			if (option == "B"){
				this->doGoUserUp();
			}
			else if (option == "H"){
				this->doVisitHome();
			}
			else if (option == "L"){
				//save
				this->saveState();
				//exit
				return;
			}
			else if (option == "X"){
				//exit
				return;
			}
			else if (option == "R" &&currentUser->getType() >= 1){
				this->doReply();
			}
			else if (currentUser->getType() >= 2){
				if (option == "D"){
					//delete thread
					this->doDeleteThread();
				}
				else if (option == "M"){
					//move thread
					this->doMoveThread();
				}
				else if (option == "E"){
					//change thread title
					this->doChangeThreadTitle();
				}
				else if (option == "Y"){
					//toggle sticky
					this->doTogglePinned();
				}
				else if (option == "K"){
					//toggle locked
					this->doToggleLocked();
				}
				else if (option == "A"){
					//delete post
					this->doDeletePost();
				}
				else if (option == "S"){
					//move post
					this->doMovePost();
				}
				else{
					std::cout << "Wrong option." << std::endl;
				}
			}
			else{
				std::cout << "Wrong option" << std::endl;
			}
		}
	}
}

User * System::registerUser(){
	std::string username, password;
	std::cout << "User Name:" << std::endl;
	std::getline(std::cin, username);
	if (username.length() > this->MAXUSERNAMECHARS){ 
		std::cout << "Username must not exceed 50 characters." << std::endl;
		return NULL; 
	}
	for (int i = 0; i < this->users.size(); i++){
		if (this->users[i]->getUsername() == username){
			std::cout << "Username already exists." << std::endl;
		}
	}
	std::cout << "Password:" << std::endl;
	std::getline(std::cin, password);
	if (password.length() > this->MAXPASSWORDCHARS){
		std::cout << "Password must not exceed 50 characters." << std::endl;
		return NULL; 
	}
	User * u = new User(this->lastUserID + 1, username, password, 1);
	lastUserID++;
	this->users.push_back(u);
	std::cout << "User successfully registered" << std::endl;
	this->currentUser = u;
	return u;
}

User * System::loginUser(){
	bool logged = false;
	std::string username, password;
	while (!logged){
		std::cout << "Username:" << std::endl;
		std::getline(std::cin, username);
		if (username == "") { 
			this->currentUser = new User(0, "Visitor", "", 0);
			return currentUser;
		}
		for (int i = 0; i < this->users.size(); i++){
			if (this->users[i]->getUsername() == username){
				std::cout << "Password:" << std::endl;
				std::getline(std::cin, password);
				bool logged = false;
				if (password == users[i]->getPassword()){
					std::cout << "Login successful" << std::endl;
					this->currentUser = users[i];
					return users[i];
				}
				else{
					std::cout << "Wrong password." << std::endl;
					break;
				}
			}
		}
		std::cout << "Wrong username." << std::endl;
	}
}

Forum * System::getForum(){
	return forum;
}

std::string System::_showUserOptions(){
	std::cout << std::endl; //print empty for clarity
	if (currentUser->getCurrentForum() != this->forum){
		ExtendedForum *ef = (ExtendedForum *)currentUser->getCurrentForum();
		std::cout << "Current forum: " << 
			ef->getName() << std::endl;
	}
	if (currentUser->getCurrentThread() != NULL){
		std::cout << "Current thread: " <<
			currentUser->getCurrentThread()->getName() << std::endl;
	}
	if (currentUser->getCurrentThread() == NULL){
		currentUser->getCurrentForum()->printContent();
	}
	if (currentUser->getCurrentForum() == this->forum){
		std::cout << "Select:" << std::endl;
		std::cout << "ID of forum to enter" << std::endl;
		std::cout << "L to save current state and exit the system" << std::endl;
		std::cout << "X to exit system without saving" << std::endl;
		if (currentUser->getType() == 3){
			//admin
			std::cout << "N to create new forum" << std::endl;
			std::cout << "U to manage users" << std::endl;
		}
	}
	else if (currentUser->getCurrentThread() == NULL){
		std::cout << "Select:" << std::endl;
		std::cout << "F to visit a forum" << std::endl;
		std::cout << "T to visit a thread" << std::endl;
		std::cout << "B to go back" << std::endl;
		std::cout << "H to return to front page" << std::endl;
		std::cout << "L to save current state and exit the system" << std::endl;
		std::cout << "X to exit system without saving" << std::endl;
		if (currentUser->getType() >= 1){
			std::cout << "C to create a thread" << std::endl;
		}
		if (currentUser->getType() == 3){
			//admin
			std::cout << "N to create a forum" << std::endl;
			std::cout << "E to change forum title" << std::endl;
			std::cout << "D to delete forum with all content" << std::endl;
			std::cout << "M to move forum" << std::endl;
		}
	}
	else{
		//inside thread
		Thread *t = currentUser->getCurrentThread();
		Post *p;
		for (int i = 0; i < t->getNumberOfPosts(); i++){
			p = t->getPostByIndex(i);
			std::cout << this->getUser(p->getUserID())->getUsername() << " :" << p->getContent()
				<< "(ID:" << p->getID() << ")" << std::endl;
		}
		std::cout << "B to go back" << std::endl;
		std::cout << "H to return to front page" << std::endl;
		std::cout << "L to save current state and exit the system" << std::endl;
		std::cout << "X to exit system without saving" << std::endl;
		if (currentUser->getType() >= 1){
			std::cout << "R to reply" << std::endl;
		}
		if (currentUser->getType() >= 2){
			std::cout << "D to delete thread" << std::endl;
			std::cout << "M to move thread" << std::endl;
			std::cout << "E to change thread title" << std::endl;
			std::cout << "Y to toggle sticky" << std::endl;
			std::cout << "K to toggle locked" << std::endl;
			std::cout << "A to delete a message" << std::endl;
			std::cout << "S to move a message" << std::endl;
		}
	}
	std::string option;
	std::getline(std::cin, option);
	return option;
}

std::string System::_showUManagementOptions(){
	std::cout << "Select:" << std::endl;
	std::cout << "C to list all users" << std::endl;
	std::cout << "M to change group of user" << std::endl;
	std::cout << "D to delete a user" << std::endl;
	std::cout << "U to change username of a user" << std::endl;
	std::cout << "P to change password of a user" << std::endl;
	std::cout << "L to save current state and exit the system" << std::endl;
	std::cout << "X to exit system without saving" << std::endl;
	std::string option;
	std::getline(std::cin, option);
	return option;
}

User * System::getUser(int id){
	for (int i = 0; i < users.size();i++){
		if (users[i]->getID() == id){
			return users[i];
		}
	}
	return NULL; //Nothing found
}

void System::deleteUser(int id){
	User *u;
	int ind;
	for (int i = 0; i < users.size(); i++){
		if (users[i]->getID() == id){
			u = users[i];
			users.erase(users.begin() + i);
			delete u;
		}
	}
}

//local functions

//Checks if string is valid number
bool isValidNumber(const std::string& s)
{
	std::string::const_iterator it;
	for (it = s.begin(); it != s.end(); it++){
		if (!std::isdigit(*it)){
			return false;
		}
	}
	return true;
}