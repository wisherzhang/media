/*
 * config.cpp
 */
#include "config.h"
using namespace std;
bool IsSpace(char c)
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}


bool IsCommentChar(char c)
{
    switch(c) {
    case COMMENT_CHAR:
        return true;
    default:
        return false;
    }
}


///���ִ���β���˵Ŀո��Ƴ�
void Trim(string & str)
{
    if (str.empty()) {
        return;
    }
    string::size_type i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // ȫ���ǿհ��ַ���
        str = "";
        return;
    }
    
    start_pos = i;
    
    for (i = str.size() - 1; i >= 0; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
}


bool AnalyseLine(const string & line, string & key, string & value)
{
    if (line.empty())
        return false;
    string::size_type start_pos = 0, end_pos = line.size() - 1, pos = string::npos;
    if ((pos = line.find(COMMENT_CHAR)) != string::npos) {
        if (0 == pos) {  // �еĵ�һ���ַ�����ע���ַ�
            return false;
        }
        end_pos = pos - 1;
    }
    string new_line = line.substr(start_pos, end_pos+1-start_pos);  // Ԥ����ɾ��ע�Ͳ���
    
    if ((pos = new_line.find('=')) == string::npos)
        return false;  // û��=��
        
    string sKey = new_line.substr(0, pos);
    Trim(sKey);
    if (0 != sKey.compare(key))
       return false;

    value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));
    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(value);
    return true;
}
Config :: Config(const string filename) : m_filename(filename)
{
	///ÿ�ε��ù��캯����ʱ��ʹ��ļ�
	m_infile.open(m_filename.c_str());
}
Config :: Config(const Config& c) : m_filename(c.m_filename)
{

}
Config :: ~Config()
{
	///��������ʱ��ر��ļ�
	if(m_infile)
	{
		m_infile.close();
	}
}
bool Config::ReadConfig(const string& key, string& keyvalue)
{
	if (!m_infile) {
		return false;
	}
    string line, key_,value;
    key_ = key;
    while (getline(m_infile, line)) {
        if (AnalyseLine(line, key_, value)) {
        keyvalue = value;
        return true;
        }
    }
    return false;
}