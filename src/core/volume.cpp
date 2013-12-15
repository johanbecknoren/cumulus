#include "volume.h"
#include <stdexcept>
//#ifndef __GL_H__
#include <GL/glew.h>
//#endif
#include <GLFW/glfw3.h>
Volume::Volume(unsigned int x, unsigned int y, unsigned int z) : 
	xdim_(x),
	ydim_(y),
	zdim_(z)
{
	data_ = new GLfloat[x*y*z*3]();
}

// Assign voxel at (x,y,z) to val
void Volume::setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z){
	if(x*y*z >= 3*xdim_*ydim_*zdim_ || xdim_*ydim_*zdim_ < 0)
		throw std::out_of_range ("Too large index");
	else {
		unsigned int id = getIndexAt(x,y,z);
		data_[id] = val;
		data_[id+1] = val;
		data_[id+2] = val;
	}
}

void Volume::setValueAt(float val, int i) {
	data_[i] = val;
}

float Volume::getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
	if(x*y*z >= 3*xdim_*ydim_*zdim_)
		throw std::out_of_range ("Too large index");
	else {
		unsigned int id = getIndexAt(x,y,z);
		return data_[id];
	}
}

// Return linear index for (x,y,z)
unsigned int Volume::getIndexAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
	return x + y*xdim_ + z*xdim_*ydim_;
}

float Volume::getMax() {
	float retVal = FLT_MIN;
	for(unsigned int i=0; i<xdim_*ydim_*zdim_*3; ++i) {
		if(data_[i] > retVal)
			retVal = data_[i];
	}

	return retVal;
}