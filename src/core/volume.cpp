#include "volume.h"
#include <stdexcept>

Volume::Volume(unsigned int x, unsigned int y, unsigned int z) : 
	xdim_(x),
	ydim_(y),
	zdim_(z)
{
	data_ = new float[x*y*z]();
}

// Assign voxel at (x,y,z) to val
void Volume::setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z){
	if(x*y*z >= xdim_*ydim_*zdim_ || xdim_*ydim_*zdim_ < 0)
		throw std::out_of_range ("Too large index");
	else {
		unsigned int id = getIndexAt(x,y,z);
		data_[id] = val;
	}
}

float Volume::getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
	if(x*y*z >= xdim_*ydim_*zdim_)
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