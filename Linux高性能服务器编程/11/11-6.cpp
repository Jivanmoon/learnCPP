#ifndef MIN_HEAP
#define MIN_HEAP

#include<iostream>
#include<netinet/in.h>
#include<time.h>
using namespace std;

#define BUFFER_SIZE 64

class heap_timer;//前向声明
//绑定socket和定时器
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    heap_timer *timer;
};

//定时器类
class heap_timer {
public:
    heap_timer(int delay) {
        expire = time(NULL) +delay;
    }
public:
    time_t expire;//定时器生效的绝对时间
    void (*cb_func)(client_data *);//定时器的回调函数
    client_data *user_data;//用户数据
};

//时间堆类
class timer_heap {
public:
    //构造函数之一，初始化一个cap大小的空堆
    timer_heap(int cap) throw (std::exception) : capacity(cap), cur_size(0) {
        array = new heap_timer *[capacity];//创建堆数组
        if(!array) {
            throw std::exception();
        }
        for(int i = 0; i < capacity; ++i) {
            array[i] = NULL;
        }
    }

    //构造函数之二，用已有的数组来初始化堆
    timer_heap(heap_timer **init_array, int size, int capacity) throw (std::exception) : cur_size(size), capacity(capacity) {
            if(capacity < size) {
                throw std::exception();
            }
            array = new heap_timer *[capacity];//创建堆数组
            if(!array) {
                throw std::exception();
            }
            for(int i = 0; i < capacity; ++i) {
                array[i] = NULL;
            }
            if(size != 0) {
                //初始化堆数组
                for(int i = 0; i < size; ++i) {
                    array[i] = init_array[i];
                }
                for(int i = (cur_size - 1) / 2; i >= 0; --i) {
                    //对数组中的第(cur_size - 1) / 2 ~ 0个元素执行下虑操作
                    percoltae_down(i);
                }
            }
    }

    //销毁时间堆
    ~timer_heap() {
        for(int i = 0; i < cur_size; ++i) {
            delete array[i];
        }
        delete[] array;
    }
public:
    //添加目标定时器timer
    void add_timer(heap_timer *timer) throw (std::exception) {
        
    }

private:
    //最小堆的下虑操作，它确保堆数组中以第hole个节点作为根的子树拥有的最小堆性质
    void percolate_down(int hole) {
        heap_timer *temp = array[hole];
        int child = 0;
        for(; (hole * 2 + 1) <= (cur_size - 1); hole = child) {
            child = hole * 2 + 1;
            if((child < (cur_size - 1)) && (array[child + 1]->expire < array[child]->expire)) {
                ++child;
            }
            if(array[child]->expire < temp->expire) {
                array[hole] = array[child];
            }
            else {
                break;
            }
        }
        array[hole] = temp;
    }

    //将堆数组容量扩大1倍
    void resize() throw (std::exception) {
        heap_timer **temp = new heap_timer *[2 * capacity];
        for(int i = 0; i < 2 * capacity; ++i) {
            temp[i] = NULL;
        }
        if(!temp) {
            throw std::exception();
        }
        capacity = 2 * capacity;
        for(int i = 0; i < cur_size; ++i) {
            temp[i] = array[i];
        }
        delete[] array;
        array = temp;
    }
private:
    heap_timer **array;//堆数组
    int capacity;//堆数组的容量
    int cur_size;//堆数组当前包含的元素个数
}

#endif