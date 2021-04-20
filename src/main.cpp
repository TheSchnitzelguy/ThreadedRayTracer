#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <string.h>


#include "../inc/debug.hpp"
#include "../inc/linAlg.hpp"
#include "../inc/Ray.hpp"
#include "../inc/Camera.hpp"
#include "../inc/RenderObject.hpp"
#include "../inc/Sphere.hpp"
#include "../inc/Cube.hpp"

namespace st = std;

static st::mutex outputMutex; //static mutex so that it keeps exsisting for the lifetime of the program

st::atomic<int> jobsDone{0};
st::atomic<int> nextJob{0};

int width;
int height;
int sampleAmount; //determines the amount of random samples that must be taken for a pixel
st::string fileName; //filename of file to output ppm to

int *pixelBuffer = nullptr; //buffer that contains the current pixel that the program works on


//helper function that generates a uniform random value that we'll use with the samples
float getUniformRandom()
{
    return (float)rand() / RAND_MAX;
}

//helper function that writes the contents of the buffer to a file
void writePpmFile(int *buffer, int width, int height, const char* fileName)
{
    st::ofstream outputFile;
    outputFile.open(fileName);

    outputFile << "P3\n" << width << " " << height << "\n255\n";

    //length is used to determine how big the image is going to be
    int length = width * height * 3;

    for(int i = 0; i < length; i = i+3)
    {
        //write output from buffer to stream
        outputFile << buffer[i] << " " << buffer[i+1] << " " << buffer[i+2] << "\n";

    }
    //when we are done writing contents to the file, we close the stream
    outputFile.close();
}


/*
  the color function assigns a color to visualize the ray reflections onto the sphere plane
    first, a sphere object is being made by calling hitSphere()
    then, point t is checked if it intersects with the spere
    a color to the sphere is given based on the amount of intersections
    and finally the values are normalized and returned as a Vector3
 */
Vector3 color(const Ray& ray, Vector3 center, float radius, Sphere* sph)
{

    //call hitSphere routine from sphere object 
    float t = sph->hitSphere(ray);

    //determine if t resides within the sphere or outside
    if (t > 0.0f)
    {
        //generate vector based on the intersection between the sphere and return it's normalized value
        Vector3 normal = unitVector(ray.pointAtParameter(t) - center);

        return radius * Vector3(normal.x() +1, normal.y() + 1, normal.z() + 1);

    }

    //finally generate a blue sky gradient background
    Vector3 unitDirection = unitVector(ray.direction);
    t = 0.5f * (unitDirection.y() + 1.0f);
    return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1);


}



//function that contains the actual raytracing algorithm
void rayTrace()
{
    //Declare camera and sphere objects
    Camera cam;

    Vector3 center1 = Vector3(0, 0, -1);
    float radius1 = 0.5f;
    Sphere s1(center1, radius1);

    Vector3 center2 = Vector3(0.5f, 0, -1);
    float radius2 = 0.3f;
    Sphere s2(center2, radius2);

    do
    {
        int y = nextJob++; //TODO h

        //index keeps track wat which pixelrow we are
        int index = ((height - 1) - y) * width * 3;

        for(int w = 0; w < width; ++w)
        {
            //start with black pixel
            Vector3 pixelColor(0,0,0);

            //iterate trough amount of given samples, get a pair of random weighted points on x and y-axis every cycle
            for(int s = 0; s < sampleAmount; ++s)
            {
                float u = (w + getUniformRandom()) / (float)width;
                float v = (y + getUniformRandom()) / (float)height;

                //attach these vectors to a Ray that is being projected from the camera object's perspective
                Ray ray = cam.getRay(u,v);

                //Determine the reflection intensity of each ray onto the sphere object by assigning a color value to the ray

//                pixelColor = pixelColor + color(ray, center1, radius1, &s1) + color(ray, center2, radius2, &s2); //TODO fix light overexposure due to double color() cycle
                pixelColor = pixelColor + color(ray, center1, radius1, &s1);
            }

            //get avg color value of the pixel by dividing it by the samples
            pixelColor = pixelColor / (float)sampleAmount;

            //finally normalize the colors and place them into the image buffer
            pixelBuffer[index++] = int(255.99f * pixelColor.r());
            pixelBuffer[index++] = int(255.99f * pixelColor.g());
            pixelBuffer[index++] = int(255.99f * pixelColor.b());
        }

        ++jobsDone;

        //schedule lock for 10 jobs a a time
        if(jobsDone % 10 == 0)
        {
            //accquire mutex lock for thread synchronization
            st::lock_guard<st::mutex> lock(outputMutex);
            st::cout << jobsDone << "\\" << height << "job finished" << st::endl;
        }

    } while(nextJob < height);
}



void scheduleProgram()
{
    //use all available threads on local machine
    unsigned int numberOfThreads = st::thread::hardware_concurrency(); //use all available threads on local machine

    //declare vector of threads
    st::vector<st::thread> workers;
    unsigned int threadIndex = 0;

    //assign rayTrace() to all available supported hardware threads until the entire image is rendered
    while(threadIndex < numberOfThreads && threadIndex < (unsigned int)height)
    {
        workers.emplace_back(st::thread(rayTrace));
        ++threadIndex;
    }

    st::cout << "Spawned " << numberOfThreads << " threads" << "\t" << "to do: " << height << " jobs.. " << st::flush;

    //finally join all threads to guarantee synchronous placement of each pixel row
    for(auto& worker:workers)
    {
        worker.join();
    }

    //write contents of pixelbuffer to PPM file
    char formattedFileName[fileName.size() + 1];
    fileName.copy(formattedFileName, fileName.size() + 1);
    formattedFileName[fileName.size()] = '\0';
    writePpmFile(pixelBuffer, width, height, formattedFileName);

#ifdef DEBUG
    st::cout << "Formatted argument: " << formattedFileName << st::endl;
    st::cin.get();
#endif
}

int main(int argc, char *argv[])
{
    //first, read the horizontal resolution, vertical resolution and filename from the commandline arguments
    for(int i = 0; i < argc; i++)
    {
        width = st::stoi(argv[1]);
        height = st::stoi(argv[2]);
        sampleAmount =  st::stoi(argv[3]);
        fileName = argv[4];
    }
    //append .ppm file extention to filename
    fileName = fileName + ".ppm";

#ifdef DEBUG
    st::cout << "Command line arguments given: \n\t" << st::endl;
    st::cout << "argv[1]: " << width << "\t" <<
        "argv[2]: " << height << "\n" <<
        "argv[3]: " << sampleAmount << "\t" <<
        "argv[4]: " << fileName << "\n\n\n" << st::endl; 
    st::cin.get();
#endif
    //create a pixelbuffer that holds the image whilst it's being rendered
    pixelBuffer = new int[width * height * 3];



    //start the thread scheduler
    scheduleProgram();

    st::cout << "Finished rendering! exiting program..." << st::endl;
#ifdef DEBUG
    st::cin.get();
#endif

    //when we are done creating the image file, we can delete the old pixel buffer from memory
    delete[] pixelBuffer;


    return 0;

}
