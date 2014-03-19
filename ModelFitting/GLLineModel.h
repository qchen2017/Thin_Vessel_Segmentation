#include "stdafx.h"
#include "GLViewer.h"
#include "Data3D.h"


// For multithreading
#include <Windows.h>
#include <process.h>

#include <vector>
using namespace std;

#include "Line3D.h"

namespace GLViewer{ 
	// This class is thread safe
	// This class is thread safe
	// This class is thread safe
	// This class is thread safe
	// This class is thread safe
	class GLLineModel : public GLViewer::Object {	
		HANDLE hMutex; 
		cv::Vec3i size; 
		
		// The following two vectors have same sizes
		// A vector of the points to lines
		vector<Line3D*> lines; 
		// a vector of the colors for the lines
		vector<Vec3b> lineColors; // 3 unsigned char

		// The following two vectors have same sizes
		vector<cv::Vec3i> dataPoints; 
		vector<int> labelings;
		
	public:
		GLLineModel( cv::Vec3i size ); 

		virtual ~GLLineModel( void ); 

		virtual void render( void ); 

		void updatePoints( const vector<Vec3i>& pts ); 
		void updateModel( const vector<Line3D*>& lns, const vector<int>& lbls ); 
		// size of the object
		virtual unsigned int size_x(void) const { return size[0]; }
		virtual unsigned int size_y(void) const { return size[1]; }
		virtual unsigned int size_z(void) const { return size[2]; }

		void init(void);
	};
}