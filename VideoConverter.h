/*@brief VideoConverter
*
* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Samu (Nahshon) has learnt the rules of Conway's Game of Life. This is 
 * an example of the paper entitled "Samu in his prenatal development".
 */


#ifndef VC_H
#define VC_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <exception>
#include <iomanip>

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QPushButton>
#include <QGridLayout>

 typedef std::vector<std::vector<std::vector<bool>>> latticesVect;

class VideoConverter : public QDialog
{
	Q_OBJECT
public:
															
																		
	VideoConverter(size_t width, size_t height);									// The constructor for the video converter 
	latticesVect mLattices;															//something like that bool ***mLattices

public slots:
	void Convert();

private:

	QString videoName;

	cv::VideoCapture movCap;														// The video capture for the movie
	
	void ConvertFrameToGS(cv::Mat& frame);

	
	size_t sGWidth, sGHeight;														// The width and height of Samu's grid in cells
	size_t movieWidth, movieHeight;													// The width and height of the actual movie in pixels
	size_t sMWidth, sMHeight;														// The width and height of Samu's cells in the movie
	
	QPushButton *convert;
	QPushButton *launchSamu;
};

#endif