#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
condition_variable cv;
bool alarm_set = false;

void timer(int wait_second)
{
    this_thread::sleep_for(chrono::seconds(wait_second)); // lock에 포함되면 안됨.
    {
        unique_lock<mutex> lock(mtx);
        alarm_set = true;
        cv.notify_one();
    }
}

// bool Alarm_set() { return alarm_set; };

void wait_for_alarm()
{
    unique_lock<mutex> lock(mtx);
    // 람다 함수, 일반 함수 == return
    cv.wait(lock, [] { return alarm_set; /* return Alarm_set(); */ });
    cout << "알람이 울립니다!!" << endl;
}

int main()
{
    int wait_second = 5;
    cout << "알람을 기다리는 중.. " << endl;

    thread t_timer(timer, wait_second);
    thread t_wait(wait_for_alarm);

    t_timer.join();
    t_wait.join();

    return 0;
}