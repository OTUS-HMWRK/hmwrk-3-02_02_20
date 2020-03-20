#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <charconv>
#include <sstream>
namespace IP {

class IPv4 {
    uint32_t intIP{};
    std::string strIP{};
public:
    IPv4(const uint32_t ip = 0) : intIP(ip) {}
    IPv4(const uint8_t p1, const uint8_t p2, const uint8_t p3, const uint8_t p4) {
        setP1(p1);
        setP2(p2);
        setP2(p3);
        setP3(p4);  
    }
    IPv4(const IPv4 &ip) : intIP{ip.intIP}, strIP{ip.strIP} {}
    IPv4(const std::string &str) {
        try {
            auto point = str.c_str();
            if (auto strSize = str.size(); strSize > 6 && strSize < 16 ) {
                uint8_t valP;
                for (auto i = 0; i < 4; i++) {                
                    if(auto [p, ec] = std::from_chars(point, str.c_str() + strSize, valP ); ec == std::errc() && valP >= 0 && valP < 256){                    
                        setP(valP, static_cast<uint8_t>(i));
                        point = ++p;
                    } else {
                        throw std::invalid_argument(__FUNCSIG__);   
                    }                
                }
                strIP = str;
            } else {strIP = "pizdec";}
        } 
        catch(const std::invalid_argument &e) {
            std::cout << "!!! invalid_argument exception in function: \""
                  << e.what() << "\"\n";
        }
        catch(const std::exception &e) {
            std::cout << "!!! standard exception was caught, with message '"
                  << e.what() << "'\n";
        }
    }
    ~IPv4() {}

    friend bool operator > (IP::IPv4 lhs, IP::IPv4 rhs);
    friend bool operator < (IP::IPv4 lhs, IP::IPv4 rhs);

    const std::string getStr() {        
        std::ostringstream oss;
        oss << (int)*getpP1();
        oss << ".";
        oss << (int)*getpP2();
        oss << ".";
        oss << (int)*getpP3();
        oss << ".";
        oss << (int)*getpP4();
        std::string _strIP = oss.str();
        return _strIP;
    }
    const int getUInt() {
        return intIP;
    }

    const uint8_t * const getpP1() const {return 3 + (uint8_t *)&this->intIP;}
    const uint8_t * const getpP2() const {return 2 + (uint8_t *)&this->intIP;}
    const uint8_t * const getpP3() const {return 1 + (uint8_t *)&this->intIP;}
    const uint8_t * const getpP4() const {return 0 + (uint8_t *)&this->intIP;}
    const uint8_t * const getpP(const uint8_t &n = 0) const { if (n < 4) return n + (uint8_t *)&this->intIP;}
    void setP1(const uint8_t &p) { *(3 + (uint8_t *)&this->intIP) = p;}
    void setP2(const uint8_t &p) { *(2+ (uint8_t *)&this->intIP) = p;}
    void setP3(const uint8_t &p) { *(1 + (uint8_t *)&this->intIP) = p;}
    void setP4(const uint8_t &p) { *(0 + (uint8_t *)&this->intIP) = p;}
    void setP(const uint8_t &p, const uint8_t &n) { *(3 - n + (uint8_t *)&this->intIP) = p;}
};


bool operator > (IP::IPv4 lhs, IP::IPv4 rhs) { return lhs.intIP > rhs.intIP; }
bool operator < (IP::IPv4 lhs, IP::IPv4 rhs) { return lhs.intIP < rhs.intIP; }
}

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.emplace_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(str.substr(start));

    return r;
}

class ip_comparator
{    
    bool ip_cmp_rcrsv(
        std::vector<std::string>::const_iterator a, 
        std::vector<std::string>::const_iterator b,
        std::vector<std::string>::const_iterator a_end, 
        std::vector<std::string>::const_iterator b_end        
    ) 
    {
        bool ret = false;           
        if ((a != a_end) && (b != b_end)) {
            int a_int{};
            int b_int{};
            if(auto [p, ec] = std::from_chars(a->c_str(), a->c_str() + a->size(), a_int); ec == std::errc()){}
            if(auto [p, ec] = std::from_chars(b->c_str(), b->c_str() + b->size(), b_int); ec == std::errc()){}
            if (
                a_int == b_int            
            ) {;
                ret = ip_cmp_rcrsv(++a, ++b, a_end, b_end);
            } else
                ret = (
                    a_int > b_int               
                );
        }
        return ret;       
    }
public:
    bool operator() (
        std::vector<std::string> &a, 
        std::vector<std::string> &b
    )
    {
        return ip_cmp_rcrsv(a.begin(), b.begin(), a.end(), b.end());
    }   
};

void print_ip_pool(std::vector<std::vector<std::string>> &pool)
{
    for(auto ip = pool.cbegin(); ip != pool.cend(); ++ip) {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part) {
            if (ip_part != ip->cbegin())
                { std::cout << "."; }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}

int main(int, char const **) {
    try {
        std::stringstream ss;                
        std::vector<IP::IPv4> IPv4_pool;

        for(std::string line; std::getline(std::cin, line); ) {
            IPv4_pool.emplace_back(split(line, '\t').at(0)).getStr();
        }


        // std::for_each(
        //     std::begin(vs), 
        //     std::end(vs), 
        //     [&IPv4_pool](auto &str){ 
        //         IPv4_pool.emplace_back(str);
        //     }
        // );

        // std::for_each(
        //     std::begin(IPv4_pool), 
        //     std::end(IPv4_pool), 
        //     [](IP::IPv4 &pool){ 
        //         std::cout                 
        //             << "String: " << pool.getStr() 
        //             << "\t\t Int: " <<  static_cast<unsigned>(pool.getUInt()) 
        //             << "\t pInt: " <<  (int)&pool.IP
        //             << "\t p1: " << (int)*pool.getpP1() 
        //             << "\t pp1: " << (int)pool.getpP1()
        //             << "\t p2: " << (int)*pool.getpP2() 
        //             << "\t pp2: " << (int)pool.getpP2()
        //             << "\t p3: " << (int)*pool.getpP3()
        //             << "\t pp3: " << (int)pool.getpP3()
        //             << "\t p4: " << (int)*pool.getpP4() 
        //             << "\t pp4: " << (int)pool.getpP4() 
        //         << std::endl;
        //     }
        // );
        //std::cout << "IPv4_pool size: " << IPv4_pool.size() << std::endl;
        //IPv4_pool.reserve(600);
        std::sort<>(std::begin(IPv4_pool),std::end(IPv4_pool), std::greater<IP::IPv4>());
        
        // std::vector<IP::IPv4> IPv4_pool2;
        // IPv4_pool2.resize(IPv4_pool.size());
        // std::copy(std::begin(IPv4_pool), std::end(IPv4_pool), std::begin(IPv4_pool2));
        std::for_each(
            std::begin(IPv4_pool), 
            std::end(IPv4_pool), 
            [&ss](IP::IPv4 &pool){ 
                //std::cout
                    ss 
                    << pool.getStr()
                    << "\n"; 
               // << std::endl; 
            }
        );

        std::for_each(
            std::begin(IPv4_pool), 
            std::end(IPv4_pool), 
            [&ss](IP::IPv4 &pool){
                if (*pool.getpP1() == 1) 
                //std::cout
                    ss 
                    << pool.getStr()
                    << "\n"; 
               // << std::endl; 
            }
        );

        std::for_each(
            std::begin(IPv4_pool), 
            std::end(IPv4_pool), 
            [&ss](IP::IPv4 &pool){
                if (
                    *pool.getpP1() == 46
                    &&
                    *pool.getpP2() == 70
                ) 
                //std::cout
                    ss 
                    << pool.getStr()
                    << "\n"; 
               // << std::endl; 
            }
        );
               
                std::for_each(
            std::begin(IPv4_pool), 
            std::end(IPv4_pool), 
            [&ss](IP::IPv4 &pool){
                if (
                    *pool.getpP1() == 46
                    ||
                    *pool.getpP2() == 46
                    ||
                    *pool.getpP3() == 46
                    ||
                    *pool.getpP4() == 46
                ) 
                //std::cout
                    ss 
                    << pool.getStr()
                    << "\n"; 
               // << std::endl;  
            }
        );
        std::cout << ss.str();




        // std::vector<std::vector<std::string> > ip_pool;

        // for(std::string line; std::getline(std::cin, line);)
        // {
        //     std::vector<std::string> v = split(line, '\t');
        //     ip_pool.push_back(split(v.at(0), '.'));
        // }

        // TODO reverse lexicographically sort
        
        // ip_comparator comparator;
        // std::sort(
        //     ip_pool.begin(),
        //     ip_pool.end(),
        //     comparator
        // );

        //print_ip_pool(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // std::vector<std::vector<std::string>>  ip_pool_1;
        // std::for_each(
        //     ip_pool.begin(), 
        //     ip_pool.end(),  
        //     [&ip_pool_1](std::vector<std::string> i)
        //     {   
        //         if ("1" == i[0])
        //             ip_pool_1.push_back(i);             
        //     }
        // );
        //std::cout << "ip_pool_1 size: " << ip_pool_1.size() << std::endl;
        
        //print_ip_pool(ip_pool_1);


        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // std::vector<std::vector<std::string>>  ip_pool_2;
        // std::for_each(
        //     ip_pool.begin(), 
        //     ip_pool.end(),  
        //     [&ip_pool_2](std::vector<std::string> i)
        //     {   
        //         if ("46" == i[0] && "70" == i[1])
        //             ip_pool_2.push_back(i);             
        //     }
        // );
        //std::cout << "ip_pool_2 size: " << ip_pool_2.size() << std::endl;
        
        //print_ip_pool(ip_pool_2);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // std::vector<std::vector<std::string>>  ip_pool_3;
        // std::for_each(
        //     ip_pool.begin(), 
        //     ip_pool.end(),  
        //     [&ip_pool_3](std::vector<std::string> i)
        //     {   
        //         if (("46" == i[0]) || ("46" == i[1]) || ("46" == i[2]) || ("46" == i[3]))
        //             ip_pool_3.push_back(i);             
        //     }
        // );
        //std::cout << "ip_pool_1 size: " << ip_pool_3.size() << std::endl;
        
        //print_ip_pool(ip_pool_3);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
