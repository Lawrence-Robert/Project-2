/*
Robert Lawrence
William Michael
CS 2300-002
Project 2
10/6/19

For this assignment, you will be given a file that contains target box lower left coordinate,
target box size, and information describing a line in parametric form. Using this information,
you will plot the line using character output.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;

//define number of files to read
const int NUM_LINES = 5;

//define structs
struct pt{ //point struct
    float x;
    float y;
    pt(float newX, float newY);
    pt();
};
//constructors for points
pt::pt(float newX, float newY){
    x = newX;
    y = newY;
}
pt::pt(){
    x = 0;
    y = 0;
}
struct vec{
    int x = 0;
    int y = 0;
};
struct line{
    int llx = 0; //lowerleft x
    int lly = 0; //lowerleft y
    int d = 0; //dimensions (both axes)
    pt p; //point
    vec v; //vector
};

//function prototypes
line readLineFile(string);
void printImplicit(line);
void drawGraph(line);
bool onLine(int, int, line);
float pointLineDistance(pt, line);

int main()
{
    //create array of lines
    line lines[NUM_LINES];

    //process all lines
    for (int i = 0; i < NUM_LINES; i++){
        //file name is automatically generated based on i + 1
        //then read the file and store the data in the array
        lines[i] = readLineFile(string("line") + to_string(i + 1) + string(".txt"));
        cout << "Line #" << i + 1 << endl;
        //draw a text based graph
        drawGraph(lines[i]);
        //print the implicit form of the line
        printImplicit(lines[i]);
        cout << endl;
    }

    return 0;
}

line readLineFile(string inPath){
    ifstream inFile;
    string strIn = "";
    line newLine;

    //open file in read mode
    inFile.open (inPath, ios::in);

    //get line information
    getline(inFile, strIn, ' '); //space delimiter
    newLine.llx = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.lly = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.d = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.p.x = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.p.y = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.v.x = stoi(strIn);
    getline(inFile, strIn, ' ');
    newLine.v.y = stoi(strIn);

    return newLine;
}

//Ax + By - C = 0
void printImplicit(line thisLine){
    //calculate normal
    vec normal;
    normal.x = thisLine.v.y * -1;
    normal.y = thisLine.v.x;

    //calculate C
    int c = (normal.x * thisLine.p.x) + (normal.y * thisLine.p.y);

    //print implicit
    cout << normal.x << "x ";
    if (normal.y >= 0)
        cout << "+ " << normal.y << "y ";
    else
        cout << "- " << abs(normal.y) << "y ";
    if (c < 0) //swap signs, formula is minus c (-C)
        cout << "+ " << abs(c);
    else
        cout << "- " << c;

    cout << " = 0"<< endl;
}

void drawGraph(line thisLine){
    cout << right;
    //start printing from top to bottom
    for (int y = thisLine.lly + thisLine.d; y >= thisLine.lly; y--){
        cout << setw(3) << y << " |"; //print y values
        //print x values left to right
        for (int x = thisLine.llx; x <= thisLine.llx + thisLine.d; x++){
            if (onLine(x,y,thisLine))
                cout << '*'; //print '*' if the line is anywhere within this 'pixel'
            else
                cout << ' '; //otherwise print space
        }
        cout << '|' << endl; //print right side of the frame
    }
    cout << "     " << left; //indent
    //print x axis in multiples of 5
    for (int i = 0; i <= thisLine.d; i = i + 5)
        cout << setw(5) << thisLine.llx + i;
    cout << endl;
}

bool onLine(int x, int y, line thisLine){
    const float offsets[2] = {0,.99};
    if (pointLineDistance(pt(x,y),thisLine) == 0)
        return true;
    //the sign (+ or -) of pointLineDistance is determined by the side of the line
    bool positive = pointLineDistance(pt(x + offsets[0],y + offsets[0]),thisLine) > 0;
    bool compare = false;

    //check each corner of the 'pixel' to see if it is on a different side
    //if any corner of the 'pixel' is on a different side of the line
    //then the line has to have crossed through the 'pixel'
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 2; j++){
            compare = pointLineDistance(pt(x + offsets[i],y + offsets[j]),thisLine) > 0;
            if(compare != positive)
                return true;
        }
    }

    return false;
}

float pointLineDistance(pt point, line thisLine){
    //calculate normal
    vec normal;
    normal.x = thisLine.v.y * -1;
    normal.y = thisLine.v.x;

    //calculate C
    int c = (normal.x * thisLine.p.x) + (normal.y * thisLine.p.y);

    //plug in values to get point distance
    return (float)(normal.x * point.x) + (normal.y * point.y) - c;
}
