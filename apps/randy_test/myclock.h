#ifndef MYCLOCK_H
#define MYCLOCK_H

#include <chrono>
#include <ctime>

class myClock
{
public:
    myClock(){ ; }
    virtual ~myClock(){ ; }

    void start()
    {
        c_start = std::clock();
        t_start = std::chrono::high_resolution_clock::now();
    }

    void end()
    {
        c_end = std::clock();
        t_end = std::chrono::high_resolution_clock::now();

        std::cout << std::fixed << std::setprecision(2) << "- CPU time used: "
            << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
            << "- Wall clock time passed: "
            << std::chrono::duration<double, std::milli>(t_end - t_start).count()
            << " ms\n";
    }

private:
    std::clock_t c_start;
    std::clock_t c_end;
    std::chrono::high_resolution_clock::time_point t_start;
    std::chrono::high_resolution_clock::time_point t_end;
};

class myAutoClock : public myClock
{
public:
    myAutoClock(std::string description) : _description(description)
    {
        std::cout << "Starting: " << _description << std::endl;
        start();
    }

    virtual ~myAutoClock()
    {
        std::cout << "Ending: " << _description << std::endl;
        end();
    }

private:
    std::string _description;
};


#endif // MYCLOCK_H
