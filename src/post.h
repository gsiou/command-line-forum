#ifndef POST_H
#define POST_H

#include <string>

class Post{
public:
	Post(int id, int user_id, const std::string& content);

	int getID();
	int getUserID();
	std::string getContent();
private:
	int id;
	int user_id;
	std::string content;
};

#endif