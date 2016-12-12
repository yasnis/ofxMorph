//
//  ofxMorphUtility.h
//  ofxMorphExample
//
//  Created by X006819_nishimura on 2016/12/05.
//
//

#ifndef ofxMorphUtility_h
#define ofxMorphUtility_h

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxDraggableMarker.h"
#include "ofxDraggableMarkerGroup.h"

#include "ofxMorphImage.h"


class ofxMorphUtility:public ofxMorphImage {
private:
    ofxDraggableMarkerGroup markers;
    ofxDelaunay delaunay;
    ofVec2f offset;

public:
    ofxMorphUtility();
    ~ofxMorphUtility();
    void setup(string imagePath);
    void update();
    void draw();
    void draw(bool debug);
    void drawNoOffset(bool debug);
    void drawDebug(bool useOffset = true);
    
    void reset();
    
    void copyFrom(ofxMorphUtility *image);
    
    void addMarker();
    void addMarker(ofxDraggableMarker *marker);
    void generateMesh();
    
    void loadSetting(ofXml xml);
    ofXml toXml();
    
    void setOffset(ofVec2f v);
    ofVec2f getOffset();
};


#endif /* ofxMorphUtility_h */
