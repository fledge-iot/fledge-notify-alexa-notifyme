#ifndef _ALEXA_H
#define _ALEXA_H
#include <config_category.h>
#include <string>
#include <logger.h>
#include <mutex>

#define NOTIFYME_URL	"https://api.notifymyecho.com/v1/NotifyMe"

/**
 * A simple Alexa notification class that sends a message
 * via the NotifyMe skill to Amazon Alexa devices
 */
class Alexa {
	public:
		Alexa(ConfigCategory *config);
		~Alexa();
		void	notify(const std::string& notificationName, const std::string& triggerReason, const std::string& message);
		void	reconfigure(const std::string& newConfig);
	private:
		std::string	m_accessCode;
		std::string	m_title;
		std::mutex	m_mutex;
};
#endif
