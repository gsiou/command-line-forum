#include "post.h"

Post::Post(int id, int user_id, const std::string& content)
	: id(id), user_id(user_id), content(content){}

int Post::getID(){ return this->id; }

int Post::getUserID(){ return this->user_id; }

std::string Post::getContent(){	return this->content; }