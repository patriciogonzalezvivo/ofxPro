//
//  GridBox.h
//  Basic
//
//  Created by Patricio Gonzalez Vivo on 8/30/13.
//
//

#pragma once

#include "GridCross.h"

class GridBox : public GridCross {
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
                    
                    ofPoint pos = offset + ofPoint(x+i*resolution, y+j*resolution);
                    
                    if ( !needToSkip(pos) ){
                        ofRectangle box;
                        
                        float size = resolution*boxSize;
                        float line = resolution*crossSize;
                        box.setFromCenter(pos,size,size);
                        
                        //  TopLeft
                        //
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopLeft());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopLeft() + ofPoint(+line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopLeft());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopLeft() + ofPoint(0,+line));
                        
                        //  TopRight
                        //
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopRight());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopRight() + ofPoint(-line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopRight());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getTopRight() + ofPoint(0,+line));
                        
                        //  BottomLeft
                        //
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomLeft());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomLeft() + ofPoint(+line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomLeft());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomLeft() + ofPoint(0,-line));
                        
                        //  BottomRight
                        //
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomRight());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomRight() + ofPoint(-line,0));
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomRight());
                        
                        mesh.addColor(tmpColor);
                        mesh.addNormal(normal);
                        mesh.addVertex(box.getBottomRight() + ofPoint(0,-line));
                    }
                }
            }
        }
    }

    float   boxSize;
};
