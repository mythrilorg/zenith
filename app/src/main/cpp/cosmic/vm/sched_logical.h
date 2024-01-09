#pragma once
#include <vector>
#include <list>

#include <common/types.h>
namespace cosmic::vm {
    using SchedulerInvokable = std::function<void(u64, bool)>;
    using CallBackId = u64;
    using CallBackParam = std::tuple<u64, bool>;

    enum AffinityControl {
        EmotionEngine = 0x3,
        GS = 0x6,
        VUs = 0x7
    };
    enum AffinityModels {
        Normal,
        PrioritizeVectors,
        GraphicsFirst,
    };
    struct TimerTask {
        i64 target;
        i64 lastUpdate;
        i64 runAt;
    };
    struct CommonSched {
        CommonSched() {
        }
        TimerTask timer;
        std::tuple<u64, bool> params;
        SchedulerInvokable callback;

    };

    struct TimerSched : CommonSched {
        bool isPaused;
        bool canOverflow;
        bool hasTarget;
        CallBackId childEvent;
        u64 overflowMask;
    };
    struct EventSched : CommonSched {
    };

    class Scheduler {
    public:
        struct MachineCycles {
            u32 highClock;
            u64 remain;
            u32 cycles;
        };
        enum VirtDeviceLTimer : u8 {
            Mips,
            IOP,
            Bus
        };

        Scheduler();
        void resetCycles();

        u32 getNextCycles(VirtDeviceLTimer high0);
        void updateCyclesCount();

        [[nodiscard]] CallBackId makeEt(bool isEvent = true, SchedulerInvokable invoke = {});
        [[nodiscard]] CallBackId spawnTimer(CallBackId id, u64 ovMask, CallBackParam param);
        CallBackId pushUpcomingEt(CallBackId id, u64 run, CallBackParam param);

        void runEvents();

        u32 affinity{};
    private:
        MachineCycles eeCycles,
            busCycles,
            iopCycles;
        i64 nextEventCycle;

        std::vector<CommonSched> schedTimers;
        std::vector<CommonSched> schedEvents;

        std::vector<TimerSched> timers;
        std::list<EventSched> events;
    };
}
