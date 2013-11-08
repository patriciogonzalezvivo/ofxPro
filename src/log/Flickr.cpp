//
//  Flickr.cpp
//
//  Created by Brett Renfer on 12/1/12.
//

#include "Flickr.h"

namespace Flickr {
    
#pragma mark Flickr::Media
    
    Media::Media(){
        id = "";
        farm = "";
        secret = "";
        server = "";
        originalsecret = "";
        originalformat = "";
        type = FLICKR_UNKNOWN;
        rotation = 0;
    }
    
    //--------------------------------------------------------------
    void Media::loadFromXML( string XML ){
        cout << XML << endl;
        ofxXmlSettings xml;
        xml.loadFromBuffer(XML);
        xml.pushTag("rsp");{
            id = xml.getAttribute("photo", "id", "");
            farm = xml.getAttribute("photo", "farm", "");
            secret = xml.getAttribute("photo", "secret", "");
            server = xml.getAttribute("photo", "server", "");
            originalsecret = xml.getAttribute("photo", "originalsecret", "");
            originalformat = xml.getAttribute("photo", "originalformat", "");
            
            string t = xml.getAttribute("photo", "media", "");
            if ( t == "photo"){
                type = FLICKR_PHOTO;
            } else if ( t == "video"){
                type = FLICKR_VIDEO;
            } else {
                type = FLICKR_UNKNOWN;
            }
        } xml.popTag();
        
        cout << getURL() << endl;
        // to-do: other stuff
    }
    
    //--------------------------------------------------------------
    string Media::getURL( Size size ){
        if ( size == FLICKR_SIZE_DEFAULT ){
            return "http://farm" + farm +".staticflickr.com/" + server +"/" + id +"_" + secret +".jpg";
        } else if ( size == FLICKR_SIZE_ORIGINAL ){
            return "http://farm" + farm +".staticflickr.com/" + server +"/" + id +"_" + originalsecret +"_o." + originalformat;
        } else {
            return "http://farm" + farm +".staticflickr.com/" + server +"/" + id +"_" + secret +"_" + getSizeURLParam( size ) +".jpg";
        }
    }
    
    string Media::getShortURL(){
        //http://www.flickr.com/groups/api/discuss/72157616713786392/
        //
        uint64 id_num = 0;
        istringstream cur(id);
        cur >> id_num;
        
        string alphabet = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
        uint64 base_count = 58;
        string toBase58 = "";
        
        while (id_num > 0) {
            uint64 num = id_num / base_count;
            uint64 remainder = id_num % base_count;
            toBase58 += alphabet[remainder];
            id_num = num;
        }
        
        string encoded = "";
        for (int i = toBase58.length()-1; i >= 0 ; i--) {
            encoded += toBase58[i];
        }
        
        return "http://flic.kr/p/" + encoded;
    }
    
    
    //--------------------------------------------------------------
    MediaType Media::getType(){
        return type;
    }
    
    //--------------------------------------------------------------
    string getSizeURLParam( Size size ){
        switch (size) {
            case FLICKR_SIZE_S:
                return "s";
                break;
            case FLICKR_SIZE_Q:
                return "q";
                break;
            case FLICKR_SIZE_T:
                return "t";
                break;
            case FLICKR_SIZE_M:
                return "m";
                break;
            case FLICKR_SIZE_N:
                return "n";
                break;
            case FLICKR_SIZE_Z:
                return "z";
                break;
            case FLICKR_SIZE_C:
                return "c";
                break;
            case FLICKR_SIZE_B:
                return "b";
                break;
            default:
                return "o";
                break;
        }
    }
    
    //--------------------------------------------------------------
    string getSortString( Sort sort ){
        switch (sort) {
            case FLICKR_SORT_DATE_POSTED_ASC:
                return "date-posted-asc";
                break;
            case FLICKR_SORT_DATE_POSTED_DESC:
                return "date-posted-desc";
                break;
            case FLICKR_SORT_DATE_TAKEN_DESC:
                return "date-taken-desc";
                break;
            case FLICKR_SORT_DATE_TAKEN_ASC:
                return "date-taken-asc";
                break;
            case FLICKR_SORT_INTERESTINGNESS_DESC:
                return "interestingness-desc";
                break;
            case FLICKR_SORT_INTERESTINGNESS_ASC:
                return "interestingness-asc";
                break;
            case FLICKR_SORT_RELEVANCE:
                return "relevance";
                break;
        }
    }
    
    //--------------------------------------------------------------
    string getPrivacyString( PrivacyFilter filter ){
        switch (filter) {
            case FLICKR_PRIVACY_PUBLIC:
                return "1";
                break;
            case FLICKR_PRIVACY_FRIENDS:
                return "2";
                break;
            case FLICKR_PRIVACY_FAMILY:
                return "3";
                break;
            case FLICKR_PRIVACY_FRIENDS_FAMILY:
                return "4";
                break;
            case FLICKR_PRIVACY_PRIVATE:
                return "5";
                break;
        }
        
    }
    
#pragma mark Flickr::Query
    
    //--------------------------------------------------------------
    Query::Query():
    tagMode(FLICKR_SEARCH_ANY),
    sort(FLICKR_SORT_DATE_POSTED_DESC),
    privacy(FLICKR_PRIVACY_PUBLIC),
    per_page(100),
    page(1),
    bNeedsAuth(false)
    {
        text = user_id = min_upload_date = max_upload_date = min_taken_date = max_taken_date = license = boundingBox ="";
        license = boundingBox= accuracy = safe_search = content_type = machine_tags = machine_tag_mode = group_id = contacts = woe_id = place_id = media = has_geo = geo_context = lat = lon = radius = radius_units = is_commons = in_gallery = is_getty = extras = "";
        api_key = "";
    }
    
    //--------------------------------------------------------------
    map<string,string> Query::getQueryParameters(){
        map<string,string> params;
        params["api_key"]   = api_key;
        if ( text != "" )       params["text"]          = text;
        if ( user_id != "" )    params["user_id"]       = text;
        
        params["sort"]              = getSortString( sort );
        params["privacy_filter"]    = getPrivacyString( privacy );
        params["per_page"]          = ofToString( per_page );
        params["page"]              = ofToString( page );
        
        if ( min_upload_date != "" )    params["min_upload_date"]       = min_upload_date;
        if ( max_upload_date != "" )    params["max_upload_date"]       = max_upload_date;
        if ( min_taken_date != "" )     params["min_taken_date"]        = min_taken_date;
        if ( max_taken_date != "" )     params["max_taken_date"]        = max_taken_date;
        if ( license != "" )            params["license"]               = license;
        if ( boundingBox != "" )        params["boundingBox"]           = boundingBox;
        if ( accuracy != "" )           params["accuracy"]              = accuracy;
        if ( safe_search != "" )        params["safe_search"]           = safe_search;
        if ( content_type != "" )       params["content_type"]          = content_type;
        if ( machine_tags != "" )       params["machine_tags"]          = machine_tags;
        if ( machine_tag_mode != "" )   params["machine_tag_mode"]      = machine_tag_mode;
        if ( group_id != "" )           params["group_id"]              = group_id;
        if ( contacts != "" )           params["contacts"]              = contacts;
        if ( woe_id != "" )             params["woe_id"]                = woe_id;
        if ( place_id != "" )           params["place_id"]              = place_id;
        if ( media != "" )              params["media"]                 = media;
        if ( has_geo != "" )            params["has_geo"]               = has_geo;
        if ( lat != "" )                params["lat"]                   = lat;
        if ( lon != "" )                params["lon"]                   = lon;
        if ( radius != "" )             params["radius"]                = radius;
        if ( radius_units != "" )       params["radius_units"]          = radius_units;
        if ( is_commons != "" )         params["is_commons"]            = is_commons;
        if ( in_gallery != "" )         params["in_gallery"]            = in_gallery;
        if ( is_getty != "" )           params["is_getty"]              = is_getty;
        if ( extras != "" )             params["extras"]                = extras;
        
        return params;
    }
    
    //--------------------------------------------------------------
    bool Query::requiresAuthentication(){
        return privacy != FLICKR_PRIVACY_PUBLIC || user_id == "me";
    }
    
    //--------------------------------------------------------------
    void Query::addTag( string tag ){
        tags.push_back(tag);
    }
    
    //--------------------------------------------------------------
    string Query::getTagString(){
        string ret;
        for (int i=0; i<tags.size(); i++){
            ret += tags[i];
            if ( i + i < tags.size()){
                ret +=",";
            }
        }
        return ret;
    }
    
#pragma mark Flickr::API
    
    //--------------------------------------------------------------
    API::API() :
    bAuthenticated(false),
    currentPerms(FLICKR_NONE)
    {
//        api_key = "6394ea9fdcad0043386fbfd07f57a419";
//        api_secret = "abf7c1706ee0fd7f";
//        auth_token = "72157637127766253-d2060c92fa935409";
//        currentPerms = Flickr::FLICKR_WRITE;
//        bAuthenticated = true;
    };
    
    //--------------------------------------------------------------
    bool API::authenticate( string _api_key, string _api_secret, Permissions perms  ){
        api_key = _api_key;
        api_secret = _api_secret;
        
        // try to load from xml
        ofxXmlSettings xml;
        bool bLoaded = xml.loadFile("flickr.xml");
        if ( bLoaded ){
            xml.pushTag("settings");{
                auth_token = xml.getValue("token", "");
                if (auth_token == "" ){
                    bLoaded = false;
                }
            }; xml.popTag();
        }
        
        // not loaded, definitely need to get one
        if ( !bLoaded ) {
            bAuthenticated = getAuthToken( perms );
        } else {
            // loaded, might not be valid! check to see if we need to load
            if ( !checkAuthToken( api_key, auth_token, perms ) ){
                bAuthenticated = getAuthToken( perms );
            } else {
                bAuthenticated = true;
            }
        }
        
        if ( bAuthenticated ) {
            currentPerms = perms;
        } else {
            currentPerms = FLICKR_NONE;
        }
        
        return bAuthenticated;
    }
    
    //--------------------------------------------------------------
    bool API::checkAuthToken( string api_key, string auth_token, Permissions perms ){
        // build call
        
        cout << "CHECKING" << endl;
        map<string,string> args;
        args["api_key"]     = api_key;
        args["auth_token"]  = auth_token;
        
        // get frob
        string result = makeAPICall( "flickr.auth.checkToken", args, FLICKR_XML, true );
        ofxXmlSettings xml;
        xml.loadFromBuffer(result);
        string status = xml.getAttribute("rsp", "stat", "");
        
        bool bNeedNewToken  = true;
        string perm         = "";
        
        if (status == "ok"){
            xml.pushTag("rsp");{
                xml.pushTag("auth");{
                    perm = xml.getValue("perms", "");
                }; xml.popTag();
            }; xml.popTag();
            
            cout << perm << endl;
            
            // check if we have permissions
            switch (perms) {
                case FLICKR_WRITE:
                    if ( perm == "write" || perm == "delete") bNeedNewToken = false;
                    break;
                case FLICKR_READ:
                    if ( perm == "read") bNeedNewToken = false;
                    break;
                case FLICKR_DELETE:
                    if ( perm == "delete") bNeedNewToken = false;
                    break;
            }
        } else {
            cout << result << endl;
        }
        return !bNeedNewToken;
    }
    
    //--------------------------------------------------------------
    bool API::getAuthToken( Permissions perms ){
        // build call
        map<string,string> args;
        args["api_key"] = api_key;
        
        // get frob
        string result = makeAPICall( "flickr.auth.getFrob", args, FLICKR_XML, true );
        
        ofxXmlSettings xml;
        xml.loadFromBuffer(result);
        xml.pushTag("rsp");{
            frob = xml.getValue("frob", "");
        }; xml.popTag();
        
        // authenticate
        
        // %a = API key, %b = perms, %c = frob, %d = api_sig
        string authURL = auth;
        ofStringReplace(authURL, "%a", api_key);
        string perm = "write";
        
        switch (perms) {
            case FLICKR_WRITE:
                perm = "write";
                break;
            case FLICKR_READ:
                perm = "read";
                break;
            case FLICKR_DELETE:
                perm = "delete";
                break;
        }
        map<string,string> toEncode;
        toEncode["api_key"] = api_key;
        toEncode["perms"]   = perm;
        toEncode["frob"]    = frob;
        
        ofStringReplace(authURL, "%b", perm);
        ofStringReplace(authURL, "%c", frob);
        ofStringReplace(authURL, "%d", apiSig(toEncode));
        
        // this part is weird! ofLaunchBrowser has a tiny bug
#ifdef TARGET_OSX
		string commandStr = "open '"+authURL +"'";
		system(commandStr.c_str());
#else
        ofLaunchBrowser(authURL);
#endif
        bool bValidToken = false;
        int  numSeconds  = 0;
        int  secondsWait = 2;
        
        for( numSeconds; numSeconds<30; numSeconds+=secondsWait ){
            map<string,string> auth_args;
            auth_args["api_key"]    = api_key;
            auth_args["frob"]       = frob;
            
            // get frob
            string auth_result = makeAPICall( "flickr.auth.getToken", auth_args, FLICKR_XML, true );
//            cout << auth_result << endl;
            xml.loadFromBuffer(auth_result);
            xml.pushTag("rsp"); {
                xml.pushTag("auth"); {
                    auth_token = xml.getValue("token", "");
                } xml.popTag();
            } xml.popTag();
            
            bValidToken = !(auth_token == "" );
            if ( bValidToken ) break;
            numSeconds += secondsWait;
            sleep(1);
        }
        
        if ( !bValidToken ){
            ofLogError( "OAuth didn't succeed. Maybe you took too long?");
            return false;
        }
        
        // save auth token to XML for safe keeping
        ofxXmlSettings toSave;
        toSave.addTag("settings");
        toSave.pushTag("settings");{
            toSave.addValue("token", auth_token);
        }; toSave.popTag();
        toSave.saveFile("flickr.xml");
        
        return true;
    }
    
    //--------------------------------------------------------------
    string API::apiSig( map<string, string> params ){
        string toEncode = api_secret;
        
        // concatenate errbody
        map<string,string>::iterator it = params.begin();
        for ( it; it != params.end(); ++it ){
            toEncode += it->first + it->second;
        }
        
        return md5( toEncode );
    }
    
    //-------------------------------------------------------------
    string API::makeAPICall( string method, map<string,string> args, Format format, bool bSigned  ){
        string path     = buildAPICall( method, args, format, bSigned );
        string result   = "";
        
        try
        {
            // Get REST style xml as string from flickr
            std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open( "http://" + api_base + path ));
            StreamCopier::copyToString(*pStr.get(), result);
        }
        catch (Exception &ex)
        {
            cerr << ex.displayText() << endl;
        }
        return result;
    }
    
    //-------------------------------------------------------------
    string API::buildAPICall( string method, map<string,string> args, Format format, bool bSigned ){
        string call = "/services/rest/?method=" + method;
        
        switch ( format ) {
            case FLICKR_XML:
                args["format"]          = "rest";
                break;
                
            case FLICKR_JSON:
                args["format"]          = "json";
                args["nojsoncallback"]  = "1";
                break;
                
            case FLICKR_JSONP:
                args["format"]          = "json";
                break;
        }
        
        map<string,string>::iterator it = args.begin();
        for ( it; it != args.end(); ++it ){
            call += "&" + it->first + "=" + it->second;
        }
        
        if ( bSigned ){
            args["method"] = method;
            call += "&api_sig=" + apiSig( args );
        }
        
        return call;
    }
    
    //-------------------------------------------------------------
    string API::upload( string image ){
        if ( !bAuthenticated ){
            ofLogWarning( "Not authenticated! Please call authenticate() with proper api key and secret" );
            return "";
        } else if ( currentPerms != FLICKR_WRITE ){
            ofLogWarning( "You do not have proper permissions to upload! Please call authenticate() with permissions of ofxFlickr::FLICKR_WRITE" );
            return "";
        }
        
        fileToUpload = image;
        startThread(true, false);
        return "1";
    }
    
    void API::threadedFunction(){
        while( isThreadRunning() != 0 ){
            
            map<string,string> args;
            args["api_key"] = api_key;
            args["auth_token"] = auth_token;
            
            string result;
            FilePartSource * fps = new FilePartSource(fileToUpload, "image/jpeg");
            
            try{
                // prepare session
                HTTPClientSession session( api_base );
                HTTPRequest req(HTTPRequest::HTTP_POST, "/services/upload/", HTTPMessage::HTTP_1_0);
                req.setContentType("multipart/form-data");
                
                // setup form
                HTMLForm form;
                form.set("api_key", api_key);
                form.set("auth_token", auth_token);
                form.set("api_sig", apiSig( args ));
                form.setEncoding(HTMLForm::ENCODING_MULTIPART);
                form.addPart("photo", fps);
                form.prepareSubmit(req);
                
                std::ostringstream oszMessage;
                form.write(oszMessage);
                std::string szMessage = oszMessage.str();
                
                req.setContentLength((int) szMessage.length() );
                
                session.setKeepAlive(true);
                
                // send form
                ostream & out = session.sendRequest(req) << szMessage;
                
                // get response
                HTTPResponse res;
                cout << res.getStatus() << " " << res.getReason() << endl;
                
                // print response
                istream &is = session.receiveResponse(res);
                StreamCopier::copyToString(is, result);
            }
            catch (Exception &ex)
            {
                cerr << "error? " + ex.displayText() <<endl;
            }
        
            string photoid;
            
            ofxXmlSettings xml;
            xml.loadFromBuffer(result);
            xml.pushTag("rsp");{
                photoid = xml.getValue("photoid", "");
            }; xml.popTag();
            
            ofNotifyEvent(uploadComplete,photoid,this);
            stopThread();
        }
    }
    
    //-------------------------------------------------------------
    vector<Media> API::search( string text ){
        Query q; q.text = text; q.api_key = api_key;
        return search(q);
    }
    
    //-------------------------------------------------------------
    vector<Media> API::search( Query query ){
        vector<Media> toReturn;
        if ( query.api_key == "" ) query.api_key = api_key;
        
        if ( query.requiresAuthentication() && !bAuthenticated ){
            ofLogWarning("You must authenticate to use some of these parameters!");
            return toReturn;
        }
        
        string result = makeAPICall("flickr.photos.search", query.getQueryParameters(), FLICKR_XML, query.requiresAuthentication());
        ofxXmlSettings xml; xml.loadFromBuffer(result);
        xml.pushTag("rsp");{
            xml.pushTag("photos"); {
                
                for (int i=0; i<xml.getNumTags("photo"); i++){
                    Media med;
                    
                    med.id = xml.getAttribute("photo", "id", "", i);
                    med.farm = xml.getAttribute("photo", "farm", "", i);
                    med.secret = xml.getAttribute("photo", "secret", "", i);
                    med.server = xml.getAttribute("photo", "server", "", i);
                    med.originalsecret = xml.getAttribute("photo", "originalsecret", "", i);
                    med.originalformat = xml.getAttribute("photo", "originalformat", "", i);
                    
                    string t = xml.getAttribute("photo", "media", "", i);
                    if ( t == "photo"){
                        med.type = FLICKR_PHOTO;
                    } else if ( t == "video"){
                        med.type = FLICKR_VIDEO;
                    } else {
                        med.type = FLICKR_UNKNOWN;
                    }
                    
                    toReturn.push_back(med);
                }
                
            } xml.popTag();
        } xml.popTag();
        return toReturn;
    }
    
    //-------------------------------------------------------------
    Media & API::getMediaById( string id ){
        if ( loadedMedia.count(id) < 1 ){
            // build call
            map<string,string> args;
            args["api_key"]     = api_key;
            args["photo_id"]    = id;
            
            // get frob
            string result = makeAPICall(  "flickr.photos.getInfo", args, FLICKR_XML );
            loadedMedia[id] = Media();
            loadedMedia[id].loadFromXML( result );
        }
        return loadedMedia[id];
    }
}
