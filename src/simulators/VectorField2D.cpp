#include "VectorField2D.h"
#define STRINGIFY(A) #A

//------------------------------------------------------------------------------------
VectorField2D::VectorField2D(){
    buffer[0] = NULL;
    buffer[1] = NULL;
    
    x = 0;
    y = 0;
    
    activeBuffer = 0;
}

VectorField2D::~VectorField2D(){
    for(int i = 0; i < 2; i++){
        if (buffer[i] != NULL){
            delete [] buffer[i];
        }
    }
}

//------------------------------------------------------------------------------------
void VectorField2D::setupField(float _scale){
    setupField(width/_scale,height/_scale,width,height);
    scale = _scale;
}

void VectorField2D::setupField(int _cols, int _rows, int _width, int _height){
	
	cols    = _cols;
	rows    = _rows;
	width   = _width;
	height  = _height;
	
    nTotal = cols * rows;
    
	for(int i = 0; i < 2; i++){
        if (buffer[i] != NULL){
            delete [] buffer[i];
        }
        buffer[i] = new ofPoint[nTotal];
    }
    
    clear();
}

void VectorField2D::loadCustomValues(ofxXmlSettings &_XML){
    cols = _XML.getValue("cols", cols);
    rows = _XML.getValue("rows", rows);
}

void VectorField2D::saveCustomValues(ofxXmlSettings &_XML){
    _XML.setValue("cols", cols);
    _XML.setValue("rows", rows);
}

//------------------------------------------------------------------------------------

ofPoint& VectorField2D::operator[](int _index){
    if (( _index > 0 ) && (_index < nTotal))
        return buffer[activeBuffer%2][_index];
    else {
        ofPoint empty = ofPoint(0,0,0);
        return empty;
    }
}


void VectorField2D::clear(){
    for (int i = 0; i < nTotal; i++){
        buffer[activeBuffer%2][i].set(0,0,0);
    }
}

//------------------------------------------------------------------------------------
int VectorField2D::getIndexFor(float _xpos, float _ypos){
    
    if ( inside(_xpos, _ypos) ){
        
        // convert xpos and ypos into pcts
        //
        float xPct = (_xpos-x) / (float)width ;
        float yPct = (_ypos-y) / (float)height;
        
        // DOUBLE CHECK: if we are less then 0 or greater then 1 in x or y, return no force.
        //
        if ((xPct < 0 || xPct > 1) || (yPct < 0 || yPct > 1)){
            return -1;
        }
        
        // Where are we in the array
        //
        int fieldPosX = (int)(xPct * cols);
        int fieldPosY = (int)(yPct * rows);
        
        // Saftey :)
        //
        fieldPosX = MAX(0, MIN(fieldPosX, cols-1));
        fieldPosY = MAX(0, MIN(fieldPosY, rows-1));
        
        // Pos in array
        return fieldPosY * cols + fieldPosX;
    } else {
        return -1;
    }
}

ofPoint	VectorField2D::getForceFromPos(ofPoint pos){
    return getForceFromPos(pos.x,pos.y);
}

ofPoint VectorField2D::getForceFromPos(float _xpos, float _ypos){
    int _index = getIndexFor(_xpos,_ypos);
    if (_index < 0)
        return ofPoint(0,0);
	
	return ofPoint(buffer[activeBuffer%2][_index].x * 0.1,
                   buffer[activeBuffer%2][_index].y * 0.1,
                   0.0 );  // scale here as values are pretty large.
}

//------------------------------------------------------------------------------------

void VectorField2D::addRandom(float _pct){
	for (int i = 0; i < nTotal; i++){
        buffer[activeBuffer%2][i] = buffer[activeBuffer%2][i]*(1.0-_pct) + ofPoint(ofRandom(-1,1),ofRandom(-1,1))*_pct;
    }
}

void VectorField2D::addNoise( float _pct  ,float _speed, float _turbulence, bool _signed){
    
    float t = ofGetElapsedTimef() * 0.5;
    
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            // pos in array
            int index = j * cols + i;
            
            float normx = ofNormalize(i, 0, cols);
            float normy = ofNormalize(j, 0, rows);

            float u, v;
            if (_signed){
                u = ofSignedNoise(t + TWO_PI, normx * _turbulence + TWO_PI, normy * _turbulence + TWO_PI);
                v = ofSignedNoise(t - TWO_PI, normx * _turbulence - TWO_PI, normy * _turbulence + TWO_PI);
            } else {
                u = ofNoise(t + TWO_PI, normx * _turbulence + TWO_PI, normy * _turbulence + TWO_PI);
                v = ofNoise(t - TWO_PI, normx * _turbulence - TWO_PI, normy * _turbulence + TWO_PI);
            }
            
            buffer[activeBuffer%2][ index ] = buffer[activeBuffer%2][ index ]*(1.0-_pct) + ofPoint(u,v)*_pct;
        }
    }
}

void VectorField2D::addTextureNormals(ofBaseHasTexture &_texture, float _pct){
    ofFbo fbo;
    fbo.allocate(_texture.getTextureReference().getWidth(),
                 _texture.getTextureReference().getHeight());
    
    string normalsShaderProgram = STRINGIFY(uniform sampler2DRect tex0;
                                            float xOffset = 1.0;
                                            float yOffset = 1.0;
                                            
                                            float getLuminance(vec2 _st){
                                                return dot(vec3(0.2125, 0.7154, 0.0721), texture2DRect(tex0, _st).rgb);
                                            }
                                            
                                            void main(){
                                                float sCoord	= gl_TexCoord[0].s;
                                                float tCoord	= gl_TexCoord[0].t;
                                                
                                                float center	= getLuminance( vec2( sCoord, tCoord ));
                                                float topLeft	= getLuminance( vec2(sCoord - xOffset, tCoord - yOffset ) );
                                                float left		= getLuminance( vec2(sCoord - xOffset, tCoord           ) );
                                                float bottomLeft= getLuminance( vec2(sCoord - xOffset, tCoord + yOffset ) );
                                                float top		= getLuminance( vec2(sCoord			 , tCoord - yOffset ) );
                                                float bottom	= getLuminance( vec2(sCoord			 , tCoord + yOffset	) );
                                                float topRight	= getLuminance( vec2(sCoord + xOffset, tCoord - yOffset	) );
                                                float right		= getLuminance( vec2(sCoord + xOffset, tCoord			) );
                                                float bottomRight=getLuminance( vec2(sCoord + xOffset, tCoord + yOffset	) );
                                                
                                                float dX = topRight + 2.0 * right + bottomRight - topLeft - 2.0 * left - bottomLeft;
                                                float dY = bottomLeft + 2.0 * bottom + bottomRight - topLeft - 2.0 * top - topRight;
                                                
                                                vec3 N = normalize( vec3( dX, dY, 0.01) );
                                                
                                                N *= 0.5;
                                                N += 0.5;
                                                
                                                gl_FragColor.rgb = N;
                                                gl_FragColor.a = 1.0;
                                            });
    ofShader normals;
    normals.setupShaderFromSource(GL_FRAGMENT_SHADER, normalsShaderProgram);
    normals.linkProgram();
    
    fbo.begin();
    normals.begin();
    _texture.getTextureReference().draw(0,0,_texture.getTextureReference().getWidth(),_texture.getTextureReference().getHeight());
    normals.end();
    fbo.end();
    
    ofPixels normalsPixels;
    fbo.getTextureReference().readToPixels(normalsPixels);
    
    int widthScaleFactor = fbo.getWidth()/cols;
    int heightScaleFactor = fbo.getHeight()/rows;
    
    for(int i = 0; i < cols; i++){
        for(int j = 0; j < rows; j++){
            int index = j * cols + i;
            
            int scaledX = ofClamp(i*widthScaleFactor,0,fbo.getWidth());
            int scaledY = ofClamp(j*heightScaleFactor,0,fbo.getHeight());
            
            ofFloatColor normalColor = normalsPixels.getColor(scaledX, scaledY);
            
            ofPoint norm = ofPoint((normalColor.r - 0.5) * -2.0,
                                   (normalColor.g - 0.5) * -2.0);
        
            buffer[activeBuffer%2][ index ] = buffer[activeBuffer%2][ index ]*(1.0-_pct) + norm*_pct;
        }
    }
}

//------------------------------------------------------------------------------------

void VectorField2D::fade(float _fadeAmount){
	for (int i = 0; i < nTotal; i++){
        buffer[activeBuffer%2][i].set(buffer[activeBuffer%2][i].x*_fadeAmount,
                                      buffer[activeBuffer%2][i].y*_fadeAmount,0.0);
    }
}

void VectorField2D::propagate(){

    //  Based in this entry http://freespace.virgin.net/hugo.elias/graphics/x_water.htm
    //  Here is my explanation: http://www.patriciogonzalezvivo.com/blog/?p=657
    //
    int one = activeBuffer%2;      // Buffer 1
    int two = (activeBuffer+1)%2;  // Buffer 2
    
    //  damping = some non-integer between 0 and 1
    //
    float damping = 0.99;
    
    //  for every non-edge element:
    //
    for (int r = 1; r < rows-1; r++){
        for (int c = 1; c < cols-1; c++){
            
            int i = r * cols + c;
            
            int A = i-1;
            int B = i+1;
            int C = i-cols;
            int D = i+cols;
            
            buffer[two][i] = (buffer[one][A] +
                              buffer[one][B] +
                              buffer[one][C] +
                              buffer[one][D] ) * 0.5 - buffer[two][i];
            buffer[two][i] *= damping;
        }
    }
    
    //  Swap the buffers
    //
    activeBuffer = two;
}

//------------------------------------------------------------------------------------
void VectorField2D::draw(){
    {
        ofPushStyle();
        ofDisableSmoothing();
        float scalex = (float)width / (float)cols;
        float scaley = (float)height / (float)rows;
        
        for (int i = 0; i < cols; i++){
            for (int j = 0; j < rows; j++){
                
                // pos in array
                //
                int pos = j * cols + i;
                
                // pos externally
                //
                float px = 	x + i * scalex;
                float py = 	y + j * scaley;
                float px2 = px + buffer[activeBuffer%2][pos].x * 5;
                float py2 = py + buffer[activeBuffer%2][pos].y * 5;
                
                float angle = atan2(buffer[activeBuffer%2][pos].y,buffer[activeBuffer%2][pos].x);
                
                ofLine(px,py, px2, py2);
                
                // draw an baseline to show direction
                // get the line as vector, calculate length, then normalize.
                // rotate and draw based on length
                
                ofVec2f line;
                line.set(px2-px, py2-py);
                float length = line.length();
                line.normalize();
                line.rotate(90);  // these are angles in degrees
                ofLine(px - line.x*length*0.2, py - line.y*length*0.2, px + line.x*length*0.2, py + line.y*length*0.2);
            }
        }
        ofPopStyle();
    }
}

//------------------------------------------------------------------------------------
void VectorField2D::addInwardCircle(float _x, float _y, float _radius, float _strength){
	
    if (inside(_x, _y)){
        // x y and radius are in external dimensions.  Let's put them into internal dimensions:
        // first convert to pct:
        
        float pctx			= (_x-x) / (float)width;
        float pcty			= (_y-y) / (float)height;
        float radiusPct		= _radius / (float)width;
        
        // then, use them here:
        int fieldPosX		= (int)(pctx * (float)cols);
        int fieldPosY		= (int)(pcty * (float)rows);
        float fieldRadius	= (float)(radiusPct * cols);
        
        // we used to do this search through every pixel, ie:
        //    for (int i = 0; i < cols; i++){
        //    for (int j = 0; j < rows; j++){
        // but we can be smarter :)
        // now, as we search, we can reduce the "pixels" we look at by
        // using the x y +/- radius.
        // use min and max to make sure we don't look over the edges
        
        int startx	= MAX(fieldPosX - fieldRadius, 0);
        int starty	= MAX(fieldPosY - fieldRadius, 0);
        int endx	= MIN(fieldPosX + fieldRadius, cols);
        int endy	= MIN(fieldPosY + fieldRadius, rows);
        
        
        for (int i = startx; i < endx; i++){
            for (int j = starty; j < endy; j++){
                
                int pos = j * cols + i;
                float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                             (fieldPosY-j)*(fieldPosY-j));
                
                if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
                
                if (distance < fieldRadius){
                    
                    float pct = 1.0f - (distance / fieldRadius);
                    
                    float strongness = _strength * pct;
                    float unit_px = ( fieldPosX - i) / distance;
                    float unit_py = ( fieldPosY - j) / distance;
                    buffer[activeBuffer%2][pos].x += unit_px * strongness;
                    buffer[activeBuffer%2][pos].y += unit_py * strongness;
                }
            }
        }
    }
}


//------------------------------------------------------------------------------------
void VectorField2D::addOutwardCircle(float _x, float _y, float _radius, float _strength){
	
	if (inside(_x, _y)){
        // x y and radius are in external dimensions.  Let's put them into internal dimensions:
        // first convert to pct:
        
        float pctx			= (_x-x) / (float)width;
        float pcty			= (_y-y) / (float)height;
        float radiusPct		= _radius / (float)width;
        
        // then, use them here:
        int fieldPosX		= (int)(pctx * (float)cols);
        int fieldPosY		= (int)(pcty * (float)rows);
        float fieldRadius	= (float)(radiusPct * cols);
        
        // we used to do this search through every pixel, ie:
        //    for (int i = 0; i < cols; i++){
        //    for (int j = 0; j < rows; j++){
        // but we can be smarter :)
        // now, as we search, we can reduce the "pixels" we look at by
        // using the x y +/- radius.
        // use min and max to make sure we don't look over the edges
        
        int startx	= MAX(fieldPosX - fieldRadius, 0);
        int starty	= MAX(fieldPosY - fieldRadius, 0);
        int endx	= MIN(fieldPosX + fieldRadius, cols);
        int endy	= MIN(fieldPosY + fieldRadius, rows);
        
        
        for (int i = startx; i < endx; i++){
            for (int j = starty; j < endy; j++){
                
                int pos = j * cols + i;
                float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                             (fieldPosY-j)*(fieldPosY-j));
                
                if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
                
                if (distance < fieldRadius){
                    
                    float pct = 1.0f - (distance / fieldRadius);
                    float strongness = _strength * pct;
                    float unit_px = ( fieldPosX - i) / distance;
                    float unit_py = ( fieldPosY - j) / distance;
                    buffer[activeBuffer%2][pos].x -= unit_px * strongness;
                    buffer[activeBuffer%2][pos].y -= unit_py * strongness;
                }
            }
        }
    }
}

//------------------------------------------------------------------------------------
void VectorField2D::addClockwiseCircle(float _x, float _y, float _radius, float _strength){
	
    if(inside(_x, _y)){
        // x y and radius are in external dimensions.  Let's put them into internal dimensions:
        // first convert to pct:
        
        float pctx			= (_x-x) / (float)width;
        float pcty			= (_y-y) / (float)height;
        float radiusPct		= _radius / (float)width;
        
        // then, use them here:
        int fieldPosX		= (int)(pctx * (float)cols);
        int fieldPosY		= (int)(pcty * (float)rows);
        float fieldRadius	= (float)(radiusPct * cols);
        
        // we used to do this search through every pixel, ie:
        //    for (int i = 0; i < cols; i++){
        //    for (int j = 0; j < rows; j++){
        // but we can be smarter :)
        // now, as we search, we can reduce the "pixels" we look at by
        // using the x y +/- radius.
        // use min and max to make sure we don't look over the edges
        
        int startx	= MAX(fieldPosX - fieldRadius, 0);
        int starty	= MAX(fieldPosY - fieldRadius, 0);
        int endx	= MIN(fieldPosX + fieldRadius, cols);
        int endy	= MIN(fieldPosY + fieldRadius, rows);
        
        
        for (int i = startx; i < endx; i++){
            for (int j = starty; j < endy; j++){
                
                int pos = j * cols + i;
                float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                             (fieldPosY-j)*(fieldPosY-j));
                
                if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
                
                if (distance < fieldRadius){
                    
                    float pct = 1.0f - (distance / fieldRadius);
                    
                    float strongness = _strength * pct;
                    float unit_px = ( fieldPosX - i) / distance;
                    float unit_py = ( fieldPosY - j) / distance;
                    buffer[activeBuffer%2][pos].x += unit_py * strongness;   /// Note: px and py switched, for perpendicular
                    buffer[activeBuffer%2][pos].y -= unit_px * strongness;
                }
            }
        }
    }
}



//------------------------------------------------------------------------------------
void VectorField2D::addCounterClockwiseCircle(float _x, float _y, float _radius, float _strength){
	
	if(inside(_x, _y)){
        // x y and radius are in external dimensions.  Let's put them into internal dimensions:
        // first convert to pct:
        
        float pctx			= (_x-x) / (float)width;
        float pcty			= (_y-y) / (float)height;
        float radiusPct		= _radius / (float)width;
        
        // then, use them here:
        int fieldPosX		= (int)(pctx * (float)cols);
        int fieldPosY		= (int)(pcty * (float)rows);
        float fieldRadius	= (float)(radiusPct * cols);
        
        // we used to do this search through every pixel, ie:
        //    for (int i = 0; i < cols; i++){
        //    for (int j = 0; j < rows; j++){
        // but we can be smarter :)
        // now, as we search, we can reduce the "pixels" we look at by
        // using the x y +/- radius.
        // use min and max to make sure we don't look over the edges
        
        int startx	= MAX(fieldPosX - fieldRadius, 0);
        int starty	= MAX(fieldPosY - fieldRadius, 0);
        int endx	= MIN(fieldPosX + fieldRadius, cols);
        int endy	= MIN(fieldPosY + fieldRadius, rows);
        
        
        for (int i = startx; i < endx; i++){
            for (int j = starty; j < endy; j++){
                
                int pos = j * cols + i;
                float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                             (fieldPosY-j)*(fieldPosY-j));
                
                if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
                
                if (distance < fieldRadius){
                    
                    float pct = 1.0f - (distance / fieldRadius);
                    
                    float strongness = _strength * pct;
                    float unit_px = ( fieldPosX - i) / distance;
                    float unit_py = ( fieldPosY - j) / distance;
                    buffer[activeBuffer%2][pos].x -= unit_py * strongness;   /// Note: px and py switched, for perpendicular
                    buffer[activeBuffer%2][pos].y += unit_px * strongness;
                }
            }
        }
    }
}


//------------------------------------------------------------------------------------
void VectorField2D::addVectorCircle(float _x, float _y, float _vx, float _vy, float _radius, float _strength){
	
	if(inside(_x, _y)){
        // x y and radius are in external dimensions.  Let's put them into internal dimensions:
        // first convert to pct:
        
        float pctx			= (_x-x) / (float)width;
        float pcty			= (_y-y) / (float)height;
        float radiusPct		= _radius / (float)width;
        
        // then, use them here:
        int fieldPosX		= (int)(pctx * (float)cols);
        int fieldPosY		= (int)(pcty * (float)rows);
        float fieldRadius	= (float)(radiusPct * cols);
        
        // we used to do this search through every pixel, ie:
        //    for (int i = 0; i < cols; i++){
        //    for (int j = 0; j < rows; j++){
        // but we can be smarter :)
        // now, as we search, we can reduce the "pixels" we look at by
        // using the x y +/- radius.
        // use min and max to make sure we don't look over the edges
        
        int startx	= MAX(fieldPosX - fieldRadius, 0);
        int starty	= MAX(fieldPosY - fieldRadius, 0);
        int endx	= MIN(fieldPosX + fieldRadius, cols);
        int endy	= MIN(fieldPosY + fieldRadius, rows);
        
        
        for (int i = startx; i < endx; i++){
            for (int j = starty; j < endy; j++){
                
                int pos = j * cols + i;
                float distance = (float)sqrt((fieldPosX-i)*(fieldPosX-i) +
                                             (fieldPosY-j)*(fieldPosY-j));
                
                if (distance < 0.0001) distance = 0.0001;  // since we divide by distance, do some checking here, devide by 0 is BADDDD
                
                if (distance < fieldRadius){
                    
                    float pct = 1.0f - (distance / fieldRadius);
                    float strongness = _strength * pct;
                    buffer[activeBuffer%2][pos].x += _vx * strongness;
                    buffer[activeBuffer%2][pos].y += _vy * strongness;
                }
            }
        }
	}
}