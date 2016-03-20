#ifndef THREAD_H
#define THREAD_H

#include <string>
#include <vector>
#include "post.h"

class Thread{
public:
	Thread(int id, int user_id, const std::string& name, bool isLocked, bool isPinned);

	~Thread();

	//Adds a post to this thread
	void createPost(int id, int user_id, const std::string& content);
	
	//Returns ID of thread
	int getID();

	//Returns name of thread
	std::string getName();

	//Sets a new name
	void setName(const std::string& name);

	//True if locked
	bool isLocked();

	//True if pinned/sticky
	bool isPinned();

	//Returns number of posts
	int getNumberOfPosts();

	//Returns post with id
	//or NULL if not found
	Post * getPost(int id);

	//Returns post by index
	//or NULL if out of bounds
	Post * getPostByIndex(int index);

	//Locks thread
	void lock();

	//Unlocks thread
	void unlock();

	//Pins thread
	void pin();

	//Unpins thread
	void unpin();

	//Returns post with id
	Post * getPostByID(int id);

	//Removes post with id
	//Just removes the reference
	//Does not delete/free
	void removePostByID(int id);

	//adds a post as a child to this thread
	void addPost(Post *p);

	//returns id of user that created it
	int getCreatorID();
private:
	int id;
	int user_id;
	std::string name;
	bool locked;
	bool pinned;
	std::vector<Post*> posts;
};

#endif