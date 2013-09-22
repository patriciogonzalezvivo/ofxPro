#pragma once

#include "UIClass.h"

/**
 * Material concept: "Anything graphical applied to the polygons"
 *
 * @author	Diederick Huijbers <diederick[at]apollomedia[dot]nl>
 *
 * @link	Wavefront material file spec:
 *			http://people.sc.fsu.edu/~jburkardt/data/mtl/mtl.html
 * @link	Ogre3D
 *			http://www.ogre3d.org/docs/manual/manual_11.html#SEC14
 * @link	assim material
 *			http://assimp.sourceforge.net/lib_html/ai_material_8h.html#7dd415ff703a2cc53d1c22ddbbd7dde0
 */

class UIMaterial : public UIClass {
public:
	UIMaterial();
    UIMaterial( string _name );
	virtual ~UIMaterial();
	
    void    setupUI();
    void    setName( string _name );
	void    setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive);
    
    string  getClassName();
    
	// apply the material
	virtual void begin();
	virtual void end();
	
protected:
    void    guiEvent(ofxUIEventArgs &e);
    string  name;
    
	ofFloatColor diffuse;
	ofFloatColor ambient;
	ofFloatColor specular;
	ofFloatColor emissive;
	
	ofFloatColor prev_diffuse, prev_diffuse_back;
	ofFloatColor prev_ambient, prev_ambient_back;
	ofFloatColor prev_specular, prev_specular_back;
	ofFloatColor prev_emissive, prev_emissive_back;
	float prev_shininess, prev_shininess_back;
    float shininess;
};
