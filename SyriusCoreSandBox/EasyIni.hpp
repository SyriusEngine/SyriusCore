#pragma once

#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>


namespace EasyIni{

    class Element{
    public:
        Element(const std::string& name, const std::string& value)
                : m_Name(name),
                  m_Value(value){

        }

        ~Element(){

        }

        void save(std::ofstream& file){
            file << m_Name << " = " << m_Value << "\n";
        }

        [[nodiscard]] const std::string& getName() const{
            return m_Name;
        }

        [[nodiscard]] const std::string& getValue() const{
            return m_Value;
        }

        void setValue(const std::string& value){
            m_Value = value;
        }

        template<typename T>
        void operator=(const T& value){
            m_Value = std::to_string(value);
        }

        void operator=(const std::string& value){
            m_Value = value;
        }

        template<typename T>
        void operator=(const std::vector<T>& value){
            m_Value = "(";
            for (uint32_t i = 0; i < value.size(); i++) {
                m_Value += std::to_string(value[i]);
                if (i < value.size() - 1) {
                    m_Value += ", ";
                }
            }
            m_Value += ")";
        }

        void operator=(const std::vector<std::string>& value){
            m_Value = "(";
            for (uint32_t i = 0; i < value.size(); i++) {
                m_Value += value[i];
                if (i < value.size() - 1) {
                    m_Value += ", ";
                }
            }
            m_Value += ")";
        }

        template<typename T>
        [[nodiscard]] T get() const{
            std::stringstream ss(m_Value);
            T retVal;
            ss >> retVal;
            return retVal;
        }

        [[nodiscard]] std::string get() const{
            return m_Value;
        }

        template<typename T>
        [[nodiscard]] T getOrDefault(const T& def){
            if (m_Value.empty()) {
                m_Value = std::to_string(def);
                return def;
            }
            else{
                return get<T>();
            }
        }

        [[nodiscard]] std::string getOrDefault(const std::string& def){
            if (m_Value.empty()) {
                m_Value = def;
            }
            return m_Value;
        }

        template<typename T>
        [[nodiscard]] std::vector<T> getVector() const{
            std::vector<T> retVal;
            auto toErase = m_Value;
            toErase.erase(std::remove(toErase.begin(), toErase.end(), '('), toErase.end());
            toErase.erase(std::remove(toErase.begin(), toErase.end(), ')'), toErase.end());
            std::stringstream ll(toErase);
            while (ll.good()){
                std::string substr;
                getline(ll, substr, ',');
                T val;
                std::stringstream ss(substr);
                ss >> val;
                retVal.push_back(val);
            }
            return retVal;
        }

        template<typename T>
        [[nodiscard]] std::vector<T> getVectorOrDefault(const std::vector<T>& def){
            if (m_Value.empty()){
                m_Value = "(";
                for (uint32_t i = 0; i < def.size(); i++) {
                    m_Value += std::to_string(def[i]);
                    if (i < def.size() - 1) {
                        m_Value += ", ";
                    }
                }
                m_Value += ")";
                return def;
            }
            else{
                return getVector<T>();
            }
        }

    private:
        std::string m_Name;
        std::string m_Value;
    };

    class Section{
    public:

        explicit Section(const std::string& name)
                : m_Name(name){

        }

        ~Section(){

        }

        void save(std::ofstream& file){
            file << "[" << m_Name << "]\n";
            for(auto& element : m_Elements){
                element.save(file);
            }
            file << "\n";
        }

        [[nodiscard]] const std::string& getName() const{
            return m_Name;
        }

        void addElement(const std::string& name, const std::string& value){
            m_Elements.emplace_back(name, value);
        }

        Element& getElement(const std::string& name){
            uint64_t index = m_Elements.size();
            for (uint64_t i = 0; i < m_Elements.size(); i++){
                if (m_Elements[i].getName() == name){
                    index = i;
                    break;
                }
            }
            if (index == m_Elements.size()){
                m_Elements.emplace_back(name, "");
            }
            return m_Elements[index];
        }

        Element& operator[](const std::string& name){
            return getElement(name);
        }

    private:
        std::vector<Element> m_Elements;
        std::string m_Name;

    };

    class Configuration{
    public:
        explicit Configuration(const std::string& fileName)
                : m_FileName(fileName){
            std::ifstream file(fileName);
            if(file.is_open()){
                Section section("NULL");
                std::string line;
                while (std::getline(file, line)){
                    auto assignPos = line.find('=');
                    auto bracketOPos = line.find('[');
                    auto bracketCPos = line.find(']');
                    if (bracketOPos != std::string::npos and bracketCPos != std::string::npos){
                        std::string nodeName = line.substr(bracketOPos + 1, bracketCPos - 1);
                        if (section.getName() != "NULL"){
                            m_Sections.push_back(section);
                        }
                        section = Section(nodeName);

                    }
                    else if (assignPos != std::string::npos){
                        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
                        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
                        std::string key = line.substr(0, assignPos - 1);
                        std::string entry = line.substr(assignPos, std::string::npos);
                        section.addElement(key, entry);
                    }
                }
                if (section.getName() != "NULL"){
                    m_Sections.push_back(section);
                }
            }
        }

        ~Configuration(){

        }

        void save(){
            save(m_FileName);
        }

        void save(const std::string& fileName){
            std::ofstream file(fileName);
            for (auto& section : m_Sections){
                section.save(file);
            }
            file.close();
        }

        void addSection(const std::string& name){
            m_Sections.emplace_back(name);
        }

        Section& getSection(const std::string& name){
            uint64_t index = m_Sections.size();
            for (uint64_t i = 0; i < m_Sections.size(); i++){
                if (m_Sections[i].getName() == name){
                    index = i;
                    break;
                }
            }
            if (index == m_Sections.size()){
                m_Sections.emplace_back(name);
            }
            return m_Sections[index];
        }

        Section& operator[](const std::string& name){
            return getSection(name);
        }

    private:
        std::vector<Section> m_Sections;
        std::string m_FileName;

    };
}