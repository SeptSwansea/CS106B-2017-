/*
 * File: fractals.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {

    if(x<0||y<0||order<0||size<0){
        cout<<"exception"<<endl;
    }else{
        // get the coordinates
        if(order==0){
            //base case:do nothing
        }else{
            gw.drawLine(x,y,x+size,y);
            gw.drawLine(x,y,x+size/2,y+sqrt(3)*size/2);
            gw.drawLine(x+size/2,y+sqrt(3)*size/2,x+size,y);
            double size1=size/2;
            double x1=x+size1/2;
            double x2=x+size1;
            double y1=y+sqrt(3)*size1/2;
            order=order-1;
            //the middle triangle is created through three triangles from different starting points
            drawSierpinskiTriangle(gw,x,y,size1,order);
            drawSierpinskiTriangle(gw,x1,y1,size1,order);
            drawSierpinskiTriangle(gw,x2,y,size1,order);
        }
    }
}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void TreeHelper(GWindow& gw, double x, double y, double size, int order,int angle){
    if(order==1){
        gw.setColor("#2e8b57");

    }else{
        gw.setColor("#8b7765");
    }
    if(order==0){
        return;
    }else{
        GPoint GP1=gw.drawPolarLine(x,y,size,angle-45);
        GPoint GP2=gw.drawPolarLine(x,y,size,angle-30);
        GPoint GP3=gw.drawPolarLine(x,y,size,angle-15);
        GPoint GP4=gw.drawPolarLine(x,y,size,angle);
        GPoint GP5=gw.drawPolarLine(x,y,size,angle+15);
        GPoint GP6=gw.drawPolarLine(x,y,size,angle+30);
        GPoint GP7=gw.drawPolarLine(x,y,size,angle+45);
        size=size/2;
        order=order-1;
        TreeHelper(gw,GP1.getX(),GP1.getY(),size,order,angle-45);
        TreeHelper(gw,GP2.getX(),GP2.getY(),size,order,angle-30);
        TreeHelper(gw,GP3.getX(),GP3.getY(),size,order,angle-15);
        TreeHelper(gw,GP4.getX(),GP4.getY(),size,order,angle);
        TreeHelper(gw,GP5.getX(),GP5.getY(),size,order,angle+15);
        TreeHelper(gw,GP6.getX(),GP6.getY(),size,order,angle+30);
        TreeHelper(gw,GP7.getX(),GP7.getY(),size,order,angle+45);
    }
}


void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if(x<0||y<0||size<0||order<0){
        cout<<"exception"<<endl;
    }else{
        if(order==1){
            gw.setColor("#2e8b57");

        }else{
            gw.setColor("#8b7765");
        }
        GPoint GP=gw.drawPolarLine(x+size/2,y+size,size,90);
        x=GP.getX();
        y=GP.getY();
        size=size/2;
        order=order-1;
        TreeHelper(gw,x,y,size,order,90);
     }

}



//**
// * Draws a Mandelbrot Set in the graphical window give, with maxIterations
// * (size in GUI) and in a given color (zero for palette)
// *
// * This will be called by fractalgui.cpp.
// *
// * @param gw - The window in which to draw the Mandelbrot set.
// * @param minX - left-most column of grid
// * @param incX - increment value of columns of grid
// * @param minY - top-most row of grid
// * @param incY - increment value of rows of grid
// * @param maxIterations - The maximum number of iterations to run recursive step
// * @param color - The color of the fractal; zero if palette is to be used
// */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    // TODO: Put your Mandelbrot Set code here
    for(int r=0;r<pixels.numRows();r=r+10){
        for(int c=0;c<pixels.numCols();c=c+10){
            Complex coord = Complex(minX+c*incX, minY+r*incY);
            int numIterations=mandelbrotSetIterations(coord, maxIterations);
            pixels[r][c] = palette[numIterations % palette.size()];
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

//**
// * Runs the Mandelbrot Set recursive formula on complex number c a maximum
// * of maxIterations times.
// *
// * This will be called by you. Think about how this fits with the other two functions.
// *
// * @param c - Complex number to use for recursive formula.
// * @param maxIterations - The maximum number of iterations to run recursive step
// * @return number of iterations needed to determine if c is unbounded
// */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    // TODO: Write this function
    Complex z=Complex(0,0);
    int remainingIterations=mandelbrotSetIterations(z, c, maxIterations) ;
    int numIterations=maxIterations-remainingIterations;
    return numIterations;
}
//**
// * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
// * run for a maximum of maxIterations.
// *
// * This will be called by you. Think about how this fits with the other two functions.
// *
// * @param z - Complex number for a given number of iterations
// * @param c - Complex number to use for recursive formula.
// * @param remainingIterations - The remaining number of iterations to run recursive step
// * @return number of iterations needed to determine if c is unbounded
// */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    // TODO: write this function
    if (remainingIterations==0){
        //base case
        return 0;
    }else{
        z=z*z+c;
        if(z.abs()>4){
            remainingIterations--;
            return remainingIterations;
        }else{
            remainingIterations--;
            mandelbrotSetIterations(z,c,remainingIterations);
        }

    }
    return 0;
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;

}

