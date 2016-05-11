#include "VideoConverter.h"

VideoConverter::VideoConverter(size_t width, size_t height) 
								: sGWidth{width}, sGHeight{height}
{
	videoName = QFileDialog::getOpenFileName(this, 
		QObject::tr("Select Video"), "/home/alex/", QObject::tr("Video Files (*.avi)"));

	movCap.open(videoName.toUtf8().constData());

	
	if ( !movCap.isOpened() )																	// check if the stream is opened
	{
		std::cerr << "Can't open video file!" << std::endl;
		throw std::exception();
	}

	movieWidth = movCap.get(CV_CAP_PROP_FRAME_WIDTH);
	movieHeight = movCap.get(CV_CAP_PROP_FRAME_HEIGHT);

	sMWidth = movieWidth / sGWidth;																		
	sMHeight = movieHeight / sGHeight;

	convert = new QPushButton(tr("Convert"));

	connect(convert, SIGNAL(clicked()), this, SLOT(Convert()));

	launchSamu = new QPushButton(tr("Launch Samu"));

	connect(launchSamu, SIGNAL(clicked()), this, SLOT(accept()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(convert, 1, 0);
	mainLayout->addWidget(launchSamu, 1, 1);
	setLayout(mainLayout);

	setWindowTitle(tr("Samu Movie"));
	resize(600, 300);
	
}

void VideoConverter::Convert()
{
	
	cv::Mat tmpFrame;																				// create a Mat for the frames to read in

	latticesVect tmpFrames;

	size_t frameCounter = 0;

	
	
	for (;;)																						// we do things for all of the frames
	{
		
		movCap >> tmpFrame;																			// get the next frame from the video stream

		if (tmpFrame.empty()) break;

		tmpFrames.push_back(std::vector<std::vector<bool>>(sGHeight, std::vector<bool>(sGWidth)));

		ConvertFrameToGS(tmpFrame);																	// convert frame to grayscale
		
		for (auto i = 0; i < tmpFrame.rows; i += sMHeight)
		{
			for (auto j = 0; j < tmpFrame.cols; j += sMWidth)
			{	

				cv::Mat mask = cv::Mat::zeros(tmpFrame.rows, tmpFrame.cols, CV_8U);

				cv::Rect maskRegion = cv::Rect(i, j, sMWidth, sMHeight);

				mask(maskRegion) = 1;															// fill the ROI with 1's in the mask
				
				if ((cv::mean(tmpFrame, mask))[0] > 127)
				{
					
					tmpFrames[frameCounter][i / sMHeight][j / sMWidth] = false;						// if the value is high enough we can set it to white
				}
				else
				{
					
					tmpFrames[frameCounter][i / sMHeight][j / sMWidth] = true;						// else we can set it to black
				}
				
			}
		}

		std::cout << "Converting frame:\t[" << std::setfill('0') 
				  << std::setw(3) << frameCounter << "]" << std::endl;


		frameCounter++;
		
		//std::cerr << tmpFrame.type() << std::endl;

		//cv::imshow("w1", tmpFrame);
		//if (cv::waitKey(25) == 27) break;
	}

	// move the temporary vector to the member variable
	mLattices = std::move(tmpFrames);
}

void VideoConverter::ConvertFrameToGS(cv::Mat& frame)
{
	cv::Mat gMat;

	cv::cvtColor(frame, gMat, CV_BGR2GRAY);

	frame = gMat;
}