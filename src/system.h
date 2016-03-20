#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "forum.h"
#include "users.h"

class User;

class System{
public:
	System(const std::string& name,const std::string& f_forum,const std::string& f_user, 
		const std::string& f_thread, const std::string& f_post );

	~System();

	/* Loads data files and creates objects from them */
	void load();

	/* Takes keyboard input and performs current user action */
	void operate();

	/* Creates a new user. */
	/* Returns a pointer to that user */
	User * registerUser();

	/* Logs in user if username/password combination is correct. */
	/* Returns pointer to that user*/
	User * loginUser();
	
	/*****************/
	/* Forum actions */
	/*****************/

	//Creates a forum and attaches it to system
	//void createForum(int id, const std::string& name);
	
	//Creates a forum by copying another one and attaches it to system
	//void createForum(const Forum&);

	//Deletes forum with id and frees memory
	//void deleteForumById(int id);

	//Returns pointer to forum with id or null if not found
	Forum * getForum();
private:
	std::string name;
	//std::vector<Forum*> forums;
	Forum *forum;
	std::vector<User*> users;

	//Logged in user
	User * currentUser;

	/* For generating IDs */
	int lastPostID;
	int lastThreadID;
	int lastForumID;
	int lastUserID;

	/* File paths */
	std::string f_forum, f_user, f_thread, f_post;

	//Saves forum state to the files it was loaded from
	void saveState();

	//Recursive helper function for saveState()
	void visitAllForums(ExtendedForum *f, std::string prefix, std::ofstream& ofile);

	void visitAllThreads(ExtendedForum *f, std::string prefix, std::ofstream& ofile);

	void visitAllPosts(ExtendedForum *f, std::string prefix, std::ofstream& ofile);

	//Max number of characters when registering username
	static const int MAXUSERNAMECHARS = 50;

	//Max number of characters when registering password
	static const int MAXPASSWORDCHARS = 50;

	//Helper functions for printing

	//Prints user menu
	//Returns user input
	std::string _showUserOptions();

	//Prints user management menu
	//Returns user input
	std::string _showUManagementOptions();

	//Returns user with id or NULL if not found
	User * getUser(int id);

	//Deletes user with id
	//Frees up memory
	void deleteUser(int id);

	/**************************************************/
	/* Following functions are defined in actions.cpp */
	/**************************************************/

	//Creates a forum and attaches it to user's current location
	//Reads a name from the keyboard
	void doCreateForum();

	//Lists all users
	void doListUsers();

	//Changes the category of a user
	//Reads the id of the user and his new category
	void doChangeType();

	//Deletes user by id
	void doDeleteUser();

	//Changes username of user with id
	//Reads the id and new name from the keyboard
	void doChangeUsername();

	// Changes password of user with id
	//Reads the id and new pass from the keyboard
	void doChangePassword();

	// Changes user's current forum if id exists
	void doNavigateUser(int forum_id);

	// Asks for forum id and navigates user if it exists
	void doNavigateUser();

	//Asks for thread id and makes user visit it
	void doVisitThread();

	//Returns user to home page
	void doVisitHome();

	//Returns user to parent of current forum
	void doGoUserUp();

	//Creates new thread inside current user's forum.
	//Asks for a title and first message
	void doMakeThread();

	//Creates new post on current user's thread
	void doMakePost();

	//Changes current forum's title
	void doChangeForumTitle();

	//Moves current forum
	void doMoveForum();

	//Deletes current forum
	//Frees up memory TODO test this
	void doDeleteForum();

	//Replys to current thread
	void doReply();

	//Change thread title
	void doChangeThreadTitle();

	//Toggle pinned
	void doTogglePinned();

	//Toggle locked
	void doToggleLocked();

	//Delete thread
	//Frees up memory TODO
	void doDeleteThread();

	//Move thread
	//reads path in id.id.id format
	void doMoveThread();

	//Deletes post 
	//Frees memory
	void doDeletePost();

	//Move post
	//reads path in id.id.id format
	void doMovePost();
};

#endif