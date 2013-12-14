#include "volume.h"
#include <stdexcept>

Volume::Volume(unsigned int x, unsigned int y, unsigned int z) : 
	xdim_(x),
	ydim_(y),
	zdim_(z)
{}

// Assign voxel at (x,y,z) to val
void Volume::setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z){
	if(x*y*z >= xdim_*ydim_*zdim_)
		throw std::out_of_range ("Too large index");
	else
		data_[ getIndexAt(x,y,z) ] = val;
}

float Volume::getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
	if(x*y*z >= xdim_*ydim_*zdim_)
		throw std::out_of_range ("Too large index");
	else
		return data_[ getIndexAt(x,y,z) ];
}

// Return linear index for (x,y,z)
unsigned int Volume::getIndexAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
	return x + y*xdim_ + z*xdim_*ydim_;
}