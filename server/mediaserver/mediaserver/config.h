/**
 * @file config.h
 */


#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <fstream>
#define COMMENT_CHAR '#'
#define CONFIG_NAME "../config/config.ini"
class Config
{
public:
Config(const std::string filename);

Config(const Config& c);

~Config();

bool ReadConfig(const std::string& key, std::string& keyvalue);

private:
///ÎÄ¼þÃû³Æ
    std::string m_filename; 
    std::ifstream m_infile;
};
#endif