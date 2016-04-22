/*
  The MIT License

  Copyright (c) 2016 Thomas Sarlandie thomas@sarlandie.net

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/
#pragma once

#include <Arduino.h> // for millis()
#include <stdint.h>
#include <limits.h>
#include "List.h"

class Task {
  protected:
    const char *_taskName;

  public:
    Task(const char *taskName) : _taskName(taskName) {};
    virtual ~Task() {};

    virtual const char *getTaskName() const {
      return _taskName;
    };

    /**
     * Performs initial setup of the task. It is ok to take some time
     * to perform the initial setup (especially when initializing hardware,
     * etc)
     */
    virtual void setup() {};

    /**
     * Perform your work as fast as possible, do not block.
     */
    virtual void loop() = 0;
};

class IntervalTask : public Task {
  private:
    uint32_t interval;
    uint32_t lastRun;
    Task *task;

  public:
    IntervalTask(Task *t, uint32_t interval) : Task(t->getTaskName()), interval(interval),lastRun(0) {
      task = t;
    };

    ~IntervalTask() {
      delete(task);
    };

    void setup() {
      task->setup();
    };

    void loop() {
      if (millis() > lastRun + interval) {
        task->loop();
        lastRun = millis();
      }
    };
};


class RunStat {
  private:
    unsigned int runCount;
    unsigned long totalRunTime;
    unsigned long minRunTime;
    unsigned long maxRunTime;

  public:
    RunStat() : runCount(0), totalRunTime(0), minRunTime(ULONG_MAX), maxRunTime(0) { };

    void recordRun(unsigned long runTimeUs) {
      runCount++;
      totalRunTime += runTimeUs;
      if (minRunTime == 0 || runTimeUs < minRunTime) {
        minRunTime = runTimeUs;
      }
      if (runTimeUs > maxRunTime) {
        maxRunTime = runTimeUs;
      }
    };

    inline unsigned long count() { return runCount; };
    inline unsigned long totalTime() { return totalRunTime; };
    inline unsigned long maxTime() const { return maxRunTime; };

    inline unsigned long minTime() const {
      if (runCount > 0) {
        return minRunTime;
      }
      else {
        return 0;
      }
    };

    inline unsigned long avgTime() {
      if (runCount == 0) {
        return 0;
      }
      return totalRunTime / runCount;
    }
};

class TaskManager {
  private:
    bool running = false;
    LinkedList<Task*> tasks;
    RunStat loopStats;
    RunStat *taskStats = 0;

    elapsedMillis statDisplayTimer;
    unsigned long statDisplayInterval = 5000;

  public:
    TaskManager() { };

    void addTask(Task *t);
    void restartStats();

    void setup();
    void loop();
};

