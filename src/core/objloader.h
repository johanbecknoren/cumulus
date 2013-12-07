#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__
#include <string>

namespace core
{
class ObjLoader {
public:
	ObjLoader() {};
	~ObjLoader() {};
	void loadObj(std::string path);
private:
	
};
}

#endif
