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

public:
    ofxMorphUtility();
    ~ofxMorphUtility();
    void setup(string imagePath);
    void update();
    void draw();
    void draw(bool debug);
    void drawDebug();
    
    void reset();
    
    void copyFrom(ofxMorphUtility *image);
    
    void addMarker();
    void addMarker(ofxDraggableMarker *marker);
    void generateMesh();
    
    void loadSetting(ofXml xml);
    ofXml toXml();
};


#endif /* ofxMorphUtility_h */
