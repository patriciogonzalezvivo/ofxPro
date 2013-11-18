//
//  FileUploader.h
//  example
//
//  Created by Patricio Gonzalez Vivo on 11/17/13.
//
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

class FileUploader: public ofThread{
public:
    
    FileUploader():req(HTTPMessage::HTTP_1_0){
        result = "";
        fps = NULL;
        bFinish = true;
    }
    
    virtual ~FileUploader(){
        waitForThread(true);
    }
    
    void start(){
        req.setMethod(HTTPRequest::HTTP_POST);
        req.setURI("/services/upload/");
        req.setContentType("multipart/form-data");
        
        form.prepareSubmit( req);
        form.write( oszMessage);
        szMessage = oszMessage.str();
        req.setContentLength( (int)szMessage.length() );
        
        bFinish = false;
        startThread(true, false);
    };
    
    bool isDownloading(){
        if( lock() ){
            return  (result == "");
            unlock();
        } else {
            return false;
        }
    };
    
    ofEvent<string> isDone;
    
    FilePartSource * fps;
    HTTPClientSession session;
    HTTPRequest req;
    HTMLForm form;
    std::ostringstream oszMessage;
    std::string szMessage;
    
    bool    bFinish;
    
protected:
    void threadedFunction(){
        while( isThreadRunning() != 0 ){
            
            ostream & out = session.sendRequest(req) << szMessage;
            
            // get response
            HTTPResponse res;
            cout << res.getStatus() << " " << res.getReason() << endl;
            
            // print response
            istream &is = session.receiveResponse(res);
            StreamCopier::copyToString(is, result);
            
            stopThread();
            bFinish = true;
            ofNotifyEvent(isDone,result,this);
        }
    };
    
    std::string result;
};