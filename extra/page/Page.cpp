//
//  Page.cpp
//


#include "Page.h"

//--------------------------------------------------------------
Page::Page(){
    meshDefinition = 20;
    
    coneAxisDist = 1.0;
    handLerpPct = 0.99;
    
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

void Page::reset(){
    hand.x = 1.0;
    hand.y = 1.0;
    bChange = true;
}

void Page::update(){
    if (bChange){
        computeConeValues();
        updateMesh();
        bChange = false;
    }
}

struct vectorFace{
    ofPoint a,b,c,d;    //  TexCoord
    ofPoint A,B,C,D;    //  Vectex
};

void Page::computeConeValues(){
    //  Update angles and parameters for making the Cone and the cylinder
    //
    
    float valueDep = hand.y;
    float valueIndep = hand.x;
    
    if (!bLeftRight){
        valueDep = hand.x;
        valueIndep = hand.y;
    }

    if (valueIndep > 0.95f){
        theta = ofMap(valueIndep,0.95,1.0,60,90);
        axis =  ofMap(valueIndep,0.95,1.0,-5,-20);
        angle = 0.0;
    } else if (valueIndep > 0.8333f) {
        theta = ofMap(valueIndep, 0.8333, 0.95, 55, 60);
        axis =  ofMap(valueIndep, 0.8333, 0.95, -4, -5);
        angle = ofMap(valueIndep, 0.8333, 0.95, 13, 0);
    } else if (valueIndep > 0.3333f) {
        theta = ofMap(valueIndep, 0.3333,0.8333, 45, 55);
        axis =  ofMap(valueIndep, 0.3333,0.8333, -10, -4);
        angle = ofMap(valueIndep, 0.3333,0.8333, 35, 13);
    } else if (valueIndep > 0.1666f) {
        theta = ofMap(valueIndep, 0.1666,0.3333, 25, 45);
        axis =  ofMap(valueIndep, 0.1666,0.3333, -30, -10);
        angle = ofMap(valueIndep, 0.1666,0.3333, 60, 35);
    } else {
        theta = ofMap(valueIndep, 0.0,0.1666,20,25);
        axis =  ofMap(valueIndep, 0.0,0.1666,-40,-30);
        angle = ofMap(valueIndep, 0.0,0.1666,95,60);
    }
    
    axis *= coneAxisDist;
    
    theta = ofDegToRad(theta);
    angle = ofDegToRad(angle);
    conicContribution = (valueDep-0.5)*2.0;
    
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
        if (bLeftRight){
            axis = -axis + height;
        } else {
            axis = -axis + width;
        }
        
    }
    
    conicContribution = abs(conicContribution);
    
    // As the user grabs towards the middle-right of the page, we curl the paper by deforming it
    // on to a cylinder. The cylinder radius is taken as the endpoint of the cone parameters:
    // ie: cylRadius = R*sin(theta) where R is the distance to the rightmost point on the page, all the way up.
    //
    float cylR;
    if (bLeftRight){
        cylR = sqrt(width*width+(height*0.5-axis)*(height*0.5-axis));
    } else {
        cylR = sqrt((width*0.5-axis)*(width*0.5-axis)+height*height);
    }
    cylRadius = cylR * sin(theta);
    
    // This is a hack to fix the bump at the start of a curl. If theta is greater than 50 degrees,
    // cylRadius needs to start approaching "infinity".
//    if (theta > ofDegToRad(50.0f)){
//        cylRadius = ofMap(theta, ofDegToRad(50.0f),ofDegToRad(90.0f),200.f,cylR*sin(ofDegToRad(50)));
//    }
}

ofPoint Page::getCurlPos(ofPoint pos , float _zOffset ){
    
    //  Calculate position on CONE
    //
    ofPoint conePos(pos);
    {
        // Compute conical parameters
        float R,r,beta;
        if(bLeftRight){
            R = sqrt(pos.x*pos.x + (pos.y-axis)*(pos.y-axis));
            r = R * sin(theta);
            beta  = asin(pos.x / R) / sin(theta);
        } else {
            R = sqrt((pos.x-axis)*(pos.x-axis) + pos.y*pos.y);
            r = R * sin(theta);
            beta  = asin(pos.y / R) / sin(theta);
        }
        
        // Cone it up
        if(bLeftRight){
            conePos.x = r * sin(beta);
            conePos.y = R + axis * posNegOne - r * (1 - cos(beta)) * sin(theta);
            conePos.z = r * (1 - cos(beta)) * cos(theta);
        } else {
            conePos.x = R + axis * posNegOne - r * (1 - cos(beta)) * sin(theta);
            conePos.y = r * sin(beta);
            conePos.z = r * (1 - cos(beta)) * cos(theta);
        }
        
        // Then rotate by angle about the y axis
        if(bLeftRight){
            conePos.x = conePos.x * cos(angle) - conePos.z * sin(angle);
            conePos.z = conePos.x * sin(angle) + conePos.z * cos(angle);
        } else {
            conePos.y = conePos.y * cos(angle) - conePos.z * sin(angle);
            conePos.z = conePos.y * sin(angle) + conePos.z * cos(angle);
        }
        
        // Transform coordinates to the page
        if(bLeftRight){
            conePos.x = conePos.x ;
            conePos.y = (-posNegOne * conePos.y) + height;
            conePos.z = conePos.z;
        } else {
            conePos.x = (-posNegOne * conePos.x) + width;
            conePos.y = conePos.y;
            conePos.z = conePos.z;
        }
    }
    
    //  Calculate position on CYLINDER
    //
    ofPoint cylPos(pos);
    {
        float beta;
        if(bLeftRight){
            beta = cylPos.x / (cylRadius + _zOffset );
        } else {
            beta = cylPos.y / (cylRadius + _zOffset );
        }
        
        // Rotate (0,0,0) by beta around line given by x = 0, z = cylRadius.
        // aka Rotate (0,0,-cylRadius) by beta, then add cylRadius back to z coordinate
        if(bLeftRight){
            cylPos.z = -cylRadius;
            cylPos.x = -cylPos.z * sin(beta);
            cylPos.z = cylPos.z * cos(beta);
            cylPos.z += cylRadius;
        } else {
            cylPos.z = -cylRadius;
            cylPos.y = -cylPos.z * sin(beta);
            cylPos.z = cylPos.z * cos(beta);
            cylPos.z += cylRadius;
        }
        
        // Then rotate by angle about the y axis
        if(bLeftRight){
            cylPos.x = cylPos.x * cos(angle) - cylPos.z * sin(angle);
            cylPos.z = cylPos.x * sin(angle) + cylPos.z * cos(angle);
        } else {
            cylPos.y = cylPos.y * cos(angle) - cylPos.z * sin(angle);
            cylPos.z = cylPos.y * sin(angle) + cylPos.z * cos(angle);
        }
        
        // Transform coordinates to the page
        if(bLeftRight){
            cylPos.x = cylPos.x;
            cylPos.y = -cylPos.y + height;
            cylPos.z = cylPos.z;
        } else {
            cylPos.x = -cylPos.x + width;
            cylPos.y = cylPos.y;
            cylPos.z = cylPos.z;
        }
    }
    
    // Combine CONE & CYLINDER
    //
    return conePos * conicContribution + cylPos * (1.0-conicContribution);
}


//------------------------------------------------------

void Page::updateMesh(){
    //  Re-build mesh
    //
    mesh.clear();
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //  FRONT
    //
    for(int j = 0; j < height; j += meshDefinition){
        for(int i = 0; i < width; i += meshDefinition){
            
            vectorFace face;
            
            //  texCoords
            //
            ofPoint a, b, c, d;
            a = ofPoint(i,j,0);
            b = ofPoint(i+meshDefinition,j,0);
            c = ofPoint(i+meshDefinition,j+meshDefinition,0);
            d = ofPoint(i,j+meshDefinition,0);
            
            //  Vectex
            //
            ofPoint A = getCurlPos(a);
            ofPoint B = getCurlPos(b);
            ofPoint C = getCurlPos(c);
            ofPoint D = getCurlPos(d);
            
            ofVec3f normal = ((B - A).cross(C - A)).normalize();
            
            float xA = i;
            float xB = i+meshDefinition;
            float yA = j;
            float yB = j+meshDefinition;
            
            if(bFlipHorizontal){
                xA = width-i;
                xB = width-i-meshDefinition;
            }
            
            if(bFlipVertical){
                yA = height-j;
                yB = height-j-meshDefinition;
            }
            
            if(bFlipVertical||bFlipHorizontal){
                a = ofPoint(xA,yA,0);
                b = ofPoint(xB,yA,0);
                c = ofPoint(xB,yB,0);
                d = ofPoint(xA,yB,0);
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