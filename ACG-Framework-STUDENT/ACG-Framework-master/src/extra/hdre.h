
#pragma once

#include <map>
#include <string>
#include <cassert>

#define N_LEVELS 6
#define N_FACES 6

typedef struct {

	char signature[4];
	float version;

	short width;
	short height;

	float maxFileSize;

	short numChannels;
	short bitsPerChannel;
	short headerSize;
	short endianEncoding;

	float maxLuminance;
	short type;

	short includesSH;
	float numCoeffs;
	float coeffs[27];

} sHDREHeader;

typedef struct {

	int width;
	int height;

	float* data;
	float** faces;

} sHDRELevel;

class HDRE {

private:

	float* data; // only f32 now
	float* pixels[N_LEVELS][N_FACES]; // Xpos, Xneg, Ypos, Yneg, Zpos, Zneg
	float* faces_array[N_LEVELS];

	sHDREHeader header;
	bool clean();

public:

	int width;
	int height;

	float version;
	short numChannels;
	short bitsPerChannel;
	float maxLuminance;

	short type;
	float numCoeffs;
	float* coeffs;

	HDRE();
	~HDRE();

	// class manager
	static std::map<std::string, HDRE*> sHDRELoaded;

	bool load(const char* filename);

	static HDRE* Get(const char* filename);
	void setName(const char* name) { sHDRELoaded[name] = this; }

	// useful methods
	float getMaxLuminance() { return this->header.maxLuminance; };
	//float* getSHCoeffs() { if (this->numCoeffs > 0) return this->header.coeffs; return nullptr; }

	float* getData(); // All pixel data
	float* getFace(int level, int face);	// Specific level and face
	float** getFaces(int level = 0);		// [[]]: Array per face with all level data

	sHDRELevel getLevel(int level = 0);
};