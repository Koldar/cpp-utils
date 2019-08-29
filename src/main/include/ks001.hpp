#ifndef _KS001_HEADER__
#define _KS001_HEADER__

#include <string>
#include <unordered_map>
#include <vector>

namespace cpp_utils {

class KS001 {
public:
    KS001(char colon=':', char pipe='|', char underscore='_', char equal='=') : colon{colon}, pipe{pipe}, underscore{underscore}, equal{equal}, identifier{nullptr}, dicts{} {

    }
    virtual ~KS001();
    void set_identifier(const std::string& identifier);
    const std::string* get_identifier() const;
    
    void add_key_value(const std::string& name, const std::string& key, const std::string& value);
    void add_key_value(int index, const std::string& key, const std::string& value);
    std::string toString() const;
private:
    char colon;
    char pipe;
    char underscore;
    char equal;
    std::string* identifier;
    std::vector<std::pair<std::string*, std::unordered_map<std::string, std::string>>> dicts;
};

}

#endif