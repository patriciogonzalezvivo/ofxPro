//
//  LeapFinger.h
//  04 - LeapBrush
//
//  Created by Patricio Gonzalez Vivo on 10/5/13.
//
//

#pragma once

class LeapFinger : public ofPoint {
public:
    
    void lerp(const LeapFinger &_otherFinger, float _amount ){
        if(_amount == 1.0){
            *this   = _otherFinger;
            vel     = _otherFinger.vel;
            dir     = _otherFinger.dir;
            end     = _otherFinger.end;
            lenght  = _otherFinger.lenght;
        } else {
            *this   += (_otherFinger - *this )*_amount;
            vel     += (_otherFinger.vel - vel)*_amount;
            dir     += (_otherFinger.dir - dir)*_amount;
            end     += (_otherFinger.end - end)*_amount;
            lenght  += (_otherFinger.lenght - lenght)*_amount;
        }
        life = _otherFinger.life;
    };
    
    void xeno(const LeapFinger &_otherFinger, float _pct ){
        if(_pct == 1.0){
            set(_otherFinger);
            vel = _otherFinger.vel;
            dir = _otherFinger.dir;
            end = _otherFinger.end;
            lenght  = _otherFinger.lenght;
        } else {
            set( _pct * _otherFinger + (1.0-_pct) * (*this) );
            vel     = _pct * _otherFinger.vel + (1.0-_pct) * vel;
            dir     = _pct * _otherFinger.dir + (1.0-_pct) * dir;
            end     = _pct * _otherFinger.end + (1.0-_pct) * end;
            lenght  = _pct * _otherFinger.lenght + (1.0-_pct) * lenght;
        }
        life = _otherFinger.life;
    }
    
    ofPoint vel;
    ofPoint dir;
    ofPoint end;
    
    float   life;
    
    int64_t id;
    
    float   lenght;
    bool    bSync;
};