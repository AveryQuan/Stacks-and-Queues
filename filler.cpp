#include <vector>
#include <cmath>
using namespace std;

/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */

/**
 * Performs a multi-point flood fill using breadth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillBFS(FillerConfig &config)
{
    return fill<Queue>(config);
}

/**
 * Performs a multi-point flood fill using depth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillDFS(FillerConfig &config)
{
    return fill<Stack>(config);

}



// bool filler::contains(vector<point> &points, point p) {
//     for (int i = 0; i < (int)points.size(); i++) {
//         point t = points[i];
//         if (t.x == p.x && t.y == p.y ) {
//             return true;
//         }
//     }
//     return false;
// }

void filler::add(FillerConfig &config, point around, colorPicker *curr, vector<vector<bool>>& marked, OrderingStructure<point> &o) {
    if (around.x >= 0 && around.x < config.img.width() && around.y >= 0 && around.y < config.img.height() ) {
        if (!marked[around.x][around.y]) { 



             if (config.img.getPixel(around.x, around.y)->dist(around.c.color) <= config.tolerance){
                *config.img.getPixel(around.x, around.y) = (*curr)(around);
                o.add(around);
             }
                
             

            marked[around.x][around.y] = true;


        }
        else {
            //cout <<" did not add " ;
        }
    }         else {
            //cout <<" out of bounds : " ;
        }

}

/**
 * Run a multi-point flood fill on an image
 *
 * @param  config     FillerConfig struct with data for flood fill of image
 * @return animation  object illustrating progression of flood fill algorithm
 */
template <template <class T> class OrderingStructure> animation filler::fill(FillerConfig &config)
{
    int runs = config.centers.size();
    point next; 
    colorPicker *curr;
    OrderingStructure<point> o;
    vector<vector<bool>> marked (config.img.width(), vector<bool>(config.img.height()));

    for (int i = 0; i < (int)config.img.width(); i++) {
        for (int c = 0; c < (int)config.img.height(); c++) {
            marked[i][c] = false;
        }
    }

    center c;
    animation ani;
    int k = 0;

     ani.addFrame(config.img);
    for (int i = 0; i < runs; i++) {
        //cout << "for" << i << endl;
        //cout << "runs" << runs << endl;
        c = config.centers[i];
        curr = config.pickers[i];

        point temp(c.x, c.y, c);
        add(config, temp, curr, marked, o);

        
        //cout << c.x << "   " << c.y << endl;

            

        while (!o.isEmpty()) 
        {
            
            next = o.remove();
            point around(next.x - 1, next.y, c);   //LEFT
            add(config, around, curr, marked, o);

//cout << around.x << "   " << around.y << endl;            
            around.y++;
            around.x++;
            add(config, around, curr, marked, o);  //DOWN

//cout << around.x << "   " << around.y << endl;
            around.y--;
            around.x++;
            add(config, around, curr, marked, o);  //RIGHT
//cout << around.x << "   " << around.y <<  endl;
            around.y--;
            around.x--;
            add(config, around, curr, marked, o);   //UP


            k++;
            if (k%config.frameFreq == 0) {
                ani.addFrame(config.img);
            }
//cout << around.x << "   " << around.y << endl;

            //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        }

        
    }
    ani.addFrame(config.img);
        return ani;
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the OS, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure.
     * Until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        1.    add its unprocessed neighbors whose color values are
     *              within (or equal to) tolerance distance from the center,
     *              to the ordering structure, and
     *              mark them as processed.
     *        2.    if it is an appropriate frame, send the current PNG to the
     *              animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! The order you should put
     *        neighboring pixels **ONTO** the queue or stack is as follows:
     *        ** LEFT(-x), DOWN(+y), RIGHT(+x), UP(-y). IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.** To reiterate, when you are exploring (filling out)
     *        from a given pixel, you must first try to fill the pixel to
     *        it's LEFT, then the one DOWN from it, then to the RIGHT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        Because we are doing a multi-point flood fill you will repeat the flood
     *        fill from several locations stored in the `centers` variable of FillerConfig
     *        struct. Initial fill points should be visited in the order they stored in the
     *        `centers` vector. If a pixel lies in the fill region of multiple start points,
     *        it should only be updated by the first fill. To repeat, pixels should only
     *        be modified the first time they are visited by the filler algorithm.
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and
     *        it will be the one we test against.
     *
     */
}
