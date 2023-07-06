// calibration.h
// 2023 JUL 05
// Tershire

// DEFINITION /////////////////////////////////////////////////////////////////
#ifndef CALIBRATION_H_
#define CALIBRATION_H_


// HEADER FILE ////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"


// CLASS //////////////////////////////////////////////////////////////////////
class Calibration
{
private:

public:
	// constructor & destructor -----------------------------------------------
	Calibration::Calibration();

	// ------------------------------------------------------------------------
	Calibration::undistort(std::string file_name, cv::Mat cameraMatrix, 
	                                              cv::Mat distCoeffs);

	Calibration::save_camera_intrinsics(std::string file_name,
	                                    cv::Mat cameraMatrix, 
										cv::Mat distCoeffs)
	{
		file = cv.FileStorage(file_name, cv.FILE_STORAGE_WRITE)
		file.write('K', cameraMatrix)
		file.write('D', distCoeffs)

		file.release()
	}

    Calibration::load_camera_intrinsics(std::string file_name)
	{
		file = cv.FileStorage(file_name, cv.FILE_STORAGE_READ)

		cameraMatrix = file.getNode('K').mat()
		distCoeffs = file.getNode("D").mat()

		return cameraMatrix, distCoeffs
	}
};

#endif