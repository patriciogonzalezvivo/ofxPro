//
//  Location.cpp
//
//  Copyright (C) 2013 Patricio Gonzalez Vivo ( http://www.patriciogonzalezvivo.com )
//
//

#include "Location.h"

double CalculateESquared(double a, double b) {
    return ((a * a) - (b * b)) / (a * a);
}

double CalculateE2Squared(double a, double b) {
    return ((a * a) - (b * b)) / (b * b);
}

double sphsr(double a, double es, double sphi) {
    double sinSphi = sin(sphi);
    double dn = sqrt(1.0 - es * (sinSphi * sinSphi));
    return a * (1.0 - es) / (dn * dn * dn);
}

double sphsn(double a, double es, double sphi) {
    double sinSphi = sin(sphi);
    return a / sqrt(1.0 - es * (sinSphi * sinSphi));
}

double sphtmd(double ap, double bp, double cp, double dp, double ep,
              double sphi) {
    return (ap * sphi) - (bp * sin(2.0 * sphi)) + (cp * sin(4.0 * sphi))
    - (dp * sin(6.0 * sphi)) + (ep * sin(8.0 * sphi));
}

void Location::setFromUTM(UtmPosition _utm){
    double fe = 500000.0;
    double ok = 0.9996;
    char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    
    double a = 6378137.0;
    double f = 1/298.257223563;
    
    double recf = 1.0 / f;
    double b = a * (recf - 1) / recf;
    double eSquared = CalculateESquared(a, b);
    double e2Squared = CalculateE2Squared(a, b);
    double tn = (a - b) / (a + b);
    double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
                     ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
    double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
                                                   - (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
    / 2.0;
    double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
                                                                - (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
    double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
                            * (tn * tn * tn * tn * tn) / 16.0) / 48.0;
    double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
    double nfn = 0;
    if ((_utm.zoneY <= 'M' && _utm.zoneY >= 'C')
        || (_utm.zoneY <= 'm' && _utm.zoneY >= 'c')) {
        nfn = 10000000.0;
    }
    double tmd = (_utm.northing - nfn) / ok;
    double sr = sphsr(a, eSquared, 0.0);
    double ftphi = tmd / sr;
    double t10, t11, t14, t15;
    for (int i = 0; i < 5; i++) {
        t10 = sphtmd(ap, bp, cp, dp, ep, ftphi);
        sr = sphsr(a, eSquared, ftphi);
        ftphi = ftphi + (tmd - t10) / sr;
    }
    sr = sphsr(a, eSquared, ftphi);
    double sn = sphsn(a, eSquared, ftphi);
    double s = sin(ftphi);
    double c = cos(ftphi);
    double t = s / c;
    double eta = e2Squared * (c * c);
    double de = _utm.easting - fe;
    t10 = t / (2.0 * sr * sn * (ok * ok));
    t11 = t * (5.0 + 3.0 * (t * t) + eta - 4.0 * (eta * eta) - 9.0 * (t * t)
               * eta) / (24.0 * sr * (sn * sn * sn) * (ok * ok * ok * ok));
    lat = ftphi - (de * de) * t10 + (de * de * de * de) * t11;
    t14 = 1.0 / (sn * c * ok);
    t15 = (1.0 + 2.0 * (t * t) + eta) / (6 * (sn * sn * sn) * c
                                         * (ok * ok * ok));
    double dlam = de * t14 - (de * de * de) * t15;
    double olam = (_utm.zoneX * 6 - 183.0) * DEG_TO_RAD;
    lon = olam + dlam;
    lon *= RAD_TO_DEG;
    lat *= RAD_TO_DEG;
}

bool Location::operator == (const Location &l) const{
    return l.lat == lat && l.lon == lon;
};

UtmPosition Location::getUTM(){
    double fe = 500000.0;
    double ok = 0.9996;
    char cArray[] = "CDEFGHJKLMNPQRSTUVWX";
    
    UtmPosition utm;
    
    double a = 6378137.0;
    double f = 1/298.257223563;
    
    if (lon <= 0.0) {
        utm.zoneX = 30 + (int) (lon / 6.0);
    } else {
        utm.zoneX = 31 + (int) (lon / 6.0);
    }
    if (lat < 84.0 && lat >= 72.0) {
        // Special case: zone X is 12 degrees from north to south, not 8.
        utm.zoneY = cArray[19];
    } else {
        utm.zoneY = cArray[(int) ((lat + 80.0) / 8.0)];
    }
    if (lat >= 84.0 || lat < -80.0) {
        // Invalid coordinate; the vertical zone is set to the invalid
        // character.
        utm.zoneY = '*';
    }
    
    double latRad = lat * DEG_TO_RAD;
    double lonRad = lon * DEG_TO_RAD;
    double recf = 1.0 / f;
    double b = a * (recf - 1.0) / recf;
    double eSquared = CalculateESquared(a, b);
    double e2Squared = CalculateE2Squared(a, b);
    double tn = (a - b) / (a + b);
    double ap = a * (1.0 - tn + 5.0 * ((tn * tn) - (tn * tn * tn)) / 4.0 + 81.0 *
                     ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 64.0);
    double bp = 3.0 * a * (tn - (tn * tn) + 7.0 * ((tn * tn * tn)
                                                   - (tn * tn * tn * tn)) / 8.0 + 55.0 * (tn * tn * tn * tn * tn) / 64.0)
    / 2.0;
    double cp = 15.0 * a * ((tn * tn) - (tn * tn * tn) + 3.0 * ((tn * tn * tn * tn)
                                                                - (tn * tn * tn * tn * tn)) / 4.0) / 16.0;
    double dp = 35.0 * a * ((tn * tn * tn) - (tn * tn * tn * tn) + 11.0
                            * (tn * tn * tn * tn * tn) / 16.0) / 48.0;
    double ep = 315.0 * a * ((tn * tn * tn * tn) - (tn * tn * tn * tn * tn)) / 512.0;
    double olam = (utm.zoneX * 6 - 183) * DEG_TO_RAD;
    double dlam = lonRad - olam;
    double s = sin(latRad);
    double c = cos(latRad);
    double t = s / c;
    double eta = e2Squared * (c * c);
    double sn = sphsn(a, eSquared, latRad);
    double tmd = sphtmd(ap, bp, cp, dp, ep, latRad);
    double t1 = tmd * ok;
    double t2 = sn * s * c * ok / 2.0;
    double t3 = sn * s * (c * c * c) * ok * (5.0 - (t * t) + 9.0 * eta + 4.0
                                             * (eta * eta)) / 24.0;
    double nfn = 0;
    if (latRad < 0.0) nfn = 10000000.0;
    utm.northing = nfn + t1 + (dlam * dlam) * t2 + (dlam * dlam * dlam
                                                    * dlam) * t3 + (dlam * dlam * dlam * dlam * dlam * dlam) + 0.5;
    double t6 = sn * c * ok;
    double t7 = sn * (c * c * c) * (1.0 - (t * t) + eta) / 6.0;
    utm.easting = fe + dlam * t6 + (dlam * dlam * dlam) * t7 + 0.5;
    if (utm.northing >= 9999999.0) utm.northing = 9999999.0;
    
    return utm;
}

//  In meters
//
double Location::getDistanceTo(Location _loc) {
    UtmPosition me = getUTM();
    UtmPosition other = _loc.getUTM();
    double horizontalDist = (double) fabs(other.easting - me.easting);
    double verticalDist = (double) fabs(other.northing - me.northing);
    
    return sqrt(horizontalDist * horizontalDist + verticalDist * verticalDist);
}

//  In degrees
//
double Location::getRadTo(Location _loc) {
//    double x = _loc.lon - lon;
//    double y = _loc.lat - lat;
//    return atan2(y,x);    
    double dLon = _loc.lon - lon;
    double y = sin(dLon) * cos(_loc.lat);
    double x = cos(lat) * sin(_loc.lat) - sin(lat) * cos(_loc.lat) * cos(dLon);
    return atan2(y, x)+HALF_PI;
}

double Location::getDegTo(Location _loc) {
    double angle = getRadTo(_loc)-PI;
    angle *=RAD_TO_DEG;
    angle =angle+90.0;
    if(angle > 0){
        return 360.0 - angle;
    } else {
        return abs(angle);
    }
}

ofQuaternion Location::getQuaternion(){
    ofQuaternion latRot;
    latRot.makeRotate( lat, 1, 0, 0);
    
    ofQuaternion longRot;
    longRot.makeRotate( lon, 0, 1, 0);
    
    ofQuaternion quaternion = latRot * longRot;
    
    return latRot*longRot;
};

ofPoint Location::getSpherePosition(float _radius){
    return getQuaternion() * ofVec3f(0,0, _radius);
};