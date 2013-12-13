#ifndef VOLUME_H
#define VOLUME_H

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