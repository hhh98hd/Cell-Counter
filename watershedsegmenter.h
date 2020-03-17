#include <opencv2/opencv.hpp>

using namespace cv;

#ifndef WATERSHEDSEGMENTER_H
#define WATERSHEDSEGMENTER_H

class WatershedSegmenter
{
private:
    int segmentNum = 0;
    Mat image;
public:
    WatershedSegmenter();

    void loadImage(string path);

    int getSegmentNum();

    void setSegmentNum(int num);

    Mat segment(int ksize, bool useBlur);

    void mergeSegments(Mat & image,Mat & segments, int & numOfSegments);

    Mat sharpenImage(Mat image);

    Mat watershedSegment(Mat & image, int & noOfSegments);

    Mat createSegmentationDisplay(Mat & segments, int numOfSegments);

    Mat createSegmentationWithImageDisplay(Mat & segments, int numOfSegments, Mat & image);

    Mat erodeImage(Mat image);
};

#endif // WATERSHEDSEGMENTER_H
