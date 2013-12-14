#ifndef VOLUME_H
#define VOLUME_H

class Volume {
public:
	explicit Volume(unsigned int x, unsigned int y, unsigned int z);
	~Volume() { delete[] data_; };

	void setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z); // Assign voxel at (x,y,z) to val
	float getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
private:
	unsigned int getIndexAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const; // Return linear index for (x,y,z)

private:
	unsigned int xdim_,ydim_,zdim_;
	float *data_;
};

#endif