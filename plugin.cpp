/*
 * Fledge "alexa" notification plugin.
 *
 * Copyright (c) 2019 Dianomic Systems
 *
 * Released under the Apache 2.0 Licence
 *
 * Author: Mark Riddoch
 */

#include <string>
#include <plugin_api.h>
#include <config_category.h>
#include <logger.h>
#include <alexa.h>
#include <version.h>

#define PLUGIN_NAME "alexa"

#define DEFAULT_CONFIG "{\"plugin\" : { \"description\" : \"Alexa notification plugin\", " \
                       		"\"type\" : \"string\", " \
				"\"default\" : \"" PLUGIN_NAME "\", \"readonly\" : \"true\" }, " \
			 "\"enable\": {\"description\": \"A switch that can be used to enable or disable execution of " \
					 "the Alexa notification plugin.\", " \
				"\"type\": \"boolean\", " \
				"\"displayName\" : \"Enabled\", " \
				"\"default\": \"false\" }, " \
			"\"accessCode\" : {\"description\" : \"Alexa NotifyMe Access Code\", " \
				"\"type\" : \"string\", " \
				"\"default\" : \"\", " \
				"\"order\" : \"1\", \"displayName\" : \"Access Code \"}, " \
			"\"title\" : {\"description\" : \"Title for notification\", " \
				"\"type\" : \"string\", " \
				"\"default\" : \"\", " \
				"\"order\" : \"2\", \"displayName\" : \"Title\"} }"

using namespace std;
using namespace rapidjson;

/**
 * The Notification plugin interface
 */
extern "C" {

/**
 * The plugin information structure
 */
static PLUGIN_INFORMATION info = {
        PLUGIN_NAME,              // Name
        VERSION,                  // Version
        0,                        // Flags
        PLUGIN_TYPE_NOTIFICATION_DELIVERY,       // Type
        "1.0.0",                  // Interface version
        DEFAULT_CONFIG	          // Default plugin configuration
};

/**
 * Return the information about this plugin
 */
PLUGIN_INFORMATION *plugin_info()
{
	return &info;
}


/**
 * Initialise the plugin, called to get the plugin handle and setup the
 * plugin configuration
 *
 * @param config	The configuration category for the plugin
 * @return		An opaque handle that is used in all subsequent calls to the plugin
 */
PLUGIN_HANDLE plugin_init(ConfigCategory* config)
{
	Alexa *alexa = new Alexa(config);
	
	return (PLUGIN_HANDLE)alexa;
}

/**
 * Deliver received notification data
 *
 * @param handle		The plugin handle returned from plugin_init
 * @param deliveryName		The delivery category name
 * @param notificationName	The notification name
 * @param triggerReason		The trigger reason for notification
 * @param message		The message from notification
 */
bool plugin_deliver(PLUGIN_HANDLE handle,
                    const std::string& deliveryName,
                    const std::string& notificationName,
                    const std::string& triggerReason,
                    const std::string& message)
{
	Logger::getLogger()->info("Alexa notification plugin_deliver(): deliveryName=%s, notificationName=%s, triggerReason=%s, message=%s",
							deliveryName.c_str(), notificationName.c_str(), triggerReason.c_str(), message.c_str());
	Alexa *alexa = (Alexa *)handle;
	return alexa->notify(notificationName, triggerReason, message);
}

/**
 * Reconfigure the plugin
 */
void plugin_reconfigure(PLUGIN_HANDLE *handle, string& newConfig)
{
	Logger::getLogger()->info("Alexa notification plugin: plugin_reconfigure()");
	Alexa *alexa = (Alexa *)handle;
	
	alexa->reconfigure(newConfig);
	return;
}

/**
 * Call the shutdown method in the plugin
 */
void plugin_shutdown(PLUGIN_HANDLE *handle)
{
	Alexa *alexa = (Alexa *)handle;
	delete alexa;
}

// End of extern "C"
};

