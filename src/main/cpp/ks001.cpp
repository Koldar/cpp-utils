#include "ks001.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace cpp_utils {

KS001::~KS001() {
    if (this->identifier != nullptr) {
        delete this->identifier;
    }
    for (std::pair<std::string*, std::unordered_map<std::string, std::string>> pair : this->dicts) {
        if (pair.first != nullptr) {
            delete pair.first;
        }
    }
}

void KS001::set_identifier(const std::string& identifier) {
    this->identifier = new std::string{identifier};
}

const std::string* KS001::get_identifier() const {
    return this->identifier;
}

void KS001::add_key_value(const std::string& name, const std::string& key, const std::string& value) {
    std::unordered_map<std::string, std::string>* dict = nullptr;
    for (std::pair<std::string*, std::unordered_map<std::string, std::string>> pair : this->dicts) {
        if (pair.first != nullptr && (*pair.first == name)) {
            dict = &pair.second;
            dict->insert(std::pair<std::string, std::string>{key,value});
            return;
        }
    }

    if (dict == nullptr) {
        auto pair = std::pair<std::string*, std::unordered_map<std::string, std::string>>{new std::string{name}, std::unordered_map<std::string, std::string>{}};
        dict = &pair.second;
        dict->insert(std::pair<std::string, std::string>{key,value});
        this->dicts.push_back(pair);
    }
    
}

void KS001::add_key_value(int index, const std::string& key, const std::string& value) {
    std::unordered_map<std::string, std::string>* dict = nullptr;
    if (this->dicts.size() <= index) {
        auto pair = std::pair<std::string*, std::unordered_map<std::string, std::string>>{nullptr, std::unordered_map<std::string, std::string>{}};
        dict = &pair.second;
        dict->insert(std::pair<std::string, std::string>{key,value});
        this->dicts.push_back(pair);
    } else {
        dict = &this->dicts[index].second;
        dict->insert(std::pair<std::string, std::string>{key,value});
    }
}

std::string KS001::toString() const {
    std::stringstream ss;

    if (this->identifier != nullptr) {
        ss << *this->identifier;
    }
    ss << this->pipe;
    for (std::pair<std::string*, std::unordered_map<std::string, std::string>> pair : this->dicts) {
        if (pair.first != nullptr) {
            ss << pair.first << this->colon;
        }
        //put keys into a vector and sort the vector
        std::vector<std::string> sortedKeys{};
        for (auto it=pair.second.begin(); it!=pair.second.end(); ++it) {
            sortedKeys.push_back(it->first);
        }
        std::sort(sortedKeys.begin(), sortedKeys.end());
        int i = 0;
        for (auto key : sortedKeys) {
            ss << key << this->equal << pair.second[key];
            if ((i+1) < sortedKeys.size()) {
                ss << this->underscore;
            }
            i++;
        }
        ss << this->pipe;

    }
    return ss.str();
}

}