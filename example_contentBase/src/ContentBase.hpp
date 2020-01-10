//
//  ContentBase.hpp
//  ByensPuls
//
//  Created by Jonas Fehr on 13/07/2019.
//

#ifndef ContentBase_hpp
#define ContentBase_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxIldaPoint.h"




class ContentBase{
public:

   struct PointGroup{
       PointGroup(){};

       PointGroup(vector<ofxIlda::Point> &p, int projectorIdx = -1){
           points.swap(p);
           this->projectorIndex = projectorIdx;
       }

       ofxIlda::Point at(int i){
           return points.at(i);
       }

       int size(){
           return points.size();
       }
    
    vector<ofxIlda::Point> & getPoints(){
        return points;
    }
       
       vector<ofxIlda::Point> points;
       int projectorIndex = -1;
   };


    vector<ofxIlda::Point> points;
    vector<PointGroup> pointGroups;

    virtual void setup(){};
    virtual void update(){};
    virtual bool isDead(){ return false; };

    void clear(){
        points.clear();
        pointGroups.clear();
    }

    void addPoint(glm::vec2 pos, float brightness){
        points.push_back(ofxIlda::Point( pos, ofFloatColor(brightness,1.)));
    };

    void addPoint(ofxIlda::Point point){
        points.push_back(point);
    };

    void addPointGroup(vector<ofxIlda::Point> pointGroup, int projectorIdx = -1){
        PointGroup pG(pointGroup, projectorIdx);
        addPointGroup(pG);
    };

    void addPointGroup(PointGroup &pG){
        if( pG.size() > 0 ) pointGroups.push_back(pG);
    };

    void closePointGroup(){
        addPointGroup(this->points);
        points.clear();
    };

    int size(){
        return pointGroups.size();
    }

    vector<ofxIlda::Point> getPoints(){
        return points;
    }

    vector<ofxIlda::Point> getPointGroup(int index){
        if(index < size()){
            return pointGroups[index].points;
        } else{
            vector<ofxIlda::Point> emptyPoints;
            return emptyPoints;
        }
    };

    vector<PointGroup> getPointGroups(){
        if(points.size()>0) closePointGroup();
        return pointGroups;
    }
    
    vector<PointGroup> & getPointGroupsRef(){
        if(points.size()>0) closePointGroup();
        return pointGroups;
    }

    vector<vector<ofxIlda::Point>> getIldaPointGroups(){
        vector<vector<ofxIlda::Point>> pgCollected;
        for(auto & pG : pointGroups){
            pgCollected.push_back(pG.points);
        }
        return pgCollected;
    }
    
    void addExtraBlanking(glm::vec2 pos, int num){
        for(int i = 0; i < num; ++i){
            addPoint(pos, 0);
        }
    }
    
    glm::vec2 lerp(glm::vec2 a, glm::vec2 b, float amt){
        return (1.-amt)*a + amt*b;
    }

    void reset(){};

    bool isDirty = true;

//    int getPriority(){ return priority; }

//    enum ContentPriority{
//        CP_LOW,
//        CP_MID,
//        CP_HIGH
//    } priority;
//
//
//    enum ContentType{
//        CT_WALKER,
//        CT_PULSE,
//        CT_ZONE
//    } type;

};
#endif /* ContentBase_hpp */
