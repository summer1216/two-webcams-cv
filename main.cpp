// include the librealsense C++ header file
#include <librealsense2/rs.hpp>

// include OpenCV header file
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv.h>
//using namespace std;
//using namespace cv;
//using namespace dlib;

void draw_polyline(cv::Mat &img, const dlib::full_object_detection& d,
                   const int start, const int end, bool isClosed = false,
                   const int linecolor = 0)
{
    std::vector <cv::Point> points;
    for (int i = start; i <= end; ++i)
    {
        points.push_back(cv::Point(d.part(i).x(), d.part(i).y()));
    }

    cv::Scalar drawcolor = cv::Scalar(255,0,0);
    switch (linecolor) {
    case 1:
        drawcolor = cv::Scalar(0,0,255);
        break;
    default:
        drawcolor = cv::Scalar(255,0,0);
        break;
    }
    cv::polylines(img, points, isClosed, drawcolor, 2, 16);

}

void render_face (cv::Mat &img, const dlib::full_object_detection& d)
{
    DLIB_CASSERT
            (
                d.num_parts() == 68,
                "\n\t Invalid inputs were given to this function. "
                << "\n\t d.num_parts():  " << d.num_parts()
                );

    draw_polyline(img, d, 0, 16);           // Jaw line
    draw_polyline(img, d, 17, 21);          // Left eyebrow
    draw_polyline(img, d, 22, 26);          // Right eyebrow
    draw_polyline(img, d, 27, 30);          // Nose bridge
    draw_polyline(img, d, 30, 35, true);    // Lower nose
    draw_polyline(img, d, 36, 41, true);    // Left eye
    draw_polyline(img, d, 42, 47, true);    // Right Eye
    draw_polyline(img, d, 48, 59, true, 1);    // Outer lip
    draw_polyline(img, d, 60, 67, true, 1 );    // Inner lip

}



int main()
{
    //Contruct a pipeline which abstracts the device
    rs2::pipeline pipe;

    //Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;

    //Add desired streams to configuration
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);

 //   cfg.enable_stream(RS2_STREAM_DEPTH);
    //Instruct pipeline to start streaming with the requested configuration
    pipe.start(cfg);



    // Load face detection and pose estimation models.
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor pose_model;
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;


    // Camera warmup - dropping several first frames to let auto-exposure stabilize
    while(true)
    {

        rs2::frameset frames;

        //Wait for all configured streams to produce a frame
        frames = pipe.wait_for_frames();


        //Get each frame
        rs2::frame color_frame = frames.get_color_frame();

        // Creating OpenCV Matrix from a color image
        cv::Mat color(cv::Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);


       // cv::resize(color, color, cv::Size(), 1.0/2, 1.0/2);

        //================================================================
        //================================================================
        //================================================================




        dlib::cv_image<dlib::bgr_pixel> cimg(color);

        // Detect faces
        std::vector<dlib::rectangle> faces = detector(cimg);
        // Find the pose of each face.
        std::vector<dlib::full_object_detection> shapes;
        for (unsigned long i = 0; i < faces.size(); ++i)
            shapes.push_back(pose_model(cimg, faces[i]));



        for(auto shape: shapes)
        render_face(color,shape);



        //================================================================
        //================================================================
        //================================================================


        // Display in a GUI
        cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
        cv::imshow("Display Image", color);

//        std::cout<<color.cols<<" "<<color.rows<<std::endl;


        char c=(char)cv::waitKey(25);
        if(c==27)
            break;
        //waitKey(1);

    }

    return 0;
}



