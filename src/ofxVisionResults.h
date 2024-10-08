//
//  ofxVisionResults.h
//  example
//
//  Created by Roy Macdonald on 20-03-24.
//

#pragma once
#include "ofxVisionConstants.h"


#include "ofMain.h"

//
//#ifdef __OBJC__
//#import <Foundation/Foundation.h>
//#import <Vision/Vision.h>
//#endif
//


namespace ofxVision{

// A bunch of helping classes that only store the data from the detection processes and draws these.

class LabelDetection{
public:
    LabelDetection(){}
    virtual ~LabelDetection(){}
    LabelDetection(string label, float confidence);
    std::string label;
    float score;
    void drawLabel(glm::vec2 pos, bool bDrawScore = false);
    void drawLabel(glm::vec3 pos, bool bDrawScore = false);
    void drawLabel(float x , float y, bool bDrawScore = false);
};

class RectDetection: public LabelDetection{
public:
    RectDetection(){}
    virtual ~RectDetection(){}
    RectDetection(const ofRectangle& _rect, float confidence );
    RectDetection(float x, float y, float w, float h, float confidence );
    
    ofRectangle rect;
//    std::string label;
//    float score;
    
    void draw(bool bDrawRect, bool bDrawLabel, bool bDrawScore);
    void print();

    
};

class PointDetection{
public:
    PointDetection(){}
    PointDetection(float x, float y, float z, float confidence):point(x, y, z), score(confidence){}
    
    glm::vec3 point;
    float score;
    std::string label;
};


class PointsDetection: public RectDetection{
public:
    
    PointsDetection(size_t numPoints):points(numPoints){}
    
    std::vector<glm::vec3> points;
    
    void setPoint(const size_t& index, float x, float y, float confidence);
    
    void updateRect();
    void updateMesh();
    
    
    void drawPoints();
protected:
    ofMesh pointsMesh;
    bool bNeedsMeshUpdate = false;

};

class Points3DDetection: public PointsDetection {
public:
    Points3DDetection(size_t numPoints):PointsDetection(numPoints), points3D(numPoints), parents(numPoints) {for(auto& p: points3D) {p.setScale(0.2);}}
    std::vector<ofNode> points3D;
    std::vector<int> parents;
    
//    void setPoint(const size_t& index, float x, float y, float z);
    void setGlobalPosition(const size_t& index, glm::vec3 position);
    void setGlobalQuaternion(const size_t& index, glm::quat quat);
    void setParent(const size_t& index, const size_t& parent_id);
    void updateMesh();
    void updateRect();
    
    void drawPoints();
protected:
};

class FaceDetection: public PointsDetection{
public:
    FaceDetection():PointsDetection(FACE_N_PART){}
    
    glm::vec3 orientation;
    
};

class Body3DDetection : public Points3DDetection {
public:
    Body3DDetection():Points3DDetection(BODY3D_N_PART){camera.setScale(0.005f);}
    ofCamera camera;
    float bodyHeight;
};

template<class DetectionType>
class BaseDetectionCollection{
public:
    virtual ~BaseDetectionCollection(){}
    
    std::vector<DetectionType> detections;
    
    
    void pushTransformAndStyle(const ofRectangle& rect, const ofColor& color = ofColor::black){
        ofPushMatrix();
        ofTranslate(rect.getTopLeft());
        ofScale(rect.width, rect.height);
        ofPushStyle();
        ofSetColor(color);
    }
    
    void popTransformAndStyle(){
        ofPopStyle();
        ofPopMatrix();
    }
};


class FaceDetectionsCollection:
public BaseDetectionCollection<FaceDetection>
{
public:
    
    void draw(const ofRectangle& rect);
};

class LabelsCollection:
public BaseDetectionCollection<LabelDetection>
{
public:
    void draw(float x, float y);
};

class RectsCollection:
public BaseDetectionCollection<RectDetection>
{
public:
    
    void draw(const glm::vec2& offset, bool bDrawLabel = false, bool bDrawScore = false);
    void draw(const ofRectangle& rect, bool bDrawLabel = false, bool bDrawScore = false);
};


class PointsCollection:
public BaseDetectionCollection<PointsDetection>
{
public:
    
    void draw(const ofRectangle& rect);
};

class Body3DCollection:
public BaseDetectionCollection<Body3DDetection>
{
public:
    
    void draw(const ofRectangle& rect);
};
}

