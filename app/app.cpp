#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <charconv>

namespace IP {

class IPv4 {
public:
    uint32_t IP;
    uint8_t 
        *p1 = (uint8_t *)&IP, 
        *p2 = p1 + 1, 
        *p3 = p1 + 2, 
        *p4 = p1 + 3;
    
    uint8_t * aIP = p1;
    IPv4(const uint32_t ip = 0) : IP(ip) {}
    IPv4(const uint8_t p1, const uint8_t p2, const uint8_t p3, const uint8_t p4) {
        aIP[0] = p1;
        aIP[1] = p2;
        aIP[2] = p3;
        aIP[3] = p4;    
    }
    IPv4(const IPv4 &ip) : IP(ip.IP) {}
    IPv4(const std::string &str) {
        try {
            auto point = str.c_str();
            for (auto i = 0; i < 4; i++) {
                if(auto [p, ec] = std::from_chars(point, str.c_str() + str.size(), aIP[i]); ec == std::errc()){
                    point = ++p;
                    std::cout << "p: " << (int)p << ", value: " << (int)aIP[i] << std::endl;
                } else {
                    throw 20;
    
                }
                
            }
        } 
        catch(const std::exception &e) {
            std::cout << " a standard exception was caught, with message '"
                  << e.what() << "'\n";
        }
    }
};

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
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

class ip_comparator
{
    // std::vector<std::string>::iterator &a_end;
    // std::vector<std::string>::iterator &b_end;
    
    
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
                //std::cout << a_int << " - a" << std::endl;
            if(auto [p, ec] = std::from_chars(b->c_str(), b->c_str() + b->size(), b_int); ec == std::errc()){}
                //std::cout << b_int << " - b" << std::endl;
            if (
                a_int == b_int
                //*a == *b
            
            ) {;
                ret = ip_cmp_rcrsv(++a, ++b, a_end, b_end);
            } else
                //ret = !std::lexicographical_compare(a->begin(), a->end(), b->begin(), b->end());
                ret = (
                    a_int > b_int
                    //*a < *b
                
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
            for(std::vector<std::vector<std::string> >::const_iterator ip = pool.cbegin(); ip != pool.cend(); ++ip)
        {
            for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if (ip_part != ip->cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }
}

int main(int, char const **)
{
    try
    {
        //IP::IPv4 testIP(512);
        //IP::IPv4 testIP{0,0,1,0};
        IP::IPv4 testIP("123.345.0.99");
        std::cout << "testIP.IP: " << testIP.IP << std::endl;
        std::cout << "testIP.aIP[0]: " << (int)testIP.aIP[0] << std::endl;
        std::cout << "testIP.aIP[1]: " << (int)testIP.aIP[1] << std::endl;
        std::cout << "testIP.aIP[2]: " << (int)testIP.aIP[2] << std::endl;
        std::cout << "testIP.aIP[3]: " << (int)testIP.aIP[3] << std::endl;

        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort
        
        ip_comparator comparator;
        std::sort(
            ip_pool.begin(),
            ip_pool.end(),
            comparator
        );

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

        std::vector<std::vector<std::string>>  ip_pool_1;
        std::for_each(
            ip_pool.begin(), 
            ip_pool.end(),  
            [&ip_pool_1](std::vector<std::string> i)
            {   
                if ("1" == i[0])
                    ip_pool_1.push_back(i);             
            }
        );
        //std::cout << "ip_pool_1 size: " << ip_pool_1.size() << std::endl;
        
        //print_ip_pool(ip_pool_1);


        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        std::vector<std::vector<std::string>>  ip_pool_2;
        std::for_each(
            ip_pool.begin(), 
            ip_pool.end(),  
            [&ip_pool_2](std::vector<std::string> i)
            {   
                if ("46" == i[0] && "70" == i[1])
                    ip_pool_2.push_back(i);             
            }
        );
        //std::cout << "ip_pool_2 size: " << ip_pool_2.size() << std::endl;
        
        //print_ip_pool(ip_pool_2);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        std::vector<std::vector<std::string>>  ip_pool_3;
        std::for_each(
            ip_pool.begin(), 
            ip_pool.end(),  
            [&ip_pool_3](std::vector<std::string> i)
            {   
                if (("46" == i[0]) || ("46" == i[1]) || ("46" == i[2]) || ("46" == i[3]))
                    ip_pool_3.push_back(i);             
            }
        );
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
