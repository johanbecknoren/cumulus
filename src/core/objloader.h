#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__
#include <string>
#include <vector>
#include "model.h"

namespace core
{
class ObjLoader {
public:
	ObjLoader() {};
	~ObjLoader() {};
	void loadObj(std::string path, unsigned int id);
	void drawModels();
	void drawModel(int i);
	void drawModelsWireFrame();
private:
	std::vector<Model> models;
};
}

#endif
