//
//  GridCross.h
//  Basic
//
//  Created by Patricio Gonzalez Vivo on 8/30/13.
//
//

#pragma once

#include "Grid.h"

class GridCross : public Grid {
public:
    
    void makeGrid(){
        if ( resolution > 1.0){
            ofPoint normal = ofPoint(0.0,0.0,1.0);
            
            for(int i = 0; i <= cols; i++){
                for(int j = 0; j <= rows; j++){
                    
                    ofFloatColor tmpColor = color;
                    if (bJump && (i%2 == 1 || j%2 == 1)){
                        tmpColor.a = color.a*0.5;
                    }
                    
                    ofPoint pos = offset + ofPoint(x+i*resolution,
                                                   y+j*resolution,
                                                   -0.01);
                    
                    if ( !needToSkip(pos) ){
                        float line = resolution*crossSize;
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(pos + ofPoint(-line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(pos + ofPoint(+line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(pos + ofPoint(0,-line));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(pos + ofPoint(0,+line));
                    }
                }
            }
        }
    }

    float crossSize;
    
protected:
    
    bool    needToSkip(ofPoint _pos){
        for(int i = 0; i < skip.size(); i++){
            if ( skip[i].inside(_pos) )
                return true;
        }
        return false;
    }
};
