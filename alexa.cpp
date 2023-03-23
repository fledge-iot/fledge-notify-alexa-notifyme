/*
 * Fledge "alexa" notification delivery plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch           
 */
#include "alexa.h"
#include <logger.h>
#include <simple_https.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;


/**
 * Construct a alexa notification plugin
 *
 * @param category	The configuration of the plugin
 */
Alexa::Alexa(ConfigCategory *category)
{
	m_accessCode = category->getValue("accessCode");
	m_title = category->getValue("title");
}

/**
 * The destructure for the alexa plugin
 */
Alexa::~Alexa()
{
}

/**
 * Send a notification via the Alexa webhook
 *
 * @param notificationName 	The name of this notification
 * @param triggerReason		Why the notification is being sent
 * @param message		The message to send
 */
bool Alexa::notify(const string& notificationName, const string& triggerReason, const string& message)
{
	ostringstream   payload;
	SimpleHttps	*https;

	lock_guard<mutex> guard(m_mutex);
	payload << "{ \"notification\" : \"";
	payload << notificationName;
	payload << message;
	Document doc;
	doc.Parse(triggerReason.c_str());
	if (!doc.HasParseError() && doc.HasMember("reason"))
	{
		payload << "Notification has " << doc["reason"].GetString() << "\\n";
	}
	payload << "\",";
	payload << "\"accessCode\" : \"" << m_accessCode << "\",";
	payload << "\"title\" : \"" << m_title << "\"";
	payload << "}";

	std::vector<std::pair<std::string, std::string>> headers;
	pair<string, string> header = make_pair("Content-type", "application/json");
	headers.push_back(header);

	string url(NOTIFYME_URL);
	/**
	 * Extract host and port from URL
	 */
	size_t findProtocol = url.find_first_of(":");
	string protocol = url.substr(0,findProtocol);

	string tmpUrl = url.substr(findProtocol + 3);
	size_t findPort = tmpUrl.find_first_of(":");
	size_t findPath = tmpUrl.find_first_of("/");
	string port, hostName;
	if (findPort == string::npos)
	{
		hostName = tmpUrl.substr(0, findPath);
		https  = new SimpleHttps(hostName);
	}
	else
	{
		hostName = tmpUrl.substr(0, findPort);
		port = tmpUrl.substr(findPort + 1 , findPath - findPort -1);
		string hostAndPort(hostName + ":" + port);
		https  = new SimpleHttps(hostAndPort);
	}

	try {
		int resCode = https->sendRequest("POST", url, headers, payload.str());
	 	std::string strResCode = to_string(resCode);
         	if(strResCode[0] != '2')
         	{
			Logger::getLogger()->error("Failed to send notification to Alexa NotifyMe  %s, resCode %d", url.c_str(), resCode);
			return false;
	 	}
	} catch (exception &e) {
		Logger::getLogger()->error("Exception sending HTTP request: %s",
				e.what());
		return false;
	}
	delete https;
	return true;
}

/**
 * Reconfigure the alexa delivery plugin
 *
 * @param newConfig	The new configuration
 */
void Alexa::reconfigure(const string& newConfig)
{
	ConfigCategory category("new", newConfig);
	lock_guard<mutex> guard(m_mutex);
	m_accessCode = category.getValue("accessCode");
	m_title = category.getValue("title");
}
