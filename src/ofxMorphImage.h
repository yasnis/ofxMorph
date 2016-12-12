//
//  ofxMorphImage.h
//  Morphing
//
//  Created by X006819_nishimura on 2016/12/05.
//
//

#ifndef ofxMorphImage_h
#define ofxMorphImage_h

#include "ofMain.h"

class ofxMorphImage: public ofMesh {
private:
protected:
    string _imagePath;
    ofTexture _texture;
    vector<ofVec3f> defaultVerticies;
    
    void setDefaultVerticies(vector<ofVec3f> *verts);
    void setIndicies(vector<ofIndexType> *inds);
    
public:
    ofxMorphImage();
    ~ofxMorphImage();
    
    void draw();
    void draw(ofPoint p, bool useOffset=true);
    void draw(float x, float y, bool useOffset=true);
    
    void setGeometry(vector<ofVec3f> *verts, vector<ofIndexType> *inds);
    void setVercities(vector<ofVec3f> *verts);
    void resetVerticies();
    const vector<ofVec3f> getDefaultVerticies();
    
    //テクスチャ関連
    void setImagePath(string path);
    void setTexture(ofTexture texture);
    void setImage(ofImage image);
    int getWidth();
    int getHeight();
    
    ofXml toXml();
    void loadSetting(ofXml xml);
};

#endif /* ofxMorphImage_h */
