#ifndef ROUTE_HPP
#define ROUTE_HPP




#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <Poco/StringTokenizer.h>
#include <regex.h>

namespace hi {

    class route {
    private:

        class route_element {
        public:
            route_element() = delete;

            route_element(const std::string& method, const std::string& pattern, const std::string& class_name) :
            method(method), pattern(pattern), class_name(class_name) {
            }
            virtual ~route_element() = default;

            route_element(const route_element& other) :
            method(other.method), pattern(other.pattern), class_name(other.class_name) {
            }

            route_element& operator=(const route_element& right) {
                if (this == &right) {
                    return *this;
                } else {
                    this->method = right.method;
                    this->pattern = right.pattern;
                    this->class_name = right.class_name;
                }

                return *this;
            }

            const std::string& get_method()const {
                return this->method;
            }

            const std::string& get_pattern()const {
                return this->pattern;
            }

            const std::string& get_class_name()const {
                return this->class_name;
            }
        private:

            std::string method, pattern, class_name;
        };
    public:
        route() = delete;
        virtual~route() = default;
        route(const route& other) = delete;
        route& operator=(const route& right) = delete;

        route(const std::string& path)
        : route_path(path), route_data() {
            this->init();
        }

        size_t size()const {
            return this->route_data.size();
        }

        const route_element& at(size_t i)const {
            return this->route_data[i];
        }

    public:

        void update() {
            this->route_data.clear();
            this->init();
        }

        std::string get_route(const std::string& method, const std::string& path, std::vector<std::string>& route_part) {
            std::string result;
            for (auto &item : this->route_data) {
                const std::string& M = item.get_method();
                const std::string& P = item.get_pattern();
                const int n_matches = 10;
                int status = 0, nomatch = 0;
                const char * p = 0;
                regmatch_t m[n_matches];
                if (method == M) {
                    regex_t r;
                    p = path.c_str();
                    status = regcomp(&r, P.c_str(), REG_EXTENDED);
                    if (status) {
                        regfree(&r);
                        continue;
                    }
                    nomatch = regexec(&r, p, n_matches, m, 0);
                    if (nomatch) {
                        regfree(&r);
                        continue;
                    }
                    for (int i = 0; i < n_matches; ++i) {
                        if (m[i].rm_so > -1) {
                            route_part.push_back(std::string(p + m[i].rm_so, m[i].rm_eo - m[i].rm_so));
                        }
                    }
                    result = item.get_class_name();
                    regfree(&r);
                    break;
                }
            }

            return result;
        }
    private:

        void init() {
            if (!this->route_path.empty()) {
                std::ifstream input(this->route_path);
                if (input) {
                    std::string line;
                    while (std::getline(input, line)) {
                        if (line.front() != '#' && !line.empty()) {
                            Poco::StringTokenizer st(line, ",;", Poco::StringTokenizer::TOK_TRIM | Poco::StringTokenizer::TOK_IGNORE_EMPTY);
                            if (st.count() == 3) {
                                this->route_data.push_back(route_element(st[0], st[1], st[2]));
                            }
                        }
                    }
                }
            }
        }
        std::string route_path;
        std::vector<route_element> route_data;
    };

}

#endif /* ROUTE_HPP */

