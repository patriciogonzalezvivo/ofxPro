#pragma once

#include "ofColor.h"
#include "ofConstants.h"

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

class Material {
public:
	Material(){
        diffuse.set(0.8f, 0.8f, 0.8f, 1.0f);
        specular.set(0.0f, 0.0f, 0.0f, 1.0f);
        ambient.set(0.2f, 0.2f, 0.2f, 1.0f);
        emissive.set(0.0f, 0.0f, 0.0f, 1.0f);
        shininess = 0.2;
    };
	virtual ~Material(){};
	
	// set colors
	void setColors(ofFloatColor oDiffuse, ofFloatColor oAmbient, ofFloatColor oSpecular, ofFloatColor oEmissive){
        setDiffuseColor(oDiffuse);
        setAmbientColor(oAmbient);
        setSpecularColor(oSpecular);
        setEmissiveColor(oEmissive);
    };
    
    void setDiffuseColor(ofFloatColor oDiffuse){ diffuse = oDiffuse; };
	void setAmbientColor(ofFloatColor oAmbient){ ambient = oAmbient; };
	void setSpecularColor(ofFloatColor oSpecular){ specular = oSpecular; };
	void setEmissiveColor(ofFloatColor oEmmisive){ emissive = oEmmisive; };
	void setShininess(float nShininess){ shininess = nShininess; };
    
	ofFloatColor& getDiffuseColor(){return diffuse;};
	ofFloatColor& getAmbientColor(){return ambient;};
	ofFloatColor& getSpecularColor(){return specular;};
	ofFloatColor& getEmissiveColor(){return emissive;};
	float& getShininess(){return shininess;};
	
	// apply the material
	virtual void begin(){
        // save previous values, opengl es cannot use push/pop attrib
        glGetMaterialfv(GL_FRONT,GL_DIFFUSE,&prev_diffuse.r);
        glGetMaterialfv(GL_FRONT,GL_SPECULAR,&prev_specular.r);
        glGetMaterialfv(GL_FRONT,GL_AMBIENT,&prev_ambient.r);
        glGetMaterialfv(GL_FRONT,GL_EMISSION,&prev_emissive.r);
        glGetMaterialfv(GL_FRONT, GL_SHININESS, &prev_shininess);
        
        glGetMaterialfv(GL_BACK,GL_DIFFUSE,&prev_diffuse_back.r);
        glGetMaterialfv(GL_BACK,GL_SPECULAR,&prev_specular_back.r);
        glGetMaterialfv(GL_BACK,GL_AMBIENT,&prev_ambient_back.r);
        glGetMaterialfv(GL_BACK,GL_EMISSION,&prev_emissive.r);
        glGetMaterialfv(GL_BACK, GL_SHININESS, &prev_shininess_back);
        
        // Material colors and properties
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &diffuse.r);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &specular.r);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &ambient.r);
        glMaterialfv(GL_FRONT, GL_EMISSION, &emissive.r);
        glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
        
        glMaterialfv(GL_BACK, GL_DIFFUSE, &diffuse.r);
        glMaterialfv(GL_BACK, GL_SPECULAR, &specular.r);
        glMaterialfv(GL_BACK, GL_AMBIENT, &ambient.r);
        glMaterialfv(GL_BACK, GL_EMISSION, &emissive.r);
        glMaterialfv(GL_BACK, GL_SHININESS, &shininess);
    };
    
	virtual void end(){
        // Set previous material colors and properties
        glMaterialfv(GL_FRONT, GL_DIFFUSE, &prev_diffuse.r);
        glMaterialfv(GL_FRONT, GL_SPECULAR, &prev_specular.r);
        glMaterialfv(GL_FRONT, GL_AMBIENT, &prev_ambient.r);
        glMaterialfv(GL_FRONT, GL_EMISSION, &prev_emissive.r);
        glMaterialfv(GL_FRONT, GL_SHININESS, &prev_shininess);
        
        glMaterialfv(GL_BACK, GL_DIFFUSE, &prev_diffuse_back.r);
        glMaterialfv(GL_BACK, GL_SPECULAR, &prev_specular_back.r);
        glMaterialfv(GL_BACK, GL_AMBIENT, &prev_ambient_back.r);
        glMaterialfv(GL_BACK, GL_EMISSION, &prev_emissive_back.r);
        glMaterialfv(GL_BACK, GL_SHININESS, &prev_shininess_back);
    };
	
protected:
    
	ofFloatColor diffuse;
	ofFloatColor ambient;
	ofFloatColor specular;
	ofFloatColor emissive;
	float shininess;
    
	ofFloatColor prev_diffuse, prev_diffuse_back;
	ofFloatColor prev_ambient, prev_ambient_back;
	ofFloatColor prev_specular, prev_specular_back;
	ofFloatColor prev_emissive, prev_emissive_back;
	float prev_shininess, prev_shininess_back;
};
