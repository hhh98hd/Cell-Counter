#include "watershedsegmenter.h"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

WatershedSegmenter::WatershedSegmenter()
{
    this->segmentNum = 0;
}

void WatershedSegmenter::loadImage(string path)
{
    this->image = imread(path);
}

int WatershedSegmenter::getSegmentNum()
{
    return this->segmentNum;
}

void WatershedSegmenter::setSegmentNum(int num)
{
    this->segmentNum = num;
}

Mat WatershedSegmenter::segment(int ksize, bool useBlur)
{
    RNG rng(12345);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    this->image = sharpenImage(this->image);
    if(useBlur)
    {
        medianBlur(this->image, this->image, ksize);
    }
    this->image = erodeImage(this->image);
    cvtColor(this->image, this->image, CV_Lab2BGR);
    Mat segments = watershedSegment(this->image, segmentNum);
    mergeSegments(this->image, segments, segmentNum);
    Mat wshed = createSegmentationDisplay(segments, segmentNum);
    Mat wshedWithImage = createSegmentationWithImageDisplay(segments, segmentNum, this->image);

    imwrite("./watershed.jpg", wshedWithImage);
    imwrite("./blur.jpg", this->image);

    this->segmentNum = 0;

    cvtColor(wshed, wshed, CV_BGR2GRAY);

    Mat sobelX, sobelY;
    Sobel(wshed, sobelX, CV_8U, 1, 0, 3);
    Sobel(wshed, sobelY, CV_8U, 0, 1, 3);
    addWeighted(sobelX, 0.5, sobelY, 0.5, 0, wshed);

    imwrite("./edge.jpg", wshed);

    findContours(wshed, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );
    Mat drawing = Mat::zeros( wshed.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        if(hierarchy[i][3] == -1
           && contourArea(contours[i]) > 10
           && !isContourConvex(contours[i]))
        {
            Moments m =  moments(contours[i], true);
            int cX = m.m10 / m.m00;
            int cY = m.m01 / m.m00;
            Scalar color = Scalar(0, 0, 255);
            drawContours(wshedWithImage, contours, i, color, 4, 8, hierarchy, 0, Point());
            this->segmentNum += 1;
            string num = to_string(this->segmentNum);
            putText(wshedWithImage, num, Point(cX, cY), FONT_HERSHEY_SIMPLEX, 2, color);
        }

    }
    wshedWithImage.convertTo(wshedWithImage, CV_8UC3);
    return wshedWithImage;

    imwrite("./contour_final.jpg", drawing);
}

Mat WatershedSegmenter::createSegmentationWithImageDisplay(Mat & segments, int numOfSegments, Mat & image)
{
    //create a new image
    Mat wshed(segments.size(), CV_8UC3);

    //Create color tab for coloring the segments
    vector<Vec3b> colorTab;
    for(int i = 0; i < numOfSegments; i++ )
    {
        int b = theRNG().uniform(0, 255);
        int g = theRNG().uniform(0, 255);
        int r = theRNG().uniform(0, 255);

        colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    //assign different color to different segments
    for(int i = 0; i < segments.rows; i++ )
    {
        for(int j = 0; j < segments.cols; j++ )
        {
            int index = segments.at<int>(i,j);
            if( index == -1 )
                wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
            else if( index <= 0 || index > numOfSegments )
                wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
            else
                wshed.at<Vec3b>(i,j) = colorTab[index - 1];
        }
    }

        //If the original image available then merge with the colors of segments
        if(image.dims > 0)
        {
            wshed = wshed * 0.4 + image * 0.6;
        }

        return wshed;
}

Mat WatershedSegmenter::erodeImage(Mat image)
{
    Mat result;
    cvtColor(image, result, CV_BGR2GRAY);
    int erosionSize = 3;
    int erosionType = MORPH_ELLIPSE;
    Mat element = getStructuringElement(erosionType,
        Size(2 * erosionType + 1, 2 * erosionSize + 1),
        Point(erosionSize, erosionSize));
    dilate(image, result, element);
    return result;
}

Mat WatershedSegmenter::createSegmentationDisplay(Mat &segments, int numOfSegments)
{
        //create a new image
        Mat wshed(segments.size(), CV_8UC3);

        //Create color tab for coloring the segments
        vector<Vec3b> colorTab;
        for(int i = 0; i < numOfSegments; i++ )
        {
            int b = theRNG().uniform(0, 255);
            int g = theRNG().uniform(0, 255);
            int r = theRNG().uniform(0, 255);

            colorTab.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
        }

        //assign different color to different segments
        for(int i = 0; i < segments.rows; i++ )
        {
            for(int j = 0; j < segments.cols; j++ )
            {
                int index = segments.at<int>(i,j);
                if( index == -1 )
                    wshed.at<Vec3b>(i,j) = Vec3b(255,255,255);
                else if( index <= 0 || index > numOfSegments )
                    wshed.at<Vec3b>(i,j) = Vec3b(0,0,0);
                else
                    wshed.at<Vec3b>(i,j) = colorTab[index - 1];
            }
        }

        return wshed;
}

void WatershedSegmenter::mergeSegments(Mat &image, Mat &segments, int &numOfSegments)
{
    //To collect pixels from each segment of the image
        vector<Mat> samples;
        //In case of multiple merging iterations, the numOfSegments should be updated
        int newNumOfSegments = numOfSegments;

        //Initialize the segment samples
        for(int i=0;i<=numOfSegments;i++)
        {
            Mat sampleImage;
            samples.push_back(sampleImage);
        }

        //collect pixels from each segments
        for(int i = 0; i < segments.rows; i++ )
        {
            for(int j = 0; j < segments.cols; j++ )
            {
                //check what segment the image pixel belongs to
                int index = segments.at<int>(i,j);
                if(index >= 0 && index<numOfSegments)
                {
                    samples[index].push_back(image(Rect(j,i,1,1)));
                }
            }
        }

        //create histograms
        vector<MatND> hist_bases;
        Mat hsv_base;
        /// Using 35 bins for hue component
        int h_bins = 35;
        /// Using 30 bins for saturation component
        int s_bins = 30;
        int histSize[] = { h_bins,s_bins };

        // hue varies from 0 to 256, saturation from 0 to 180
        float h_ranges[] = { 0, 256 };
        float s_ranges[] = { 0, 180 };

        const float* ranges[] = { h_ranges, s_ranges };

        // Use the 0-th and 1-st channels
        int channels[] = { 0,1 };

        // To store the histograms
        MatND hist_base;
        for(int c = 1;c < numOfSegments;c++)
        {
            if(samples[c].dims>0){
                //convert the sample to HSV
                cvtColor( samples[c], hsv_base, CV_BGR2HSV );
                //calculate the histogram
                calcHist( &hsv_base, 1, channels, Mat(), hist_base,2, histSize, ranges, true, false );
                //normalize the histogram
                normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );
                //append to the collection
                hist_bases.push_back(hist_base);
            }else
            {
                hist_bases.push_back(MatND());
            }

            hist_base.release();
        }

        //To store the similarity of histograms
        double similarity = 0;
        //to keep the track of already merged segments
        vector<bool> mearged;
        //initialize the merged segments tracker
        for(int k = 0; k < hist_bases.size(); k++)
        {
            mearged.push_back(false);
        }

        //calculate the similarity of the histograms of each pair of segments
        for(int c=0;c<hist_bases.size();c++)
        {
            for(int q=c+1;q<hist_bases.size();q++)
            {
                //if the segment is not merged alreay
                if(!mearged[q])
                {
                    if(hist_bases[c].dims>0 && hist_bases[q].dims>0)
                    {
                        //calculate the histogram similarity
                        similarity = compareHist(hist_bases[c],hist_bases[q],CV_COMP_BHATTACHARYYA);
                        //if similay
                        if(similarity>0.8)
                        {
                            mearged[q]=true;
                            if(q!=c)
                            {
                                //reduce number of segments
                                newNumOfSegments--;
                                for(int i = 0; i < segments.rows; i++ )
                                {
                                    for(int j = 0; j < segments.cols; j++ )
                                    {
                                        int index = segments.at<int>(i,j);
                                        //merge the segment q with c
                                        if(index==q){
                                            segments.at<int>(i,j) = c;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        numOfSegments = newNumOfSegments;
}

Mat WatershedSegmenter::sharpenImage(Mat image)
{
    Mat result, chanL, chanA, chanB, cl;
    Mat chanLAB[] = {chanL, chanA, chanB};
    cvtColor(image, image, CV_BGR2Lab);
    split(image, chanLAB);
    Ptr<CLAHE> clahe = createCLAHE(3.0, Size(8, 8));
    clahe->apply(chanL, chanL);
    merge(chanLAB, 3, result);
    return result;
}

Mat WatershedSegmenter::watershedSegment(Mat &image, int &noOfSegments)
{
    //To store the gray version of the image
        Mat gray;
        //To store the thresholded image
        Mat ret;
        //convert the image to grayscale
        cvtColor(image, gray, CV_BGR2GRAY);
        //threshold the image
        threshold(gray, ret, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);
        //get the distance transformation
        Mat distTransformed(ret.rows, ret.cols,CV_32FC1);
        distanceTransform(ret,distTransformed,CV_DIST_L2,3);
        imwrite("./dist_tf.jpg", distTransformed);
        //normalize the transformed image in order to display
        normalize(distTransformed, distTransformed, 0.0, 1, NORM_MINMAX);
        //threshold the transformed image to obtain markers for watershed
        threshold(distTransformed,distTransformed,0.1,1,CV_THRESH_BINARY);
        //Renormalize to 0-255 to further calculations
        normalize(distTransformed, distTransformed, 0.0, 255.0, NORM_MINMAX);
        imwrite("./dist_tf&thresh.jpg", distTransformed);
        distTransformed.convertTo(distTransformed,CV_8UC1);

        //calculate the contours of markers
        int compCount = 0;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        findContours(distTransformed, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

        if( contours.empty() )
            return Mat();
        Mat markers(distTransformed.size(), CV_32S);
        markers = Scalar::all(0);
        int idx = 0;
        //draw contours
        for( ; idx >= 0; idx = hierarchy[idx][0], compCount++ )
            drawContours(markers, contours, idx, Scalar::all(compCount+1), -1, 8, hierarchy, INT_MAX);

        if( compCount == 0 )
            return Mat();

        //apply watershed with the markers as seeds
        watershed(image, markers);

        //create displayable image of segments
        Mat wshed = createSegmentationDisplay(markers, compCount);

        noOfSegments = compCount;

        //returns the segments
        return markers;
}

