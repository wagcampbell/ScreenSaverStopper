/**
 * Stop system from going to screensaver by sending F24 keystroke every 40 seconds.
 * License: GNU GPLv3
 * Source: https://raymii.org
 * Author: Remy van Elst, 2019
 */
#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00
#include <windows.h>

#include <csignal>
#include <atomic>
#include <iostream>
#include <thread>

constexpr auto sleep_seconds = 5;

std::atomic_bool stopRunning = false;
void signal_handler(int signal)
{
  stopRunning = true;
}

void sendKey(boolean release = false)
{
  std::cout << "Sending F24 key ";
  INPUT ip;
  ip.type = INPUT_KEYBOARD;
  ip.ki.wVk = 0x87; // A virtual-key code. 0x87 is the "F24" key.
  ip.ki.wScan = 0;  // A hardware scan code for key.
  if (release) {
    std::cout << "up";
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // The key is being released.
  } else {
    ip.ki.dwFlags = 0; // If not specified, the key is being pressed. 
    std::cout << "down";
  }
  ip.ki.time = 0; // The time stamp for the event, in milliseconds.
  ip.ki.dwExtraInfo = 0; // An additional value associated with the keystroke.
  std::cout << std::endl;
  SendInput(1, &ip, sizeof(INPUT));
}

int main()
{
  std::signal(SIGINT, signal_handler);
  while (!stopRunning) {
    sendKey();
    sendKey(true);
    std::cout << "Sleeping " << sleep_seconds << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sleep_seconds));
  }
  return 0;
}
