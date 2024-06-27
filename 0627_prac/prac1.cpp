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
    this_thread::sleep_for(chrono::seconds(wait_second)); // lock�� ���ԵǸ� �ȵ�.
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
    // ���� �Լ�, �Ϲ� �Լ� == return
    cv.wait(lock, [] { return alarm_set; /* return Alarm_set(); */ });
    cout << "�˶��� �︳�ϴ�!!" << endl;
}

int main()
{
    int wait_second = 5;
    cout << "�˶��� ��ٸ��� ��.. " << endl;

    thread t_timer(timer, wait_second);
    thread t_wait(wait_for_alarm);

    t_timer.join();
    t_wait.join();

    return 0;
}