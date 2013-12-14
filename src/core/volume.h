#ifndef VOLUME_H
#define VOLUME_H

<<<<<<< HEAD
class Volume {
public:
	explicit Volume(uint x, uint y, uint z);
	~Volume() { delete[] data_; };

	float valueAtIndex(const uint &i) const;		// return value at 1-dim index i
	uint indexAt(uint i, uint j, uint k) const;	// return 1-dim index for voxel at (i,j,k)

	float valueAtWorld(float w_x, float w_y, float w_z); // return value in volume for world pos (w_x, w_y, w_z)

	void setValueAtIndex(float val, uint i); // sets voxel at index i to val
	void setValueAtIndex(float val, uint i, uint j, uint k); // sets voxel at (i,j,k) to val
private:
	uint xdim_, ydim_, zdim_;
	float *data_
};

#endif VOLUME_H
=======
//#ifndef __GL_H__
#include <GL/glew.h>
//#endif
#include <GLFW/glfw3.h>

class Volume {
public:
	//Volume() {};
	Volume(unsigned int x, unsigned int y, unsigned int z);
	~Volume() { delete[] data_; };

	void setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z); // Assign voxel at (x,y,z) to val
	void setValueAt(float val, int i);
	float getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
	
	GLfloat* getData() const { return data_; }

	float getMax();

	unsigned int xdim() { return xdim_; }
	unsigned int ydim() { return ydim_; }
	unsigned int zdim() { return zdim_; }

private:
	unsigned int getIndexAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const; // Return linear index for (x,y,z)

//private:
	unsigned int xdim_,ydim_,zdim_;
	GLfloat *data_;
};

#endif
>>>>>>> ab5f0542274da599864a3c31c03def1fad6112e4
