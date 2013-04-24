#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define P(x) cout << x << endl

const string TAG_START_DELIMITER("[");
const string TAG_END_DELIMITER("]");
const string KEY_VALUE_DELIMITER("=");
const string COMMENT_WORD("#");

std::string trimWhiteSpace(const std::string& str)
{
	std::string::size_type start;
	for (std::string::size_type i = 0;i < str.size();++i)
	{
		if (' ' != str.at(i))
		{
			start = i;
			break;
		}
	}
	std::string::size_type end;
	for (std::string::size_type i = str.size() - 1;0 <= i;--i)
	{
		if (' ' != str.at(i))
		{
			end = i;
			break;
		}
	}
	return str.substr(start, end - start + 1);
}

bool parseLine(const vector<string> &vecLine, map<string, map<string, string>> &mapMapString)
{
	string strTag;
	for (auto it = vecLine.begin();it != vecLine.end();++it)
	{
		string::size_type index = it->find(TAG_START_DELIMITER);
		if (string::npos != index)
		{
			// tagの処理
			strTag = it->substr(index + 1, it->find(TAG_END_DELIMITER) - index - 1);
			continue;
		}
		// key valueの処理
		// tagが設定されていなければ無視する
		if (strTag.empty()) continue;
		index = it->find(KEY_VALUE_DELIMITER);
		// =がないものは無視する
		if (string::npos == index) continue;
		mapMapString[strTag][trimWhiteSpace(it->substr(0, index))] = trimWhiteSpace(it->substr(index + 1));
	}
	return true;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		P("no configure file path");
		return 1;
	}
	ifstream ifs(argv[1]);
	if (!ifs)
	{
		P("load file error");
		return 1;
	}
	vector<string> vecLine;
	string strLine;
	while (getline(ifs, strLine))
	{
		if (strLine.empty()) continue;
		// コメントは処理を飛ばす
		if (string::npos != strLine.find(COMMENT_WORD)) continue;
		vecLine.push_back(strLine);
	}
	ifs.close();


	typedef pair<string, map<string, string>> tag;
	typedef pair<string, string> stringValue;
	map<string, map<string, string>> mapMapString;
	if (false == parseLine(vecLine, mapMapString))
		return 1;

	for (auto it = mapMapString.begin();it != mapMapString.end();++it)
	{
		P(it->first);
		for (auto it_ = it->second.begin();it_ != it->second.end();++it_)
		{
			P("\t" << it_->first << ":" << it_->second);
		}
	}
	return 0;
}

