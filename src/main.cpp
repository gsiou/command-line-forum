#include <iostream>
#include "system.h"
#include <string.h>

using namespace std;

int main(int argc,char* argv[]){
	System *s = new System("EPIC FORUMS =)","forumhierarchy.save", "users.save", "thread.save", "post.save");
	s->load();
	if(argc == 2 && strcmp(argv[1],"-R") == 0){
		s->registerUser();
	}
	else{
		s->loginUser();
	}
	s->operate();
	delete s;
}