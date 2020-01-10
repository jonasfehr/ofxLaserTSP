//
//  OptimiserTSP.h
//  example
//
//  Created by Jonas Fehr on 08/01/2020.
//

#ifndef OptimiserTSP_h
#define OptimiserTSP_h
#include "ofxLaserTSP.h"
#include "ContentBase.hpp"

class OptimiserTSP : public ofThread{
public:
    

    ContentBase optimisedContentBase;
    ContentBase contentBase;

    ofThreadChannel<ContentBase> toOptimise;
    ofThreadChannel<ContentBase> optimised;
    
    bool newFrame;
    
    
    OptimiserTSP()
    :newFrame(true){
        startThread();
        setThreadName("OptimiserTSP");
    }

    ~OptimiserTSP(){
        toOptimise.close();
        optimised.close();
        waitForThread(true);
    }

    void optimise(ContentBase & contentBase){
        toOptimise.send(contentBase);
    }

    void update(){
        newFrame = false;

        while(optimised.tryReceive(this->contentBase)){
            newFrame = true;
        }

    }
    
    void forceStartThread(){
        if(!isThreadRunning()) startThread();
    }

    bool isFrameNew(){
        return newFrame;
    }

    ContentBase & getContentBase(){
        return contentBase;
    }

    
    void threadedFunction(){
        // wait until there's a new frame
        // this blocks the thread, so it doesn't use
        // the CPU at all, until a frame arrives.
        // also receive doesn't allocate or make any copies
        ContentBase cb;
        while(toOptimise.receive(cb)){
            optimize(cb.getPointGroupsRef(),optimisedContentBase.getPointGroupsRef() , 4);
            optimised.send(optimisedContentBase);//std::move(cb));
        }
    }
    
    //--------------------------------------------------------------
    void optimize(vector<ContentBase::PointGroup> & rawPointGroups, vector<ContentBase::PointGroup> & optimisedPointGroups, int passes) {
    
    ofxLaserTSPLaserProgram<ContentBase::PointGroup> program(rawPointGroups, false);
    
//    optimisedPointGroups.clear();
//    drawingLengths.clear();
//    auto t = ofGetElapsedTimeMicros();

    for (int i = 0; i <= passes; i++){
        program.optimize(i);
    }
    optimisedPointGroups.clear();
    optimisedPointGroups = program.getRoute();
//    drawingLengths.push_back(computeLengthOfPoints(optimisedPointGroups));
//    cout << "optimizeDrawing: time taken: " << ofGetElapsedTimeMicros() - t <<endl;
//
//    cout << "Length  = " << computeLengthOfPoints(rawPointGroups) << endl;
//
//    for (int i = 0; i <= passes; i++){
//        cout << "Length after " << i << " passes = " << drawingLengths[i] << endl;
//    }
    }

    //--------------------------------------------------------------
    float computeLengthOfPoints(vector<ContentBase::PointGroup> pointGroups) {

        float len = 0;
        glm::vec2 prevP = glm::vec2(ofGetWidth()/2);

        for(auto & pg : pointGroups){
               for(auto p : pg.points){
                   
                   len += glm::distance(prevP, p);
                   prevP = p;
               }
           }
       
        return len;
    }
};

#endif /* OptimiserTSP_h */
