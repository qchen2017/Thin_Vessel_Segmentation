#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>

#include <opencv2/core/core.hpp>

// For computing vesselness
#include "VesselnessTypes.h"
#include "VesselDetector.h"

// For visualization
#include "GLViewerVesselness.h"

// Linux header: provide system call functions such as sleep()
#include <unistd.h>


#define INPUT_DIR "../data/"
#define OUTPUT_DIR "../temp/"


using namespace std;
using namespace cv;

GLViewerVesselness viewer;

namespace sample_code
{
// Compute vesselness measure
int vesselness( bool isDisplay, std::string dataname = "data15", short threshold = 2900 );

// Extract Vessel centrelines with non-maximum suppression
int centreline( bool isDisplay, std::string dataname = "data15" );
}

int main(void)
{
    Mat temp = Mat(200, 200, CV_8UC3);
    // cv::imshow( "", temp );

    Image3D<short> im_data;
    im_data.load( "../temp/vessel3d_rd_sp.data" );
    im_data.setROI( );
    im_data.saveROI( "../temp/roi16.data" );

    viewer.addObject( im_data.getROI(),  GLViewer::Volumn::MIP );
    viewer.display(600, 400, 1);

    return 0;


    sample_code::vesselness(false, "data15", 2800 );
    sample_code::centreline(true,  "data15");
    return 0;
}

int sample_code::vesselness( bool isDisplay, string dataname, short threshold )
{
    // create output folders if it does not exist
    // CreateDirectory(L"../temp", NULL);

    // Sigma: Parameters for Vesselness
    // [sigma_from, sigma_to]: the potential size rang of the vessels
    // sigma_step: precision of computation
    float sigma_from = 1.0f;
    float sigma_to   = 8.10f;
    float sigma_step = 0.3f;
    // Parameters for vesselness, please refer to Frangi's papaer
    // or this [blog](http://yzhong.co/?p=351)
    float alpha = 1.0e-1f;
    float beta  = 5.0e0f;
    float gamma = 3.5e5f;

    // laoding data
    Image3D<short> im_short_orig;
    bool flag = im_short_orig.load( INPUT_DIR + dataname + ".data" );
    if( !flag ) return 0;

    Data3D<short> im_short;
    IP::threshold( im_short_orig, im_short, threshold ); // [2500, 4500]

    // Compute Vesselness
    Data3D<Vesselness_Sig> vn_sig;
    VesselDetector::compute_vesselness( im_short, vn_sig,
                                        sigma_from, sigma_to, sigma_step,
                                        alpha, beta, gamma );
    vn_sig.save( OUTPUT_DIR + dataname + ".vn_sig" );

    // If you want to visulize the data using Maximum-Intensity Projection
    if( isDisplay )
    {
        viewer.addObject( im_short_orig,  GLViewer::Volumn::MIP );
        viewer.addObject( im_short,  GLViewer::Volumn::MIP );
        viewer.addObject( vn_sig,  GLViewer::Volumn::MIP );
        viewer.addDiretionObject( vn_sig );
        viewer.display(600, 400, 4);
    }

    return 0;
}

int sample_code::centreline( bool isDisplay, string dataname )
{
    // load vesselness data
    Data3D<Vesselness_Sig> vn_sig( OUTPUT_DIR + dataname + ".vn_sig" );

    // non-maximum suppression
    Data3D<Vesselness_Sig> vn_sig_nms;
    IP::non_max_suppress( vn_sig, vn_sig_nms );
    vn_sig_nms.save( OUTPUT_DIR + dataname + ".nms.vn_sig" );

    // edge tracing
    Data3D<Vesselness_Sig> vn_sig_et;
    IP::edge_tracing( vn_sig_nms, vn_sig_et, 0.45f, 0.05f );
    vn_sig_et.save( OUTPUT_DIR + dataname + ".et.vn_sig" );

    if( isDisplay )
    {
        viewer.addObject( vn_sig_et,  GLViewer::Volumn::MIP );
        viewer.addDiretionObject( vn_sig_et );
        viewer.display(600, 400, 2);
    }
    return 0;
}



