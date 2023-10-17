#pragma once
#include <console/virtual_devices.h>
#include <link/glb_memory.h>

namespace zenith::console {
    class EmuVM {
    public:
        EmuVM(std::shared_ptr<link::GlobalMemory>& memory,
              std::shared_ptr<console::VirtualDevices>& devices);

        void resetVM();

        std::shared_ptr<kernel::BiosHLE> biosHLE;
    private:
        std::shared_ptr<link::GlobalMemory> emuMem;
        std::shared_ptr<eeiv::EEMipsCore> mips;
        std::shared_ptr<iop::IOMipsCore> iop;

        u8 frames;
    };
}
