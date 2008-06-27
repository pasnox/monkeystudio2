#ifndef QTASSISTANTCONFIG_H
#define QTASSISTANTCONFIG_H

class Config;
class QtAssistant;

class QtAssistantConfig
{
public:
	static Config* instance( QtAssistant* plugin );
};

#endif // QTASSISTANTCONFIG_H
