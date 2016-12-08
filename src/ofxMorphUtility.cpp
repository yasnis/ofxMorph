//
//  ofxMorphUtility.cpp
//  ofxMorphExample
//
//  Created by X006819_nishimura on 2016/12/05.
//
//

#include "ofxMorphUtility.h"

ofxMorphUtility::ofxMorphUtility() {
}

ofxMorphUtility::~ofxMorphUtility() {
    
}

void ofxMorphUtility::setup(string imagePath){
    setImagePath(imagePath);
    ofxDraggableMarker *m1 = new ofxDraggableMarker(ofVec2f(0, 0), 0);
    ofxDraggableMarker *m2 = new ofxDraggableMarker(ofVec2f(0, _texture.getHeight()), 1);
    ofxDraggableMarker *m3 = new ofxDraggableMarker(ofVec2f(_texture.getWidth(), _texture.getHeight()), 2);
    ofxDraggableMarker *m4 = new ofxDraggableMarker(ofVec2f(_texture.getWidth(), 0), 3);
    
    markers.clear();
    markers.addMarker(m1);
    markers.addMarker(m2);
    markers.addMarker(m3);
    markers.addMarker(m4);
    
    delaunay.reset();
    delaunay.addPoint(*m1);
    delaunay.addPoint(*m2);
    delaunay.addPoint(*m3);
    delaunay.addPoint(*m4);
}

/*
 * 頂点情報を更新する
 */
void ofxMorphUtility::update(){
    
    vector<ofxDraggableMarker *>::iterator iterator = markers.begin();
    while (iterator!=markers.end()) {
        delaunay.setPointAtIndex(**iterator, (*iterator)->getID());
        iterator++;
    }
    delaunay.triangulate();
    
    vector<ofVec3f> verticies;
    for (int i = 0; i<markers.size(); i++) {
        verticies.push_back(*markers[i]);
    }
    setVercities(&verticies);
}
/*
 * 描画する
 */
void ofxMorphUtility::draw() {
    ofxMorphImage::draw();
}
void ofxMorphUtility::draw(bool debug) {
    draw();
    if(debug){
        drawDebug();
    }
}

void ofxMorphUtility::drawDebug(){
    ofNoFill();
    delaunay.draw();
    markers.draw();
}


void ofxMorphUtility::copyFrom(ofxMorphUtility *image) {
    vector<ofVec3f> verts = image->getDefaultVerticies();
    vector<ofIndexType> inds = image->getIndices();
    reset();
    for (int i = 4; i<verts.size(); i++) {
        addMarker(new ofxDraggableMarker(verts[i], i));
    }
}

void ofxMorphUtility::reset() {
//    ofxMorphImage::rese
    ofxMorphImage::resetVerticies();
    clear();
    setup(_imagePath);
}

/*
 * マーカーを追加する
 */
void ofxMorphUtility::addMarker(){
    ofxDraggableMarker *marker = new ofxDraggableMarker(ofVec3f(ofRandom(getWidth()), ofRandom(getHeight())), markers.size());
    addMarker(marker);
}

void ofxMorphUtility::addMarker(ofxDraggableMarker *marker) {
    markers.addMarker(marker);
    delaunay.addPoint(*marker);
}


/*
 * メッシュの構造を設定する
 */
void ofxMorphUtility::generateMesh(){
    ofMesh triangle = delaunay.triangleMesh;
    vector<ofVec3f> verticies = triangle.getVertices();
    vector<ofIndexType> indicies = triangle.getIndices();
    setGeometry(&verticies, &indicies);
}


void ofxMorphUtility::loadSetting(ofXml xml){
//    cout << "ofxMorphUtility::loadSetting" << endl;
    reset();
    ofxMorphImage::loadSetting(xml);
    vector<ofVec3f> verts = getDefaultVerticies();
    for (int i = 4; i<verts.size(); i++) {
        addMarker(new ofxDraggableMarker(verts[i], i));
    }
}

ofXml ofxMorphUtility::toXml(){
    return ofxMorphImage::toXml();
}
