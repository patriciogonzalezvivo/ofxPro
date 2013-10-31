//
//  Flickr.h
//
//  Created by Brett Renfer on 12/1/12.
//

#pragma once

#include "ofMain.h"
#include "md5.h"
#include "ofxXmlSettings.h"

// Poco stuff
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/StreamCopier.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

using namespace Poco::Net;
using namespace Poco;

namespace Flickr {

    enum Permissions {
        FLICKR_READ = 0,
        FLICKR_WRITE,
        FLICKR_DELETE,
        FLICKR_NONE
    };
    
    enum Format {
        FLICKR_XML = 0,
        FLICKR_JSONP,
        FLICKR_JSON
    };
    
    enum Size {
        FLICKR_SIZE_DEFAULT = 0,
        FLICKR_SIZE_ORIGINAL,
        FLICKR_SIZE_S,
        FLICKR_SIZE_Q,
        FLICKR_SIZE_T,
        FLICKR_SIZE_M,
        FLICKR_SIZE_N,
        FLICKR_SIZE_Z,
        FLICKR_SIZE_C,
        FLICKR_SIZE_B,
    };
    
    static string getSizeURLParam( Size size );
    
    enum MediaType {
        FLICKR_PHOTO = 0,
        FLICKR_VIDEO,
        FLICKR_UNKNOWN
    };
    
    /*
     From Flickr API:
     http://www.flickr.com/services/api/misc.urls.html
     
     default	medium, 500 on longest side
     o          original image, either a jpg, gif or png, depending on source format
     s          small square 75x75
     q          large square 150x150
     t          thumbnail, 100 on longest side
     m          small, 240 on longest side
     n          small, 320 on longest side
     z          medium 640, 640 on longest side
     c          medium 800, 800 on longest side 
     b          large, 1024 on longest side*

     */

    static const std::string api_base = "api.flickr.com";
    static const std::string auth_base = "http://flickr.com/services/auth/";
    
    // %a = API key, %b = perms, %c = frob, %d = api_sig
    static const std::string auth = "http://flickr.com/services/auth/?api_key=%a&perms=%b&frob=%c&api_sig=%d";
    
    class Media {
    public:
        
        Media();
        
        // stuff for making url
        string  id, farm, secret, server, originalsecret, originalformat;
        // more info
        string  title;
        string  isfavorite, license;
        int     rotation;
        
        void    loadFromXML( string XML );
        string  getURL( Size size = FLICKR_SIZE_DEFAULT );
        string  getShortURL();
        
        MediaType getType();
        
        // usually photo
        MediaType type;
        
    };
    
    enum SearchMode {
        FLICKR_SEARCH_ANY = 0, // default
        FLICKR_SEARCH_ALL
    };
    
    enum Sort {
        FLICKR_SORT_DATE_POSTED_DESC = 0, // default
        FLICKR_SORT_DATE_POSTED_ASC,
        FLICKR_SORT_DATE_TAKEN_DESC,
        FLICKR_SORT_DATE_TAKEN_ASC,
        FLICKR_SORT_INTERESTINGNESS_DESC,
        FLICKR_SORT_INTERESTINGNESS_ASC,
        FLICKR_SORT_RELEVANCE
    };
    
    static string getSortString( Sort sort );
    
    enum PrivacyFilter {
        FLICKR_PRIVACY_PUBLIC = 0,
        FLICKR_PRIVACY_FRIENDS,
        FLICKR_PRIVACY_FAMILY,
        FLICKR_PRIVACY_FRIENDS_FAMILY,
        FLICKR_PRIVACY_PRIVATE
    };
    
    static string getPrivacyString( PrivacyFilter filter );
    
    // see http://www.flickr.com/services/api/flickr.photos.search.html
    
    class Query {
    public:
        
        Query();
        
        bool requiresAuthentication();
        map<string,string> getQueryParameters();
        
        void addTag( string tag );
        string getTagString();
        
        // required
        string api_key;
        
        // optional
        string          text;
        string          user_id;
        Sort            sort;
        SearchMode      tagMode;
        PrivacyFilter   privacy;
        int             per_page, page;
        
        // to be implemented in better way (see API docs for now to use correct values)
        string min_upload_date, max_upload_date, min_taken_date, max_taken_date;
        string license;
        string boundingBox;
        string accuracy;
        string safe_search;
        string content_type;
        string machine_tags;
        string machine_tag_mode;
        string group_id;
        string contacts;
        string woe_id;
        string place_id;
        string media;
        string has_geo;
        string geo_context;
        string lat;
        string lon;
        string radius;
        string radius_units;
        string is_commons;
        string in_gallery;
        string is_getty;
        string extras;
        
    private:
        bool bNeedsAuth;
        vector<string> tags;
    };
    
    class API : public ofThread {
    public:
        
        API();
        
        /**
         * Authenticate with Flickr. Will open a browser asking you to allow access to your account.
         * One it does authenticate, will save a tiny XML file called flickr.xml to store your current auth_token
         * @param {std::string} api_key
         * @param {std::string} api_secret
         * @param {Flickr::Permissions} perms
         */
        bool authenticate( string api_key, string auth_token, Permissions perms = FLICKR_WRITE );
        
        /**
         * Check API token
         * @param {std::string} api_key
         * @param {std::string} auth_token
         * @param {Flickr::Permissions} perms
         */
        bool  checkAuthToken( string api_key, string auth_token, Permissions perms = FLICKR_WRITE );
        
        /**
         * Upload an image from disk
         * @returns {std::string} id of uploaded image. Use getMediaById to get Flickr::Media, which includes the URL, etc.
         */
        string  upload( string image );
        ofEvent<string> uploadComplete;
        
        /**
         * Get URL of photo by its ID (helpful after upload)
         * @param {std::string} id ID of photo you'd like to load into a Flickr::Media object
         */
        Media & getMediaById( string id );
        
        /**
         * Search photos!
         * @param {std::string} text Text to search
         */
        vector<Media> search( string text );
        
        /**
         * Search photos!
         * @param {Flickr::Query} query
         */
        vector<Media> search( Query query );
        
        bool                bAuthenticated;
        
    private:
        void    threadedFunction();
        string  fileToUpload;
        
        Permissions         currentPerms;
        ofURLFileLoader     dummyLoader; // this is needed because OF doesn't have a way to say "hey, we already made an HTTPStreamFactory!"
        map<string, Media>  loadedMedia;
        
        bool getAuthToken( Permissions perms );
        
        string apiSig( map<string, string> params );
        
        string makeAPICall( string method, map<string,string> args, Format format = FLICKR_XML, bool bSigned = false  );
        string buildAPICall( string method, map<string,string> args, Format format = FLICKR_XML, bool bSigned = false );
        string api_key, api_secret, frob, auth_token;
    };

}