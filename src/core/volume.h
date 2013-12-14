#ifndef VOLUME_H
#define VOLUME_H

class Volume {
public:
	//Volume() {};
	Volume(unsigned int x, unsigned int y, unsigned int z);
	~Volume() { delete[] data_; };

	void setValueAt(float val, const unsigned int &x, const unsigned int &y, const unsigned int &z); // Assign voxel at (x,y,z) to val
	float getValueAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
	
	float* getData() const { return data_; }

	float getMax();

	unsigned int xdim() { return xdim_; }
	unsigned int ydim() { return ydim_; }
	unsigned int zdim() { return zdim_; }

private:
	unsigned int getIndexAt(const unsigned int &x, const unsigned int &y, const unsigned int &z) const; // Return linear index for (x,y,z)

//private:
	unsigned int xdim_,ydim_,zdim_;
	float *data_;
};

#endif