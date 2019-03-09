#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>

#include "Event.h"

class Log
{
    public:

        template <typename T>
        static void printLine(T arg)
        {
            std::lock_guard<std::recursive_mutex> lock(getMutexInstance());

            std::cout << arg << std::endl;
        }

        template <typename T, typename ...Args>
        static void printLine(T arg, Args... args)
        {
            std::lock_guard<std::recursive_mutex> lock(getMutexInstance());

            std::cout << arg;
            printLine(args...);
        }

    private:

        static std::recursive_mutex &getMutexInstance()
        {
            static std::recursive_mutex mutex;
            return mutex;
        }
};

class ProducerConsumer
{
    public:

        ProducerConsumer() :
            notification(false, false),
            terminated(false)
        {
            threads.push_back(std::thread(&ProducerConsumer::producerThread, this));

            const unsigned numberOfConsumers = 4;
            for (unsigned consumerId = 0; consumerId < numberOfConsumers; consumerId++)
                threads.push_back(std::thread(&ProducerConsumer::consumerThread, this, consumerId));
        }

        ~ProducerConsumer()
        {
            terminated = true;
            notification.change(true);
            notification.set();

            for(auto &thread : threads)
                if (thread.joinable())
                    thread.join();
        }

    private:

        Moya::Event notification;

        std::mutex productsLock;
        std::queue<int> products;

        std::atomic<bool> terminated;
        std::vector<std::thread> threads;

        void producerThread()
        {
            for (int productId = 1; !terminated; productId++) {

                Log::printLine("[ProducerConsumer] Producing ", productId);
                produce(productId);

                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }
        }

        void consumerThread(int consumerId)
        {
            while (!terminated) {
                notification.wait();
                if (terminated)
                    break;

                int productId;
                while (consume(productId)) {
                    Log::printLine("[ProducerConsumer] Consumer ", consumerId, " consumes ", productId);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
        }

        void produce(int &productId)
        {
            std::lock_guard<std::mutex> lock(productsLock);

            products.push(productId);
            notification.set();
        }

        bool consume(int &productId)
        {
            std::lock_guard<std::mutex> lock(productsLock);

            if (products.empty())
                return false;

            productId = products.front();
            products.pop();
            return true;
        }
};

int main()
{
    ProducerConsumer producerConsumer;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    Log::printLine("[Main] Beginning termination procedure...");
    return 0;
}
