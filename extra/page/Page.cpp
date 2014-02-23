//
//  Page.cpp
//


#include "Page.h"

//--------------------------------------------------------------
Page::Page(){
    meshDefinition = 20;
    
    bFlipVertical = true;
    bLeftRight = true;
    bChange = true;
    
    bDebug = false;
    
    front = NULL;
    back = NULL;
}

void Page::setHandAt(ofPoint _hand, float _lerpPct){
    if(_lerpPct == -1){
        _lerpPct = handLerpPct;
    }
    
    if ( inside(_hand) ){
        hand.x = ofLerp(hand.x,ofNormalize(_hand.x, x, x+width),_lerpPct);
        hand.y = ofLerp(hand.y,ofNormalize(_hand.y, y, y+height),_lerpPct);
        bChange = true;
    } else {
        
        if ( (hand != ofPoint(1.0,1.0)) && ( hand != ofPoint(0.0,0.5))){
        
            if (hand.x >= 0.5){
                hand.x = ofLerp(hand.x, 1.0,_lerpPct);
                hand.y = ofLerp(hand.y, 1.0,_lerpPct);
            } else {
                hand.x = ofLerp(hand.x, 0.0,_lerpPct);
                hand.y = ofLerp(hand.y, 0.5,_lerpPct);
            }
            bChange = true;
        }
    }
}

void Page::setNormHandAt(ofPoint _hand, float _lerpPct){
    if(_lerpPct == -1){
        _lerpPct = handLerpPct;
    }
    
    if ( hand != _hand ){
        hand.x = ofLerp(hand.x,_hand.x,_lerpPct);
        hand.y = ofLerp(hand.y,_hand.y,_lerpPct);
        bChange = true;
    }
}

float Page::getTransition(){
    return ofMap(angle, 0.0, 1.2, 1.0, 0.0);
}

ofMesh& Page::getMesh(){
    return mesh;
}

float Page::strightLine(float _x1, float _y1, float _x2, float _y2, float _x){
    return (((_y2 - _y1) / (_x2 - _x1)) * (_x - _x1) + _y1);
}

void Page::update(){
    
    if (bChange){
        
        if (bLeftRight){
            //  Update angles and parameters for making the Cone and the cylinder
            //
            if (hand.x > 0.95f){
                theta = STRAIGHT_LINE(1.0f,    90.0f,   0.95f,  60.0f, hand.x);
                ay =    STRAIGHT_LINE(1.0f,   -20.0f,   0.95f,  -5.0f, hand.x);
                angle = 0.0;
            } else if (hand.x > 0.8333f) {
                theta = STRAIGHT_LINE(0.95f,   60.0f, 0.8333f,  55.0f, hand.x);
                ay =    STRAIGHT_LINE(0.95f,   -5.0f, 0.8333f,  -4.0f, hand.x);
                angle = STRAIGHT_LINE(0.95f,    0.0f, 0.8333f,  13.0f, hand.x);
            } else if (hand.x > 0.3333f) {
                theta = STRAIGHT_LINE(0.8333f, 55.0f, 0.3333f,  45.0f, hand.x);
                ay =    STRAIGHT_LINE(0.8333f, -4.0f, 0.3333f,  -10.0f, hand.x);
                angle = STRAIGHT_LINE(0.8333f, 13.0f, 0.3333f,  35.0f, hand.x);
            } else if (hand.x > 0.1666f) {
                theta = STRAIGHT_LINE(0.3333f, 45.0f, 0.1666f,  25.0f, hand.x);
                ay =    STRAIGHT_LINE(0.3333f, -10.0f, 0.1666f, -30.0f, hand.x);
                angle = STRAIGHT_LINE(0.3333f, 35.0f, 0.1666f,  60.0f, hand.x);
            } else {
                theta = STRAIGHT_LINE(0.1666f, 25.0f,    0.0f,  20.0f, hand.x);
                ay =    STRAIGHT_LINE(0.1666f, -30.0f,   0.0f, -40.0f, hand.x);
                angle = STRAIGHT_LINE(0.1666f, 60.0f,    0.0f,  95.0f, hand.x);
            }
            
            theta = ofDegToRad(theta);
            angle = ofDegToRad(angle);
            conicContribution = (hand.y-0.5)*2.0;
            
            // Flipping from top corner or bottom corner?
            //
            if (conicContribution > 0){
                // flipping from top corner
                //
                posNegOne = 1.0f;
            } else {
                // flipping from bottom corner
                //
                posNegOne = -1.0f;
                ay = -ay + height;
            }
            
            conicContribution = abs(conicContribution);
            
            // As the user grabs towards the middle-right of the page, we curl the paper by deforming it
            // on to a cylinder. The cylinder radius is taken as the endpoint of the cone parameters:
            // ie: cylRadius = R*sin(theta) where R is the distance to the rightmost point on the page, all the way up.
            //
            cylR = sqrt( width*width + (height*0.5-ay) * (height*0.5-ay));
            cylRadius = cylR * sin(theta);
            
            // This is a hack to fix the bump at the start of a curl. If theta is greater than 50 degrees,
            // cylRadius needs to start approaching "infinity".
            float cylRadiusAt50 = cylR * sin(ofDegToRad(50));
            if (theta > ofDegToRad(50.0f)){
                float x1 = ofDegToRad(90.0f);
                float x2 = ofDegToRad(50.0f);
                cylRadius = STRAIGHT_LINE(x1, 200.0f, x2, cylRadiusAt50, theta);
            }
            
            //  Re-build mesh
            //
            mesh.clear();
            mesh.setMode(OF_PRIMITIVE_TRIANGLES);
            
            //  FRONT
            //
            int jump = meshDefinition;
            for(int j = 0; j < height; j += jump){
                for(int i = 0; i < width; i += jump){
                    
                    vectorFace face;
                    
                    //  texCoords
                    //
                    ofPoint a, b, c, d;
                    a = ofPoint(i,j,0);
                    b = ofPoint(i+jump,j,0);
                    c = ofPoint(i+jump,j+jump,0);
                    d = ofPoint(i,j+jump,0);
                    
                    //  Vectex
                    //
                    ofPoint A = getCurlPos( a );
                    ofPoint B = getCurlPos( b );
                    ofPoint C = getCurlPos( c );
                    ofPoint D = getCurlPos( d );
                    
                    ofVec3f normal = ((B - A).cross(C - A)).normalize();
                    
                    if(bFlipVertical){
                        a = ofPoint(i,height-j,0);
                        b = ofPoint(i+jump,height-j,0);
                        c = ofPoint(i+jump,height-j-jump,0);
                        d = ofPoint(i,height-j-jump,0);
                    }
                    
                    mesh.addNormal(normal);
                    mesh.addVertex(A);
                    mesh.addTexCoord(a);
                    
                    mesh.addNormal(normal);
                    mesh.addVertex(B);
                    mesh.addTexCoord(b);
                    
                    mesh.addNormal(normal);
                    mesh.addVertex(C);
                    mesh.addTexCoord(c);
                    
                    normal = ((C - A).cross(D - A)).normalize();
                    mesh.addNormal(normal);
                    mesh.addVertex(A);
                    mesh.addTexCoord(a);
                    
                    mesh.addNormal(normal);
                    mesh.addVertex(C);
                    mesh.addTexCoord(c);
                    
                    mesh.addNormal(normal);
                    mesh.addVertex(D);
                    mesh.addTexCoord(d);
                }
            }
        }
        
        bChange = false;
    }
}

ofPoint Page::getCurlPos(ofPoint pos , float _zOffset ){
    
    // Be careful with units here. "pos" range from [0, COLS] & [0, ROWS] which are just the vertex indices.
    // 'ay' is in the coordinate system of the vertex indices. The actual values of the vertices are in
    // page units (pageMin[X,Y], pageMax[X,Y]).
    
    //  Calculate position on CONE
    //
    ofPoint conePos(pos);
    {
        // Compute conical parameters
        float R = sqrt(pos.x * pos.x + (pos.y - ay)*(pos.y - ay));
        float r = R * sin(theta);
        float beta  = asin(pos.x / R) / sin(theta);
        
        // Cone it up
        conePos.x = r * sin(beta);
        conePos.y = R + ay * posNegOne - r * (1 - cos(beta)) * sin(theta);
        conePos.z = r * (1 - cos(beta)) * cos(theta);
        
        // Then rotate by angle about the y axis
        conePos.x = conePos.x * cos(angle) - conePos.z * sin(angle);
        conePos.z = conePos.x * sin(angle) + conePos.z * cos(angle);
        
        // Transform coordinates to the page
        conePos.x = conePos.x ;
        conePos.y = (-posNegOne * conePos.y) + height;
        conePos.z = conePos.z;
    }
    
    //  Calculate position on CYLINDER
    //
    ofPoint cylPos(pos);
    {
        float beta = cylPos.x / (cylRadius + _zOffset );
        
        // Rotate (0,0,0) by beta around line given by x = 0, z = cylRadius.
        // aka Rotate (0,0,-cylRadius) by beta, then add cylRadius back to z coordinate
        cylPos.z = -cylRadius;
        cylPos.x = -cylPos.z * sin(beta);
        cylPos.z = cylPos.z * cos(beta);
        cylPos.z += cylRadius;
        
        // Then rotate by angle about the y axis
        cylPos.x = cylPos.x * cos(angle) - cylPos.z * sin(angle);
        cylPos.z = cylPos.x * sin(angle) + cylPos.z * cos(angle);
        
        // Transform coordinates to the page
        cylPos.x = cylPos.x;
        cylPos.y = -cylPos.y + height;
        cylPos.z = cylPos.z;
    }
    
    // Combine CONE & CYLINDER
    //
    return conePos * conicContribution + cylPos * (1.0-conicContribution);
}

void Page::draw(){
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(x, y);
    
    //  FRONT
    //
    if (front != NULL){
        front->bind();
        ofSetColor(ofFloatColor(1.0,getTransition()));
        mesh.draw();
        front->unbind();
    }
    
    //  BACK
    //
    {
        float pct = ofMap(getTransition(),0.0,0.1,1.0,0.0);
        ofSetColor(ofFloatColor(1.0, pct ) );
        ofRectangle backSide(*this);
        ofPoint centerRight = getCurlPos( ofPoint(width,height*0.5) );
        backSide.x = ofMap(centerRight.x,-0.0,-300,0,-this->width,true);
        backSide.y = 0;
        backSide.width = abs(backSide.x);
        
        ofSetColor(255);
        if (back != NULL){
            drawPage( *back , backSide );
        } else {
            ofRect(backSide);
        }
    }
    
    //  DEBUG
    //
    if (bDebug){
        mesh.drawWireframe();
        
        //  Center of the book
        //
        ofTranslate(0, height*0.5);
        ofSetColor(255, 0, 0);
        float cSize = 5;
        ofLine(-cSize, 0, cSize, 0);
        ofLine(0, -cSize, 0, cSize);
        
        ofPopMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(x+hand.x*width, y+hand.y*height, -10);
        
        ofNoFill();
        
        ofSetColor(0,0,255);
        ofRect(*this);
    }
    
    ofPopStyle();
    ofPopMatrix();
}