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

#include "UIMaterial.h"

UIMaterial::UIMaterial(){
    diffuse.set(ofFloatColor(0.8f, 0.8f, 0.8f, 1.0f));
    specular.set(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    ambient.set(ofFloatColor(0.2f, 0.2f, 0.2f, 1.0f));
    emissive.set(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    shininess = 0.2;

    name = "MATERIAL";
};

UIMaterial::UIMaterial(string _name){
    diffuse.set(ofFloatColor(0.8f, 0.8f, 0.8f, 1.0f));
    specular.set(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    ambient.set(ofFloatColor(0.2f, 0.2f, 0.2f, 1.0f));
    emissive.set(ofFloatColor(0.0f, 0.0f, 0.0f, 1.0f));
    shininess = 0.2;

    setName(_name);
};

UIMaterial::~UIMaterial(){
};

void UIMaterial::operator = (ofMaterial &_mat){
    set(_mat);
}

void UIMaterial::set(ofMaterial &_mat){
    setColors(_mat.getDiffuseColor(),_mat.getAmbientColor(),_mat.getSpecularColor(),_mat.getEmissiveColor());
    shininess = _mat.getShininess();
}

// set colors
void UIMaterial::setColors(ofFloatColor _Diffuse, ofFloatColor _Ambient, ofFloatColor _Specular, ofFloatColor _Emissive){
    diffuse.set(_Diffuse);
    ambient.set(_Ambient);
    specular.set(_Specular);
    emissive.set(_Emissive);
};

void UIMaterial::setName(string _name){
    if (gui == NULL){
        name = _name;
    }
}

void UIMaterial::setupUI(){
    addUIColor("AMBIENT", ambient);
    addUIColor("DIFFUSE", diffuse);
    addUIColor("EMISSIVE", emissive);
    addUIColor("SPECULAR", specular);

    gui->addMinimalSlider("SHINY", 0.0, 1.0, &shininess)->setShowValue(false);
}

string UIMaterial::getClassName(){
    return name;
}

void UIMaterial::guiEvent(ofxUIEventArgs &e){

}

// apply the Material
void UIMaterial::begin(){
    if (bEnable){
        
        diffuse.update();
        ambient.update();
        specular.update();
        emissive.update();
        
#ifndef TARGET_OPENGLES
        // save previous values, opengl es cannot use push/pop attrib
        glGetMaterialfv(GL_FRONT,GL_DIFFUSE,&prev_diffuse.r);
        glGetMaterialfv(GL_FRONT,GL_SPECULAR,&prev_specular.r);
        glGetMaterialfv(GL_FRONT,GL_AMBIENT,&prev_ambient.r);
        glGetMaterialfv(GL_FRONT,GL_EMISSION,&prev_emissive.r);
        glGetMaterialfv(GL_FRONT, GL_SHININESS, &prev_shininess);
        
        glGetMaterialfv(GL_BACK,GL_DIFFUSE,&prev_diffuse_back.r);
        glGetMaterialfv(GL_BACK,GL_SPECULAR,&prev_specular_back.r);
        glGetMaterialfv(GL_BACK,GL_AMBIENT,&prev_ambient_back.r);
        glGetMaterialfv(GL_BACK,GL_EMISSION,&prev_emissive_back.r);
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
#else
        // opengl es 1.1 implementation must use GL_FRONT_AND_BACK.
        
        glGetMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_diffuse.r);
        glGetMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_specular.r);
        glGetMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_ambient.r);
        glGetMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_emissive.r);
        glGetMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &prev_shininess);
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &emissive.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
#endif
    }
};

void UIMaterial::end(){
    if (bEnable){
#ifndef TARGET_OPENGLES
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
#else
        // opengl es 1.1 implementation must use GL_FRONT_AND_BACK.
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &prev_diffuse.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &prev_specular.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &prev_ambient.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &prev_emissive.r);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &prev_shininess);
#endif
    }
};
