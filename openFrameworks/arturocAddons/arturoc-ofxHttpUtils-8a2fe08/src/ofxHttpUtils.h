/*
    ofxHttpUtils v0.3
    Chris O'Shea, Arturo, Jesus, CJ

    Modified: 16th March 2009
    openFrameworks 0.06

*/

#ifndef _OFX_HTTP_UTILS
#define _OFX_HTTP_UTILS

#include "ofMain.h"

#include "ofxThread.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPBasicCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"

#include <iostream>
#include <queue>
#include <istream>

using namespace Poco::Net;
using namespace Poco;

class ofxHttpListener;
class ofxHttpEventManager;

#include "ofxHttpTypes.h"

struct ofxHttpResponse{
	ofxHttpResponse(HTTPResponse& pocoResponse, std::istream &bodyStream, string turl){
		status=pocoResponse.getStatus();
		timestamp=pocoResponse.getDate();
		reasonForStatus=pocoResponse.getReasonForStatus(pocoResponse.getStatus());
		contentType = pocoResponse.getContentType();

		StreamCopier::copyToString(bodyStream, responseBody);
        url = turl;
        pocoResponse.getCookies(cookies);
	}

	ofxHttpResponse(){}

	int status; 				// return code for the response ie: 200 = OK
	string reasonForStatus;		// text explaining the status
	string responseBody;		// the actual response
	string contentType;			// the mime type of the response
	Poco::Timestamp timestamp;		// time of the response
	string url;
	vector<HTTPCookie> cookies;
};

class ofxHttpUtils : public ofxThread{

	public:

		ofxHttpUtils();
		~ofxHttpUtils();
		//-------------------------------
		// non blocking functions

		void addForm(ofxHttpForm form);
		void addUrl(string url);

		//-------------------------------
		// blocking functions
		int submitForm(ofxHttpForm form);
		void getUrl(string url);

        int getQueueLength();
        void clearQueue();

		//-------------------------------
		// threading stuff
		void threadedFunction();

		//------------------------------
		// events
		ofEvent<ofxHttpResponse> newResponseEvent;
		ofEvent<ofxHttpResponse> errorEvent; //<- Added by Chris Mullany

        // other stuff-------------------
        void setTimeoutSeconds(int t){
            timeoutSeconds = t;
        }
        void setVerbose(bool v){
            verbose = v;
        }

        void sendReceivedCookies();

        void setBasicAuthentication(string user, string password);


		void start();
        void stop();

    protected:

		bool verbose;
        int timeoutSeconds;
        bool sendCookies;

		//--------------------------------
		// http utils
		string generateUrl(ofxHttpForm & form);
		int doPostForm(ofxHttpForm & form);

		std::queue<ofxHttpForm> forms;
		vector<HTTPCookie> cookies;
		HTTPBasicCredentials auth;


};
#endif
