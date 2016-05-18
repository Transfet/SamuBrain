#include "VideoConverter.h"

VideoConverter::VideoConverter(size_t width, size_t height) 
								: sGWidth{width}, sGHeight{height}
{

	choose = new QPushButton;
	choose->setIcon(QIcon("chooseIcon.png"));
	choose->setIconSize(QSize(60,60));

	connect(choose, SIGNAL(clicked()),this,SLOT(ChooseSlot()));
	

	convert = new QPushButton;
	convert->setIcon(QIcon("convertIcon.png"));
	convert->setIconSize(QSize(70,70));
	//convert->setStyleSheet("background-color: transparent;");

	connect(convert, SIGNAL(clicked()), this, SLOT(ConvertSlot()));

	launchSamu = new QPushButton;
	launchSamu->setIcon(QIcon("startIcon.png"));
	launchSamu->setIconSize(QSize(70,70));
	


	connect(launchSamu, SIGNAL(clicked()), this, SLOT(accept()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(choose,0, 1);
	mainLayout->addWidget(convert, 1, 0);
	mainLayout->addWidget(launchSamu, 1, 1);
	setLayout(mainLayout);

	setStyleSheet("background-image: url(Samu.jpg)");
	choose->setStyleSheet("background-image: url(a.png)");
	setWindowTitle(tr("Samu Brain"));
	resize(800, 534);
	
}

void VideoConverter::ConvertSlot()
{
	Convert(videoName,mLattices);
	Convert(videoName2,mLattices2);
	//std::cout<<"first video size : " << mLattices.size()<<std::endl<<"second video size: "<<mLattices2.size()<<std::endl;
}

void VideoConverter::ChooseSlot()
{

	videoName = QFileDialog::getOpenFileName(this, 
	QObject::tr("Select Video"), "/home/alex/", QObject::tr("Video Files (*.avi)"));

	videoName2 = QFileDialog::getOpenFileName(this, 
		QObject::tr("Select Video"), "/home/alex/", QObject::tr("Video Files (*.avi)"));

		//vectCounter++;

}

void VideoConverter::Convert(QString videoName,latticesVect &latticesV)
{

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

	cv::Mat tmpFrame;																				// create a Mat for the frames to read in

	size_t frameCounter = 0;

	for (;;)																						// we do things for all of the frames
	{
		
		movCap >> tmpFrame;																			// get the next frame from the video stream

		if (tmpFrame.empty()) break;

		latticesV.push_back(std::vector<std::vector<bool>>(sGHeight, std::vector<bool>(sGWidth)));

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
					
					latticesV[frameCounter][i / sMHeight][j / sMWidth] = false;						// if the value is high enough we can set it to white
				}
				else
				{
					
					latticesV[frameCounter][i / sMHeight][j / sMWidth] = true;						// else we can set it to black
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
}

void VideoConverter::ConvertFrameToGS(cv::Mat& frame)
{
	cv::Mat gMat;

	cv::cvtColor(frame, gMat, CV_BGR2GRAY);

	frame = gMat;
}

void VideoConverter::setmLattices(latticesVect &vect)
{
	mLattices = vect;
}

void VideoConverter::setmLattices2(latticesVect &vect)
{
	mLattices2 = vect;
}

latticesVect VideoConverter::getmLattices()
{
	return mLattices;
}

latticesVect VideoConverter::getmLattices2()
{
	return mLattices2;
}